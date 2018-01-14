//
//  formulas.cpp
//  beerDFO
//
//  Created by Edmund Oetgen on 05/01/2018.
//
//

#include "formulas.hpp"


Polynomial formulas::platoFromSG_20C20C(
                                          Polynomial() << -616.868 << 1111.14 << -630.272 << 135.997
                                          );

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

double formulas::ibuFromHop(hops const* hop, float batchSize)
{
    double ibus = 0.0;
//    double fwhAdjust = Brewtarget::toDouble(Brewtarget::option("firstWortHopAdjustment", 1.1).toString(), "Recipe::ibmFromHop()");
//    double mashHopAdjust = Brewtarget::toDouble(Brewtarget::option("mashHopAdjustment", 0).toString(), "Recipe::ibmFromHop()");
  

    if( hop->amount == 0 ){
        return 0.0;
        

    }
    
    double AArating = hop->alpha/100.0;
    double grams = hop->amount*1000.0;
    double minutes = hop->time;
    // Assume 100% utilization until further notice
    double hopUtilization = 1.0;
    // Assume 60 min boil until further notice
    int boilTime = 60;
    
    // NOTE: we used to carefully calculate the average boil gravity and use it in the
    // IBU calculations. However, due to John Palmer
    // (http://homebrew.stackexchange.com/questions/7343/does-wort-gravity-affect-hop-utilization),
    // it seems more appropriate to just use the OG directly, since it is the total
    // amount of break material that truly affects the IBUs.
    
   
    
//    if( hop->use() == Hop::Boil)
        //ibus = IbuMethods::getIbus( AArating, grams, _finalVolumeNoLosses_l, _og, minutes );
    
    ibus = ((AArating * grams * 1000) / batchSize/*finalvolumenoloss*/) * ((1.0 - exp(-0.04 * minutes))/4.15) * (1.65 * pow(0.000125, (og - 1)));
    //cout <<minutes<<endl;

   //    else if( hop->use() == Hop::First_Wort )
//        ibus = fwhAdjust * IbuMethods::getIbus( AArating, grams, _finalVolumeNoLosses_l, _og, boilTime );
//    else if( hop->use() == Hop::Mash && mashHopAdjust > 0.0 )
//        ibus = mashHopAdjust * IbuMethods::getIbus( AArating, grams, _finalVolumeNoLosses_l, _og, boilTime );
//    
    // Adjust for hop form. Tinseth's table was created from whole cone data,
    // and it seems other formulae are optimized that way as well. So, the
    // utilization is considered unadjusted for whole cones, and adjusted
    // up for plugs and pellets.
    //
    // - http://www.realbeer.com/hops/FAQ.html
    // - https://groups.google.com/forum/#!topic/brewtarget-help/mv2qvWBC4sU
//    switch( hop->form() ) {
//        case Hop::Plug:
//            hopUtilization *= 1.02;
//            break;
//        case Hop::Pellet:
           hopUtilization *= 1.10;
//            break;
//        default:
//            break;
//    }
    
    // Adjust for hop utilization. 
    ibus *= hopUtilization;
    
    return ibus;
}

void formulas::calcOgFg(float efficiency, vector<yeast*> yeastVec, vector<fermentables*> fermVec, float batchSize)
{
    unsigned int i;
    double plato;
//    double sugar_kg = 0;
//    double sugar_kg_ignoreEfficiency = 0.0;
//    double nonFermentableSugars_kg = 0.0;
    double kettleWort_l = 0.0;
    double postBoilWort_l = 0.0;
    double ratio = 0.0;
    double ferm_kg = 0.0;
    double attenuation_pct = 0.0;
    double tmp_og, tmp_fg, tmp_pnts, tmp_ferm_pnts, tmp_nonferm_pnts;
   // yeast* yeast;
   // QHash<QString,double> sugars;
    
    _og_fermentable = _fg_fermentable = 0.0;
    
    // The first time through really has to get the _og and _fg from the
    // database, not use the initialized values of 1. I (maf) tried putting
    // this in the initialize, but it just hung. So I moved it here, but only
    // if if we aren't initialized yet.
    //
    // GSG: This doesn't work, this og and fg are already set to 1.0 so
    // until we load these values from the database on startup, we have
    // to calculate.
//    if ( _uninitializedCalcs )
//    {
//        _og = Brewtarget::toDouble(this, kOG, "Recipe::recalcOgFg()");
//        _fg = Brewtarget::toDouble(this, kFG, "Recipe::recalcOgFg()");
//    }
    
    // Find out how much sugar we have.
     calcTotalPoints(fermVec);
//    sugar_kg                  = sugars.value("sugar_kg");  // Mass of sugar that *is* affected by mash efficiency
//    sugar_kg_ignoreEfficiency = sugars.value("sugar_kg_ignoreEfficiency");  // Mass of sugar that *is not* affected by mash efficiency
//    nonFermentableSugars_kg    = sugars.value("nonFermentableSugars_kg");  // Mass of sugar that is not fermentable (also counted in sugar_kg_ignoreEfficiency)
    
    // We might lose some sugar in the form of Trub/Chiller loss and lauter deadspace.
//    if( equipment() != 0 )
//    {
//        
//        kettleWort_l = (_wortFromMash_l - equipment()->lauterDeadspace_l()) + equipment()->topUpKettle_l();
//        postBoilWort_l = equipment()->wortEndOfBoil_l(kettleWort_l);
//        ratio = (postBoilWort_l - equipment()->trubChillerLoss_l()) / postBoilWort_l;
//        if( ratio > 1.0 ) // Usually happens when we don't have a mash yet.
//            ratio = 1.0;
//        else if( ratio < 0.0 )
//            ratio = 0.0;
//        else if( Algorithms::isNan(ratio) )
//            ratio = 1.0;
//        // Ignore this again since it should be included in efficiency.
//        //sugar_kg *= ratio;
//        sugar_kg_ignoreEfficiency *= ratio;
//        if ( nonFermentableSugars_kg != 0.0 )
//            nonFermentableSugars_kg *= ratio;
//    }
    
    // Total sugars after accounting for efficiency and mash losses. Implicitly includes non-fermentable sugars
    sugar_kg = sugar_kg * efficiency/100.0 + sugar_kg_ignoreEfficiency;
    plato = getPlato( sugar_kg, batchSize);
    
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
    
    // Calculage FG
   // QList<Yeast*> yeasties = yeasts();
    for( i = 0; static_cast<int>(i) < yeastVec.size(); ++i )
    {
        //yeast = yeastVec[i];
        // Get the yeast with the greatest attenuation.
        if( yeastVec[i]->attenuation > attenuation_pct )
            attenuation_pct = yeastVec[i]->attenuation;
    }
    if( yeastVec.size() > 0 && attenuation_pct <= 0.0 ) // This means we have yeast, but they neglected to provide attenuation percentages.
        attenuation_pct = 75.0; // 75% is an average attenuation.
    
    if ( nonFermentableSugars_kg != 0.0 )
    {
        tmp_ferm_pnts = (tmp_pnts-tmp_nonferm_pnts) * (1.0 - attenuation_pct/100.0);  // fg points from fermentable sugars
        tmp_pnts = tmp_ferm_pnts + tmp_nonferm_pnts;  // FG points from both fermentable and non-fermentable sugars
        //tmp_pnts *= (1.0 - attenuation_pct/100.0);  // WTF, this completely ignores all the calculations about non-fermentable sugars and just converts everything!
        tmp_fg =  1 + tmp_pnts/1000.0;  // new FG value
        _fg_fermentable =  1 + tmp_ferm_pnts/1000.0;  // FG from fermentables only
    }
    else
    {
        tmp_pnts *= (1.0 - attenuation_pct/100.0);
        tmp_fg =  1 + tmp_pnts/1000.0;
        _fg_fermentable = tmp_fg;
    }
 //   cout<< _fg_fermentable << " " << _og_fermentable <<endl;
    fg = _fg_fermentable;
    og = _og_fermentable;
}

void formulas::calcTotalPoints(vector<fermentables*> fermVec)
{
    int i;
    double _sugar_kg_ignoreEfficiency = 0.0;
    double _sugar_kg                  = 0.0;
    double _nonFermentableSugars_kg    = 0.0;
    double _lateAddition_kg           = 0.0;
    double _lateAddition_kg_ignoreEff = 0.0;
    
    fermentables* ferm;
    
   
   // QHash<QString,double> ret;
    
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
            
//            if ( !isFermentableSugar(ferm) ){
//                _nonFermentableSugars_kg += equivSucrose;
      //  }
        }
        else
        {
            _sugar_kg += equivSucrose;
            
            if (ferm->addAfterBoil == "TRUE")
                _lateAddition_kg += equivSucrose;
        }
    }
    //cout<< _sugar_kg<<endl;

    sugar_kg = _sugar_kg;
    nonFermentableSugars_kg = _nonFermentableSugars_kg;
    sugar_kg_ignoreEfficiency = _sugar_kg_ignoreEfficiency;
    lateAddition_kg = _lateAddition_kg;
    lateAddition_kg_ignoreEff = _lateAddition_kg_ignoreEff;
    
    //return ret;
    

}

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

double formulas::recalcColor_srm(vector<fermentables*> fermVec, float batchSize)
{
       double mcu = 0.0;
    double ret;
    unsigned int i;
    
       for( i = 0; static_cast<int>(i) < fermVec.size(); ++i )
    {
       // ferm = ferms[i];
        // Conversion factor for lb/gal to kg/l = 8.34538.
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

double formulas::recalcABV()
{
    double temp;
    
    // The complex formula, and variations comes from Ritchie Products Ltd, (Zymurgy, Summer 1995, vol. 18, no. 2)
    // Michael L. Hall’s article Brew by the Numbers: Add Up What’s in Your Beer, and Designing Great Beers by Daniels.
    temp = (76.08 * (og - fg) / (1.775 - og)) * (fg/ 0.794);
    
    return temp;
}

