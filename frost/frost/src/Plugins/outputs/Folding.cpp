
#include "PluginController.h"
#include "PluginIncludes.h"



Folding::Folding(){
	type = OUTPUT;
	cam = 0;
}

void Folding::setup(){
	historyImg.allocate(blob(cam)->grayDiff.width,blob(cam)->grayDiff.height);
	historyImg.set(0);
	historyImgTemp.allocate(blob(cam)->grayDiff.width,blob(cam)->grayDiff.height);
	historyImgTemp.set(0);
}

void Folding::update(){
	//	float historyMultipler = pow(sin(historyAddMultiplier/2*PI), 2);
	// float historyMultipler = 1.0-exp(-5*pow(historyAddMultiplier,2));
	// float historyMultipler = 1.0+(-pow(historyAddMultiplier-1.0, 2));
	float historyMultipler = 1.0+(pow(historyAddMultiplier-1.0, 3));
	cvAddWeighted( historyImg.getCvImage(),historyMultipler, blob(cam)->grayDiff.getCvImage(),1, -0.25, historyImgTemp.getCvImage());
	historyImg = historyImgTemp;
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
	//blob(cam)->grayDiff.draw(0,0,4,4);
	glPopMatrix();
	
	ofPopStyle();
}

