
#include "PluginController.h"
#include "PluginIncludes.h"



BlobLight::BlobLight(){
	type = OUTPUT;
	blur = 0;
	blur2 = 0;
	threshold = 0;
	r = 255;
	g = 255;
	b = 255;
	alpha = 0.0;
	beta = 1.0;
}

void BlobLight::setup(){
	w = (1280/2)/4;
	h = (960/2)/4;
	history.allocate(w,h);
	historyTmp.allocate(w,h);

	img.allocate(w,h);
}

void BlobLight::update(){
	
}
void BlobLight::draw(){
	ofSetColor(r, g, b, 255);
	ofxCvGrayscaleImage Largeimg = blob(0)->grayDiff;
	img.scaleIntoMe(Largeimg);
	getPlugin<CameraCalibration*>(controller)->applyWarp(0);
	if(blur > 0)
		img.blur(blur);
	if(threshold > 0){
		img.threshold(threshold, false);
	}
	if(blur2 > 0){
		img.blurGaussian(blur2);
	}
	cvAddWeighted(history.getCvImage(),alpha, img.getCvImage(),(1.0-alpha),0.0, history.getCvImage());
	history.flagImageChanged();
	history.draw(0,0,1,1);
	glPopMatrix();

	
}

void BlobLight::drawOnFloor(){

}

