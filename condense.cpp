#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <list>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

bool getRow(string line, vector<string>& rowData){
  string datum = "";
  int count_comma = 0;
  for(unsigned int i=0; i<line.size(); i++){
    if(line[i] != ','){
      datum.push_back(line[i]);
    }
    else{
      count_comma++;
      rowData.push_back(datum);
      datum = "";
    } // Split data

  } // Go over the row
  rowData.push_back(datum);
  if (count_comma != 3) {
    return false;
  }
  return true;
}

float getSquaredist(float lat_1, float lat_2, float lon_1, float lon_2){
  float pi = atan(1)*4;
  float R = 6371;
  float dist_lat = abs(lat_2-lat_1)*pi/180*R;
  float dist_long = abs(lon_2-lon_1)*pi/180*cos(abs(lat_2))*R;
  return dist_lat + dist_long;
}

float timeStamp(string datetime){
  int datestamp = 0;
  // int timestamp = 0;
  int mark = 0;
  int stamp = 0;
  string temp = "";
  for(unsigned int i=0; i<datetime.size(); i++){
    if(datetime[i] == '-' || datetime[i] == ' '){
      stamp = atoi(temp.c_str());
      mark++;
      if(mark == 1){
        // timestamp += 385*stamp;
        datestamp += 420*stamp;
      }
      if(mark == 2){
        datestamp += 32*stamp;
        // timestamp += 32*stamp;
      }
      if(mark == 3){
        // timestamp += stamp;
        datestamp += stamp;
      }
      temp = "";
      continue;
    }
    temp.push_back(datetime[i]);
  }
  // stamp = atoi(temp.c_str());
  // timestamp += stamp;
  // list<float> dtStamp;
  // dtStamp.push_back((float)datestamp);
  // dtStamp.push_back((float)timestamp);
  return datestamp;
}

float durCal(string pickup, string dropoff, bool& non_sense){
  int pickupTime = 0;
  int dropoffTime = 0;
  int stamp = 0;
  int mark = 0;
  string pickupTemp = "";
  string dropoffTemp = "";
  for(unsigned int i=0; i<pickup.size(); i++){
    if(pickup[i] == '-' || pickup[i] == ':' || pickup[i] == ' '){
      mark++;
      stamp = atoi(pickupTemp.c_str());
      if(mark == 4) pickupTime += 60*60*stamp;
      if(mark == 5) pickupTime += 60*stamp;
      pickupTemp = "";
      continue;
    }
    pickupTemp.push_back(pickup[i]);
  }
  stamp = atoi(pickupTemp.c_str());
  pickupTime += stamp;
  mark = 0;
  for(unsigned int i=0; i<dropoff.size(); i++){
    if(dropoff[i] == '-' || dropoff[i] == ':' || dropoff[i] == ' '){
      mark++;
      stamp = atoi(dropoffTemp.c_str());
      if(mark == 4) dropoffTime += 60*60*stamp;
      if(mark == 5) dropoffTime += 60*stamp;
      dropoffTemp = "";
      continue;
    }
    dropoffTemp.push_back(dropoff[i]);
  }
  stamp = atoi(dropoffTemp.c_str());
  dropoffTime += stamp;
  if(timeStamp(pickup) != timeStamp(dropoff)) dropoffTime += 60*60*24;
  float duration = (float)dropoffTime - (float)pickupTime;
  if(duration <= 0 || duration > 60*60*12) non_sense = true;
  return duration;
}

string date2str(int datestamp){
  ostringstream temp1;
  int mod = datestamp % 420;
  int day = mod % 32;
  int month = (mod - day) / 32;
  int year = (datestamp - mod) / 420;
  temp1 << year;
  string str_year = temp1.str();
  ostringstream temp2;
  temp2 << month;
  string str_month = temp2.str();
  ostringstream temp3;
  temp3 << day;
  string str_day = temp3.str();
  return str_year + "-" + str_month + "-" + str_day;
}

// void timeCollision(const map<float, vector<string> >& oneDay, float& t){
//   map<float, vector<string> >::const_iterator it = oneDay.find(t);
//   if(it != oneDay.end()){
//     t += 0.1;
//     timeCollision(oneDay, t);
//   }
// }
//
// void timeStampcolli(const map<float, map<float, vector<string> > >& dataAll, float d, float& t){
//   map<float, map<float, vector<string> > >::const_iterator it = dataAll.find(d);
//   if(it != dataAll.end())
//     timeCollision(it->second, t);
// }

int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "Wrong Input" << endl;
    return 1;
  }

  ifstream inFile(argv[1]);
  ofstream outFile(argv[2]);

  if (!outFile || !inFile) {
    cerr << "Error: files cannot open\n";
    return 1;
  }

  vector<string> rowData;
  map<float, vector<vector<string> > > dataAll;
  // inFile.open("train_part.txt");
  string line;
  getline(inFile, line);
  while(!inFile.eof()){
    vector<string> rowData;
    getline(inFile, line);
    if(!getRow(line, rowData)) continue;
    float dateStamp = timeStamp(rowData[0]);
    // float d = dtstamps.front();
    // dtstamps.pop_front();
    // float t = dtstamps.front();
    // dtstamps.pop_front();
    // dateStamps.push_back(d);
    // timeStampcolli(dataAll, d, t);
    // timeStamps.push_back(t);
    // dataAll[d][t] = rowData;
    dataAll[dateStamp].push_back(rowData);
  } // Cycle through rows

  vector<vector<float> > statistics;
  vector<string> dateVec;
  map<float, vector<vector<string> > >::iterator it1;
  // map<float, vector<string> >::iterator it2;

    for(it1=dataAll.begin(); it1 != dataAll.end(); it1++){
     vector<float> temp;
     int count = 0;
     float totalPassen = 0;
     float time_dist = 0;
     // float time_dist_km = 0;
     float sumDur = 0;
     float sumDist = 0;
     // float sumDist_km = 0;

     for(unsigned int i=0; i<it1->second.size(); i++){
       bool non_sense = false;
       float duration = durCal(it1->second[i][0], it1->second[i][1], non_sense);
       if(non_sense) continue;
       count++;
       float passen = strtof(it1->second[i][2].c_str(), 0);
       if(passen == 0) continue;
       totalPassen += passen;
       float dist = strtof(it1->second[i][3].c_str(), 0);
       if(dist == 0) continue;
       sumDist += dist;
       sumDur += duration;
       time_dist += dist/duration*(float)3600;
     }

     // for(it2=it1->second.begin(); it2 != it1->second.end(); it2++){
     //   count++;
     //   float passen = strtof(it2->second[3].c_str(), 0);
     //   totalPassen += passen;
     //   float start_long = strtof(it2->second[6].c_str(),0);
     //   float end_long = strtof(it2->second[4].c_str(),0);
     //   float start_lat = strtof(it2->second[7].c_str(),0);
     //   float end_lat = strtof(it2->second[5].c_str(),0);
     //   float dist = abs(start_long - end_long)+abs(start_lat - end_lat);
     //   float dist_km = getSquaredist(start_long, end_long, start_lat, end_lat);
     //   float tripDur = strtof(it2->second[8].c_str(),0);
     //   time_dist += dist/tripDur;
     //   time_dist_km += dist_km/tripDur;
     //   duration += tripDur;
     //   sumDist += dist;
     //   sumDist_km += dist_km;
     // }
     float avg_Passen = totalPassen/(float)count;
     float avg_speed = time_dist/(float)count;
     // float avg_t_dist_km = time_dist_km/(float)count;
     float avg_duration = sumDur/(float)count;
     float avg_Dist = sumDist/(float)count;
     string date_str = date2str((int)it1->first);

     //cout << time_dist << " " << count << endl;
     //cout << avg_t_dist << endl;

     dateVec.push_back(date_str);
     temp.push_back(totalPassen);
     temp.push_back(avg_Passen);
     // temp.push_back(avg_t_dist);
     // temp.push_back(avg_t_dist_km);
     // temp.push_back(avg_duration);
     temp.push_back(count);
     temp.push_back(sumDist);
     temp.push_back(avg_Dist);
     temp.push_back(avg_speed);
     temp.push_back(avg_duration);
     // temp.push_back(sumDist_km);
     statistics.push_back(temp);
   }

   outFile << "Date Total_Passenger Avg_Passen  Total_trip Total_dist(mi) Avg_Dist(mi)  Avg_speed(mi/h) Avg_duration(s)" << endl;

    for(unsigned int i=0; i<statistics.size(); i++){
      outFile << dateVec[i] << "  ";
      for(unsigned int j=0; j<statistics[i].size(); j++)
        outFile << statistics[i][j] << "  ";

      outFile << endl;
    }


}
