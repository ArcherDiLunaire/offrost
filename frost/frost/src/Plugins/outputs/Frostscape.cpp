
#include "PluginController.h"
#include "PluginIncludes.h"

float Frostscape::randomFactor = 5.0;

BlackSpotObject::BlackSpotObject(){
}


void BlackSpotObject::updateBlob(ofxCvBlob b, PluginController * controller){
	vector<ofxVec2f> tmpPoints, tmpPoints2;
	ProjectionSurfaces* proj = getPlugin<ProjectionSurfaces*>(controller);
	//points.clear();
	
	for(int p=0;p<b.nPts;p++){
		ofxVec2f r = proj->convertToCoordinate(proj->getFloor(), ofxVec2f(b.pts[p].x,b.pts[p].y));			
		tmpPoints.push_back(r);
	}
	
	contourSimp.simplify(tmpPoints, tmpPoints2, 0.001);
	/*while (points.size() > 100) {
	 contourSimp.simplify(tmpPoints, points, 0.001);
	 }*/
	if(points.size()<tmpPoints2.size()){
		for(int i=0;i<tmpPoints2.size()-points.size();i++){
			points.push_back(ofxVec2f());
			pointsV.push_back(ofxVec2f());
		}
		
	}
	//	cout<<points.size()<<"   "<<tmpPoints2.size()<<endl;
	
	float tmpId = 0;
	float a = (float)tmpPoints2.size()/points.size();
	for(int i=0;i<points.size();i++){
		pointsV[i] *= 0.95;
		pointsV[i] += (tmpPoints2[floor(tmpId)]-points[i])/10.0;
		points[i] += pointsV[i]*1.0/ofGetFrameRate();
		tmpId += a;
	}
	
	
}

void BlackSpotObject::draw(){
	/*for(int i=0;i<10;i++){
	 glBegin(GL_POLYGON);
	 for(int p=0;p<points.size();p++){
	 glVertex2f(points[p].x+ofRandom(-0.01, 0.01)*Frostscape::randomFactor,points[p].y+ofRandom(-0.01, 0.01)*Frostscape::randomFactor);
	 }
	 glEnd();
	 }*/
	/*	glBegin(GL_POLYGON);
	 for(int p=0;p<points.size();p++){
	 glVertex2f(points[p].x,points[p].y);
	 }
	 glEnd();*/
	ofBeginShape();
	for(int p=0;p<points.size();p++){
		ofVertex(points[p].x,points[p].y);
	}
	ofEndShape(true);
}



Frostscape::Frostscape(){
	type = OUTPUT;
	cam = 0;
}

void Frostscape::setup(){
	blackSpots.push_back(BlackSpotObject());
	blackSpots.push_back(BlackSpotObject());
}

void Frostscape::update(){
	for(int i=0;i<MIN(blob(cam)->numBlobs(),blackSpots.size());i++){
		ofxCvBlob b = blob(cam)->getBlob(i);
		blackSpots[i].updateBlob(b, controller);
	}
}
void Frostscape::draw(){
}

void Frostscape::drawOnFloor(){
	ofFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, projection()->getFloor()->aspect, 1);
	
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0,100);
	for(int i=0;i<blackSpots.size();i++){
		blackSpots[i].draw();
	}
	/*
	 ofEnableAlphaBlending();
	 ofSetColor(0, 0, 0,30);
	 for(int i=0;i<blob(cam)->numBlobs();i++){
	 ofxCvBlob b = blob(cam)->getBlob(i);
	 for(int u=0;u<10;u++){
	 glBegin(GL_POLYGON);
	 for(int p=0;p<b.nPts;p++){
	 glVertex2f(b.pts[p].x+ofRandom(-0.01, 0.01)*randomFactor,b.pts[p].y+ofRandom(-0.01, 0.01)*randomFactor);
	 }
	 glEnd();
	 }	
	 }*/
	
}
