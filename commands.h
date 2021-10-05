
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <sstream>
#include <vector>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};
class DataCommands{
	HybridAnomalyDetector* hybrid_anomaly;
	public:
		vector <AnomalyReport> anomaly_report;
		int num_of_row;
		DataCommands(){this->hybrid_anomaly = new HybridAnomalyDetector();}
		HybridAnomalyDetector* getHybridAnomaly(){return this->hybrid_anomaly; }
		float getThresold(){ return this->hybrid_anomaly->getThres(); } 
		void setThresold(float thresh){this->hybrid_anomaly->setThresold(thresh); } 
		~DataCommands(){};

};
class Command{
	protected:
		DefaultIO* dio;
		DataCommands* data;
		string descript;
	public:
		Command(DefaultIO* dio, DataCommands* data):dio(dio), data(data){}
		virtual void getDescript(){ dio->write(descript + "\n");}
		virtual void execute()=0;
		virtual ~Command(){};
};
class UploadCommend:public Command{
	public:
		UploadCommend(DefaultIO* dio, DataCommands* data):Command(dio,data){
			this->descript = "1.upload a time series csv file";
		}
		void execute(){
			creatFile("Please upload your local train CSV file.","anomalyTrain.csv");
			creatFile("Please upload your local test CSV file.","anomalyTest.csv");
		}
		void creatFile(string str, string csv){
			ofstream file;
			string lineInput = "";
			if (csv.compare("anomalyTrain.csv") == 0){
				file.open("anomalyTrain.csv");
			}
			else{
				file.open("anomalyTest.csv");
			}
			dio->write(str + "\n");
			while(lineInput != "done\n"){
				lineInput = dio->read() + '\n';
				if (lineInput == "done\n"){
					break;
				}
				file << lineInput;
			}
			dio->write("Upload complete.\n");
			file.close();
		}
};
class algorithmsSettings:public Command{
	public:
		algorithmsSettings(DefaultIO* dio, DataCommands* data):Command(dio,data){
			this->descript = "2.algorithm settings";
		}
		void execute(){
			float current_threshold = this->data->getThresold();
			current_threshold =  floor(current_threshold * 10000) / 10000;
			std::ostringstream ss;
			ss << current_threshold;
			std::string threshold_in_string(ss.str());
			dio->write("The current correlation threshold is " + threshold_in_string + "\n");
			dio->write("Type a new threshold\n");
			string change_threshold = dio->read();
			while(!((stof(change_threshold) > 0) && (stof(change_threshold) <= 1))){
				dio->write("please choose a value between 0 and 1.\n");
				change_threshold = dio->read();
			}
			this->data->setThresold(stof(change_threshold));
		}
};
class detectAnomalies:public Command{
	public:
		detectAnomalies(DefaultIO* dio, DataCommands* data):Command(dio,data){
			this->descript = "3.detect anomalies";
		}
		void execute(){
			vector <AnomalyReport> v_train, v_test, v_report;
			TimeSeries anomalyTrain("anomalyTrain.csv");
			this->data->getHybridAnomaly()->learnNormal(anomalyTrain);
			TimeSeries anomalyTest("anomalyTest.csv");
			v_test = this->data->getHybridAnomaly()->detect(anomalyTest);
			for(int i = 0; i < v_test.size(); i++){
				this->data->anomaly_report.push_back(v_test[i]);
			}
			this->data->num_of_row = anomalyTest.getValuesInVector(anomalyTest.getKeys()[0]).size();
			dio->write("anomaly detection complete.\n");
		}
};
class displayResults:public Command{
	public:
		displayResults(DefaultIO* dio, DataCommands* data):Command(dio,data){
			this->descript = "4.display results";
		}
		void execute(){
			vector <AnomalyReport> v_report = this->data->anomaly_report;
			for(int i = 0; i < v_report.size(); i++){
				dio->write(v_report[i].timeStep);
				dio->write("	 ");
				dio->write(v_report[i].description+ "\n");
			}
			dio->write("Done.\n");	
		}
};
class UplodAnomalies:public Command{
	public:
		UplodAnomalies(DefaultIO* dio, DataCommands* data):Command(dio,data){
			this->descript = "5.upload anomalies and analyze results";
		}
		void execute(){
			vector<pair<int,int>> v_anomalies_input;
			vector<int> temp_v;
			int num=0;
			string substr;
			this->dio->write("Please upload your local anomalies file.\n");
			string lineInput = this->dio->read();
			while(lineInput != "done") {
				string first, second;
				stringstream line(lineInput);
				while (getline(line, substr, ',')){
					temp_v.push_back(stoi(substr));
				}
				v_anomalies_input.push_back({temp_v[num], temp_v[num+1]});
				num = num+2;
				lineInput = dio->read();
			}
			this->dio->write("Upload complete.\n");
			vector<pair<int,int>> v_ts_anomaly;
			pair<int, int> pair_of_anomaly;
			int first_match,last_match;
			for (int i = 1; i < this->data->anomaly_report.size(); i++) {
				first_match = i-1;
				last_match = first_match;
				if((this->data->anomaly_report[i-1].description == this->data->anomaly_report[i].description) && 
				((this->data->anomaly_report[i].timeStep) == this->data->anomaly_report[i-1].timeStep) + 1){
					while((this->data->anomaly_report[i-1].description == this->data->anomaly_report[i].description) && 
					((this->data->anomaly_report[i].timeStep) == this->data->anomaly_report[i-1].timeStep) + 1) {
						last_match++;
						if (i++ == this->data->anomaly_report.size()) {
							break;
						}
					}
				}
				pair_of_anomaly.first = this->data->anomaly_report[first_match].timeStep;
				pair_of_anomaly.second = this->data->anomaly_report[last_match].timeStep;
				v_ts_anomaly.push_back(pair_of_anomaly);
			}
			int TP = 0;
			// run in loop and keep just the  true positive rate
			for(int i = 0; i < v_ts_anomaly.size(); i++) {
				for(int j = 0; j < v_anomalies_input.size(); j++) {
					if ((v_ts_anomaly[i].first <= v_anomalies_input[j].first && v_ts_anomaly[i].second >= v_anomalies_input[j].first)
					|| (v_anomalies_input[j].first <= v_ts_anomaly[i].first  && v_anomalies_input[j].second >= v_ts_anomaly[i].first)) {
						TP++;
					}
					continue;
				}
			}
			std::ostringstream ss_tpr;	
			// calculate the true positive rate, keep it with 3 number after the point 
			float true_postiv_rate = (float)TP / (float) v_anomalies_input.size();
			true_postiv_rate = floor(true_postiv_rate * 1000) / 1000;
			ss_tpr <<  true_postiv_rate;
			std::string string_of_tpr(ss_tpr.str());
			this->dio->write("True Positive Rate: " + string_of_tpr + "\n");
			// calculate the time of all the anomalies that was in the input
			int sum_timesteps_anomalies = 0;
			for (int i = 0; i < v_anomalies_input.size(); i++) {
				sum_timesteps_anomalies += (v_anomalies_input[i].second - v_anomalies_input[i].first) + 1;
			}
			// calculate the false alarm rate, keep it with 3 number after the point
			int FP = v_ts_anomaly.size() - TP;
			// N represent num of lines that were not write as an anomaly in the input we got
			int N = this->data->num_of_row - sum_timesteps_anomalies;
			std::ostringstream ss_far;
			float false_alarm_rate =  (float)FP / (float)N;
			false_alarm_rate = floor(false_alarm_rate * 1000) / 1000;
			ss_far <<  false_alarm_rate;
			std::string string_of_far(ss_far.str());
			this->dio->write("False Positive Rate: " + string_of_far + "\n");	
		}
};
class Exit:public Command{
	public:
		Exit(DefaultIO* dio, DataCommands* data):Command(dio,data){
			this->descript = "6.exit";
		}
		void execute(){free(this->data->getHybridAnomaly());}
	};

#endif /* COMMANDS_H_ */