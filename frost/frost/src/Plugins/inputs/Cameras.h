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
	
	ofxVideoGrabber * vidGrabber;
	
	
	
};
