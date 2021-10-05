

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
protected:
	virtual void fullCf(Line line,int size,float p,string f1, string f2,Point** ps) override;
	virtual bool isAnomalous(float x, float y,correlatedFeatures c) override;

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
