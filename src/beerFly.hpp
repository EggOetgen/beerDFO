//
//  beerFly.hpp
//  beerDFO
//
//  Created by Edmund Oetgen on 06/01/2018.
//
//

#pragma once
#include "fly.hpp"
#include "ingredients.hpp"

class beerFly:public fly{

public:
    void init(double dimensions,vector<hops*> hopVec, vector<fermentables*> fermVec, vector<yeast*> yeastVec,  vector<double> ranges);
    //vector<double> ranges;
    double ibu, col;
    //void setVals(float i, float c);
};
