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

// Enter data to the vector of list
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

// Deprecated, approximate distance if meters not provided
float getSquaredist(float lat_1, float lat_2, float lon_1, float lon_2){
  float pi = atan(1)*4;
  float R = 6371;
  float dist_lat = abs(lat_2-lat_1)*pi/180*R;
  float dist_long = abs(lon_2-lon_1)*pi/180*cos(abs(lat_2))*R;
  return dist_lat + dist_long;
}

// Use a timestamp to represent the date
float timeStamp(string datetime){
  int datestamp = 0;
  int mark = 0;
  int stamp = 0;
  string temp = "";
  for(unsigned int i=0; i<datetime.size(); i++){
    if(datetime[i] == '-' || datetime[i] == ' '){
      stamp = atoi(temp.c_str());
      mark++;
      if(mark == 1)
        datestamp += 420*stamp;
      if(mark == 2)
        datestamp += 32*stamp;
      if(mark == 3)
        datestamp += stamp;
      temp = "";
      continue;
    }
    temp.push_back(datetime[i]);
  }
  return datestamp;
}

// Calculate duration from pickup and dropoff time
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

// Convert datestamp to date string
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

// Start main
int main(int argc, char* argv[]){
  // Check inputs
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

  // Condense start
  vector<string> rowData;
  map<float, vector<vector<string> > > dataAll;
  string line;
  getline(inFile, line);
  // Loop over the file to get data
  while(!inFile.eof()){
    vector<string> rowData;
    getline(inFile, line);
    if(!getRow(line, rowData)) continue;
    float dateStamp = timeStamp(rowData[0]);
    // Put data into a map
    dataAll[dateStamp].push_back(rowData);
  } // Cycle through rows

  vector<vector<float> > statistics;
  vector<string> dateVec;
  map<float, vector<vector<string> > >::iterator it1;

  // Calculate stats
    for(it1=dataAll.begin(); it1 != dataAll.end(); it1++){
     vector<float> temp;
     int count = 0;
     float totalPassen = 0;
     float time_dist = 0;
     float sumDur = 0;
     float sumDist = 0;

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

     float avg_Passen = totalPassen/(float)count;
     float avg_speed = time_dist/(float)count;
     float avg_duration = sumDur/(float)count;
     float avg_Dist = sumDist/(float)count;
     string date_str = date2str((int)it1->first);
     // Push back all the stats
     dateVec.push_back(date_str);
     temp.push_back(totalPassen);
     temp.push_back(avg_Passen);
     temp.push_back(count);
     temp.push_back(sumDist);
     temp.push_back(avg_Dist);
     temp.push_back(avg_speed);
     temp.push_back(avg_duration);
     statistics.push_back(temp);
   }

   // Header
   outFile << "Date Total_Passenger Avg_Passen  Total_trip Total_dist(mi) Avg_Dist(mi)  Avg_speed(mi/h) Avg_duration(s)" << endl;

   // Output the stats
    for(unsigned int i=0; i<statistics.size(); i++){
      outFile << dateVec[i] << "  ";
      for(unsigned int j=0; j<statistics[i].size(); j++)
        outFile << statistics[i][j] << "  ";

      outFile << endl;
    }

}
