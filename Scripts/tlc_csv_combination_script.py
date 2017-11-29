import csv
import os


#Purpose: Combine the data from the yellow or green data set into a single file
# Cut down the number of rows to pickup datetime, dropoff datetime, passengers in that ride, and trip distance

#Data was taken from the TLC taxi data
#	Source: http://www.nyc.gov/html/tlc/html/about/trip_record_data.shtml

#To Use, download data from the website and put it in a folder called: "green" or "yellow"

def readDir(write_file, directory):
	for filename in os.listdir(directory):
		file_dir_name = directory + "/" + filename
		print file_dir_name
		count = 0
		with open(file_dir_name,"rb") as source:
			col1val, col2val, col3val, col4val = 0, 0, 0, 0
			rdr= csv.reader( source )
			for r in rdr:
				if count == 0:
					head_c = 0
					for rItem in r:
						if "pep_pick" in str(rItem).lower():
							print rItem
							col1val = head_c
							#col2val = head_c + 1
						if "pep_drop" in str(rItem).lower():
							print rItem
							col2val = head_c
						if "passenger" in str(rItem).lower():
							print rItem
							col3val = head_c
							#col4val = head_c + 1
						if "trip_dist" in str(rItem).lower():
							print rItem
							col4val = head_c
						head_c += 1
				#print r
				#if count > 1: write_file.writerow( (r[1], r[2], r[9], r[10]) )
				if count > 1: 
					#print r[col4val]
					#if r[col3val] == "0": print r
					#if r[col4val] != ".00":
					write_file.writerow( (r[col1val], r[col2val], r[col3val], r[col4val]) )
					#write_file.writerow( (r[col1val], r[col2val], r[col3val], r[col4val]) ) #Green
				#if count > 1: write_file.writerow( (r[9], r[10]) )
				else: count += 1		
	return

dir_name = "yellow"
with open(dir_name+"_tlc_data.csv","wb") as result:
	write_file = csv.writer( result )
	write_file.writerow( ("Pickup_datetime", "Dropoff_datetime", "Passenger_count", "Trip_distance") )
	#write_file.writerow( ("Passenger_count", "Trip_distance") )
	readDir(write_file, dir_name)