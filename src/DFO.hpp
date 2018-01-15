//
//  DFO.hpp
//  DFO
//
//  Created by Edmund Oetgen on 12/10/2017.
//
//

#pragma once

/*THIS CLASS IS BASED ON THE JAVASCRIPT DFO IMPLEMENTATION BY MOHAMMA MAJID AL-RIFAIE WITH ADDED FEATURES AND FITNESS FUNCTION*/

#include "ofMain.h"
#include "beerFly.hpp"
#include "formulas.hpp"
#include "ingredients.hpp"

class DFO{
public:
    DFO();
    void setup(double dimensions_, int popSize_,double targetOG_, double targetFG_, double targetABV_, double targetIBU_, double targetCOl_, vector<hops*> hopVec_, vector<fermentables*> fermVec_, vector<yeast*> yeastVec_, float efficiency, float batchSize_);

    void findNeighbors(int curr);
    double returnFeature(int f);
    double evaluate (vector<double> a);
    void findBestFly();
    void getRandF_or_RingT_Neighbours(int curr, Boolean topology);

    void updateTarget(double newOF, double newFG,double newABV,double newIBU, double newCOL);
    void run();
    void display();
    
    double wrapAround( double range,double input);
   
    double euclid(vector<double> a, vector<double> b);
    
    void calcBestResults();
    double returnBestOG();
    double returnBestFG();
    double returnBestABV();
    double returnBestIBU();
    double returnBestCOL();
    
    float batchSize;
    
    vector<hops*> hopVec;
    vector<fermentables*> fermVec;
    vector<yeast*> yeastVec;
    vector<double> ranges;
    double dimensions;
    int popSize;
    float dt; //disturbaance threshold
    
    int imgW, imgH;
    float width, height;
    int scaleF;
    
    vector<beerFly *> flies;
    int leftNeighbor;
    int rightNeighbor;
    int bestFlyIndex;
    
    int evalCount;
    int FE_allowed;
    double offset;
    
    double targetABV;
    double targetIBU;
    double targetCOL;
    double targetOG;
    double targetFG;
    
    double bestABV;
    double bestIBU;
    double bestCOL;
    double bestOG;
    double bestFG;
    
    
    vector<double> target;
    float eff;
    
    formulas form;
};