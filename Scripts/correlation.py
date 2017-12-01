import warnings
warnings.filterwarnings("ignore")

import numpy as np
import pandas as pd

import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.pylab as pylab

import seaborn as sns

%matplotlib inline
mpl.style.use( 'ggplot' )
sns.set_style( 'white' )
pylab.rcParams[ 'figure.figsize' ] = 8, 6

def plot_correlation_map( df ):
    corr = df.corr()
    _, ax = plt.subplots( figsize = (12,10) )
    cmap = sns.diverging_palette( 220, 10, as_cmap=True )
    _ = sns.heatmap(
        corr,
        cmap=cmap,
        square=True,
        cbar_kws= {'shrink': .9},
        ax = ax,
        annot=True,
        annot_kws= {'fontsize': 12}
    )


fname_weather = "Datasets/Wunderground/Monthly_Weather_Raw_JFK_Updated.csv"
fname_stats = "Datasets/new_stats_green_tlc.csv"

weather = pd.read_csv(fname_weather)
stats = pd.read_csv(fname_stats)

weather.drop('Month', axis=1, inplace=True)
weather.drop('Day', axis=1, inplace=True)
weather.drop('Sea Level Press. (in) - Max', axis=1, inplace=True)
weather.drop('Sea Level Press. (in) - Min', axis=1, inplace=True)
weather.drop('Temp. (Degrees F) - Max', axis=1, inplace=True)
weather.drop('Temp. (Degrees F) - Min', axis=1, inplace=True)
weather.drop('Humidity (%) - Max', axis=1, inplace=True)
weather.drop('Humidity (%) - Min', axis=1, inplace=True)
weather.drop('Visibility (mi) - Max', axis=1, inplace=True)
weather.drop('Visibility (mi) - Min', axis=1, inplace=True)
weather.drop('Wind (mph) - Max', axis=1, inplace=True)
weather.drop('Wind (mph) - Max?!?', axis=1, inplace=True)
weather.drop('Dew Point (Degrees F) - Max', axis=1, inplace=True)
weather.drop('Dew Point (Degrees F) - Min', axis=1, inplace=True)


frames = [stats, weather]
result = pd.concat(frames, axis=1)
result.drop('Event', axis=1, inplace=True)
#result.drop('Date', axis=1, inplace=True)

plot_correlation_map(result)