
#include "PluginController.h"
#include "PluginIncludes.h"



Folding::Folding(){
	type = OUTPUT;
	cam = 1;
}

void Folding::setup(){
	historyImg.allocate(blob(cam)->grayDiff.width,blob(cam)->grayDiff.height);
	historyImg.set(0);
	historyImgTemp.allocate(blob(cam)->grayDiff.width,blob(cam)->grayDiff.height);
	historyImgTemp.set(0);
	
	histPos = 0;
	
	for(int i=0;i<25.0*10;i++){
		ofxCvGrayscaleImage  img;
		img.allocate(blob(cam)->grayDiff.width,blob(cam)->grayDiff.height);
		img.set(0);
		history.push_back(img);
		
		ofxCvGrayscaleImage  img2;
		img2.allocate(blob(cam)->grayDiff.width,blob(cam)->grayDiff.height);
		img2.set(0);
		now.push_back(img2);
	}
	
	fish.loadMovie("goldfishCrop.mov");
	
	reset = false;
	
	aspect = 1.0/projection()->getCurtain(0)->aspect;
	
	//	fbo.allocate(640, 480, true);
	
}

void Folding::getTrackingRect(ofxPoint2f * corners){
	ofxPoint2f col1 = projection()->getColumnCoordinate(0);
	ofxPoint2f col2 = projection()->getColumnCoordinate(1);
	
	ofxVec2f colVec = col2 - col1;	
	ofxVec2f centerVec = ofxVec2f(-colVec.y,colVec.x);
	
	ofxPoint2f center = col1 + colVec * (0.5 + 0.5*offsetX) + centerVec * (1.5 * + 1.0*offsetY);
	
	corners[0] = center - colVec * 0.5 * scale - centerVec * aspect * 0.5 * scale;
	corners[1] = center + colVec * 0.5 * scale - centerVec * aspect * 0.5 * scale;
	corners[2] = center + colVec * 0.5 * scale + centerVec * aspect * 0.5 * scale;
	corners[3] = center - colVec * 0.5 * scale + centerVec * aspect * 0.5 * scale;
	
	for(int i=0;i<4;i++){
		corners[i] = projection()->convertToProjectionCoordinate(projection()->getFloor(), corners[i]);
	}	
}

void Folding::update(){
	
	if(reset){
		
		for(int i=0;i<25.0*10;i++){
			history[i].set(0);
			now[i].set(0);
		}
		
		reset = false;
	}
	
	//	float historyMultipler = pow(sin(historyAddMultiplier/2*PI), 2);
	// float historyMultipler = 1.0-exp(-5*pow(historyAddMultiplier,2));
	// float historyMultipler = 1.0+(-pow(historyAddMultiplier-1.0, 2));
	if (getPlugin<Cameras*>(controller)->isFrameNew(cam) && updateHistoryFromBlob) {
		
		if(histPos >= history.size())
			histPos = 0;
		
		float historyMultipler = 1.0+(pow(historyAddMultiplier-1.0, 3));
		cvAddWeighted( historyImg.getCvImage(),historyMultipler, blob(cam)->grayDiff.getCvImage(),1, -0.25, historyImgTemp.getCvImage());
		historyImg = historyImgTemp;
		historyImg.blur(3);
		
		//		*img = historyImg;
		cvCopy(historyImg.getCvImage(), history[histPos].getCvImage());
		history[histPos] = historyImg;
		history[histPos].flagImageChanged();
		
		cvCopy(blob(cam)->grayDiff.getCvImage(), now[histPos].getCvImage());
		now[histPos].flagImageChanged();
		now[histPos].blur(2);
		
		histPos ++;
	}
	
}

void Folding::drawImage(ofxCvGrayscaleImage img, float pushFactor){
	float w = img.width;
	float h = img.height;
	
	ofxPoint2f corners[4];
	getTrackingRect(corners);
	
	ofxPoint2f cameraCorners[4];
	
	for(int i=0;i<4;i++){
		
		cameraCorners[i] = getPlugin<CameraCalibration*>(controller)->cameras[1]->coordWarp->inversetransform(corners[i].x, corners[i].y);
	}
	
	
	img.draw(0, 0,0,0); //Hack for at lave nogle interne kald for at generere texture
	
	img.getTextureReference().bind();
	glBegin(GL_QUADS);
	
	glTexCoord2f(cameraCorners[0].x*w, cameraCorners[0].y*h);	glVertex2f(0.0, 0.0);
	glTexCoord2f(cameraCorners[1].x*w, cameraCorners[1].y*h);	glVertex2f(1.0/aspect, 0.0);
	glTexCoord2f(cameraCorners[2].x*w, cameraCorners[2].y*h);	glVertex2f(1.0/aspect, 1.0);
	glTexCoord2f(cameraCorners[3].x*w, cameraCorners[3].y*h);	glVertex2f(0.0, 1.0);
	
	glEnd();
	img.getTextureReference().unbind();
}

void Folding::draw(){
	//ofPushStyle();
	ofDisableAlphaBlending();
	
	//UPPER CURTAIN
	
	projection()->applyCurtainProjection(0, 0); {
		glPushMatrix();	{
			ofEnableAlphaBlending();
			glBlendFunc(GL_SRC_COLOR, GL_ONE);
			
			if(histPos > 0){
				ofSetColor(250 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
				drawImage(history[histPos-1], push1);
				ofSetColor(255 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
				drawImage(now[histPos-1], push1);
			}
			
		} glPopMatrix();
		ofDisableAlphaBlending();
	} glPopMatrix();	
	
	//MIDDLE CURTAIN
	
	projection()->applyCurtainProjection(0, 1); {
		glPushMatrix();	{
			ofEnableAlphaBlending();
			glBlendFunc(GL_SRC_COLOR, GL_ONE);
			
			int b = histPos - 25.0*1.5;
			if(b < 0){
				b = history.size() + b;
			}
			ofSetColor(250 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
			drawImage(history[b], push2);
			ofSetColor(255 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
			drawImage(now[b], push2);
			
		} glPopMatrix();
		ofDisableAlphaBlending();
	} glPopMatrix();	
	
	//LOWER CURTAIN
	
	projection()->applyCurtainProjection(0, 2); {
		glPushMatrix();	{
			ofEnableAlphaBlending();
			glBlendFunc(GL_SRC_COLOR, GL_ONE);
			
			int b = histPos - 25.0*3;
			if(b < 0){
				b = history.size() + b;
			}
			ofSetColor(250 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
			drawImage(history[b], push3);
			ofSetColor(255 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
			drawImage(now[b], push3);
			
		} glPopMatrix();
		ofDisableAlphaBlending();
	} glPopMatrix();	
	
	//FISH
	
	if(fishAlpha > 0){
		fish.setLoopState(OF_LOOP_NORMAL);
		fish.play();
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(255, 255, 255, 255.0*fishAlpha * masterAlpha);
		projection()->applyProjection(projection()->getCurtain(4));
		fish.draw(0, 0, 1.37,1);
		glPopMatrix();
	} 
}

void Folding::drawOnFloor(){
	glPushMatrix();
	ofPushStyle();
	if(foldingFloorbox > 0){
		ofxPoint2f points[4];
		getTrackingRect(points);
		
		//RECTANGLE
		ofSetColor(255 * masterAlpha * foldingFloorboxAlpha, 255 * masterAlpha * foldingFloorboxAlpha, 255 * masterAlpha * foldingFloorboxAlpha);
		ofFill();
		
		glBegin(GL_QUADS);
		for(int i=0;i<4;i++){
			points[i] = projection()->convertToFloorCoordinate(points[i]);
			glVertex2f(points[i].x, points[i].y);
		}
		glEnd();		
	}
	ofPopStyle();
	glPopMatrix();
}