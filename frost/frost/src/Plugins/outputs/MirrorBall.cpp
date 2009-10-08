/*
 *  MirrorBall.cpp
 *  openFrameworks
 *
 *  Created by frost on 06/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "PluginController.h"
#include "PluginIncludes.h"

MirrorBall::MirrorBall(){
	type = OUTPUT;

	rotationDirection = 0;
		
	center = ofxVec2f(0.5,0.5);
	
	cam = 0;
}

#pragma mark Callback methods

void MirrorBall::setup(){
	mirrorBallImage.loadImage("spotlight.png");
}

void MirrorBall::draw(){
	
}

void MirrorBall::drawOnFloor(){
	glPushMatrix();
	ofPushStyle();
	
	

	ofSetColor(255, 255, 255);
	
	glTranslated(0.5*projection()->getFloor()->aspect, 0.5, 0);
	
	for (int i=1; i < 3; i++) {

		for (int j=1; j < i*i; j++) {

			glPushMatrix();
			
			glRotated(1.0*((i*i)/j), 0, 0, 1.0);
			
			glTranslated(0, i*0.2, 0);

			mirrorBallImage.draw(-0.02,-0.02,0.04,0.04);
			
			glPopMatrix();
			
		}
		
	}
	ofPopStyle();
	
	glPopMatrix();
}

void MirrorBall::update(){
	
}

void MirrorBall:: addDot(float x, float y){
	
}