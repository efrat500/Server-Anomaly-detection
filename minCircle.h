// 207013491 Efrat Naaman 206475907 Shahar Harel

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

// implement
Circle circleFrom3P(Point A, Point B,Point C);
Circle circleForm2P(Point point1, Point point2);
Circle minCircle1(Point** points, vector<Point> partOfPoints, size_t size);
Circle findMinCircle(Point** points,size_t size);


#endif /* MINCIRCLE_H_ */
