#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE
using namespace std;

double dist(pair<pair<double, double>, pair<double, double>> pair) {
    return sqrt(pow(pair.first.first - pair.second.first, 2) + pow(pair.first.second - pair.second.second, 2));
}

pair<pair<double, double>,pair<double, double>> findClosestPair(vector<pair<double, double>> list){
    pair<pair<double, double>,pair<double, double>> result, lmin, rmin;
    double dlmin, drmin, dmin, tempmin;
    bool lset, rset;
    lset = false;
    rset = false;
    int size = list.size();
    if (size == 2) return {list.at(0), list.at(1)};
    vector<pair<double, double>> left;
    vector<pair<double, double>> right;
    int i, j;
    for (i = 0; i<size; i++){
        if (i< (size/2)) left.push_back(list.at(i));
        else right.push_back(list.at(i));
    }

    if (left.size() > 1) {
        lmin = findClosestPair(left);
        lset = true;
    }
    if (right.size() > 1){
        rmin = findClosestPair(right);
        rset = true;
    } 
    dlmin = dist(lmin);
    drmin = dist(rmin);
    if (dlmin < drmin && lset) {
        dmin = dlmin;
        result = lmin;
    } else if (rset) {
        dmin = drmin;
        result = rmin;
    }

    double middle = list.at(list.size()/2).first;

    for (i = left.size() - 1; i >= 0; i--) {
        if (middle - left.at(i).first < dmin) {
            // search points in right within (dmin, 2*dmin)-rectangle
            for(j = 0; j < right.size(); j++){
                // if dist < dmin --> dmin = dist & lrmin = pair
                if (right.at(j).first - middle < dmin) {
                    if(abs(left.at(i).second - right.at(i).second) < dmin){
                        tempmin = dist({left.at(i), right.at(j)});
                        if (!rset && !lset) {
                            dmin = tempmin;
                            result = {left.at(i), right.at(j)};
                            continue;
                        }
                        if (tempmin < dmin) {
                            dmin = tempmin;
                            result = {left.at(i), right.at(j)};
                        }
                    }
                } else break; // went too far to the right. no closer pair possible
            }
        } else break; // went too far to the left. no closer pair possible
    }
    return result;
}

vector<pair<double, double>> merge(vector<pair<double, double>> left, vector<pair<double, double>> right) {
    vector<pair<double, double>> result;
    
    while (!left.empty() && !right.empty()) {
        if (left.at(0).first <= right.at(0).first){
            result.push_back(left.at(0));
            left.erase(left.begin());
        } else {
            result.push_back(right.at(0));
            right.erase(right.begin());
        }
    }

    while (!left.empty()) {
        result.push_back(left.at(0));
        left.erase(left.begin());
    }
    while (!right.empty()) {
        result.push_back(right.at(0));
        right.erase(right.begin());
    }
    return result;
}

vector<pair<double, double>> merge_sort(vector<pair<double, double>> list) {
    int size = list.size();
    if (size <= 1) return list;
    vector<pair<double, double>> left;
    vector<pair<double, double>> right;
    int i;
    for (i = 0; i<size; i++){
        if (i< (size/2)) left.push_back(list.at(i));
        else right.push_back(list.at(i));
    }

    left = merge_sort(left);
    right = merge_sort(right);

    return merge(left, right);
}

int main(int argc, char **argv) {
    cout << "Hello, world!\n";
    // generate a random pair of 100 values
    vector<pair<double, double>> list;
    double r1, r2;
    srand ( time(NULL) );
    for (int i = 0; i < 100; i++) {
        r1 = (double)rand() / (double)RAND_MAX ;
        r2 = (double)rand() / (double)RAND_MAX ;
        list.push_back({r1, r2});
    }
    // sort points according to their x-coordinates
    list = merge_sort(list);

    // find closest point with divide and conquer (O(N log N))
    pair<pair<double, double>,pair<double, double>> closestPair = findClosestPair(list);
    // plot it

    QApplication app(argc, argv);
    QChartView *plot = new QChartView();
    QScatterSeries *series = new QScatterSeries();
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series->setMarkerSize(7.0);
    series->setName("All Points");
    for (int i = 0; i < 100; i++) {
        series->append(list.at(i).first, list.at(i).second);
    }

    QScatterSeries *dmin = new QScatterSeries();
    dmin->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    dmin->setMarkerSize(9.0);
    dmin->setColor(QColor(255, 0, 0, 127));
    dmin->setName("Closest Pair, distance: " + QString::fromStdString(to_string(dist(closestPair))));
    dmin->append(closestPair.first.first, closestPair.first.second);
    dmin->append(closestPair.second.first, closestPair.second.second);

    plot->setRenderHint(QPainter::Antialiasing);
    plot->chart()->addSeries(series);
    plot->chart()->addSeries(dmin);

    //chart()->setTitle("Points");
    plot->chart()->createDefaultAxes();
    plot->chart()->setDropShadowEnabled(false);
    //![4]

    //![5]
    plot->chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

    QMainWindow window;
    window.setCentralWidget(plot);
    window.resize(500, 500);
    window.show();

    return app.exec();

}
