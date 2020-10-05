# Problem Statement 1
The implementation follows the lecture slides from the machine learning course by Andrew Ng on coursera (https://www.coursera.org/learn/machine-learning)

I did not know, which graph to plot exactly and which should be the examined feature. At the moment learning_rate, iterations and testwise CO(GT) as examined feature are hardcoded. There was no time left for a neat writeup of code and documentation.

# HOW TO
Filename of data file is hardcoded as "AirQualityUCI.csv". It has to be in the current working directory as the executable

# TODOs:
- read csv file
    + String
    + Date
    + Time
    + float
    + int
- handle missing values
    + average ove whole data
    + or use average over 2-4 datapoints
- optionally normalize data
- lin reg with grad desc alg
- make qt application