#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(5, 8, 15);
    
	ofSetFrameRate(120);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
	ofxGamepadHandler::get()->enableHotplug();
	
	//CHECK IF THERE EVEN IS A GAMEPAD CONNECTED
	if(ofxGamepadHandler::get()->getNumPads()>0){
        ofxGamepad* pad = ofxGamepadHandler::get()->getGamepad(0);
        ofAddListener(pad->onAxisChanged, this, &testApp::axisChanged);
        ofAddListener(pad->onButtonPressed, this, &testApp::buttonPressed);
        ofAddListener(pad->onButtonReleased, this, &testApp::buttonReleased);
	}
    
    ipAddress = "127.0.0.1"; // initial IP address
    sendPort = 12345; // initial send port number
    
    setupGui();
    
    oscSender.setup(ipAddress, sendPort);
    
    buttonState = new bool[5];
    for (int i = 0; i < 5; i++) {
        buttonState[i] = false;
    }
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

void testApp::exit() {
    gui->saveSettings("settings.xml");
    delete gui;
    delete buttonState;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::axisChanged(ofxGamepadAxisEvent& e)
{
    if (e.axis == 0) { // left & right (up & down: axis 1)
        if (e.value == 0) {
            ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("LEFT_BUTTON");
            button->setState(OFX_UI_STATE_NORMAL);
            button->stateChange();
            buttonState[0] = false; // left button released
            
            // GUI event
            ofxUIEventArgs event = ofxUIEventArgs(button);
            guiEvent(event);
            
            button = (ofxUIMultiImageButton *) gui->getWidget("RIGHT_BUTTON");
            button->setState(OFX_UI_STATE_NORMAL);
            button->stateChange();
            buttonState[1] = false; // right button released
            
            // GUI event
            event = ofxUIEventArgs(button);
            guiEvent(event);
        } else if (e.value < -0.9) {
            ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("LEFT_BUTTON");
            button->setState(OFX_UI_STATE_DOWN);
            button->stateChange();
            buttonState[0] = true; // left button pressed
            
            // GUI event
            ofxUIEventArgs event = ofxUIEventArgs(button);
            guiEvent(event);
        } else if (e.value > 0.9) {
            ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("RIGHT_BUTTON");
            button->setState(OFX_UI_STATE_DOWN);
            button->stateChange();
            buttonState[1] = true; // right button pressed
            
            // GUI event
            ofxUIEventArgs event = ofxUIEventArgs(button);
            guiEvent(event);
        }
    } else if (e.axis == 1) {
        if (e.value == 0) {
            ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("DOWN_BUTTON");
            button->setState(OFX_UI_STATE_NORMAL);
            button->stateChange();
            buttonState[4] = false; // down button released
            
            // GUI event
            ofxUIEventArgs event = ofxUIEventArgs(button);
            guiEvent(event);
        } else if (e.value > 0.9) {
            ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("DOWN_BUTTON");
            button->setState(OFX_UI_STATE_DOWN);
            button->stateChange();
            buttonState[4] = true; // down button pressed
            
            // GUI event
            ofxUIEventArgs event = ofxUIEventArgs(button);
            guiEvent(event);
        }
    }
}

void testApp::buttonPressed(ofxGamepadButtonEvent& e)
{
    if (e.button == 0) { // A button
        ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("A_BUTTON");
        button->setState(OFX_UI_STATE_DOWN);
        button->stateChange();
        buttonState[3] = true; // A button pressed
        
        // GUI event
        ofxUIEventArgs event = ofxUIEventArgs(button);
        guiEvent(event);
    } else if (e.button == 1) { // B button
        ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("B_BUTTON");
        button->setState(OFX_UI_STATE_DOWN);
        button->stateChange();
        buttonState[2] = true; // B button pressed
        
        // GUI event
        ofxUIEventArgs event = ofxUIEventArgs(button);
        guiEvent(event);
    } else if (e.button == 7) { // start button
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamestart");
        oscSender.sendMessage(oscMessage);
    } else if (e.button == 6) { // select button
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamestop");
        oscSender.sendMessage(oscMessage);
    }
}

void testApp::buttonReleased(ofxGamepadButtonEvent& e)
{
	if (e.button == 0) { // A button
        ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("A_BUTTON");
        button->setState(OFX_UI_STATE_NORMAL);
        button->stateChange();
        buttonState[3] = false; // A button released
        
        // GUI event
        ofxUIEventArgs event = ofxUIEventArgs(button);
        guiEvent(event);
    } else if (e.button == 1) { // B button
        ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("B_BUTTON");
        button->setState(OFX_UI_STATE_NORMAL);
        button->stateChange();
        buttonState[2] = false; // B button released
        
        // GUI event
        ofxUIEventArgs event = ofxUIEventArgs(button);
        guiEvent(event);
    }
}

void testApp::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "IP_ADDRESS") {
        ofxUITextInput *text = (ofxUITextInput *) e.widget;
        ipAddress = text->getTextString();
        oscSender.setup(ipAddress, sendPort);
    } else if (e.getName() == "PORT") {
        ofxUITextInput *text = (ofxUITextInput *) e.widget;
        sendPort = ofToInt(text->getTextString());
        oscSender.setup(ipAddress, sendPort);
    } else if (e.getName() == "CONTROLLER") {
        ofxUITextInput *text = (ofxUITextInput *) e.widget;
        controllerNumber = ofToInt(text->getTextString());
    } else if (e.getName() == "LEFT_BUTTON") {
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamepad");
        oscMessage.addIntArg(controllerNumber);
        oscMessage.addIntArg(0); // left button
        oscMessage.addIntArg(buttonState[0] ? 1 : 0);
        oscSender.sendMessage(oscMessage);
    } else if (e.getName() == "RIGHT_BUTTON") {
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamepad");
        oscMessage.addIntArg(controllerNumber);
        oscMessage.addIntArg(1); // right button
        oscMessage.addIntArg(buttonState[1] ? 1 : 0);
        oscSender.sendMessage(oscMessage);
    } else if (e.getName() == "DOWN_BUTTON") {
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamepad");
        oscMessage.addIntArg(controllerNumber);
        oscMessage.addIntArg(4); // down button
        oscMessage.addIntArg(buttonState[4] ? 1 : 0);
        oscSender.sendMessage(oscMessage);
    } else if (e.getName() == "A_BUTTON") {
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamepad");
        oscMessage.addIntArg(controllerNumber);
        oscMessage.addIntArg(3); // A button
        oscMessage.addIntArg(buttonState[3] ? 1 : 0);
        oscSender.sendMessage(oscMessage);
    } else if (e.getName() == "B_BUTTON") {
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamepad");
        oscMessage.addIntArg(controllerNumber);
        oscMessage.addIntArg(2); // B button
        oscMessage.addIntArg(buttonState[2] ? 1 : 0);
        oscSender.sendMessage(oscMessage);
    }
}

void testApp::setupGui() {
    gui = new ofxUICanvas();
    
    // canvas size
    gui->setWidth(660);
    gui->setHeight(700);
    
    gui->setPadding(10); // padding
    gui->setDrawPadding(false);
    
    // set font size
    gui->setFontSize(OFX_UI_FONT_SMALL, 20, OFX_UI_FONT_RESOLUTION);
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 40, OFX_UI_FONT_RESOLUTION);
    gui->setFontSize(OFX_UI_FONT_LARGE, 50, OFX_UI_FONT_RESOLUTION);
    
    // set colors
    ofxUIColor cb = ofxUIColor( 255, 255, 255, 30 );
    ofxUIColor co = ofxUIColor( 0, 0, 0, 0 );
    ofxUIColor coh = ofxUIColor( 0, 0, 0, 0 );
    ofxUIColor cf = ofxUIColor( 255, 255, 255, 170 );
    ofxUIColor cfh = ofxUIColor( 255, 255, 255, 255 );
    ofxUIColor cp = ofxUIColor( 0, 0, 0, 0 );
    ofxUIColor cpo = ofxUIColor( 0, 0, 0, 0 );
    gui->setUIColors( cb, co, coh, cf, cfh, cp, cpo );
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addLabel("IP Address", OFX_UI_FONT_SMALL); // label
    gui->setWidgetFontSize(OFX_UI_FONT_MEDIUM); // set font size
    gui->addTextInput("IP_ADDRESS", "127.0.0.1")->setAutoClear(false); // text input
    
    gui->addSpacer();
    
    gui->addLabel("Port", OFX_UI_FONT_SMALL); // label
    gui->setWidgetFontSize(OFX_UI_FONT_LARGE); // set font size
    gui->addTextInput("PORT", "12345")->setAutoClear(false); // text input
    
    gui->addSpacer();
    
    gui->addLabel("Controller Number", OFX_UI_FONT_SMALL); // label
    gui->setWidgetFontSize(OFX_UI_FONT_LARGE); // set font size
    gui->addTextInput("CONTROLLER", "0")->setAutoClear(false); // text input
    
    gui->addSpacer();
    
    gui->addSpacer(0, 10);
    gui->addSpacer(30, 0);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    
    gui->addMultiImageButton("LEFT_BUTTON", "left_button.png", false, 80, 80);
    
    gui->setWidgetSpacing(30); // space between buttons
    
    gui->addMultiImageButton("DOWN_BUTTON", "down_button.png", false, 80, 80);
    gui->addMultiImageButton("RIGHT_BUTTON", "right_button.png", false, 80, 80);
    gui->addMultiImageButton("B_BUTTON", "b_button.png", false, 80, 80);
    gui->addMultiImageButton("A_BUTTON", "a_button.png", false, 80, 80);
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("settings.xml");
}