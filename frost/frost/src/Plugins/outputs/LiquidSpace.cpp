/*
 *  LiquidSpace.cpp
 *  openFrameworks
 *
 *  Created by frost on 30/09/09.
 *
 */

#include "PluginController.h"
#include "PluginIncludes.h"

LiquidSpace::LiquidSpace(){
	type = OUTPUT;
	cam = 1;
}

void LiquidSpace::setup(){

}

void LiquidSpace::update(){
	
}

void LiquidSpace::draw(){
	projection()->applyFloorProjection(0, 0);
	ofSetColor(255, 255,255);
	//ofRect(0, 0, 1, 1);
	glPushMatrix();
	glTranslated(-1.4, 0.4, 0);
	glRotated(-25, 0, 0, 1.0);
	blob(1)->grayDiff.draw(0,0,4,4);
	glPopMatrix();
}