
#include "ProjectionSurfaces.h"

ProjectionSurfaces::ProjectionSurfaces(){
	type = DATA;
	floorProjection = new Warp();
	drawDebug = false;
}
void ProjectionSurfaces::setup(){
//	floorProjection->SetCorner(0, 0.1, 0);
	floorProjection->MatrixCalculate();
}
void ProjectionSurfaces::update(){
	
}
void ProjectionSurfaces::draw(){
	if(drawDebug){
		ofSetColor(255, 255, 0, 90);
		applyFloorProjection();	
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
		glPopMatrix();
	}
}

void ProjectionSurfaces::applyFloorProjection(){
	glPushMatrix();
	floorProjection->MatrixMultiply();
	glScaled(ofGetWidth(), ofGetWidth(), 1.0);
}