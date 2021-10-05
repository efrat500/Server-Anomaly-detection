#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	Point* p_center;
	bool isMinCircle;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	float threshold = 0.9;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	virtual Point** createArrayOpP(float* x ,float* y, int size);
	virtual void deleteArrayOpP(Point** arrayOfPoints, int size);
	virtual float devMax(Point** arrayOfPoints, int size, Line line);
	virtual void fullCf(Line line,int size,float p,string f1, string f2,Point** ps);
	virtual bool isAnomalous(float x, float y,correlatedFeatures c);
	virtual float getThres();
	virtual void setThresold(float thresh);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
protected:
	vector<correlatedFeatures> cf;
};


#endif /* SIMPLEANOMALYDETECTOR_H_ */

