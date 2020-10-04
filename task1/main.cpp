#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include "chartview.h"

QT_CHARTS_USE_NAMESPACE
using namespace std;
using namespace QtCharts;

const string filename = "AirQualityUCI.csv";

vector<pair<string, vector<float>>> read_csv(string filename) {
    vector<pair<string,vector<float>>> result;

    ifstream myFile(filename);
    if (!myFile.is_open()) throw runtime_error("Could not open file");
    string line, colname, val, year, month, day;

    if(myFile.good())
    {
        getline(myFile, line);
        stringstream ss(line);
        while(getline(ss, colname, ';')) {
            if (colname.empty()) break;
            result.push_back({colname, vector<float> {}});
        }
    }

    while(getline(myFile, line) && line.substr(0,1) != ";")
    {
        stringstream ss(line);
        int colIdx = 0;
        while (getline(ss, val, ';') && colIdx < result.size())
        {
           if (colIdx == 0) {
               day = val.substr(0,2);
               month = val.substr(3,2);
               year = val.substr(6,4);
               val = year + month + day;
           }
            replace(val.begin(), val.end(), ',', '.');
            result.at(colIdx).second.push_back(stof(val));
            if(ss.peek() == ';') ss.ignore();
            colIdx++;

        }
        
    }
    myFile.close();
    return result;
}

int main(int argc, char **argv)
{
    cout << "reading data ...\n";
    const string filename = "AirQualityUCI.csv";
    // read data
    vector<pair<string, vector<float>>> data = read_csv(filename);
    cout << "... data read\n";

    // handle missing data

    // normalize data

    // lin reg

    // show results
    QApplication DataViewer(argc, argv);
    
    //![4]
    ChartView *chartView = new ChartView();
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();
    //![4]

    /*if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";
    else std::cout << "pre-standard C++\n";
    char *path=NULL;
    size_t size;
    path=getcwd(path,size);
    cout<<"\n current Path: "<<path<<"\n";*/

    return DataViewer.exec();
}