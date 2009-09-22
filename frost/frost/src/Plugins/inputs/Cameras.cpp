#include "Cameras.h"

Cameras::Cameras(){
	type = INPUT;
	
	camWidth = 1024;	// try to grab at this size.
	camHeight = 768;
	
	for (int i=0; i<3; i++) {
		cameraInited[i] = false;
		cameraGUIDs[i] = 0x0ll;
	}
}

void Cameras::setup(){

	for (int i=0; i<3; i++) {
		
		Libdc1394Grabber * libdc1394Grabber = new Libdc1394Grabber;
		libdc1394Grabber->listDevices();
		
		if(cameraGUIDs[0] == 0x0ll && cameraGUIDs[1] == 0x0ll && cameraGUIDs[2] == 0x0ll){
			if (i < libdc1394Grabber->list->num) {
				libdc1394Grabber->setDeviceGUID(libdc1394Grabber->list->ids[i].guid);
				cameraGUIDs[i] = libdc1394Grabber->list->ids[i].guid;
			}
		}
	}

	// first init the cams that are bound and exist
	for (int i=0; i<3; i++) {
		if(cameraGUIDexists(cameraGUIDs[i])){
			initGrabber(i, cameraGUIDs[i]);
		}
	}

	// then init the cams that are NOT bound
	for (int i=0; i<3; i++) {
		if(!cameraGUIDexists(cameraGUIDs[i])){
			initGrabber(i, cameraGUIDs[i]);
		}
	}
}

void Cameras::update(){
	for (int i=0; i<3; i++) {
		if(cameraInited[i]){
			vidGrabber[i]->update();
		}
	}	
}

void Cameras::draw(){

}

void Cameras::initGrabber(int _grabber, uint64_t _cameraGUID){
	Libdc1394Grabber * libdc1394Grabber = new Libdc1394Grabber;
	if (_cameraGUID != 0x0ll) {
		libdc1394Grabber->setDeviceGUID(_cameraGUID);
	}
	
	vidGrabber[_grabber] = new ofxVideoGrabber;
	
	if(vidGrabber[_grabber]->initGrabber( camWidth, camHeight, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 30, true, libdc1394Grabber )) {
		cameraGUIDs[_grabber] = ((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->getDeviceGUID();
		ofLog(OF_LOG_NOTICE,"Camera succesfully initialized.");
		cameraInited[_grabber] = true;
	} else {
		cameraGUIDs[_grabber] = 0x0ll;
		ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
		cameraInited[_grabber] = false;
	}	
}

bool Cameras::cameraGUIDexists(uint64_t _cameraGUID){

	if(_cameraGUID == 0x0ll) return false;
	
	bool _cameraGUIDexists = false;
	
	Libdc1394Grabber * libdc1394Grabber = new Libdc1394Grabber;

	for (int i=0; i < libdc1394Grabber->list->num; i++) {
		if(libdc1394Grabber->list->ids[i].guid == _cameraGUID){
			_cameraGUIDexists = true;
		}
	}
	return _cameraGUIDexists;
}

bool Cameras::setGUID(int _grabber, uint64_t _cameraGUID){
	
	ofLog(OF_LOG_NOTICE, "setGUID %llx", _cameraGUID);
	
	Libdc1394Grabber * libdc1394Grabber = new Libdc1394Grabber;
	
	if(_grabber < 3 && _grabber >= 0){
		if (_cameraGUID != getGUID(_grabber)) {

			for (int i=0; i<3; i++) {
				delete vidGrabber[i];
				vidGrabber[i] = NULL;
			}
			
			uint64_t grabberOldGuid = cameraGUIDs[_grabber];
			
			bool guidWasActive = false;
			
			for (int i=0; i<3; i++) {
				if (cameraGUIDs[i] == _cameraGUID) {
					guidWasActive = true;
					for (int j=0; j<3; j++) {
						if(cameraGUIDs[j] == grabberOldGuid || cameraGUIDs[j] == _cameraGUID ){
							cameraGUIDs[j] = 0x0ll;
						}
					}
					cameraGUIDs[i] = grabberOldGuid;
					i=4; // stop the forloop
				}
			}
			if (!guidWasActive) {
				for (int i=0; i<3; i++) {
					if(cameraGUIDs[i] == 0x0ll){
						cameraGUIDs[i] = grabberOldGuid;
						i = 4;
					}
				}
			}
			if (cameraGUIDexists(_cameraGUID)) {
				cameraGUIDs[_grabber] = _cameraGUID;
				initGrabber(_grabber, _cameraGUID);
			}

			// first init the cams that are bound (guid != 0x0ll)
			for (int i=0; i<3; i++) {
				if(i != _grabber && cameraGUIDs[i] != 0x0ll){
					initGrabber(i, cameraGUIDs[i]);
				}
			}
			// then init the cams that are NOT bound (guid == 0x0ll)

			for (int i=0; i<3; i++) {
				if(cameraGUIDs[i] == 0x0ll){
					initGrabber(i, cameraGUIDs[i]);
				}
			}
			if (!cameraGUIDexists(_cameraGUID)) {
				cameraGUIDs[_grabber] = _cameraGUID;
				initGrabber(_grabber, _cameraGUID);
			}
		}
	}
}

void Cameras::setGUIDs(uint64_t _cameraGUID1, uint64_t _cameraGUID2, uint64_t _cameraGUID3){
	cameraGUIDs[0] = _cameraGUID1;
	cameraGUIDs[1] = _cameraGUID2;
	cameraGUIDs[2] = _cameraGUID3;
}

uint64_t Cameras::getGUID(int _grabber){
	if(_grabber < 3 && _grabber >= 0){
		return cameraGUIDs[_grabber];
	}
	
}