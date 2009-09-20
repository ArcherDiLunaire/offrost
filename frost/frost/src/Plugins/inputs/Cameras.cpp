
#include "Cameras.h"

Cameras::Cameras(){
	type = INPUT;
}
void Cameras::setup(){

	vidGrabber = new ofxVideoGrabber;

	Libdc1394Grabber * libdc1394Grabber = new Libdc1394Grabber; libdc1394Grabber->listDevices();
	//libdc1394Grabber->setDeviceGUID(0xb09d01008c1393LL);

	int camWidth 		= 1024;	// try to grab at this size.
	int camHeight 		= 768;
	
	vidGrabber = new ofxVideoGrabber;
	if(vidGrabber->initGrabber( camWidth, camHeight, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 30, true, libdc1394Grabber )) {
	    ofLog(OF_LOG_NOTICE,"Camera succesfully initialized.");
	} else {
	    ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
	}
	
}
void Cameras::update(){
	vidGrabber->update();

}
void Cameras::draw(){

}

