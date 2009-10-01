
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


BlackSpotObject::BlackSpotObject(){
}


void BlackSpotObject::updateBlob(ofxCvBlob b, PluginController * controller, BlackSpotObject * otherObject){
	ProjectionSurfaces* proj = getPlugin<ProjectionSurfaces*>(controller);
	//points.clear();
	tmpPoints.clear();
	
	for(int p=0;p<b.nPts;p++){
		ofxVec2f r = proj->convertToCoordinate(proj->getFloor(), ofxVec2f(b.pts[p].x,b.pts[p].y));			
		tmpPoints.push_back(r);
	}
	
	if(points.size()>0){
		contourNorm.makeNormals(tmpPoints, normals);
	}
	
	for(int p=0;p<points.size();p++){
		tmpPoints[p] -= normals[p] * 0.03;
	}
	
	contourSimp.simplify(&tmpPoints, &tmpPoints2, 0.01*Frostscape::slider1);
	
	/*while (points.size() > 100) {
	 contourSimp.simplify(tmpPoints, points, 0.001);
	 }*/
	if(points.size()<MIN(tmpPoints2.size(),100)){
		for(int i=0;i<MIN(tmpPoints2.size(),100)-points.size();i++){
			points.push_back(ofxVec2f());
			pointsV.push_back(ofxVec2f());
			vector<ofxVec2f> v;
			for(int i=0;i<numNoise;i++){
				v.push_back(ofxVec2f());
			}
			noise.push_back(v);
		}
		
	}
	
	
	//	cout<<points.size()<<"   "<<tmpPoints2.size()<<endl;
	
	float tmpId = 0;
	
	centroidV = ofxVec2f();
	for(int i=0;i<pointsV.size();i++){
		centroidV += pointsV[i];
	}
	centroidV /= pointsV.size();
	if(Frostscape::applyToOther){
		float a = (float)tmpPoints2.size()/MIN(points.size(),otherObject->points.size());

		for(int i=0;i<MIN(points.size(),otherObject->points.size());i++){
			otherObject->pointsV[i] *= Frostscape::slider2;
			otherObject->pointsV[i] += (tmpPoints2[floor(tmpId)]-points[i])*10.0*Frostscape::slider3;
			otherObject->points[i] += pointsV[i]*1.0/ofGetFrameRate();
			for(int u=0;u<numNoise;u++){
				otherObject->noise[i][u]  *= Frostscape::slider6;
				
				otherObject->noise[i][u] = otherObject->noise[i][u] + ofxVec2f(ofRandom(-1, 1),ofRandom(-1, 1)) * (otherObject->pointsV[i] - otherObject->centroidV).length()*0.1*Frostscape::slider4;
			}
			tmpId += a;
		}

	} else {
		float a = (float)tmpPoints2.size()/points.size();

		for(int i=0;i<points.size();i++){
			//	cout<<i<<endl;
			pointsV[i] *= Frostscape::slider2;
			pointsV[i] += (tmpPoints2[floor(tmpId)]-points[i])*10.0*Frostscape::slider3;
			points[i] += pointsV[i]*1.0/ofGetFrameRate();
			for(int u=0;u<numNoise;u++){
				noise[i][u]  *= Frostscape::slider6;
				
				noise[i][u] = noise[i][u] + ofxVec2f(ofRandom(-1, 1),ofRandom(-1, 1)) * (pointsV[i] - centroidV).length()*0.1*Frostscape::slider4;
			}
			
			/*	for(int u=0;u<numNoise;u++){
			 //noise[i][u]  *= 0.9;
			 noise[i][u] = -normals[i] * pointsV[i].length()*0.1;
			 }*/
			tmpId += a;
		}
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
	if(points.size()>0){
		for(int i=0;i<noise[0].size();i++){
			ofBeginShape();
			for(int p=0;p<points.size();p++){
				ofVertex(points[p].x+noise[p][i].x,points[p].y+noise[p][i].y);
			}
			/*for(int p=0;p<points.size();p++){
			 ofVertex(points[p].x,points[p].y);
			 }*/
			ofEndShape(true);
			
		}
	}
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
	blob(0)->postBlur = 100*Frostscape::slider5;
	blob(0)->postThreshold = 0;
	
	BlackSpotObject * otherB = &blackSpots[1];
	for(int i=0;i<MIN(blob(cam)->numPersistentBlobs(),blackSpots.size());i++){
		ofxCvBlob b = blob(cam)->persistentBlobs[i].blob;
		blackSpots[i].centroidV = blob(cam)->persistentBlobs[i].centroidV;
		blackSpots[i].updateBlob(b, controller, otherB);
		otherB = &blackSpots[i];
	}
}
void Frostscape::draw(){
}

void Frostscape::drawOnFloor(){
	ofFill();
	ofSetColor(128, 128, 128);
	ofRect(0, 0, projection()->getFloor()->aspect, 1);
	
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0,128);
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
