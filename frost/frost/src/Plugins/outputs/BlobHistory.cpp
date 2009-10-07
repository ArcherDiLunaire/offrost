/*
 *  BlobHistory.cpp
 *  openFrameworks
 *
 *  Created by frost on 06/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "PluginController.h"
#include "PluginIncludes.h"

BlobHistory::BlobHistory(){
	type = OUTPUT;
	bIsRecording = false;
	cam = 1;
	masterAlpha = 1.0;
}

#pragma mark Callback methods

void BlobHistory::setup(){
	
}

void BlobHistory::draw(){
		
}

void BlobHistory::drawOnFloor(){

	ofPushStyle();

	ofEnableAlphaBlending();
	glColor4d(1.0, 1.0, 1.0, masterAlpha);
		
	for (int i=0; i < blobHistoryMatrix.size(); i++) {
		for (int j=0; j < blobHistoryMatrix[i].size(); j++) {
			
			ofxCvBlob b = blobHistoryMatrix[i][j];
			
			ofBeginShape();
			
			for (int p = 0; p < b.nPts; p++) {
				
				ofxVec2f v = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b.pts[p].x, b.pts[p].y));
				ofVertex(v.x, v.y);

			}

			ofEndShape();
			
		}
	}
	

	ofPopStyle();
	
}

void BlobHistory::update(){
	
	if (bIsRecording) {
		vector<ofxCvBlob> blobList;
		for (int i=0; i < blob(cam)->numBlobs(); i++) {
			blobList.push_back(blob(cam)->getBlob(i));
		}
		blobHistoryMatrix.push_back(blobList);
	}
	
}