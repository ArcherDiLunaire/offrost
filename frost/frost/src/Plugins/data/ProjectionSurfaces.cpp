
#include "ProjectionSurfaces.h"

ProjectionSurfaces::ProjectionSurfaces(){
	type = DATA;
	floorProjection = new Warp();
	drawDebug = false;
	gridFloorResolution = 10;
	selectedCorner = 0;
}
void ProjectionSurfaces::setup(){
//	floorProjection->SetCorner(0, 0.1, 0);
	floorProjection->MatrixCalculate();
}
void ProjectionSurfaces::update(){
	
}
void ProjectionSurfaces::draw(){
	if(drawDebug){
		applyFloorProjection();	
		ofSetColor(255, 255, 255);
		for(int i=0;i<gridFloorResolution;i++){
			ofLine(0, i*1.0/gridFloorResolution, 1.0, i*1.0/gridFloorResolution);
		}
		for(int i=0;i<gridFloorResolution;i++){
			ofLine(i*1.0/gridFloorResolution, 0, i*1.0/gridFloorResolution, 1.0);
		}
		glPopMatrix();
	}
}

void ProjectionSurfaces::drawSettings(){
	glPushMatrix();
	glScaled((glDelegate->m_Width/2.0), (glDelegate->m_Width/2.0), 1.0);
	for(int i=0;i<4;i++){
		ofSetColor(255,0, 0);
		if(selectedCorner == i){
			ofSetColor(255,255, 0);
		}
		ofxVec2f v = floorProjection->corners[i];
		ofRect(v.x, v.y, 0.05, 0.05);
	}	
	glPopMatrix();
}

void ProjectionSurfaces::applyFloorProjection(){
	glPushMatrix();
	floorProjection->MatrixMultiply();
	glScaled(ofGetWidth(), ofGetWidth(), 1.0);
}