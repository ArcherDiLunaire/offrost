
#include "PluginController.h"
#include "PluginIncludes.h"



BlobLight::BlobLight(){
	type = OUTPUT;
	blur = 0;
	r = 255;
	g = 255;
	b = 152;
	
}

void BlobLight::setup(){
	
}

void BlobLight::update(){
	
}
void BlobLight::draw(){
	ofSetColor(r, g, b, 255);
	ofxCvGrayscaleImage img = blob(0)->grayDiff;
	getPlugin<CameraCalibration*>(controller)->applyWarp(0);
	if(blur > 0)
		img.blur(blur);
	img.draw(0,0,1,1);
	glPopMatrix();

	
}

void BlobLight::drawOnFloor(){

}

