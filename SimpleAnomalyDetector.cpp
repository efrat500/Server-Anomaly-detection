#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	this->threshold = 0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

// function that create array of points
Point** SimpleAnomalyDetector::createArrayOpP(float* x ,float* y, int size){
	Point** arrayOfPoints = new Point*[size];
	// create the array from array of x and array of y
	for (int i = 0; i<size; i++) {
		arrayOfPoints[i] = new Point(x[i],y[i]);
	}
	return arrayOfPoints;
}

void SimpleAnomalyDetector::deleteArrayOpP(Point** arrayOfPoints, int size){
	// delete the array
	for (int i = 0; i<size; i++) {
		delete arrayOfPoints[i];
	}
	delete [] arrayOfPoints;
}
// this function return the max dev between the points in array
float SimpleAnomalyDetector::devMax(Point** arrayOfPoints, int size, Line line){
	float max_dev = 0;
	for (int i = 0; i < size; i++) {
		Point p = *(arrayOfPoints[i]);
		if (max_dev < dev(p,line)){
			max_dev = dev(p,line);
		}
	}
	return (max_dev*1.2);

}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	// call cope constructor
	TimeSeries tS(ts);
	//this->threshold = 0.9;
	// the number of the featurae in the map
	int numOfFeaturae = tS.getKeys().size();
	// vector that contain the feature
    vector<string> vectorOfKeys = tS.getKeys();    
	// vectors that contain the values of the feature
	vector<float> v1, v2;
	float optionToCorrlation;
	// run on the map and enter
	for(int i=0; i < numOfFeaturae; i++){
		v1 = tS.getValuesInVector(vectorOfKeys[i]);
		int sizeOfValue = v1.size();
		for(int j= i+1; j<numOfFeaturae; j++){
			v2 = tS.getValuesInVector(vectorOfKeys[j]);
			// convert the vectors to array
			float* x = v1.data();
			float* y = v2.data();
			float optionToCorrlation = pearson(x, y, sizeOfValue);
			Point** points = createArrayOpP(x,y,sizeOfValue);
			Line li = linear_reg(points,sizeOfValue); 
			//check if there is corraltion
			fullCf(li, sizeOfValue , optionToCorrlation, vectorOfKeys[i], vectorOfKeys[j], points);
		}	    
	}
}
void SimpleAnomalyDetector::fullCf(Line line,int size, float optionToCorrlation,string f1, string f2,Point** points){
	if (fabs(optionToCorrlation) >= this->threshold) {
		// create struct and insert all the data
		struct correlatedFeatures correlatedFeature;
		correlatedFeature.feature1 = f1;
		correlatedFeature.feature2 = f2;
		correlatedFeature.corrlation = optionToCorrlation;
		correlatedFeature.lin_reg = line;
		correlatedFeature.threshold = (devMax(points, size,line))*1.09;
		correlatedFeature.isMinCircle = false;
		this->cf.push_back(correlatedFeature);
		deleteArrayOpP(points, size);
	}
}

bool SimpleAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
	return (abs(y - c.lin_reg.f(x))>c.threshold);
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	TimeSeries tS(ts);
	vector<AnomalyReport> vector_of_anomaly;
	// run on the cf
	for(int i=0; i<cf.size(); i++){
		vector<float> v1, v2;
		// create points from the vectors and check if the dev of them is large than the threshold
		for (int j=0; j < tS.getValuesInVector(this->cf[i].feature1).size(); j++){
			float x = tS.getValuesInVector(this->cf[i].feature1)[j];
			float y = tS.getValuesInVector(this->cf[i].feature2)[j];	
			if(isAnomalous(x,y,this->cf[i])) {
				// enter the features to the anomaly vector
				string s = this->cf[i].feature1 + '-' + this->cf[i].feature2;
				long time_step = j+1;
				vector_of_anomaly.push_back(AnomalyReport(s, time_step));
			}
		}
	}
	return vector_of_anomaly;
}

float SimpleAnomalyDetector::getThres(){
	return this->threshold;
}

void SimpleAnomalyDetector::setThresold(float thresh){
	this->threshold = thresh;
}

