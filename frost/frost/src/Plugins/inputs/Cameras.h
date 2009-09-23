#pragma once
#include "ofMain.h"
#include "Input.h"
#include "ofxVectorMath.h"
#include "Warp.h"

#include "ofxVideoGrabber.h"
#include "ofxVideoGrabberFeature.h"
#include "ofxVideoGrabberSettings.h"
#include "ofxVideoGrabberSDK.h"

/* SDKs */
#include "Libdc1394Grabber.h"


class Cameras : public Input{
public:
	Cameras();
	
	void draw();
	void setup();
	void update();
	
	void initGrabber(int _grabber, uint64_t _cameraGUID = 0x0ll);
	void setGUIDs(uint64_t _cameraGUID1, uint64_t _cameraGUID2, uint64_t _cameraGUID3);
	bool cameraGUIDexists(uint64_t _cameraGUID);

	ofxVideoGrabber * getVidGrabber(int _cameraIndex);
	bool isReady(int _cameraIndex);
	
	bool setGUID(int _grabber, uint64_t _cameraGUID);

	uint64_t getGUID(int _grabber);
		
	u_int64_t cameraGUIDs[3];
	
	int camWidth;
	int camHeight;
	
	bool cameraInited[3];

private:

	ofxVideoGrabber * vidGrabber[3];
	

};