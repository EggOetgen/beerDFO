//
//  fly.cpp
//  DFO
//
//  Created by Edmund Oetgen on 10/10/2017.
//
//

#include "fly.hpp"

void fly::init(double dimensions, float w){
    
    
    featVec.clear();
    featVec.resize(dimensions);
    for (int i = 0; i < featVec.size(); i++ ){
        featVec[i] = ofRandom(-w/2, w/2);
    }
}

vector<double> fly::returnFeatureVector(){
    
    return featVec;
}

void fly::setFeatureVector(vector<double> newFeatVec){
    
    featVec.clear();
    featVec = newFeatVec;
}

double fly::getFeature(int f){
    
    return featVec[f];
    
}

double fly::getDistanceSquared(   vector<double> featVec2){
    double squaredSum = 0;
    for (int d = 0; d < featVec.size(); d++) {
        squaredSum += (featVec[d]*featVec[d])+ (featVec2[d]*featVec2[d]);
    }
    
    return squaredSum;
}

void fly::setFitness(double t) {
    fitness = t;
}

double fly::getFitness() {
    return fitness;
}