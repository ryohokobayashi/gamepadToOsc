#pragma once

#include "ofMain.h"
#include "ofxGamepadHandler.h"
#include "ofxOSC.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void axisChanged(ofxGamepadAxisEvent &e);
        void buttonPressed(ofxGamepadButtonEvent &e);
        void buttonReleased(ofxGamepadButtonEvent &e);
    
        void guiEvent(ofxUIEventArgs &e);
        void setupGui();
    
        ofxUICanvas *gui;
        string ipAddress;
        int sendPort;
        int controllerNumber;
    
        ofxOscSender oscSender;
};
