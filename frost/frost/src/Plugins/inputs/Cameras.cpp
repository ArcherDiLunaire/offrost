#include "Cameras.h"

Cameras::Cameras(){
	type = INPUT;
	
	camWidth = 1024;	// try to grab at this size.
	camHeight = 768;
	
	csize = cvSize( camWidth, camHeight );
	
	for (int i=0; i<3; i++) {
		cameraInited[i] = false;
		cameraGUIDs[i] = 0x0ll;
		calibImage[i].setUseTexture(false);
		calibImage[i].allocate( camWidth, camHeight );
		calib[i].allocate( csize, 7, 7 );
	}

}

void Cameras::setup(){

	Libdc1394Grabber * libdc1394Grabber = new Libdc1394Grabber;

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
		if(isReady(i)){
			vidGrabber[i]->update();
/*
			if(vidGrabber[i]->isFrameNew()){
				calibImage[getGrabberIndexFromGUID(getGUID(i))].setFromPixels(vidGrabber[i]->getPixels(), camWidth,camHeight);
			}
*/
		}
	}	
}

bool Cameras::calibAddSnapshot(uint64_t _cameraGUID){
	if( calib[getGrabberIndexFromGUID(_cameraGUID)].addImage( calibImage[getGrabberIndexFromGUID(_cameraGUID)].getCvImage() ) ) {
		return true;
	}
	return false;
}

bool Cameras::calibrate(uint64_t _cameraGUID){
	calib[getGrabberIndexFromGUID(_cameraGUID)].calibrate();
	calib[getGrabberIndexFromGUID(_cameraGUID)].undistort();
}

ofPoint Cameras::undistortPoint(int _grabberIndex, float _PixelX, float _PixelY){
	return calib[_grabberIndex].undistortPoint(_PixelX, _PixelY);
}

ofPoint Cameras::distortPoint(int _grabberIndex, float _PixelX, float _PixelY){
	return calib[_grabberIndex].distortPoint(_PixelX, _PixelY);
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
		initCameraCalibration(getGUID(_grabber));
		/**
		 
		 FEATURE_BRIGHTNESS			= 0,
		 FEATURE_EXPOSURE			= 1,
		 FEATURE_SHARPNESS			= 2,
		 FEATURE_WHITE_BALANCE		= 3,
		 FEATURE_HUE				= 4,
		 FEATURE_SATURATION			= 5,
		 FEATURE_GAMMA				= 6,
		 FEATURE_SHUTTER			= 7,
		 FEATURE_GAIN				= 8,
		 FEATURE_IRIS				= 9,
		 FEATURE_FOCUS				= 10,
		 FEATURE_TEMPERATURE		= 11,
		 FEATURE_TRIGGER			= 12,
		 FEATURE_TRIGGER_DELAY		= 13,
		 FEATURE_WHITE_SHADING		= 14,
		 FEATURE_FRAME_RATE			= 15,
		 FEATURE_ZOOM				= 16,
		 FEATURE_PAN				= 17,
		 FEATURE_TILT				= 18,
		 FEATURE_OPTICAL_FILTER		= 19,
		 FEATURE_CAPTURE_SIZE		= 20,
		 FEATURE_CAPTURE_QUALITY	= 21,
		((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureMode(FEATURE_MODE_MANUAL, FEATURE_SHUTTER);
		//((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureAbsoluteValue(0.08, FEATURE_SHUTTER);
		((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureMode(FEATURE_MODE_MANUAL, FEATURE_BRIGHTNESS);
		((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureMode(FEATURE_MODE_MANUAL, FEATURE_EXPOSURE);
		//((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureValue(2.0, FEATURE_EXPOSURE);
		((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureMode(FEATURE_MODE_MANUAL, FEATURE_GAMMA);
		((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureMode(FEATURE_MODE_MANUAL, FEATURE_GAIN);
		//((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureAbsoluteValue(250, FEATURE_GAIN);
		((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureMode(FEATURE_MODE_MANUAL, FEATURE_IRIS);
		((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->setFeatureMode(FEATURE_MODE_MANUAL, FEATURE_EXPOSURE);
		 **/		 
		
		ofLog(OF_LOG_NOTICE,"Camera succesfully initialized.");
		cameraInited[_grabber] = true;
	} else {
		cameraGUIDs[_grabber] = 0x0ll;
		ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
		cameraInited[_grabber] = false;
	}	
}

void Cameras::initCameraCalibration(uint64_t _cameraGUID){
	
	/**
	 * setCameraCalibration(uint64_t _cameraGUID, float _k1, float _k2, float _c1, float _c2, double fx, double cx, double fy, double cy)
	 **/
	
	switch (_cameraGUID) {
		case 0xb09d01008c1393ll:
			//setCameraCalibration(_cameraGUID, );
			break;
		case 0xb09d01008c1394ll:
			//setCameraCalibration(_cameraGUID, );
			break;
		case 0xb09d01008c139cll:
			/**
			 *
			 * 10mm 1/2"  1:1.4
			 * 
			 * Distortion Coefficients:
			 * -0.5213706493 -7.2184705734 0.0017122072 0.0209310278 
			 * Camera Matrix:
			 * 2983.3862304688 0.0000000000 360.2234497070 
			 * 0.0000000000 2991.7260742188 378.9212646484 
			 * 0.0000000000 0.0000000000 1.0000000000 
			 **/
			setCameraCalibration(_cameraGUID, 
								 -0.5213706493, -7.2184705734, 0.0017122072, 0.0209310278,
								 2983.3862304688, 360.2234497070, 2991.7260742188, 378.9212646484);
			break;
		default:
			break;
	}
	
}

ofxVideoGrabber * Cameras::getVidGrabber(int _cameraIndex){
	if (cameraInited[_cameraIndex] && vidGrabber[_cameraIndex] != NULL){
		return vidGrabber[_cameraIndex];
	} else {
		return NULL;
	}
}

bool Cameras::isReady(int _cameraIndex){
	if (cameraInited[_cameraIndex] && vidGrabber[_cameraIndex] != NULL) {
		return vidGrabber[_cameraIndex]->isReady();
	}
	return cameraInited[_cameraIndex];
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
				cameraInited[i] = false;
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

int Cameras::getGrabberIndexFromGUID(uint64_t _cameraGUID){
	for (int i=0; i<3; i++) {
		if(_cameraGUID == getGUID(i)){
			return i;
		}
	}
	return -1;
}

void Cameras::setCameraCalibration(uint64_t _cameraGUID, float _k1, float _k2, float _c1, float _c2, double _fx, double _cx, double _fy, double _cy){
	int grabberIndex = getGrabberIndexFromGUID(_cameraGUID);

	calib[grabberIndex].distortionCoeffs[0] = _k1;
	calib[grabberIndex].distortionCoeffs[1] = _k2;
	calib[grabberIndex].distortionCoeffs[2] = _c1;
	calib[grabberIndex].distortionCoeffs[3] = _c2;
	
	calib[grabberIndex].camIntrinsics[0] = _fx;
	calib[grabberIndex].camIntrinsics[1] = 0;
	calib[grabberIndex].camIntrinsics[2] = _cx;
	
	calib[grabberIndex].camIntrinsics[3] = 0;
	calib[grabberIndex].camIntrinsics[4] = _fy;
	calib[grabberIndex].camIntrinsics[5] = _cy;
	
	calib[grabberIndex].camIntrinsics[6] = 0;
	calib[grabberIndex].camIntrinsics[7] = 0;
	calib[grabberIndex].camIntrinsics[8] = 1;

}