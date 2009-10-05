
#include "PluginController.h"
#include "PluginIncludes.h"


LaLineaFloor::LaLineaFloor(){
	type = OUTPUT;
	cam = 0;
	time = 0;
}

void LaLineaFloor::setup(){
	dir = ofxVec2f(1,0);
	
}

void LaLineaFloor::update(){
	//cout<<width<<endl;
	time += ofGetFrameRate()/100.0;
	if(blob(cam)->numPersistentBlobs() > 0){
		
		ofxPoint2f goal = projection()->convertToFloorCoordinate(blob(cam)->persistentBlobs[0].getLowestPoint());
//		ofxPoint2f goal = ofxVec2f(mouseX, mouseY);// + ofRandom(0.9,1.0)*ofxVec2f(cos(time/40.0), sin(time/10.0))/40.0;
		
		ofxVec2f v = goal - pos;
		v.normalize();
		
		dir += v*0.003;
		dir.normalize();
		dir *= 0.005;
		
		pos += dir*100.0/ofGetFrameRate();
		pnts.push_back(pos);
	}
}
void LaLineaFloor::draw(){
	
}

void LaLineaFloor::reset(){
	pnts.clear();
	pos = ofxVec2f();
	dir = ofxVec2f(1,0);

}

void LaLineaFloor::drawOnFloor(){
	ofSetColor(255, 255, 255, 255);
	ofEnableSmoothing();
	if(pnts.size() > 0){
		glBegin(GL_QUAD_STRIP);
		
		for(int i=0;i<pnts.size()-1;i++){
			ofxVec2f v = pnts[i+1] - pnts[i];
			ofxVec2f hat;
			hat.x = -v.y;
			hat.y = v.x;
			hat.normalize();
			hat *= 0.02*width;
			glVertex2f(pnts[i].x-hat.x, pnts[i].y-hat.y);
			glVertex2f(pnts[i].x+hat.x, pnts[i].y+hat.y);
		}
		glEnd();
	}
}

