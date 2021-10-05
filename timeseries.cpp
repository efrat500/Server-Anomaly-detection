#include "timeseries.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

 TimeSeries::TimeSeries(const char* CSVfileName) {
    // open the file
    ifstream CSVfile(CSVfileName);
    string line; 
    getline(CSVfile,line);
    stringstream sLine(line);
    // create vector to enter the first line, run on the first line
    while (getline(sLine, line, ',')) {
       vectorOfkeys.push_back(line);
    }
    // array of vectors in size of the vector of the first line because this is will be the num of the columnד
    vector<float> vOfLine[vectorOfkeys.size()];
    // continue to run until the end on the file
    while (getline(CSVfile,line)) {
       stringstream sLine(line);
       for (int i = 0; i < vectorOfkeys.size(); i++){
          float value = 0;
          getline(sLine, line, ',');
          value = stof(line);
          // enter what write in the line to the vedtor of the line
          vOfLine[i].push_back(value);
       }
    }
    // enter the keys and the values to the map
    for (int i=0; i<vectorOfkeys.size(); i++){
          data.insert({vectorOfkeys[i],vOfLine[i]});  
    }
 }

TimeSeries::TimeSeries(const TimeSeries &ts) {
   data = ts.data;
   vectorOfkeys = ts.vectorOfkeys;
}
map<string, vector<float>> TimeSeries::getMap() const{
   return data;
}

vector<string> TimeSeries::getKeys() const{
   return vectorOfkeys;
}

vector<float> TimeSeries::getValuesInVector(string key){
   vector<float> v;
   for (std::map<string, vector<float>>::iterator it = data.begin(); it != data.end(); it++){
      if (it->first.compare(key) == 0) {
          return it->second;
      }
   }
   return v;
}


