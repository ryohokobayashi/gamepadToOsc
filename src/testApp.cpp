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
            
            button = (ofxUIMultiImageButton *) gui->getWidget("RIGHT_BUTTON");
            button->setState(OFX_UI_STATE_NORMAL);
            button->stateChange();
        } else if (e.value < 0) {
            ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("LEFT_BUTTON");
            button->setState(OFX_UI_STATE_DOWN);
            button->stateChange();
            
            // GUI event
            ofxUIEventArgs event = ofxUIEventArgs(button);
            guiEvent(event);
        } else if (e.value > 0) {
            ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("RIGHT_BUTTON");
            button->setState(OFX_UI_STATE_DOWN);
            button->stateChange();
            
            // GUI event
            ofxUIEventArgs event = ofxUIEventArgs(button);
            guiEvent(event);
        }
    }
}

void testApp::buttonPressed(ofxGamepadButtonEvent& e)
{
    // RELEASED (revise compatibility for iBuffalo)
	if (e.button == 0) { // A button
        ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("A_BUTTON");
        button->setState(OFX_UI_STATE_NORMAL);
        button->stateChange();
    } else if (e.button == 1) { // B button
        ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("B_BUTTON");
        button->setState(OFX_UI_STATE_NORMAL);
        button->stateChange();
    }
}

void testApp::buttonReleased(ofxGamepadButtonEvent& e)
{
    // PRESSED (revise compatibility for iBuffalo)
    if (e.button == 0) { // A button
        ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("A_BUTTON");
        button->setState(OFX_UI_STATE_DOWN);
        button->stateChange();
        
        // GUI event
        ofxUIEventArgs event = ofxUIEventArgs(button);
        guiEvent(event);
    } else if (e.button == 1) { // B button
        ofxUIMultiImageButton *button = (ofxUIMultiImageButton *) gui->getWidget("B_BUTTON");
        button->setState(OFX_UI_STATE_DOWN);
        button->stateChange();
        
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
        oscMessage.addIntArg(1); // left button
        oscSender.sendMessage(oscMessage);
    } else if (e.getName() == "RIGHT_BUTTON") {
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamepad");
        oscMessage.addIntArg(controllerNumber);
        oscMessage.addIntArg(2); // right button
        oscSender.sendMessage(oscMessage);
    } else if (e.getName() == "A_BUTTON") {
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamepad");
        oscMessage.addIntArg(controllerNumber);
        oscMessage.addIntArg(4); // A button
        oscSender.sendMessage(oscMessage);
    } else if (e.getName() == "B_BUTTON") {
        ofxOscMessage oscMessage;
        oscMessage.setAddress("/gamepad");
        oscMessage.addIntArg(controllerNumber);
        oscMessage.addIntArg(3); // B button
        oscSender.sendMessage(oscMessage);
    }
}

void testApp::setupGui() {
    gui = new ofxUICanvas();
    
    // canvas size
    gui->setWidth(600);
    gui->setHeight(780);
    
    gui->setPadding(10); // padding
    gui->setDrawPadding(false);
    
    // set font size
    gui->setFontSize(OFX_UI_FONT_SMALL, 20, OFX_UI_FONT_RESOLUTION);
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 30, OFX_UI_FONT_RESOLUTION);
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
    
    gui->addLabel("IP Address", OFX_UI_FONT_SMALL); // label
    gui->setWidgetFontSize(OFX_UI_FONT_LARGE); // set font size
    gui->addTextInput("IP_ADDRESS", "127.0.0.1")->setAutoClear(false); // text input
    
    gui->addSpacer();
    
    gui->addLabel("Port", OFX_UI_FONT_SMALL); // label
    gui->setWidgetFontSize(OFX_UI_FONT_LARGE); // set font size
    gui->addTextInput("PORT", "12345")->setAutoClear(false); // text input
    
    gui->addSpacer();
    
    gui->addLabel("Controller Number", OFX_UI_FONT_SMALL); // label
    gui->setWidgetFontSize(OFX_UI_FONT_LARGE); // set font size
    gui->addTextInput("CONTROLLER", "1")->setAutoClear(false); // text input
    
    gui->addSpacer();
    gui->setWidgetSpacing(60);
    
    gui->addMultiImageButton("LEFT_BUTTON", "left_button.png", false, 80, 80);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->setWidgetSpacing(40); // space between buttons
    gui->addMultiImageButton("RIGHT_BUTTON", "right_button.png", false, 80, 80);
    gui->addMultiImageButton("B_BUTTON", "b_button.png", false, 80, 80);
    gui->addMultiImageButton("A_BUTTON", "a_button.png", false, 80, 80);
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("settings.xml");
}