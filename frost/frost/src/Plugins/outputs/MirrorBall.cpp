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

	rotation = 0;
	rotationDirection = 0.001;
		
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
	
	
	ofPushStyle();
	ofEnableAlphaBlending();
	glPushMatrix();
	ofSetColor(255, 255, 255);
	
	glTranslated(0.5*projection()->getFloor()->aspect, 0.5, 0);

	ofSetColor(255,200,0,120);
	drawMirrorBall(15);
	
	glTranslated(0.005*projection()->getFloor()->aspect, 0.005, 0);
	
	ofSetColor(255,255,255,120);
	drawMirrorBall(15);
	
	glPopMatrix();
	ofPopStyle();
	
}

void MirrorBall::update(){

	rotation += rotationDirection;
	
	rotation = fmodf(rotation, 1.0);
	
}

void MirrorBall::drawMirrorBall(int numCircles){
	
	glPushMatrix();
	
	for (int i=1; i< numCircles; i++) {
		
		glTranslated(0.0006*i*i, -0.0006*i*i, 0);

		int numDots =  i*i-1;
		
		for (int j=0; j < numDots; j++) {
			glPushMatrix();
			
			glRotatef((rotation*360.0)/i, 0, 0, 1.0);

			glRotatef((j/(1.0*numDots))*360.0, 0, 0, 1.0);
			
			glTranslated(0, ((i-1)*0.09)*(i*0.12), 0);
			
			ofCircle(sin(j*10020.6)*0.003*((i+2)-(numCircles*1.0/i)), sin(j*10000.6)*0.003*((i+2)-(numCircles*1.0/i)), 0.01);
			
			//mirrorBallImage.draw(-0.02,-0.02,0.04,0.04);
			
			glPopMatrix();
			
		}
		
	}
	glPopMatrix();
	
}

void MirrorBall:: addDot(float x, float y){
	
}