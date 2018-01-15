//
//  beerFly.cpp
//  beerDFO
//
//  Created by Edmund Oetgen on 06/01/2018.
//
//


/*THIS CLASS IS BASED ON THE JAVASCRIPT DFO IMPLEMENTATION BY MOHAMMA MAJID AL-RIFAIE WITH ADDED FEATURES AND FITNESS FUNCTION*/

#include "beerFly.hpp"

//same as normal just initialsises within right range

void beerFly::init(double dimensions, vector<hops*> hopVec, vector<fermentables*> fermVec, vector<yeast*> yeastVec,  vector<double> ranges){



    featVec.clear();
    featVec.resize(dimensions);
    
       for (int i = 0; i < dimensions; i++ ){
        
           double range = ranges[i];
        featVec[i] = ofRandom(range);
    }

}

