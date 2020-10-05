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
#include <dlib/matrix.h>

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
        result.push_back({"x0", vector<float>{}});
        while(getline(ss, colname, ';')) {
            if (colname.empty()) break;
            result.push_back({colname, vector<float> {}});
        }
    }

    while(getline(myFile, line) && line.substr(0,1) != ";")
    {      
        result.at(0).second.push_back(1.0);
        stringstream ss(line);
        int colIdx = 1;
        while (getline(ss, val, ';') && colIdx < result.size())
        {
           if (colIdx == 1) {
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
    int nc = data.size();
    int nr = data.at(0).second.size();
    // convert it to a matrix of dlib
    dlib::matrix<float> M(nr, nc);
    for (long r = 0; r < M.nr(); ++r)
    {
        for (long c = 0; c < M.nc(); ++c)
        {
            M(r,c) = data.at(c).second.at(r);
        }
    }

    // handle missing data (take average of available data) and scale features
    long double average = 0;
    double max = 0;
    int count = 0;
    float val = 0;
    for (long c = 0; c < M.nc(); ++c)
    {
        max = 0;
        average = 0;
        count = 0;
        val = 0;
        for (long r = 0; r < M.nr(); ++r)
        {
            val = M(r,c);
            if(val == -200) continue;
            average += val;
            count++;
            if (val > max) max = val;
        }
        average = average/count;
        for (long r = 0; r < M.nr(); ++r)
        {
            val = M(r,c);
            if(val == -200) M(r,c) = average;
            M(r,c) = M(r,c)/max;
        }
    }
    //cout << std::setprecision(9) << M << endl;

    // lin reg with gradient descent. e.g. for y = CO(GT)
    int y_index = 3;
    dlib::matrix<float> thetas(nc, 1);
    for (int i = 0; i < thetas.nr(); i++) thetas(i) = 0;
    dlib::matrix<float> newThetas(nc, 1);
    int iterations = 1000;
    float learning_rate = 0.01;
    double m_inv = (double)1/nr;

    double sum;
    for (int its = 0; its < iterations; its++)
    {
        cout << thetas << endl;
        for (int j = 0; j < thetas.nr(); j++)
        {
            sum = 0;

            for (int i = 0; i < nr; i++)
            {
                //cout << M(i,y_index) << endl;
                //cout << (dlib::trans(thetas)*dlib::trans(dlib::rowm(M,i)));
                sum += ((dlib::trans(thetas)*dlib::trans(dlib::rowm(M,i))) - M(i,y_index))*M(i,j);
            }
            newThetas(j) = thetas(j) - learning_rate*m_inv*sum;
        }
        for (int j = 0; j < thetas.nr(); ++j) thetas(j) = newThetas(j);
    }


    // show results
    /*QApplication DataViewer(argc, argv);
    
    //![4]
    ChartView *chartView = new ChartView();
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();
    //![4]

    if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";
    else std::cout << "pre-standard C++\n";
    char *path=NULL;
    size_t size;
    path=getcwd(path,size);
    cout<<"\n current Path: "<<path<<"\n";

    return DataViewer.exec();*/
    return 0;
}