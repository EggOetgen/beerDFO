//
//  ingredients.hpp
//  beerDFO
//
//  Created by Edmund Oetgen on 05/01/2018.
//
//
#include "ofMain.h"
#pragma once

class ingredients{


public:
    float amount;
    float amountRange;
    

};

class hops:public ingredients{

public:
    float alpha = 0;
    float beta;
    float time;
    float HSI;
    float hum;
    float car;
    float coh;
    float myr;
    
    };

class fermentables:public ingredients{
    
public:
    float yield;
    float color;
    float coarse_fine_dif;
    float moisture;
    float di_pow;
    float protein;
    float most_in_batch;
    float ibu_gal_per_lb;
    string type;
    string addAfterBoil;
    
};

class yeast:public ingredients{
    
public:
    float minTemp;
    float maxTemp;
    float attenuation;
    
};