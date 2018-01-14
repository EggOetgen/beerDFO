//
//  beerFly.cpp
//  beerDFO
//
//  Created by Edmund Oetgen on 06/01/2018.
//
//

#include "beerFly.hpp"


void beerFly::init(double dimensions, vector<hops*> hopVec, vector<fermentables*> fermVec, vector<yeast*> yeastVec,  vector<double> ranges){

//    ranges.clear();
//   // ranges.resize(dimensions);
//    
//    for(int i = 0; i < hopVec.size(); i++){
//        ranges.push_back(hopVec[i]->amountRange);
//            }
//    for(int i = 0; i < fermVec.size(); i++){
//        ranges.push_back(fermVec[i]->amountRange);
//
//    }
//    for(int i = 0; i < yeastVec.size(); i++){
//        ranges.push_back(yeastVec[i]->amountRange);
//                cout<<yeastVec[i]->amountRange<<endl;
//           }
    
   // cout<<ranges.size() << " " << dimensions << endl;

    featVec.clear();
    featVec.resize(dimensions);
    
       for (int i = 0; i < dimensions; i++ ){
        
           double range = ranges[i];
          // cout<< range << endl;

      //  randAmount = ofRandom()
        featVec[i] = ofRandom(range);
    }

}

//void fly::setVals(double t, float i, float c) {
//    
//    ibu = i
//    col = c;
//}