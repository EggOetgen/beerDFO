//
//  beerFly.hpp
//  beerDFO
//
//  Created by Edmund Oetgen on 06/01/2018.
//
//



/*THIS CLASS IS BASED ON THE JAVASCRIPT DFO IMPLEMENTATION BY MOHAMMA MAJID AL-RIFAIE WITH ADDED FEATURES AND FITNESS FUNCTION*/
#pragma once
#include "fly.hpp"
#include "ingredients.hpp"

class beerFly:public fly{

public:
    void init(double dimensions,vector<hops*> hopVec, vector<fermentables*> fermVec, vector<yeast*> yeastVec,  vector<double> ranges);
    
};
