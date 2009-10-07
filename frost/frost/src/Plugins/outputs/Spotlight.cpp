/*
 *  Spotlight.cpp
 *  openFrameworks
 *
 *  Created by frost on 06/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "PluginController.h"
#include "PluginIncludes.h"

Spotlight::Spotlight(){
	type = OUTPUT;
	cam = 0;
	radius = 0;
	radiusMultiplier = 0;
	center = ofxVec2f(0.5,0.5);
}

#pragma mark Callback methods

void Spotlight::setup(){
	maxX = 0.0;
	minX = 0.0;
	maxY = 0.0;
	minY = 0.0;
	spotlightImage.loadImage("spotlight.png");
}

void Spotlight::draw(){

	glPushMatrix();
	
	glScaled(ofGetWidth(), ofGetHeight(), 1.0);

	ofFill();
	glDisable(GL_DEPTH_TEST);
	ofEnableAlphaBlending();
	glColor4f(1.0, 1.0,1.0, masterAlpha);
	
	spotlightImage.draw(center.x-(radius*radiusMultiplier), center.y-(radius*radiusMultiplier)-0.02, radius*radiusMultiplier*2, radius*radiusMultiplier*2);
		
	glPopMatrix();
	
}

void Spotlight::drawOnFloor(){
	/**
	ofPushStyle();
	
	ofFill();
	glDisable(GL_DEPTH_TEST);
	ofEnableAlphaBlending();
	glColor4f(1.0, 1.0,1.0, masterAlpha);
	
	spotlightImage.draw(center.x-(radius*radiusMultiplier), center.y-(radius*radiusMultiplier)+0.03, radius*radiusMultiplier*2, radius*radiusMultiplier*2);

	ofPopStyle();
	**/
}

void Spotlight::update(){
	
	ofxVec2f centerDst;
	ofxVec2f boundingRectNW;
	ofxVec2f boundingRectSE;
	float radiusDst;
	
	minX = 100;
	minY = 100;
	maxX = -100;
	maxY = -100;
	
	if(blob(cam)->numBlobs() > 0){
		
		int numBlobsBigEnough = 0;
		
		for(int i=0;i<blob(cam)->numBlobs();i++){
			
			if(blob(cam)->getBlob(i).area > 0.001){
				ofxVec2f c = (blob(cam)-> getBlob(i).centroid);
				
				if(c.x < minX)
					minX = c.x;
				if (c.y < minY)
					minY = c.y;
				if(c.x > maxX)
					maxX = c.x;
				if (c.y > maxY)
					maxY = c.y;
								
				centerDst += c;
				radiusDst += sqrt(blob(cam)->getBlob(i).area);
								
				numBlobsBigEnough++;
			}
		}
		
		centerDst /= numBlobsBigEnough;
		radiusDst /= numBlobsBigEnough;
		radiusDst *= 2;
		
		radiusDst += sqrt((maxX-minX) * (maxY - minY));
		
		radiusDst += 0.1;
	
	} else {
		
		if(radius > 0.01){
			radiusDst = 0.0;
			radius *=.9;
		} else {
			center = ofxVec2f(0.5,0.5);
			centerDst = ofxVec2f(0.5,0.5);
			radiusDst = 0.0;
			radius = 0.0;
		}

	}

	center += ((centerDst - center).length() < 0.04)?(centerDst - center) * 0.0005:(centerDst - center) * 0.01;
	radius += (fabs(radiusDst - radius) < 0.04)?0:fmin(fabs(radiusDst - radius) * 0.005 ,0.025)*((radiusDst - radius < 0)?-0.25:1.0);
	
	radius = 0.2;

}