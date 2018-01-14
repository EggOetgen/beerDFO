//
//  DFO.cpp
//  DFO
//
//  Created by Edmund Oetgen on 12/10/2017.
//
//

#include "DFO.hpp"

DFO::DFO(){
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    scaleF = 5;
    //    dimensions = dimensions_;
    //    popSize = popSize_;
    dt = 0.001;
    
    
    evalCount = 0;
    FE_allowed = 30000;
    offset = -0.0;
    
    
}

void DFO::setup(double dimensions_, int popSize_,double targetOG_, double targetFG_,double targetABV_, double targetIBU_, double targetCOl_, vector<hops*> hopVec_, vector<fermentables*> fermVec_, vector<yeast*> yeastVec_, float efficiency, float batchSize_){
    dimensions = dimensions_;
    popSize = popSize_;
    
//    targetOG = targetOG_;
//    targetFG = targetFG_;
//
//    targetIBU = targetIBU_;
//    targetCOL = targetCOl_;
    
    target.push_back(targetOG_);
    target.push_back(targetFG_);
    target.push_back(targetABV_);
    target.push_back(targetIBU_);
    target.push_back(targetCOl_);

    
    hopVec = hopVec_;
    fermVec = fermVec_;
    yeastVec = yeastVec_;
    
    batchSize = batchSize_;
    //imgW = 100;
    imgH =  dimensions;
    
    eff = efficiency;
    ranges.clear();
    // ranges.resize(dimensions);
    
    for(int i = 0; i < hopVec.size(); i++){
        ranges.push_back(hopVec[i]->amountRange);
    }
    for(int i = 0; i < fermVec.size(); i++){
        ranges.push_back(fermVec[i]->amountRange);
        
    }
    for(int i = 0; i < yeastVec.size(); i++){
        ranges.push_back(yeastVec[i]->amountRange);
        cout<<yeastVec[i]->amountRange<<endl;
    }
    

    flies.clear();
    flies.resize(popSize);
    for (int i = 0; i < flies.size(); i++ ){
        flies[i] = new beerFly();
        flies[i]->init(dimensions, hopVec,  fermVec, yeastVec, ranges);
    }
}


void DFO::getRandF_or_RingT_Neighbours(int curr, Boolean topology) {
    
    if (topology) // RING
    {
        leftNeighbor = curr - 1;
        rightNeighbor = curr + 1;
        
        if (curr == 0)
            leftNeighbor = popSize - 1;
        if (curr == popSize - 1)
            rightNeighbor = 0;
    }
    
    
}


//taken from Mohammad Majid al-Rifaie's example code in java
void DFO::findNeighbors(int curr){
    double minDL = 10E15;
    for (int i = 0; i < popSize; i++) {
        if (i == curr)
            continue;
        
        double d = flies[curr]->getDistanceSquared(flies[i]->returnFeatureVector());
        if (d < minDL) {
            minDL = d;
            leftNeighbor = i;
        }
    }
    
    double minDR = 10E15;
    for (int i = 0; i < popSize; i++) {
        if (i == curr)
            continue;
        if (i == leftNeighbor)
            continue;
        
        double d = flies[curr]->getDistanceSquared(flies[i]->returnFeatureVector());
        if (d < minDR) {
            minDR = d;
            rightNeighbor = i;
        }
    }
    
    // println( leftN + " :: " + rightN );
    
}

//double DFO::evaluate(vector<double> a, vector<hops*> hopVec, vector<fermentables*> fermVec, vector<yeast*> yeastVec, float batchSize) {
//   // evalCount++;
//   // return abs( Sphere(a) );
//  //  double ibu = form.calcIBU(hopVec, fermVec, yeastVec);
//   
//    for (int i = 0; i < hopVec.size(); i ++ ){
//    
//        hopVec[i]->amount = a[i];
//       // cout<<i<< " h"<<endl;
//    }
//    for (int i = hopVec.size(); i < fermVec.size()+hopVec.size(); i ++ ){
//       // cout<<i<<" f"<<endl;
//        fermVec[i-hopVec.size()]->amount = a[i];
//    }
//    for (int i = fermVec.size()+hopVec.size(); i < yeastVec.size()+hopVec.size()+fermVec.size(); i ++ ){
//        
//        yeastVec[i-(fermVec.size()+hopVec.size())]->amount = a[i];
//       // cout<<i<<" y"<<endl;
//    }
//    form.calcOgFg(eff, yeastVec, fermVec, batchSize);
//   
//    double og = form.og;
//    double fg = form.fg;
//    double ibu = form.calcIBU(hopVec, fermVec, batchSize);
//    double color = form.recalcColor_srm(fermVec, batchSize);
//    
//    double ogF   = abs(targetOG- og);
//    //ibuF *= 0.0001;
//    double fgF = abs(targetFG- fg);
//    double ibuF   = abs(targetIBU- ibu);
//    //ibuF *= 0.0001;
//    double colorF = abs(targetCOL- color);
//    
//    //cout << ibuF << " " << colorF << endl;
//
//    return ogF + fgF + ibuF + colorF;
//   // double abv = form.reca;
//    // Sphere Schwefel12 Rosenbrock GSchwefel26
//    // Rastrigin Ackley Griewank PenalizedP8 PenalizedP16
//}


double DFO::evaluate(vector<double> a) {
    // evalCount++;
    // return abs( Sphere(a) );
    //  double ibu = form.calcIBU(hopVec, fermVec, yeastVec);
    
    for (int i = 0; i < hopVec.size(); i ++ ){
        
        hopVec[i]->amount = a[i];
        // cout<<i<< " h"<<endl;
    }
    for (int i = hopVec.size(); i < fermVec.size()+hopVec.size(); i ++ ){
        // cout<<i<<" f"<<endl;
        fermVec[i-hopVec.size()]->amount = a[i];
    }
    for (int i = fermVec.size()+hopVec.size(); i < yeastVec.size()+hopVec.size()+fermVec.size(); i ++ ){
        
        yeastVec[i-(fermVec.size()+hopVec.size())]->amount = a[i];
        // cout<<i<<" y"<<endl;
    }
    form.calcOgFg(eff, yeastVec, fermVec, batchSize);
    vector<double> solution;
    double og = form.og;
    double fg = form.fg;
    double abv = form.recalcABV();
    double ibu = form.calcIBU(hopVec, fermVec, batchSize);
    double color = form.recalcColor_srm(fermVec, batchSize);
    
     solution.push_back(og);
     solution.push_back(fg);
     solution.push_back(abv);
     solution.push_back(ibu);
     solution.push_back(color);
//    double ogF   = abs(targetOG- og);
//    //ibuF *= 0.0001;
//    double fgF = abs(targetFG- fg);
//    double ibuF   = abs(targetIBU- ibu);
//    //ibuF *= 0.0001;
//    double colorF = abs(targetCOL- color);
    
    double fitness = euclid(target, solution);
    //cout << ibuF << " " << colorF << endl;
    
    return fitness;
    // double abv = form.reca;
    // Sphere Schwefel12 Rosenbrock GSchwefel26
    // Rastrigin Ackley Griewank PenalizedP8 PenalizedP16
}

void DFO::findBestFly() {
    double min = 10E10;
    
    for (int i = 0; i < popSize; i++) {
        if (flies[i]->getFitness() < min) {
            min = flies[i]->getFitness();
            bestFlyIndex = i;
        }
    }
}

//void DFO::run(vector<hops*> hopVec, vector<fermentables*> fermVec, vector<yeast*> yeastVec, float batchSize){
//    
//    if (evalCount < FE_allowed) {
//        // EVALUATION Phase
//        for (int i = 0; i < popSize; i++)
//        {
//            flies[i]->setFitness(evaluate(flies[i]->returnFeatureVector(), hopVec, fermVec, yeastVec, batchSize));
//        }
//        
//        findBestFly();
//        
//        // INTERACTION Phase
//        for (int i = 0; i < popSize; i++) {
//            if (i == bestFlyIndex)
//                continue;
//            
//           // findNeighbors(i);
//            // neighbours
//            getRandF_or_RingT_Neighbours(i, true);
//            double leftP, rightP;
//            if (true) {
//                leftP = flies[leftNeighbor]->getFitness();
//                rightP = flies[rightNeighbor]->getFitness();
//            }
//            
//            
//            int chosen;
//            if (leftP < rightP)
//                chosen = leftNeighbor;
//            else
//                chosen = rightNeighbor;
//            
//            int dCounter = 0;
//            // ================ Apply the update equation
//            // =======================
//            vector<double> temp;
//            temp.resize(dimensions);
//            
//            for (int d = 0; d < dimensions; d++) {
//                
//                double range = flies[i]->ranges[d];
//              //  cout<<range<<endl;
//                temp[d] = flies[chosen]->getFeature(d) +
//                ofRandom(1.0) * (flies[bestFlyIndex]->getFeature(d) - flies[i]->getFeature(d));// FINAL
//                
//                            // disturbance mechanism
//             
//                
//                    if (ofRandom(1.0) < dt)
//                    {
//                        temp[d] =ofRandom(0, range);
//                  //  temp[d] =ofRandom(-imgW,imgW);
//                     //   dCounter++;
//                    }
//                //cout<<temp[d] <<" "<< "A"<<endl;
//
//              temp[d] =   wrapAround( range, temp[d]);
//
//              //   cout<<temp[d] <<" "<< "B"<<endl;
//            }
//            
//            flies[i]->setFeatureVector(temp);
//            
//        }
//        
//    }
//}
//
//
void DFO::run(){
    
    
    if (evalCount < FE_allowed) {
        // EVALUATION Phase
        for (int i = 0; i < popSize; i++)
        {
            flies[i]->setFitness(evaluate(flies[i]->returnFeatureVector()));
            
        }
        
        findBestFly();
        
        // INTERACTION Phase
        for (int i = 0; i < popSize; i++) {
            if (i == bestFlyIndex)
                        continue;

            //  findNeighbors(i);
            getRandF_or_RingT_Neighbours(i, true);
            double leftP, rightP;
            
            leftP = flies[leftNeighbor]->getFitness();// leftD *
            // fly[leftN].getFitness();
            rightP = flies[rightNeighbor]->getFitness();// rightD *
            // fly[rightN].getFitness();
            
            
            int chosen;
            if (leftP < rightP)
                chosen = leftNeighbor;
            else
                chosen = rightNeighbor;
            int dCounter = 0;
            vector<double> temp;
            temp.resize(dimensions);
            
            for (int d = 0; d < dimensions; d++) {
                double range =ranges[d];

             // temp[d] = flies[i]->getFeature(d) + ofRandom(1.0) *  (flies[chosen]->getFeature(d) - flies[i]->getFeature(d));// FINAL
                
               temp[d] = flies[chosen]->getFeature(d) + (ofRandom(1.0) * (flies[bestFlyIndex]->getFeature(d) - flies[i]->getFeature(d)));
             
                if (ofRandom(1.0) < dt)
                                        {
                                            temp[d] =ofRandom(0.0, range);
                                      
                                        }
                                    //cout<<temp[d] <<" "<< "A"<<endl;
                    
                                  temp[d] =   wrapAround( range, temp[d]);
                    
                                  //   cout<<temp[d] <<" "<< "B"<<endl;
                                }
                                
                                flies[i]->setFeatureVector(temp);
                                

            
        }
        
    }
}
double DFO::wrapAround(double range, double input) {

    
        double result = input;
       // double range = flies[i]->ranges[j];
   // cout<<range<<endl;
        if ( input > range )
            result = fmod(input, range);
        else if ( input < 0.00 )
            result = range+input;
        
        return result;
    

}
double DFO::Sphere(vector<double> p) {
    double a = 0;
    for (int i = 0; i < dimensions; i++) {
        a = a + pow(p[i] + offset, 2);
    }
    
    return a;
}

void DFO::display(){
    
    // Draw the flies and update their positions
    for (int i = 0; i < popSize; i++) {
        ofSetColor(255, 255, 255);
        
        
        if (true) {
            ofSetColor(200, 200, 200, 5);
            ofSetLineWidth((float) 10.5);
            ofLine(width / 2, 0, width / 2, height); // y
            ofPushMatrix();
            ofTranslate(100, 200);
            
            int gap = height / (dimensions + 1);
            for (int d = 0; d < dimensions; d++) {
                ofSetColor(0, 0, 0, 100);
                ofSetLineWidth((float) 0.3);
                // Horizontal Lines
                int xGap = imgW * scaleF / 2;
                int yGap = -imgW * scaleF / 2 + gap * (d + 1);
                
                ofLine(0, yGap, 600, yGap);
                
                // flies position
                int eSize = 5;
                if (i == bestFlyIndex) {
                    ofSetColor(255, 0, 0);
                    eSize = eSize * 2;// 5;
                }else{
                    ofSetColor(0);
                }
                float x = ofMap(flies[i]->getFeature(d), 0.00, ranges[d], 0,600);
                ofDrawEllipse(x, yGap, eSize, eSize);
                
                // }
            }
            
            ofPopMatrix();
            
        }
        
    }
    
    
}

double DFO::euclid(vector<double> a, vector<double> b){

    double runTot = 0;
    for (int i = 0; i < a.size(); i++){
    
        runTot += ((a[i] - b[i])*(a[i] - b[i]));
    }
    
    double result = sqrt(runTot);
    return result;
}


void DFO::calcBestResults(){
    
    vector<double> a = flies[bestFlyIndex]->featVec;
    for (int i = 0; i < hopVec.size(); i ++ ){
        
        hopVec[i]->amount = a[i];
        // cout<<i<< " h"<<endl;
    }
    for (int i = hopVec.size(); i < fermVec.size()+hopVec.size(); i ++ ){
        // cout<<i<<" f"<<endl;
        fermVec[i-hopVec.size()]->amount = a[i];
    }
    for (int i = fermVec.size()+hopVec.size(); i < yeastVec.size()+hopVec.size()+fermVec.size(); i ++ ){
        
        yeastVec[i-(fermVec.size()+hopVec.size())]->amount = a[i];
        // cout<<i<<" y"<<endl;
    }
    form.calcOgFg(eff, yeastVec, fermVec, batchSize);
    
    bestOG = form.og;
    bestFG = form.fg;
    bestABV = form.recalcABV();
    bestIBU = form.calcIBU(hopVec, fermVec, batchSize);
    bestCOL = form.recalcColor_srm(fermVec, batchSize);
}

double DFO::returnBestOG(){
   calcBestResults();
    
    return bestOG;
}

double DFO::returnBestFG(){
    
    
    return bestFG;
}


double DFO::returnBestABV(){
    
    
    return bestABV;
}

double DFO::returnBestIBU(){
    
    
    return bestIBU;
}

double DFO::returnBestCOL(){
    
    
    return bestCOL;
}





