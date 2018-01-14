//
//  formulas.hpp
//  beerDFO
//
//  Created by Edmund Oetgen on 05/01/2018.
//
//

#pragma once
#include "ofMain.h"
#include "ingredients.hpp"

#define ROOT_PRECISION 0.000001

namespace PhysicalConstants{
    //! \brief Sucrose density in kg per L.
    const double sucroseDensity_kgL = 1.587;
    //! \brief This estimate for grain density is from my own (Philip G. Lee) experiments.
    const double grainDensity_kgL = 0.963;
    //! \brief Liquid extract density in kg per L.
    const double liquidExtractDensity_kgL = 1.412;
    //! \brief Dry extract density in kg per L.
    const double dryExtractDensity_kgL = sucroseDensity_kgL;
    
    //! \brief How many liters of water get absorbed by 1 kg of grain.
    const double grainAbsorption_Lkg = 1.085;
}


class Polynomial
{
public:
    //! \brief Default constructor
    Polynomial() :
    _coeffs()
    {
    }
    
    //! \brief Copy constructor
    Polynomial( Polynomial const& other ) :
    _coeffs( other._coeffs )
    {
    }
    
    //! \brief Constructs the 0 polynomial with given \c order
    Polynomial( size_t order ) :
    _coeffs(order+1, 0.0)
    {
    }
    
    //! \brief Constructor from an array of coefficients
    Polynomial( double const* coeffs, size_t order ) :
    _coeffs(coeffs, coeffs+order+1)
    {
    }
    
    //! \brief Add a coefficient for x^(\c order() + 1)
    Polynomial& operator<< ( double coeff )
    {
        _coeffs.push_back(coeff);
        return *this;
    }
    
    //! \brief Get the polynomial's order (highest exponent)
    size_t order() const { return _coeffs.size()-1; }
    
    //! \brief Get coefficient of x^n where \c n <= \c order()
    double operator[] (size_t n) const
    {
        assert( n <= _coeffs.size() );
        return _coeffs[n];
    }
    
    //! \brief Get coefficient of x^n where \c n <= \c order() (non-const)
    double& operator[] (size_t n)
    {
        assert( n < _coeffs.size() );
        return _coeffs[n];
    }
    
    //! \brief Evaluate the polynomial at point \c x
    double eval(double x) const
    {
        double ret = 0.0;
        size_t i;
        
        for( i = order(); i > 0; --i )
            ret += _coeffs[i] * intPow( x, i );
        ret += _coeffs[0];
        
        return ret;
    }
    
    


/*!
 * \brief Root-finding by the secant method.
 *
 * \param x0 - one of two initial \b distinct guesses at the root
 * \param x1 - one of two initial \b distinct guesses at the root
 * \returns \c HUGE_VAL on failure, otherwise a root of the polynomial
 */
double rootFind( double x0, double x1 ) 
{
    double guesses[] = { x0, x1 };
    double newGuess = x0;
    double maxAllowableSeparation = abs( x0 - x1 ) * 1e3;
    
    while( abs( guesses[0] - guesses[1] ) > ROOT_PRECISION )
    {
        newGuess = guesses[1] - (guesses[1] - guesses[0]) * eval(guesses[1]) / ( eval(guesses[1]) - eval(guesses[0]) );
        
        guesses[0] = guesses[1];
        guesses[1] = newGuess;
        
        if( abs( guesses[0] - guesses[1] ) > maxAllowableSeparation )
            return HUGE_VAL;
    }
    
    return newGuess;
}

private:
std::vector<double> _coeffs;

//! \brief returns base^pow
static double intPow( double base, unsigned int pow )
{
    double ret = 1;
    for(; pow > 0; pow--)
        ret *= base;
    
    return ret;
}
};

class formulas{

public:
    
    double calcIBU(vector<hops*> hopVec, vector<fermentables*> fermVec,float batchSize);
    double ibuFromHop(hops const* hop, float batchSize);

    void calcOgFg(float efficiency, vector<yeast*> yeastVec, vector<fermentables*> fermVec, float batchSize);
    void calcTotalPoints(vector<fermentables*> fermVec);
    double getPlato( double sugar_kg,  double wort_l  );
    
    double sugar_kg, nonFermentableSugars_kg, sugar_kg_ignoreEfficiency, lateAddition_kg,lateAddition_kg_ignoreEff;
    
    static double PlatoToSG_20C20C( double plato );

    
    double fg, og,  _og_fermentable, _fg_fermentable;
    
    double recalcColor_srm(vector<fermentables*> fermVec, float batchSize);
    
    double calcIBUGU(double ibu);
    double recalcABV();

    
private:
    static Polynomial platoFromSG_20C20C;
};



