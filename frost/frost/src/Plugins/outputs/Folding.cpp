
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
	
	//	fbo.allocate(640, 480, true);
	
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
		historyImg.blur(9);
				
		//		*img = historyImg;
		cvCopy(historyImg.getCvImage(), history[histPos].getCvImage());
		history[histPos] = historyImg;
		history[histPos].flagImageChanged();
		
		cvCopy(blob(cam)->grayDiff.getCvImage(), now[histPos].getCvImage());
		now[histPos].flagImageChanged();
		now[histPos].blur(1);
		
		histPos ++;
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
	ofDisableAlphaBlending();
	
	projection()->applyCurtainProjection(0, 0); {
		glPushMatrix();	{
			glTranslated(-1.4-push1*projection()->getCurtain(0)->aspect, 0.4, 0);
			glPushMatrix(); { 
				glRotated(-25, 0, 0, 1.0);
				
				ofEnableAlphaBlending();
				glBlendFunc(GL_SRC_COLOR, GL_ONE);

				if(histPos > 0){
					ofSetColor(220 * masterAlpha * historyAlpha, 245 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
					history[histPos-1].draw(0,0,4,4);
					ofSetColor(255 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
					now[histPos-1].draw(0,0,4,4);
				}
				
			} glPopMatrix();
			
		} glPopMatrix();
		
		ofDisableAlphaBlending();

		ofSetColor(0, 0, 0);
		ofRect((1.0-push1)*projection()->getCurtain(0)->aspect, -1, 2, 2);
				
		ofRect(-1, 0, projection()->getCurtain(0)->aspect+1, -1); // top
		ofRect(-3, 1, projection()->getCurtain(0)->aspect+6, 4); // bottom
		
	} glPopMatrix();	
	

	projection()->applyCurtainProjection(0, 1); {
		glPushMatrix();	{
			glTranslated(-1.4-push2*projection()->getCurtain(1)->aspect, 0.4, 0);
			glPushMatrix(); { 
				glRotated(-25, 0, 0, 1.0);
				
				ofEnableAlphaBlending();
				glBlendFunc(GL_SRC_COLOR, GL_ONE);
				
				int b = histPos - 25.0*1.5;
				if(b < 0){
					b = history.size() + b;
				}
				ofSetColor(220 * masterAlpha * historyAlpha, 245 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
				history[b].draw(0,0,4,4);
				ofSetColor(255 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
				now[b].draw(0,0,4,4);
				
			} glPopMatrix();
			
		} glPopMatrix();
		
		ofDisableAlphaBlending();
		
		ofSetColor(0, 0, 0);
		ofRect((1.0-push2)*projection()->getCurtain(1)->aspect, 0, 2, 1);
		
		ofRect(-3, 1, projection()->getCurtain(1)->aspect+6, 4); // bottom
		
	} glPopMatrix();	
	
	
	projection()->applyCurtainProjection(0, 2); {
		glPushMatrix();	{
			glTranslated(-1.4-push3*projection()->getCurtain(1)->aspect, 0.4, 0);
			glPushMatrix(); { 
				glRotated(-25, 0, 0, 1.0);
				
				ofEnableAlphaBlending();
				glBlendFunc(GL_SRC_COLOR, GL_ONE);
				
				int b = histPos - 25.0*3;
				if(b < 0){
					b = history.size() + b;
				}
				ofSetColor(220 * masterAlpha * historyAlpha, 245 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
				history[b].draw(0,0,4,4);
				ofSetColor(255 * masterAlpha * historyAlpha, 255 * masterAlpha * historyAlpha,255 * masterAlpha * historyAlpha);
				now[b].draw(0,0,4,4);
				
			} glPopMatrix();
			
		} glPopMatrix();
		
		ofDisableAlphaBlending();
		
		ofSetColor(0, 0, 0);
		ofRect((1.0-push3)*projection()->getCurtain(2)->aspect, 0, 2, 1);
		
		ofRect(-3, 1, projection()->getCurtain(2)->aspect+6, 4); // bottom
		
	} glPopMatrix();	
				
	//Left mask
	ofFill();
	projection()->applyColumnProjection(0); {
		ofDisableAlphaBlending();
		ofSetColor(0, 0, 0,255);
		ofRect(projection()->getColumn(0)->aspect, 0, -100, 100);
	} glPopMatrix();	

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
		glRotated(25, 0, 0, 1);
		ofSetColor(255 * masterAlpha, 255 * masterAlpha, 255 * masterAlpha);
		ofFill();
		ofRect(0.625, 0.12, 0.28*foldingFloorbox, 0.125);
	}
	ofPopStyle();
	glPopMatrix();
}