import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import pandas as pd
import matplotlib.dates as dates



def dataPlot(x, y, x_label, y_label, title, logPlot):
    fig, ax = plt.subplots()
    fit = np.polyfit(x, y, deg=1)
    ax.plot(x, fit[0] * x + fit[1], color='red')
    ax.scatter(x, y, marker='.', label='Day in 2016')

    plt.title(title)
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.legend()
    #ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1), fancybox=True, shadow=True, ncol=5)
    if logPlot == False:
        plt.savefig('Plots/'+title+'.png', bbox_inches='tight')
    else:
        plt.xscale('log')
        plt.yscale('log')
        plt.savefig('Plots/'+title+' - log scaled'+'.png', bbox_inches='tight')
    
    #plt.show()
    return

def datePlot(x, y, x_label, y_label, title):
    plt.plot(x, y, marker='.')
    plt.title(title)
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.gcf().autofmt_xdate()
    plt.show()
    return

def eventList(event, weather_data, stats_data):
    Event_List = weather_data['Event'].values
    Total_Trips = stats_data['Total_trip'].values

    Total_Event_trips = []

    countIndex = 0
    for val in Event_List:
        if event in str(val):
            Total_Event_trips.append(Total_Trips[countIndex])
        countIndex += 1

    plt.boxplot(Total_Event_trips)
    plt.title("Total Passengers during day of " + event)
    plt.ylabel("Total Passengers per day")
    plt.show()

def eventBarPlot(weather_data, stats_column, label_label, label_format):
    Event_List = weather_data['Event'].values

    Events = ['Fog', 'nan', 'Rain', 'Snow', 'Thunderstorm']
    Event_freq = [0.0, 0.0, 0.0, 0.0, 0.0]
    colors = ['#624ea7', 'g', 'yellow', 'k', 'maroon']

    listItr = 0
    for eventItr in Events:
        sumTotal = 0
        sumCount = 0

        EventListItr = 0
        for item in Event_List:
            if eventItr in str(item):
                sumCount += 1
                sumTotal += stats_column[EventListItr]
            EventListItr += 1
        Event_freq[listItr] = float(sumTotal)/float(sumCount)
        listItr += 1
    print Event_freq
    Events[1] = "None"

    plt.bar(Events, Event_freq, color=colors)
    plt.gca().set_ylim(bottom=max(Event_freq)/1.25)
    plt.gca().set_axisbelow(True)
    plt.gca().yaxis.grid(True)

    plt.xticks(Events)
    plt.ylabel(label_label + ' per Day' + label_format)
    plt.title(label_label +' For Each Event (NYC, Green Line, 2016)')
    plt.savefig('Plots/Event Bar Chat - '+label_label+'.png', bbox_inches='tight')
    #plt.show()
    plt.close()
    return



#read and collect data
weather_data = pd.read_csv("Monthly_Weather_Raw_JFK_Updated.csv")
stats_data = pd.read_csv("new_stats.csv")


date_values = stats_data['Date'].values

#Get Weather data columns
Temp_avg = weather_data['Temp. (Degrees F) - Avg'].values
Precipitation_Sum = weather_data['Precipitation (in) - Sum'].values
Wind_avg = weather_data['Wind (mph) - Avg'].values


#Get statistics data columns
Total_dist = stats_data['Total_dist(mi)'].values
Avg_Dist = stats_data['Avg_Dist(mi)'].values
Avg_speed = stats_data['Avg_speed(mi/h)'].values
Avg_duration = stats_data['Avg_duration(s)'].values

Total_trip = stats_data['Total_trip'].values
Total_Passenger = stats_data['Total_Passenger'].values
Avg_Passen = stats_data['Avg_Passen'].values



weather_label = "Precipitation Sum (in)"
logPlotvar = False

#Bar plot code
eventBarPlot(weather_data, Total_dist, 'Average Total Taxi Distance', ' (mi)')
eventBarPlot(weather_data, Avg_Dist, 'Average Taxi Distance', ' (mi)')
eventBarPlot(weather_data, Avg_speed, 'Average Taxi Speed', ' (mph)')
eventBarPlot(weather_data, Avg_duration, 'Average Taxi Trip Duration', ' (s)')

eventBarPlot(weather_data, Total_trip, 'Average Total Taxi Trips', '')
eventBarPlot(weather_data, Total_Passenger, 'Average Total Taxi Passengers', '')
eventBarPlot(weather_data, Avg_Passen, 'Average Taxi Passengers', '')


#Scatter plot code
"""
#Total Distance
dataPlot(Precipitation_Sum, Total_dist, weather_label, 'Total Taxi Distance(mi)', 'Precipitation Sum vs Total Taxi Distance (NYC, Green Line)', logPlotvar)

#Avg Distance
dataPlot(Precipitation_Sum, Avg_Dist, weather_label, 'Average Taxi Distance(mi)', 'Precipitation Sum vs Average Taxi Distance (NYC, Green Line)', logPlotvar)

#Avg speed
dataPlot(Precipitation_Sum, Avg_speed, weather_label, 'Average Taxi Speed (mi/h)', 'Precipitation Sum vs Average Taxi Speed (NYC, Green Line)', logPlotvar)

#Avg duration
dataPlot(Precipitation_Sum, Avg_duration, weather_label, 'Average Taxi Trip Duration (s)', 'Precipitation Sum vs Average Taxi Trip Duration (NYC, Green Line)', logPlotvar)

#---------------------------------------------------------------------------------------

#Total trips
dataPlot(Precipitation_Sum, Total_trip, weather_label, 'Total Taxi Trips', 'Precipitation Sum vs Total Taxi Trips (NYC, Green Line)', logPlotvar)

#Total Passengers
dataPlot(Precipitation_Sum, Total_Passenger, weather_label, 'Total Taxi Passengers', 'Precipitation Sum vs Total Taxi Passengers (NYC, Green Line)', logPlotvar)

#Avg Passengers
dataPlot(Precipitation_Sum, Avg_Passen, weather_label, 'Average Taxi Passengers', 'Precipitation Sum vs Average Taxi Passengers (NYC, Green Line)', logPlotvar)
"""

#Unused functions that were not finished
#datePlot(new_x, x, 'Day', 'Total Distance per day (mi)', 'Total Distance vs Average Temperature per Day')
#eventList("Rain", weather_data, stats_data)

#export DISPLAY=localhost:0.0