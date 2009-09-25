
#include "PluginController.h"
#include "PluginIncludes.h"



LaLinea::LaLinea(){
	type = OUTPUT;
	cam = 2;
}

void LaLinea::setup(){
}

void LaLinea::update(){
	
}
void LaLinea::draw(){
}

void LaLinea::drawOnWall(){
	int sections = 300;
	float s[sections];
	for(int i=0;i<sections;i++){
		s[i] = 0.9;
	}
	
	for(int i=0;i<blob(cam)->numBlobs();i++){
		ofxCvBlob b = blob(cam)->getBlob(i);
		
		glColor4f(255, 0, 255,255);
		
		glBegin(GL_LINE_STRIP);
		
		for(int i=0;i<b.nPts-3;i+=3){
			int section = 0;
			ofxVec2f r = projection()->convertToCoordinate(projection()->getWall(), ofxVec2f(b.pts[i].x, b.pts[i].y));			
			for(int u=0;u<sections;u++){
				if(r.x < u*1.0/sections){
					section = u;
					break;	
				}
			}
			if(s[section] > r.y){
				s[section] = r.y;
			}

			glVertex3d(r.x, r.y,0);
		}
		glEnd();	
	}
	
	ofSetColor(255, 255, 255);
	ofSetLineWidth(4);
	
	float x = 0;
	for(int i=0;i<sections-1;i++){ 
		ofLine(x, s[i], x+1.0/sections, s[i+1]);		
		x += 1.0/sections;
	}
	
}

