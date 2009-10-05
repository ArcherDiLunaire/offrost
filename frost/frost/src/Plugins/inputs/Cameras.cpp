#include "Cameras.h"

Cameras::Cameras(){
	type = INPUT;
	
	camWidth = 1280/2;	// try to grab at this size.
	camHeight = 960/2;
	
	csize = cvSize( camWidth, camHeight );
	
	for (int i=0; i<3; i++) {
		cameraInited[i] = false;
		cameraGUIDs[i] = 0x0ll;
		calibImage[i].setUseTexture(false);
		calibImage[i].allocate( camWidth, camHeight );
		calib[i].allocate( csize, 7, 7 );
		videoPlayerActivated[i] = false;
		frameNew[i] = false;
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
	
	/**
	 // test videoPlayer
	 for (int i=0; i<3; i++) {
	 videoPlayerLoadUrl(i, "fingers.mov");
	 videoPlayerPlay(i);
	 videoPlayerActivate(i);
	 }
	 //**/
	
	/**	
	if(videoPlayerLoadUrl(0, "FilmFrostscape.mov")){
		cout << "                      FILMEN LOADET" << endl;
	} else {
		cout << "                      FILMEN ikke LOADET" << endl;
	}
	videoPlayerPlay(0);
	videoPlayerActivate(0);
	//**/
}

bool Cameras::isFrameNew(int _grabberIndex){
	return frameNew[_grabberIndex];
}

void Cameras::update(){
	for (int i=0; i<3; i++) {
		if (videoPlayerActivated[i]) {
			videoPlayer[i].update();
			frameNew[i] = videoPlayer[i].isFrameNew();
			if(frameNew[i]){
				ofImage img;
				img.setUseTexture(false);
				img.setFromPixels(videoPlayer[i].getPixels(), videoPlayer[i].getWidth(),videoPlayer[i].getHeight(), OF_IMAGE_COLOR);
				img.setImageType(OF_IMAGE_GRAYSCALE);
				calibImage[i].setFromPixels(img.getPixels(),videoPlayer[i].getWidth(),videoPlayer[i].getHeight());
				img.clear();
			}
		} else {
			if(isReady(i)){
				vidGrabber[i]->update();
				frameNew[i] = vidGrabber[i]->isFrameNew();
				if(frameNew[i]){
					calibImage[getGrabberIndexFromGUID(getGUID(i))].setFromPixels(vidGrabber[i]->getPixels(), camWidth,camHeight);
					frameNew[getGrabberIndexFromGUID(getGUID(i))] = true;
				}
			}
		}
	}
}

unsigned char* Cameras::getPixels(int _grabberIndex){
	return calibImage[_grabberIndex].getPixels();
}


int Cameras::getHeight(int _grabberIndex){
	return camHeight;
}

int Cameras::getWidth(int _grabberIndex){
	return camWidth;
}

int Cameras::getHeight(){
	return getHeight(0);
}

int Cameras::getWidth(){
	return getWidth(0);
}


void Cameras::videoPlayerActivate(int _grabberIndex){
	videoPlayerActivated[_grabberIndex] = true;
}

void Cameras::videoPlayerDeactivate(int _grabberIndex){
	videoPlayerActivated[_grabberIndex] = false;
}

bool Cameras::videoPlayerActive(int _grabberIndex){
	return videoPlayerActivated[_grabberIndex];
}

void Cameras::videoPlayerPlay(int _grabberIndex){
	videoPlayer[_grabberIndex].play();
}

void Cameras::videoPlayerStop(int _grabberIndex){
	videoPlayer[_grabberIndex].stop();
}

void Cameras::videoPlayerSetLoopState(int _grabberIndex, int _state){
	videoPlayer[_grabberIndex].setLoopState(_state);
}

bool Cameras::videoPlayerLoadUrl(int _grabberIndex, string url){
	return videoPlayer[_grabberIndex].loadMovie(url);
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


void Cameras::draw(int _grabberIndex, float _x, float _y, float _w, float _h)
{
	if(videoPlayerActive(_grabberIndex)){
		videoPlayer[_grabberIndex].draw(_x, _y, _w, _h);
	} else {
		if(isReady(_grabberIndex)){
			vidGrabber[_grabberIndex]->draw(_x, _y, _w, _h);
		}
	}
}

void Cameras::draw(int _grabberIndex, float _x, float _y)
{
	draw(_grabberIndex, _x, _y, (float)camWidth, (float)camHeight);
}

void Cameras::draw(){
	//draw(0, 0, 0);
}

void Cameras::initGrabber(int _grabber, uint64_t _cameraGUID){
	Libdc1394Grabber * libdc1394Grabber = new Libdc1394Grabber;
	if (_cameraGUID != 0x0ll) {
		libdc1394Grabber->setDeviceGUID(_cameraGUID);
	}
	
	vidGrabber[_grabber] = new ofxVideoGrabber;
	
	if(vidGrabber[_grabber]->initGrabber( camWidth, camHeight, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 25, true, libdc1394Grabber )) {
		cameraGUIDs[_grabber] = ((Libdc1394Grabber*)vidGrabber[_grabber]->videoGrabber)->getDeviceGUID();
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
	initCameraCalibration(getGUID(_grabber));
	
}

void Cameras::initCameraCalibration(uint64_t _cameraGUID){
	
	/**
	 * setCameraCalibration(uint64_t _cameraGUID, float _k1, float _k2, float _c1, float _c2, double fx, double cx, double fy, double cy)
	 **/
	
	switch (_cameraGUID) {
		case 0xb09d01008c1393ll:
			//setCameraCalibration(_cameraGUID, );
			setCameraCalibration(_cameraGUID, 
								 -0.3796366751, 0.1794384271, 0.0015031601, 0.0049067354,
								 1054.0688476562, 513.7350463867, 1051.2467041016, 308.5940551758);
			break;
		case 0xb09d01008c1394ll:
			/**
			 *
			 * 3.5mm 1/2"
			 *
			 * Distortion Coefficients:
			 * -0.3796366751 0.1794384271 0.0015031601 0.0049067354 
			 * Camera Matrix:
			 * 1054.0688476562 0.0000000000 513.7350463867 
			 * 0.0000000000 1051.2467041016 308.5940551758 
			 * 0.0000000000 0.0000000000 1.0000000000 
			 **/
			setCameraCalibration(_cameraGUID, 
								 -0.3796366751, 0.1794384271, 0.0015031601, 0.0049067354,
								 1054.0688476562, 513.7350463867, 1051.2467041016, 308.5940551758);
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
			
			setCameraCalibration(_cameraGUID, 
								 -0.3796366751, 0.1794384271, 0.0015031601, 0.0049067354,
								 1054.0688476562, 513.7350463867, 1051.2467041016, 308.5940551758);
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
	if (_cameraIndex < 3 && _cameraIndex > -1) {
		if (cameraInited[_cameraIndex] && vidGrabber[_cameraIndex] != NULL) {
			return vidGrabber[_cameraIndex]->isReady();
		}
		return cameraInited[_cameraIndex];
	} else {
		return false;
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