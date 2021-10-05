
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class TimeSeries{
    map<string, vector<float>> data;
    vector<string> vectorOfkeys;

public:
	TimeSeries(const char* CSVfileName);
    TimeSeries(const TimeSeries &ts);
    map<string, vector<float>> getMap() const;
    vector<string> getKeys() const;
    vector<float> getValuesInVector(string key);
    float* getValuesInArray(vector<float> vectorOfValues, int size);

};



#endif /* TIMESERIES_H_ */
