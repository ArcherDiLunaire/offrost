
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
	invert = false;
}

void Frostscape::setup(){
	motor.generateBackgroundObjects(35, 1, projection()->getFloor()->aspect, 1.0, -3);
}

void Frostscape::update(){
	invert = false;
	motor.centerBreakRate =  Frostscape::slider1;
	motor.bodyBreakRate =  Frostscape::slider2;	
	motor.decreaseRate = Frostscape::slider5*10;
	motor.expandRate = Frostscape::slider6;
	if(invert){
		ofColor c;
		c.r = 0;
		c.g = 0;
		c.b = 0;
		motor.setColor(c);
	} else{
		ofColor c;
		motor.setColor(c);
	}
	
	if(sideFreeze > 0){
		float n = 3.0;
		for (int i=0;i<n; i++) {
			motor.addFreezePoint(ofPoint(0,i/n), sideFreeze);
		}
		for (int i=0;i<n; i++) {
			motor.addFreezePoint(ofPoint(projection()->getFloor()->aspect,i/n), sideFreeze);
		}
		for (int i=0;i<n; i++) {
			motor.addFreezePoint(ofPoint(projection()->getFloor()->aspect*i/n,0), sideFreeze);
			motor.addFreezePoint(ofPoint(projection()->getFloor()->aspect*i/n,1), sideFreeze);
			
		}
	}
	/*if(columnFreeze > 0.1){
	 for(int i=0;i<3;i++){
	 motor.addFreezePoint(projection()->getColumnCoordinate(i), columnFreeze);
	 
	 }
	 } else if(columnFreeze < -0.1){
	 for(int i=0;i<3;i++){
	 motor.addBodyCenter(projection()->getColumnCoordinate(i));
	 
	 }
	 }*/
	
	
	for(int i=0;i<3;i++){
		if(columnFreeze[i] > 0){
			//	int i=1;

			columnParticlePos[i] += 0.02*60.0/ofGetFrameRate();	
			if(columnParticlePos[i] > 1 && columnParticlePos[i] < 1.1){

				motor.addFreezePoint(projection()->getColumnCoordinate(i), columnFreeze[i]);	
			}
		} else {
			columnParticlePos[i] = -0.1;	
		}
	} 
	///*
	for(int i=0;i<MIN(blob(cam)->numPersistentBlobs(),6);i++){
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
	invert = false;
	
	if(invert){
		ofFill();
		ofSetColor(255, 255, 255,255);
		ofRect(0, 0, projection()->getFloor()->aspect, 1);
	}
	motor.draw();
	
	if(whiteBackground > 0){
		ofFill();
		ofSetColor(255, 255, 255,255.0*whiteBackground);
		ofRect(0, 0, projection()->getFloor()->aspect, 1);
	}
	
	glPopMatrix();
	
	for(int i=0;i<3;i++){
		projection()->applyProjection(projection()->getColumn(i));
		
		ofFill();
		ofSetColor(255, 255, 255, 255);
		ofRect(0, 0, projection()->getColumn(i)->aspect, 1);
		
		ofFill();
		ofSetColor(0, 0, 0, 255);
		ofRect(0, 0, projection()->getColumn(i)->aspect, MIN( columnParticlePos[i],1));
		
		/*if(columnParticlePos[i] < 1-projection()->getColumn(i)->aspect/2.0){
		 ofSetColor(0, 0, 0, 255);
		 ofEllipse(projection()->getColumn(i)->aspect/2.0, columnParticlePos[i], projection()->getColumn(i)->aspect,projection()->getColumn(i)->aspect);
		 }*/
		
		glPopMatrix();
		
	}
}

void Frostscape::fillIce(){
	motor.setValueOnAll(-3.0);
}
void Frostscape::emptyIce(){
	motor.setValueOnAll(1.0);	
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
