#include "ofxGamepadHandler.h"

using namespace OIS;
InputManager* oisInputManager;
class tempPad {
public:
	tempPad(JoyStick* s) {
		stick=s;
		handled=false;
	};
	JoyStick* stick;
	bool handled;
};

ofxGamepadHandler* ofxGamepadHandler::singleton;
bool ofxGamepadHandler::hasSingleton = false;

ofxGamepadHandler::ofxGamepadHandler():hasHotplug(false),hotplugNext(0) {
	ofAddListener(ofEvents().update, this, &ofxGamepadHandler::update);
	ofAddListener(ofEvents().exit, this, &ofxGamepadHandler::exit);
	updatePadList();
	gamepads.insert(gamepads.end(), gamepadsNew.begin(), gamepadsNew.end());
	gamepadsNew.clear();
}

ofxGamepadHandler::~ofxGamepadHandler() {
}

ofxGamepadHandler* ofxGamepadHandler::get() {
	if(!hasSingleton) {
		singleton=new ofxGamepadHandler();
		hasSingleton=true;
	}
	return singleton;
}

void ofxGamepadHandler::enableHotplug(int interval) {
	hasHotplug=true;
	hotplugInterval=interval;
	startThread(true, false);
}

void ofxGamepadHandler::threadedFunction() {
	while(isThreadRunning()) {

		updatePadList();
		lock();
		gamepads = gamepadsNew;
		unlock();

		gamepadsNew.clear();
		ofSleepMillis(hotplugInterval);
	}
}

void ofxGamepadHandler::updatePadList() {
	try {
		
		ParamList pl;
		InputManager* inputManager=InputManager::createInputSystem(pl);

		gamepadList padsOld=gamepads;
		std::vector<tempPad> sticks;
		//gamepads.clear();

		int numPads = inputManager->getNumberOfDevices(OISJoyStick);
		for( int i = 0; i < numPads; i++ ) {
			JoyStick* js = (JoyStick*)inputManager->createInputObject(OISJoyStick, true );
			sticks.push_back(tempPad(js));
		}

		std::vector<tempPad>::iterator sIt = sticks.begin();
		while(sIt!=sticks.end()) {
			gamepadList::iterator gIt = padsOld.begin();
			while(gIt!=padsOld.end()) {
				if((*sIt).stick->vendor() == (*gIt)->name) {
					ofPtr<ofxGamepadOIS> p = *gIt;
					p->updateJoystick((*sIt).stick);
					gamepadsNew.push_back(p);
					padsOld.erase(gIt);
					(*sIt).handled = true;
					break;
				}
				++gIt;
			}
			++sIt;
		}

		sIt = sticks.begin();
		while(sIt!=sticks.end()) {
			if(!(*sIt).handled)
				gamepadsNew.push_back(ofPtr<ofxGamepadOIS>(new ofxGamepadOIS((*sIt).stick)));
			++sIt;
		}

		lock();
		if(oisInputManager != NULL) {
			oisInputManager->destroyInputSystem(oisInputManager);
		}
		unlock();

		oisInputManager = inputManager;
	} catch(OIS::Exception &ex) {
		stringstream msg;
		msg << "\nException raised on joystick creation: " << ex.eText << std::endl;
		ofLog(OF_LOG_ERROR, msg.str());
	}
}

void ofxGamepadHandler::update(ofEventArgs &args) {
	update();
}

void ofxGamepadHandler::update() {
	lock();
	gamepadList::iterator it=gamepads.begin();
	while(it!=gamepads.end()) {
		(*it)->update();
		++it;
	}
	unlock();
}

void ofxGamepadHandler::draw(int x, int y) {
	ofPushMatrix();
	ofTranslate(x, y);
	ofPoint offset(x, y);

	gamepadList::iterator it=gamepads.begin();
	while(it!=gamepads.end()) {
		(*it)->draw(offset.x, offset.y);
		offset+=ofPoint((*it)->drawSize.x+20, 0);
		++it;
	}

	ofPopMatrix();
}

void ofxGamepadHandler::exit(ofEventArgs& arg) {
	gamepadList::iterator it=gamepads.begin();
	while(it!=gamepads.end()) {
		(*it)->exit();
		++it;
	}
	delete this;
}

ofxGamepad* ofxGamepadHandler::getGamepad(int num) {
	if(getNumPads()>0)
		return gamepads[num].get();
	ofLog(OF_LOG_WARNING, "ofxGamepad::getGamepad(): WARNING NO GAMEPAD CONNECTED. GAMEPAD IS NULL.");
	return NULL;
}

int ofxGamepadHandler::getNumPads() {
	return gamepads.size();
}
