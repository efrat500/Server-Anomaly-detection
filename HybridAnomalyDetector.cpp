#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	SimpleAnomalyDetector();
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::fullCf(Line line,int size,float p,string f1, string f2,Point** ps){
	SimpleAnomalyDetector::fullCf(line,size,p,f1,f2,ps);
	if ((p >= 0.5) && (p < 0.9)){
		Circle circle = findMinCircle(ps,size);
		struct correlatedFeatures correlatedFeature;
		correlatedFeature.feature1 = f1;
		correlatedFeature.feature2 = f2;
		correlatedFeature.corrlation = p;
		correlatedFeature.lin_reg = line;
		correlatedFeature.threshold = circle.radius * 1.1;
		correlatedFeature.p_center = new Point(circle.center.x, circle.center.y);
		correlatedFeature.isMinCircle = true;
		this->cf.push_back(correlatedFeature);
		deleteArrayOpP(ps, size);
	}
}

bool HybridAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
	if (c.isMinCircle) {
		float distance = sqrt(pow(c.p_center->x - x, 2) +  pow(c.p_center->y - y, 2));
		return (distance > c.threshold);
	}
	return SimpleAnomalyDetector::isAnomalous(x,y,c);
}
