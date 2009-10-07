
#include "PluginController.h"
#include "PluginIncludes.h"



float Frostscape::randomFactor = 5.0;
float Frostscape::slider1 = 0.0;
float Frostscape::slider2 = 0;
float Frostscape::slider3 = 0;
float Frostscape::slider4 = 0;
float Frostscape::slider5 = 0;
float Frostscape::slider6 = 0.0;

bool Frostscape::applyToOther = false;


#pragma mark Frostscape


Frostscape::Frostscape(){
	type = OUTPUT;
	cam = 0;
}

void Frostscape::setup(){
	motor.generateBackgroundObjects(35, 1, projection()->getFloor()->aspect, 1.0, -3);
}

void Frostscape::update(){
	motor.centerBreakRate =  Frostscape::slider1;
	motor.bodyBreakRate =  Frostscape::slider2;	
	motor.decreaseRate = Frostscape::slider5;
	motor.expandRate = Frostscape::slider6;
	///*
	for(int i=0;i<MIN(blob(cam)->numPersistentBlobs(),2);i++){
		PersistentBlob * pb = &blob(cam)->persistentBlobs[i];
		vector<ofxCvBlob> b = blob(cam)->persistentBlobs[i].blobs;
		ofxVec2f r = projection()->convertToCoordinate(projection()->getFloor(), pb->centroid);			

		motor.addBodyCenter(r);
		for(int g=0;g<b.size();g++){
			for(int u=0;u<b[g].nPts;u++){
				ofxVec2f r = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b[g].pts[u].x,b[g].pts[u].y));			
				
				motor.addBodyPoint(r);
			}
		}
		
	}
	//*/
/*
	for(int i=0;i<MIN(blob(cam)->numPersistentBlobs(),2);i++){
		PersistentBlob * pb = &blob(cam)->persistentBlobs[i];
//		vector<ofxCvBlob> b = blob(cam)->persistentBlobs[i].blobs;
		ofxVec2f r = projection()->convertToCoordinate(projection()->getFloor(), pb->centroid);			

		motor.addFreezePoint(r,0.1);
				
	}
	//*/
	
	motor.update();
}
void Frostscape::draw(){
}

void Frostscape::drawOnFloor(){
	motor.draw();
}

void Frostscape::setslider1(float val){
	Frostscape::slider1 = val;
}

void Frostscape::setslider2(float val){
	Frostscape::slider2 = val;
}
void Frostscape::setslider3(float val){
	Frostscape::slider3 = val;
}
void Frostscape::setslider4(float val){
	Frostscape::slider4 = val;
}
void Frostscape::setslider5(float val){
	Frostscape::slider5 = val;
}
void Frostscape::setslider6(float val){
	Frostscape::slider6 = val;
}
