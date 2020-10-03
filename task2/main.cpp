#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

/*pair<double, double> findClosestPair(vector<pair<double, double>> L, vector<pair<double, double>> R){
}*/

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
int main(int, char**) {
    cout << "Hello, world!\n";
    // generate a random pair of 100 values
    vector<pair<double, double>> pairs;
    double r1, r2;
    for (int i = 0; i < 100; i++) {
        r1 = (double)rand() / (double)RAND_MAX ;
        r2 = (double)rand() / (double)RAND_MAX ;
        pairs.push_back({r1, r2});
    }
    // sort points according to their x-coordinates
    pairs = merge_sort(pairs);
    cout << "Hello, sorted list!\n";

    // find closest point with divide and conquer (O(N log N))

    // plot


}
