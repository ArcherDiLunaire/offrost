
#include "PluginController.h"
#include "PluginIncludes.h"

float Frostscape::randomFactor = 5.0;
float Frostscape::slider1 = 0.0;
float Frostscape::slider2 = 0;
float Frostscape::slider3 = 0;
float Frostscape::slider4 = 0;
float Frostscape::slider5 = 0;
float Frostscape::slider6 = 0.0;

bool Frostscape::applyToOther = false;


#pragma mark Frostscape


Frostscape::Frostscape(){
	type = OUTPUT;
	cam = 0;
	invert = false;
}

void Frostscape::setup(){
	motor.generateBackgroundObjects(35, 1, projection()->getFloor()->aspect, 1.0, -3);
	iceMask.loadImage("iceMask.png");
	columnTexture.loadImage("columnTexture.png");
	for(int i=0;i<3;i++){
		columnParticleX[i] = 0;	
	}
}

void Frostscape::update(){
	if(resetLines){
		resetLines = false;
		lines[0].clear();
		lines[1].clear();
		linesOffset[0].clear();
		linesOffset[1].clear();
		linesFreezePoints.clear();
	}
	if(addingLines){
		for(int i=0;i<2;i++){
			if(blob(cam)->numPersistentBlobs() >= i+1){
				ofxPoint2f p = projection()->convertToFloorCoordinate(blob(cam)->persistentBlobs[i].getLowestPoint());
				if(lines[i].size() == 0 || p.distance(lines[i].back()) > ofRandom(0.05, 4)){
					lines[i].push_back(p);
					linesOffset[i].push_back(0.3*linesSpeed*ofxVec2f(ofRandom(-0.05, 0.05),ofRandom(-0.05, 0.05)));
				}
			}
		}		
	}
	//Freeze lines
	if(freezeLines > 0){
		for(int i=0;i<2;i++){
			if(ofRandom(0, 1) < 0.03*freezeLines && lines[i].size() > 0){
				linesFreezePoints.push_back(lines[i][floor(ofRandom(0, lines[i].size()-1))]);
			}
		}
	}
	
	for(int i=0;i<linesFreezePoints.size();i++){
		motor.addFreezePoint(linesFreezePoints[i], 0.03);
	}
	
	invert = false;
	motor.centerBreakRate =  Frostscape::slider1;
	motor.bodyBreakRate =  Frostscape::slider2;	
	motor.decreaseRate = Frostscape::slider5*10;
	motor.expandRate = Frostscape::slider6;
	if(invert){
		ofColor c;
		c.r = 0;
		c.g = 0;
		c.b = 0;
		motor.setColor(c);
	} else{
		ofColor c;
		motor.setColor(c);
	}
	
	if(sideFreeze > 0){
		float n = 3.0;
		for (int i=0;i<n; i++) {
			motor.addFreezePoint(ofPoint(0,i/n), sideFreeze);
		}
		for (int i=0;i<n; i++) {
			motor.addFreezePoint(ofPoint(projection()->getFloor()->aspect,i/n), sideFreeze);
		}
		for (int i=0;i<n; i++) {
			motor.addFreezePoint(ofPoint(projection()->getFloor()->aspect*i/n,0), sideFreeze);
			motor.addFreezePoint(ofPoint(projection()->getFloor()->aspect*i/n,1), sideFreeze);
			
		}
	}
	/*if(columnFreeze > 0.1){
	 for(int i=0;i<3;i++){
	 motor.addFreezePoint(projection()->getColumnCoordinate(i), columnFreeze);
	 
	 }
	 } else if(columnFreeze < -0.1){
	 for(int i=0;i<3;i++){
	 motor.addBodyCenter(projection()->getColumnCoordinate(i));
	 
	 }
	 }*/
	
	
	for(int i=0;i<3;i++){
		if(columnFreeze[i] > 0){
			//	int i=1;
			columnParticleX[i] += 0.003*60.0/ofGetFrameRate();
			columnParticlePos[i] += (sin(columnParticleX[i]*TWO_PI-HALF_PI)+1)*(0.003*60.0/ofGetFrameRate());	
			if(columnParticlePos[i] > 0  && columnParticlePos[i] < 5.0){
				//				cout<<"add "<<i<<"  "<<columnFreeze[i]<<endl;
				motor.addFreezePoint(projection()->getColumnCoordinate(i), columnFreeze[i]*0.01);	
			}
		} else {
			columnParticleX[i] = 0;
			columnParticlePos[i] = 0;	
		}
	} 

	for(int i=0;i<MIN(blob(cam)->numPersistentBlobs(),6);i++){
		PersistentBlob * pb = &blob(cam)->persistentBlobs[i];
		vector<ofxCvBlob> b = blob(cam)->persistentBlobs[i].blobs;
		ofxVec2f r = projection()->convertToCoordinate(projection()->getFloor(), pb->centroid);			
		
		motor.addBodyCenter(r);
		for(int g=0;g<b.size();g++){
			for(int u=0;u<b[g].nPts;u++){
				ofxVec2f r = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b[g].pts[u].x,b[g].pts[u].y));			
				
				motor.addBodyPoint(r);
			}
		}
		
	}
	
	/*
	 for(int i=0;i<MIN(blob(cam)->numPersistentBlobs(),2);i++){
	 PersistentBlob * pb = &blob(cam)->persistentBlobs[i];
	 //		vector<ofxCvBlob> b = blob(cam)->persistentBlobs[i].blobs;
	 ofxVec2f r = projection()->convertToCoordinate(projection()->getFloor(), pb->centroid);			
	 
	 motor.addFreezePoint(r,0.1);
	 
	 }
	 //*/
	
	motor.update();
}
void Frostscape::draw(){
	
}

void Frostscape::drawOnFloor(){
	ofEnableAlphaBlending();
	
	glDisable(GL_DEPTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);	

	invert = false;
	
	if(invert){
		ofFill();
		ofSetColor(255, 255, 255,255);
		ofRect(0, 0, projection()->getFloor()->aspect, 1);
	}
	
	ofSetColor(255, 255, 255,255*masterAlpha);

	motor.draw();

	if(whiteBackground > 0){
		ofFill();
		glColor4f(1.0, 1.0, 1.0,whiteBackground * masterAlpha);
		ofRect(0, 0, projection()->getFloor()->aspect, 1);
	}
	
	for(int u=0;u<2;u++){
		glColor4f(1.0, 1.0, 1.0, linesAlpha * masterAlpha);
		glBegin(GL_QUAD_STRIP);
		for(int i=0;i<lines[u].size();i++){
			glVertex2f(lines[u][i].x, lines[u][i].y);
			glVertex2f(lines[u][i].x+linesOffset[u][i].x, lines[u][i].y+linesOffset[u][i].y);
			
		}
		glEnd();
		
	}
	
	glPopMatrix();
	
	projection()->applyProjection(projection()->getFloor());
	
	ofEnableAlphaBlending();
	
	ofSetColor(255, 255, 255,255);
	iceMask.draw(0,0,projection()->getFloor()->aspect,1);
	
	ofFill();
	ofSetColor(0, 0, 0,255);
	ofRect(projection()->getFloor()->aspect, 0, 1, 1);			// right
	ofRect(-1, 1, 2 + projection()->getFloor()->aspect, 1);		// bottom
	ofRect(-1, 0, 1, 1);										// left
	ofRect(-1, -1, 2 + projection()->getFloor()->aspect, 1);	// top
		
	glPopMatrix();

	for(int i=0;i<3;i++){
		projection()->applyProjection(projection()->getColumn(i));
		
		ofFill();
		ofSetColor(255, 255, 255, 255 * masterAlpha);

		//ofEnableAlphaBlending();
		
		columnTexture.draw(-0.005, 0, projection()->getColumn(i)->aspect+0.01, 1);
		
		ofFill();
		ofSetColor(0, 0, 0, 255);
		ofRect(-0.005, 0, projection()->getColumn(i)->aspect+0.01, MIN( columnParticlePos[i],1));		

		glPopMatrix();
	}
	projection()->applyProjection(projection()->getFloor());
}

void Frostscape::fillIce(){
	motor.setValueOnAll(-3.0);
}
void Frostscape::emptyIce(){
	motor.freezePoints.clear();
	motor.setValueOnAll(1.0);	
}

void Frostscape::setslider1(float val){
	Frostscape::slider1 = val;
}

void Frostscape::setslider2(float val){
	Frostscape::slider2 = val;
}

void Frostscape::setslider3(float val){
	Frostscape::slider3 = val;
}

void Frostscape::setslider4(float val){
	Frostscape::slider4 = val;
}

void Frostscape::setslider5(float val){
	Frostscape::slider5 = val;
}

void Frostscape::setslider6(float val){
	Frostscape::slider6 = val;
}
