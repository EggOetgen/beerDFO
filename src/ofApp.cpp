#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    save = false;
    
    //Random selection of ingredients
   //  xml.loadFile("what_i_have.xml");
    
    //full apa recipe and one with right ingredients but wrong amounts
    //xml.loadFile("apa.xml");
      xml.loadFile("apaIng.xml");

    //load all our values from the xml
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
            newHop->beta   = xml.getValue("BETA", 0.0);
            newHop->time   = xml.getValue("TIME", 0.0);

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
      //  newFerm->coarse_fine_dif  = xml.getValue("COLOR", 0.0);
        newFerm->moisture = xml.getValue("MOISTURE", 0.0);
        newFerm->di_pow  = xml.getValue("DIASTATIC_POWER", 0.0);
      //  newFerm->protein = xml.getValue("PROTEIN", 0.0);
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
   
    xml.popTag();

    xml.pushTag("YEASTS");
   
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

    //how many ingredients we have
    int dimensions = numberOfFerms+numberOfYeast+numberOfHops;

//set it all up    
    dfo.setup(dimensions,  1000, desiredOG, desiredFG, desiredABV, desiredIBU, desiredCOL, hopVec, fermVec, yeastVec, efficiency, batchSize);
    
 
     forms.calcOgFg(efficiency, yeastVec, fermVec, batchSize );
      gui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //run everything and adjust if needed
    dfo.updateTarget(desiredOG, desiredFG,desiredABV, desiredIBU, desiredCOL);
    dfo.run();
    dfo.calcBestResults();
   
    //if save update all the amounrs and save them!
    if(save){
        vector<double> temp;
        temp =dfo.flies[dfo.bestFlyIndex]->returnFeatureVector();
        
        xml.pushTag("RECIPES");
        xml.pushTag("RECIPE");
        xml.setValue("NAME", "myDFDO_beer");
        xml.pushTag("HOPS");
        
        int numberOfHops = xml.getNumTags("HOP");
        for(int i = 0; i < numberOfHops; i++){
            xml.pushTag("HOP", i);
          
            xml.setValue("AMOUNT", temp[i]);
            
            xml.popTag();
        }
       
        xml.popTag();
        xml.pushTag("FERMENTABLES");
        
        int numberOfFerms = xml.getNumTags("FERMENTABLE");
        for(int i = 0; i < numberOfFerms; i++){
            xml.pushTag("FERMENTABLE", i);
          
            int amIt = i+numberOfHops;
            xml.setValue("AMOUNT", temp[amIt]);
          
            xml.popTag();
        }
        
        xml.popTag();
        xml.pushTag("YEASTS");
        
        int numberOfYeast = xml.getNumTags("YEAST");
        cout <<numberOfYeast;
        for(int i = 0; i < numberOfYeast; i++){
            xml.pushTag("YEAST", i);
           
            int amIt = i+numberOfHops + numberOfFerms;
            xml.setValue("YEAST", temp[amIt]);
           
            xml.popTag();
        }
        xml.popTag();
        xml.popTag();
        xml.popTag();
        xml.save("results/result.xml");
        save = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //gui gui gui
    gui.begin();
    
    
    {
        ImGui::SliderFloat("ABV", &desiredABV, 1.0, 15.0);
        ImGui::SliderFloat("Bitterness(ibu)", &desiredIBU, 0.5, 100.0);
        ImGui::SliderFloat("Color", &desiredCOL, 2.0, 80.0);
        ImGui::Text("The closer this value -> %.3f <- is to zero, the more confident I am you'll like my suggestion!", dfo.flies[dfo.bestFlyIndex]->fitness);
        ImGui::Text("My current predictions for this recipe are:");
        ImGui::Text("ABV             : %.3f ", dfo.returnBestABV());
        ImGui::Text("Bitterness (IBU): %.3f ", dfo.returnBestIBU());
        ImGui::Text("Colour          : %.3f ", dfo.returnBestCOL());
        ImGui::Checkbox("Save this Recipe?", &save);
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
