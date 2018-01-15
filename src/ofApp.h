#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxXmlSettings.h"
#include "DFO.hpp"
#include "ingredients.hpp"
#include "formulas.hpp"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    DFO dfo;
    ofxXmlSettings xml;
   
    //where we save ingredients
        vector<fermentables*> fermVec;
    vector<hops*> hopVec;
    vector<yeast*> yeastVec;
      formulas forms;
    int bestFly;
    float batchSize, efficiency;
    
    //default for APA
    float desiredOG = 1.107;
    float desiredFG = 1.027;
    float desiredABV = 4.8;
    float desiredIBU = 41.;
    float desiredCOL = 7.;
    
 
    
    bool save;
    
     ofxImGui::Gui gui;
		
};
