
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
	
	//	fbo.allocate(640, 480, true);
	
}

void Folding::update(){
	//	float historyMultipler = pow(sin(historyAddMultiplier/2*PI), 2);
	// float historyMultipler = 1.0-exp(-5*pow(historyAddMultiplier,2));
	// float historyMultipler = 1.0+(-pow(historyAddMultiplier-1.0, 2));
	if (getPlugin<Cameras*>(controller)->isFrameNew(cam) ) {
		float historyMultipler = 1.0+(pow(historyAddMultiplier-1.0, 3));
		cvAddWeighted( historyImg.getCvImage(),historyMultipler, blob(cam)->grayDiff.getCvImage(),1, -0.25, historyImgTemp.getCvImage());
		historyImg = historyImgTemp;
		historyImg.blur(9);
		
		
		
		//		*img = historyImg;
		cvCopy(historyImg.getCvImage(), history[histPos].getCvImage());
		history[histPos] = historyImg;
		history[histPos].flagImageChanged();
		
		cvCopy(blob(cam)->grayDiff.getCvImage(), now[histPos].getCvImage());
		now[histPos].flagImageChanged();
		
		histPos ++;
		if(histPos >= history.size())
			histPos = 0;
	}
	
	/*if (getPlugin<Cameras*>(controller)->isFrameNew(cam) || blob(cam)->mouseBlob ) {
	 GLuint index = glGenLists(1);
	 blobHistoryMatrixDisplayList.push_back(index);
	 vector<ofxCvBlob> blobList;
	 
	 glNewList(blobHistoryMatrixDisplayList.back(), GL_COMPILE);
	 
	 for (int i=0; i < blob(cam)->numBlobs(); i++) {
	 blobList.push_back(blob(cam)->getBlob(i));
	 
	 ofxCvBlob b = blobList.back();
	 
	 ofBeginShape();
	 
	 
	 for (int p = 0; p < b.nPts; p++) {
	 
	 ofxVec2f v = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b.pts[p].x, b.pts[p].y));
	 ofVertex(v.x, v.y);
	 
	 }
	 
	 ofEndShape();
	 
	 }
	 
	 glEndList();
	 }
	 
	 while(blobHistoryMatrixDisplayList.size() > 10*50){
	 glDeleteLists(blobHistoryMatrixDisplayList.front(), 1);
	 blobHistoryMatrixDisplayList.erase(blobHistoryMatrixDisplayList.begin());
	 }*/
}

void Folding::draw(){
	//ofPushStyle();
	ofEnableAlphaBlending();
	glBlendFunc (GL_SRC_COLOR, GL_ONE);	
	ofSetColor(255, 255,255);
	
	
	projection()->applyCurtainProjection(0, 0);
	glTranslated(-1.4, 0.4, 0);
	glRotated(-25, 0, 0, 1.0);
	//historyImg.draw(0,0,4,4);
	if(histPos > 0){
		history[histPos-1].draw(0,0,4,4);
		now[histPos-1].draw(0,0,4,4);
	}
	//blob(cam)->grayDiff.draw(0,0,4,4);
	glPopMatrix();	
	
	
	projection()->applyCurtainProjection(0, 1);
	glTranslated(-1.4, 0.4, 0);
	glRotated(-25, 0, 0, 1.0);
	//historyImg.draw(0,0,4,4);
	
	int b = histPos - 25.0*3;
	if(b < 0){
		b = history.size() + b;
	}	
	history[b].draw(0,0,4,4);	
	now[b].draw(0,0,4,4);	

	glPopMatrix();	
	
	
	projection()->applyCurtainProjection(0, 2);
	glTranslated(-1.4, 0.4, 0);
	glRotated(-25, 0, 0, 1.0);
	//historyImg.draw(0,0,4,4);
	
	b = histPos - 25.0*6;
	if(b < 0){
		b = history.size() + b;
	}	
	history[b].draw(0,0,4,4);	
	now[b].draw(0,0,4,4);	
	glPopMatrix();	
	
	
	
	
	/*
	 ofPushStyle();
	 ofEnableAlphaBlending();
	 //	glBlendFunc (GL_SRC_COLOR, GL_ONE);	
	 projection()->applyFloorProjection();
	 ofSetColor(255, 255,255);
	 //ofRect(0, 0, 1, 1);
	 glColor4d(1.0, 1.0, 1.0,  1.0);
	 ofFill();
	 if (blobHistoryMatrixDisplayList.size() > 0) {
	 glCallList(blobHistoryMatrixDisplayList[(blobHistoryMatrixDisplayList.size()-1)]);
	 //blob(cam)->grayDiff.draw(0,0,4,4);
	 
	 float a = 1.0;
	 for(int i=1;i<historyAddMultiplier*60;i++){
	 a /= 1.3;
	 glColor4d(1.0, 1.0, 1.0,  a);
	 glCallList(blobHistoryMatrixDisplayList[MAX((blobHistoryMatrixDisplayList.size()-i),0)]);
	 
	 
	 }
	 
	 }
	 glPopMatrix();
	 
	 ofPopStyle();
	 
	 
	 ofPushStyle();
	 
	 ofEnableAlphaBlending();
	 glBlendFunc (GL_SRC_COLOR, GL_ONE);	
	 projection()->applyFloorProjection();
	 ofSetColor(255, 255,255);
	 //ofRect(0, 0, 1, 1);
	 glColor4d(1.0, 1.0, 1.0,  1.0);
	 glTranslated(0, 0.2, 0);
	 
	 //	fbo.swapIn();
	 //fbo.setupScreenForMe();
	 
	 if (blobHistoryMatrixDisplayList.size() > 0) {
	 glCallList(blobHistoryMatrixDisplayList[MAX((blobHistoryMatrixDisplayList.size()-25*3),0)]);
	 }	//blob(cam)->grayDiff.draw(0,0,4,4);
	 
	 
	 //	fbo.swapOut();
	 //fbo.setupScreenForThem();
	 
	 //	projection()->applyFloorProjection();
	 
	 glPopMatrix();
	 
	 ofPopStyle();
	 
	 glPopMatrix();
	 
	 */		
}

