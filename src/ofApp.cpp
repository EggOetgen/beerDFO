#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

 //xml.loadFile("actualBeer.xml");
  //  xml.loadFile("what_i_have.xml");
  //   xml.loadFile("apa.xml");
      xml.loadFile("apaIng.xml");

    xml.pushTag("RECIPES");
    xml.pushTag("RECIPE");
    batchSize = xml.getValue("BATCH_SIZE", 0.0);
    efficiency = xml.getValue("EFFICIENCY", 0.0);

    xml.pushTag("HOPS");
    
        int numberOfHops = xml.getNumTags("HOP");
        for(int i = 0; i < numberOfHops; i++){
            xml.pushTag("HOP", i);
        
            hops* newHop = new hops;
            
            newHop->amountRange = xml.getValue("AMOUNT", 0.0);
                       newHop->alpha = xml.getValue("ALPHA", 0.0);
            newHop->amount = xml.getValue("AMOUNT", 0.0);

           // newHop->alpha = alpha;
       //     cout<< alpha <<endl;
           newHop->beta   = xml.getValue("BETA", 0.0);
           newHop->time   = xml.getValue("TIME", 0.0);
            newHop->HSI   = xml.getValue("HSI", 0.0);
            newHop->hum   = xml.getValue("HUMULENE", 0.0);
            newHop->car   = xml.getValue("CARYOPHYLLENE", 0.0);
            newHop->coh   = xml.getValue("COHUMULONE", 0.0);
            newHop->myr   = xml.getValue("MYRCENE", 0.0);
//        p.x = settings.getValue("X", 0);
//        p.y = settings.getValue("Y", 0);
//        p.z = settings.getValue("Z", 0);
//        
//            points.push_back(p);
//            settings.popTag();
            

            hopVec.push_back(newHop);
            xml.popTag();
    }
  //  ing.push_back(hopVec);
    xml.popTag();
    xml.pushTag("FERMENTABLES");
    int numberOfFerms = xml.getNumTags("FERMENTABLE");
    for(int i = 0; i < numberOfFerms; i++){
        xml.pushTag("FERMENTABLE", i);
        
        fermentables* newFerm = new fermentables;
        
        
        newFerm->amount = xml.getValue("AMOUNT", 0.0);
        newFerm->yield = xml.getValue("YIELD", 0.0);
        newFerm->color = xml.getValue("COLOR", 0.0);
        newFerm->coarse_fine_dif  = xml.getValue("COLOR", 0.0);
        newFerm->moisture = xml.getValue("MOISTURE", 0.0);
        newFerm->di_pow  = xml.getValue("DIASTATIC_POWER", 0.0);
        newFerm->protein = xml.getValue("PROTEIN", 0.0);
        newFerm->most_in_batch = xml.getValue("MAX_IN_BATCH", 0.0);
        newFerm->ibu_gal_per_lb = xml.getValue("IBU_GAL_PER_LB", 0.0);
        newFerm->type = xml.getValue("TYPE", 0);
        newFerm->addAfterBoil = xml.getValue("ADD_AFTER_BOIL", 0);
        
        double maxAllowed = xml.getValue("MAX_IN_BATCH", 0.0);
        if (maxAllowed < newFerm->amount)
            newFerm->amountRange = maxAllowed;
        else
             newFerm->amountRange = newFerm->amount;
        
        fermVec.push_back(newFerm);
        

        xml.popTag();
    }
    //ing.push_back(fermVec);
    xml.popTag();
//    xml.pushTag("MISCS");
    xml.pushTag("YEASTS");
//    
    int numberOfYeast = xml.getNumTags("YEAST");
    cout <<numberOfYeast;
    for(int i = 0; i < numberOfYeast; i++){
        xml.pushTag("YEAST", i);
        
        yeast* newYeast = new yeast;
        cout<<"test:"<<endl;
        newYeast->amount = xml.getValue("AMOUNT", 0.0);
        newYeast->amountRange = xml.getValue("AMOUNT", 0.0);
        newYeast->minTemp    = xml.getValue("MIN_TEMPERATURE", 0.0);
        newYeast->maxTemp    = xml.getValue("MAX_TEMPERATURE", 0.0);;
        newYeast->attenuation= xml.getValue("ATTENUATION", 0.0);
        yeastVec.push_back(newYeast);
        xml.popTag();
    }
    xml.popTag();
    xml.popTag();
    xml.popTag();


    cout<<yeastVec.size()<<endl;

   // ing.push_back(yeastVec);
    int dimensions = numberOfFerms+numberOfYeast+numberOfHops;
    
    hopVecBest   = hopVec;
    fermVecBest  = fermVec;
    yeastVecBest = yeastVec;
    
    // int dimensions = numberOfFerms+numberOfYeast+(numberOfHops * 3);
    dfo.setup(dimensions,  1000, desiredOG, desiredFG, desiredABV, desiredIBU, desiredCOL, hopVec, fermVec, yeastVec, efficiency, batchSize);
    
    bestFly = -1;
     forms.calcOgFg(efficiency, yeastVec, fermVec, batchSize );
      gui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
//    dfo.targetABV = desiredABV;
//      dfo.targetIBU = desiredIBU;
//    dfo.targetCOL = desiredCOL;
    dfo.updateTarget(desiredOG, desiredFG,desiredABV, desiredIBU, desiredCOL);
  dfo.run();
//    forms.calcOgFg(efficiency, yeastVec, fermVec, batchSize );
//    double ibu = forms.calcIBU(hopVec, fermVec, batchSize);
//    double c = forms.recalcColor_srm(fermVec, batchSize);
//    
//    cout<<ibu << " " << c << endl;
//     double IBUGU = forms.calcIBUGU(ibu);
//    forms.og = 1.107;
//    forms.fg = 1.027;
 //   cout <<dfo.flies[dfo.bestFlyIndex]->featVec.size() << endl;
//    cout << dfo.bestFlyIndex << " " << dfo.flies[dfo.bestFlyIndex]->fitness<< " " <<dfo.flies[dfo.bestFlyIndex]->featVec[0] <<  " " <<dfo.flies[dfo.bestFlyIndex]->featVec[1] <<  " " <<dfo.flies[dfo.bestFlyIndex]->featVec[2] << " " <<dfo.flies[dfo.bestFlyIndex]->featVec[3] << " " <<dfo.flies[dfo.bestFlyIndex]->featVec[4] <<" " <<dfo.flies[dfo.bestFlyIndex]->featVec[5] << " " <<dfo.flies[dfo.bestFlyIndex]->featVec[6] <<" " <<dfo.flies[dfo.bestFlyIndex]->featVec[7] <<" " <<dfo.flies[dfo.bestFlyIndex]->featVec[8] <<" " <<dfo.flies[dfo.bestFlyIndex]->featVec[9] <<" " <<dfo.flies[dfo.bestFlyIndex]->featVec[10] <<endl;
//    
//    if(dfo.bestFlyIndex != bestFly){
//        bestFly = dfo.bestFlyIndex;
//    vector<double> a = dfo.flies[dfo.bestFlyIndex]->featVec;
//    for (int i = 0; i < hopVecBest.size(); i ++ ){
//        
//        hopVecBest[i]->amount = a[i];
//    }
//    for (int i = hopVecBest.size(); i < fermVecBest.size()+hopVecBest.size(); i ++ ){
//        
//        fermVecBest[i-hopVecBest.size()]->amount = a[i];
//    }
//    for (int i = fermVecBest.size()+hopVecBest.size(); i < yeastVecBest.size()+hopVecBest.size()+fermVecBest.size(); i ++ ){
//        
//        yeastVecBest[i-(fermVecBest.size()+hopVecBest.size())]->amount = a[i];
//    }
//    double ibu = forms.calcIBU(hopVecBest, fermVecBest, batchSize);
//    double color = forms.recalcColor_srm(fermVecBest, batchSize);
  //  double abv =
    
//   cout << ibu << " " <<color<< " " << dfo.flies[dfo.bestFlyIndex]->fitness<< " " << " " << dfo.form.fg << " " << dfo.form.og << endl;
//}
    dfo.calcBestResults();
     cout << dfo.flies[dfo.bestFlyIndex]->fitness << " "<< dfo.returnBestOG() << " " <<dfo.returnBestFG() << " " << dfo.returnBestABV()<< " " << " " << dfo.returnBestIBU() << " " << dfo.returnBestCOL() << endl;
   //  cout << dfo.forms.recalcColor_srm <<endl;
    //forms.calcIBU(hopVec, fermVec, batchSize);
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.begin();
    
    
    {
        ImGui::SliderFloat("ABV", &desiredABV, 1.0, 15.0);
        ImGui::SliderFloat("Bitterness(ibu)", &desiredIBU, 0.5, 100.0);
        ImGui::SliderFloat("Color", &desiredCOL, 2.0, 80.0);

    }
    gui.end();
    dfo.display();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
