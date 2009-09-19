
#include "Cameras.h"

Cameras::Cameras(){
	type = INPUT;
}
void Cameras::setup(){

	Libdc1394Grabber * libdc1394Grabber1 = new Libdc1394Grabber; libdc1394Grabber1->listDevices();
	libdc1394Grabber1->setDeviceGUID(0xb09d01008c1393LL);
	
	int camWidth 		= 1024;	// try to grab at this size.
	int camHeight 		= 768;
	
	vidGrabber = new ofxVideoGrabber;
	if(vidGrabber->initGrabber( camWidth, camHeight, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 30, true, libdc1394Grabber1 )) {
	    ofLog(OF_LOG_NOTICE,"Camera 1 succesfully initialized.");
	} else {
	    ofLog(OF_LOG_FATAL_ERROR,"Camera 1 failed to initialize.");
	}
	
	
}
void Cameras::update(){
	vidGrabber->grabFrame();

}
void Cameras::draw(){

}

