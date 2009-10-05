
#include "PluginController.h"
#include "PluginIncludes.h"



Folding::Folding(){
	type = OUTPUT;
	cam = 1;
}

void Folding::setup(){
	historyImg.allocate(blob(cam)->grayDiff.width,blob(cam)->grayDiff.height);
	historyImg.set(0);
}

void Folding::update(){
	cvAddWeighted( blob(cam)->grayDiff.getCvImage(),(1.0-historyAddMultiplier), historyImg.getCvImage(),historyAddMultiplier,0.0, historyImg.getCvImage());
}

void Folding::draw(){
	ofPushStyle();
	ofEnableAlphaBlending();
	glBlendFunc (GL_SRC_COLOR, GL_ONE);	
	projection()->applyCurtainProjection(0, 0);
	ofSetColor(255, 255,255);
	//ofRect(0, 0, 1, 1);
	glPushMatrix();
	glTranslated(-1.4, 0.4, 0);
	glRotated(-25, 0, 0, 1.0);
	historyImg.draw(0,0,4,4);
	blob(cam)->grayDiff.draw(0,0,4,4);
	glPopMatrix();
	
	ofPopStyle();
}

