#include <iostream>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include "minCircle.h"
#include "anomaly_detection_util.h"

Circle findMinCircle(Point** points,size_t size){
    vector<Point> partOfPoints;
    return minCircle1(points, partOfPoints, size); 
}

Circle circleForm3P(Point p1,Point p2, Point p3) {  
    double xOfPoint = ((p3.y - p1.y) * (pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2)) - 
    (p2.y - p1.y)* (pow((p3.x - p1.x),2) + pow((p3.y - p1.y),2))) / 
    (2.0 * (((p2.x - p1.x) * (p3.y - p1.y)) - ((p2.y - p1.y) * (p3.x - p1.x))));
    double yOfPoint = ((p2.x - p1.x) * (pow((p3.x - p1.x),2) + pow((p3.y - p1.y),2)) - 
    (p3.x - p1.x) *(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2))) / 
    (2.0 * (((p2.x - p1.x) * (p3.y - p1.y)) - ((p2.y - p1.y) * (p3.x - p1.x))));
    xOfPoint = xOfPoint + p1.x; 
    yOfPoint = yOfPoint + p1.y;
    float distans; 
    distans = sqrt(((xOfPoint - p1.x) * (xOfPoint - p1.x)) + ((yOfPoint - p1.y) * (yOfPoint - p1.y))); 
    return {Point(xOfPoint,yOfPoint), distans }; 
}

Circle circleForm2P(Point point1, Point point2) {  
    Point center = { (point1.x + point2.x) / (float)2,(point1.y + point2.y) / (float)2}; 
    float distans = sqrt(((point2.x - point1.x) * (point2.x - point1.x)) +  ((point2.y - point1.y) * (point2.y - point1.y))); 
    float radius = distans / 2.0;
    return { center, radius}; 
} 

Circle minCircle1(Point** points, vector<Point> partOfPoints, size_t size){
    Circle min_circle();
    if (size == 0) {  
        if (partOfPoints.empty()){ 
            return {Point(0,0) , 0}; 
        } 
        if (partOfPoints.size() == 1) { 
            return {partOfPoints[0], 0}; 
        } 
        if (partOfPoints.size() == 2) { 
            return circleForm2P(partOfPoints[0],partOfPoints[1]);
        } 
    }
    if(partOfPoints.size() == 3){
        return circleForm3P(partOfPoints[0],partOfPoints[1],partOfPoints[2]);
    }
    Point p = *points[size-1]; 
    Circle c = minCircle1(points, partOfPoints, size - 1);
    if((sqrt(((c.center.x - p.x) * (c.center.x - p.x)) + ((c.center.y - p.y) * (c.center.y - p.y)))) <= c.radius){
        return c;
    } 
    partOfPoints.push_back(p); 
    return minCircle1(points, partOfPoints, size - 1); 
}