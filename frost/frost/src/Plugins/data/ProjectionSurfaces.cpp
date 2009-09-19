
#include "ProjectionSurfaces.h"

ProjectionSurfaces::ProjectionSurfaces(){
	type = DATA;
	floorProjection = new Warp();
	drawDebug = false;
	gridFloorResolution = 10;
	selectedCorner = 0;
}

void ProjectionSurfaces::guiWakeup(){
	ofAddListener(glDelegate->mousePressed,this, &ProjectionSurfaces::mousePressed);
	ofAddListener(glDelegate->mouseDragged,this, &ProjectionSurfaces::mouseDragged);
	
	w = glDelegate->m_Width/2.0;
	h = glDelegate->m_Width/2.0;
	offset = 50;
}


void ProjectionSurfaces::setup(){
	floorProjection->SetCorner(0, 0.5, 0);
	floorProjection->MatrixCalculate();
	
	
	
}
void ProjectionSurfaces::update(){
	
}
void ProjectionSurfaces::draw(){
	if(drawDebug){
		applyFloorProjection();	
		ofSetColor(255, 255, 255);
		for(int i=0;i<=gridFloorResolution;i++){
			ofLine(0, i*1.0/gridFloorResolution, 1.0, i*1.0/gridFloorResolution);
		}
		for(int i=0;i<=gridFloorResolution;i++){
			ofLine(i*1.0/gridFloorResolution, 0, i*1.0/gridFloorResolution, 1.0);
		}
		glPopMatrix();
	}
}

void ProjectionSurfaces::drawSettings(){
	glPushMatrix();
	glTranslated(offset, offset, 0);
	glPushMatrix();
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255,70);
	ofRect(0, 0, w, h);
	
	glScaled(w, h, 1.0);
	for(int i=0;i<4;i++){
		ofSetColor(255,0, 0);
		if(selectedCorner == i){
			ofSetColor(255,255, 0);
		}
		ofxVec2f v = floorProjection->corners[i];
		ofEllipse(v.x, v.y, 0.05, 0.05);
	}	
	glPopMatrix();
	
	applyFloorProjection(w,h);	
	ofSetColor(255, 255, 255);
	for(int i=0;i<=gridFloorResolution;i++){
		ofLine(0, i*1.0/gridFloorResolution, 1.0, i*1.0/gridFloorResolution);
	}
	for(int i=0;i<=gridFloorResolution;i++){
		ofLine(i*1.0/gridFloorResolution, 0, i*1.0/gridFloorResolution, 1.0);
	}
	glPopMatrix();
	glPopMatrix();
	
}

void ProjectionSurfaces::mousePressed(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/w);
	selectedCorner = floorProjection->GetClosestCorner(curMouse.x, curMouse.y);
	lastMousePos = curMouse;
}

void ProjectionSurfaces::mouseDragged(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/w);
	ofxVec2f newPos = floorProjection->corners[selectedCorner] + (curMouse-lastMousePos);
	floorProjection->SetCorner(selectedCorner, newPos.x, newPos.y);
	lastMousePos = curMouse;
	floorProjection->MatrixCalculate();
}

void ProjectionSurfaces::applyFloorProjection(float _w, float _h){
	glPushMatrix();
	glScaled(_w,  _h, 1.0);
	floorProjection->MatrixMultiply();
	//	glScaled(ofGetWidth(), ofGetHeight(), 1.0);
}