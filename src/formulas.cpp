//
//  formulas.cpp
//  beerDFO
//
//  Created by Edmund Oetgen on 05/01/2018.
//
//

/* all of this based on code from brew target!!! brewtarget.org
 
 only really changed to work with c++ and simplify
 */

#include "formulas.hpp"

//from brewtarget
Polynomial formulas::platoFromSG_20C20C(
                                          Polynomial() << -616.868 << 1111.14 << -630.272 << 135.997
                                          );
//from brewtarget
double formulas::calcIBU(vector<hops*> hopVec, vector<fermentables*> fermVec, float batchSize)
{
    unsigned int i;
    double ibus = 0.0;
    double tmp = 0.0;
    // Bitterness due to hops...
  //  _ibus.clear();
  //  vector<hop*> hhops = hops();
    for( i = 0; i < hopVec.size(); ++i )
    {
        tmp = ibuFromHop(hopVec[i], batchSize);
       // _ibus.append(tmp);
        ibus += tmp;
    }
    
    // Bitterness due to hopped extracts...
  //  fermVec
    for( i = 0; i< fermVec.size(); ++i )
    {
        // Conversion factor for lb/gal to kg/l = 8.34538.
        ibus +=
        fermVec[i]->ibu_gal_per_lb *
        (fermVec[i]->amount / batchSize) / 8.34538;
    }
       return ibus;
}

//from brewtarget and simplified a bit
double formulas::ibuFromHop(hops const* hop, float batchSize)
{
    double ibus = 0.0;

    if( hop->amount == 0 ){
        return 0.0;
        

    }
    
    double AArating = hop->alpha/100.0;
    double grams = hop->amount*1000.0;
    double minutes = hop->time;

    double hopUtilization = 1.0;
    // Assume 60 min boil until further notice
    int boilTime = 60;
    
    // NOTE: we used to carefully calculate the average boil gravity and use it in the
    // IBU calculations. However, due to John Palmer
    // (http://homebrew.stackexchange.com/questions/7343/does-wort-gravity-affect-hop-utilization),
    // it seems more appropriate to just use the OG directly, since it is the total
    // amount of break material that truly affects the IBUs.
    
   
    

//taken from algoritms
    ibus = ((AArating * grams * 1000) / batchSize/*finalvolumenoloss*/) * ((1.0 - exp(-0.04 * minutes))/4.15) * (1.65 * pow(0.000125, (og - 1)));
     // Adjust for hop form. Tinseth's table was created from whole cone data,
    // and it seems other formulae are optimized that way as well. So, the
    // utilization is considered unadjusted for whole cones, and adjusted
    // up for plugs and pellets.
    //
    // - http://www.realbeer.com/hops/FAQ.html
    // - https://groups.google.com/forum/#!topic/brewtarget-help/mv2qvWBC4sU
           hopUtilization *= 1.10;

    
    // Adjust for hop utilization. 
    ibus *= hopUtilization;
    
    return ibus;
}

void formulas::calcOgFg(float efficiency, vector<yeast*> yeastVec, vector<fermentables*> fermVec, float batchSize)
{
    unsigned int i;
    double plato;

    double kettleWort_l = 0.0;
    double postBoilWort_l = 0.0;
    double ratio = 0.0;
    double ferm_kg = 0.0;
    double attenuation_pct = 0.0;
    double tmp_og, tmp_fg, tmp_pnts, tmp_ferm_pnts, tmp_nonferm_pnts;
    
    _og_fermentable = _fg_fermentable = 0.0;
    
    //from brewtarget
    // Find out how much sugar we have.
     calcTotalPoints(fermVec);
    sugar_kg = sugar_kg * efficiency/100.0 + sugar_kg_ignoreEfficiency;
    plato = getPlato( sugar_kg, batchSize);
    
    //from brewtarget
    tmp_og = PlatoToSG_20C20C( plato );  // og from all sugars
    tmp_pnts = (tmp_og-1)*1000.0;  // points from all sugars
    if ( nonFermentableSugars_kg != 0.0 )
    {
        ferm_kg = sugar_kg - nonFermentableSugars_kg;  // Mass of only fermentable sugars
        plato = getPlato( ferm_kg, batchSize);  // Plato from fermentable sugars
        _og_fermentable = PlatoToSG_20C20C( plato );  // og from only fermentable sugars
        plato = getPlato( nonFermentableSugars_kg, batchSize);  // Plate from non-fermentable sugars
        tmp_nonferm_pnts = ((PlatoToSG_20C20C( plato ))-1)*1000.0;  // og points from non-fermentable sugars
    }
    else
    {
        _og_fermentable = tmp_og;
        tmp_nonferm_pnts = 0;
    }
    //from brewtarget
    // Calculage FG
       for( i = 0; static_cast<int>(i) < yeastVec.size(); ++i )
    {
                // Get the yeast with the greatest attenuation.
        if( yeastVec[i]->attenuation > attenuation_pct )
            attenuation_pct = yeastVec[i]->attenuation;
    }
    //from brewtarget
    if( yeastVec.size() > 0 && attenuation_pct <= 0.0 ) // This means we have yeast, but they neglected to provide attenuation percentages.
        attenuation_pct = 75.0; // 75% is an average attenuation.
    
    //from brewtarget
    if ( nonFermentableSugars_kg != 0.0 )
    {
        tmp_ferm_pnts = (tmp_pnts-tmp_nonferm_pnts) * (1.0 - attenuation_pct/100.0);  // fg points from fermentable sugars
        tmp_pnts = tmp_ferm_pnts + tmp_nonferm_pnts;  // FG points from both fermentable and non-fermentable sugars
        //tmp_pnts *= (1.0 - attenuation_pct/100.0);  // WTF, this completely ignores all the calculations about non-fermentable sugars and just converts everything!
        tmp_fg =  1 + tmp_pnts/1000.0;  // new FG value
        _fg_fermentable =  1 + tmp_ferm_pnts/1000.0;  // FG from fermentables only
    }
    //from brewtarget
    else
    {
        tmp_pnts *= (1.0 - attenuation_pct/100.0);
        tmp_fg =  1 + tmp_pnts/1000.0;
        _fg_fermentable = tmp_fg;
    }
 
    fg = _fg_fermentable;
    og = _og_fermentable;
}

//from brewtarget
void formulas::calcTotalPoints(vector<fermentables*> fermVec)
{
    int i;
    double _sugar_kg_ignoreEfficiency = 0.0;
    double _sugar_kg                  = 0.0;
    double _nonFermentableSugars_kg    = 0.0;
    double _lateAddition_kg           = 0.0;
    double _lateAddition_kg_ignoreEff = 0.0;
    
    fermentables* ferm;
    
   
    
    for( i = 0; i < fermVec.size(); ++i )
    {
        ferm = fermVec[i];
        
        // If we have some sort of non-grain, we have to ignore efficiency.
        double equivSucrose  = ferm->amount * ferm->yield * (1.0-ferm->moisture/100.0) / 100.0;
              if( ferm->type=="Sugar" || ferm->type=="Extract" )
        {
            
            _sugar_kg_ignoreEfficiency += equivSucrose;
            
            if (ferm->addAfterBoil == "TRUE")
                _lateAddition_kg_ignoreEff += equivSucrose;
            

        }
        else
        {
            _sugar_kg += equivSucrose;
            
            if (ferm->addAfterBoil == "TRUE")
                _lateAddition_kg += equivSucrose;
        }
    }
    //from brewtarget, changed to get rid of qList tho
       sugar_kg = _sugar_kg;
    nonFermentableSugars_kg = _nonFermentableSugars_kg;
    sugar_kg_ignoreEfficiency = _sugar_kg_ignoreEfficiency;
    lateAddition_kg = _lateAddition_kg;
    lateAddition_kg_ignoreEff = _lateAddition_kg_ignoreEff;
    
    
    

}
//from brewtarget
    double formulas::getPlato(double sugar_kg,  double wort_l  )
    {
        double water_kg = wort_l - sugar_kg/PhysicalConstants::sucroseDensity_kgL; // Assumes sucrose vol and water vol add to wort vol.
        
        return sugar_kg/(sugar_kg+water_kg) * 100.0;
    }

    
    double formulas::PlatoToSG_20C20C( double plato )
    {
        // Copy the polynomial, cuz we need to alter it.
        Polynomial poly(platoFromSG_20C20C);
        
        // After this, finding the root of the polynomial will be finding the SG.
        poly[0] -= plato;
        
        return poly.rootFind( 1.000, 1.050 );
    }

//from brewtarget
double formulas::recalcColor_srm(vector<fermentables*> fermVec, float batchSize)
{
       double mcu = 0.0;
    double ret;
    unsigned int i;
    
       for( i = 0; static_cast<int>(i) < fermVec.size(); ++i )
    {
       //from brewtarget
        mcu += fermVec[i]->color*8.34538 * fermVec[i]->amount/batchSize;
    }
    
    ret = 1.4922 * pow( mcu, 0.6859 );
    
    return ret;
}

double formulas::calcIBUGU(double ibu){

    double out;
    out = ibu/((og-1)*1000);
    return out;
    
}

//from brewtarget

double formulas::recalcABV()
{
    double temp;
    //from brewtarget and...
    // The complex formula, and variations comes from Ritchie Products Ltd, (Zymurgy, Summer 1995, vol. 18, no. 2)
    // Michael L. Hall’s article Brew by the Numbers: Add Up What’s in Your Beer, and Designing Great Beers by Daniels.
    temp = (76.08 * (og - fg) / (1.775 - og)) * (fg/ 0.794);
    
    return temp;
}

