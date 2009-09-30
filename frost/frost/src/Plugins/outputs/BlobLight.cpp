
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
	addblack = 0;
}

void BlobLight::setup(){
	w = (1280.0/2.0)/2.0;
	h = (960.0/2.0)/2.0;
	history.allocate(w,h);
	historyTmp.allocate(w,h);
	black.allocate(w,h);
	black.set(0);
	history.set(0);
	historyTmp.set(0);
	img.allocate(w,h);
}

void BlobLight::update(){
	
}
void BlobLight::draw(){
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

	
	cvAddWeighted(history.getCvImage(),alpha, img.getCvImage(),beta,0.0, history.getCvImage());
	cvSubS(history.getCvImage(), cvScalar(addblack*100) , history.getCvImage());

	history.flagImageChanged();
	
	ofEnableAlphaBlending();
	glBlendFunc (GL_SRC_COLOR, GL_ONE);	

	ofSetColor(historyalpha*r, historyalpha*g, historyalpha*b, historyalpha*255);
	history.draw(0,0,1,1);
	
	
	ofSetColor( blobalpha*r,  blobalpha*g,  blobalpha*b, blobalpha*255);
	img.draw(0, 0, 1,1);
	glPopMatrix();
	
//	img.draw(0, 0);

	
}

void BlobLight::drawOnFloor(){

}

