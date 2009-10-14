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
	bIsRecordingHistory = false;
	cam = 1;
	masterAlpha = 1.0;
	historyOffset = 0 ;
}

#pragma mark Callback methods

void BlobHistory::setup(){
	float s = 0.2;
	motor.generateBackgroundObjects(35.0/s, 1*s, projection()->getFloor()->aspect, 1.0, 1);
}

void BlobHistory::draw(){
		
}

void BlobHistory::drawOnFloor(){
	motor.draw();
	ofPushStyle();

	ofEnableAlphaBlending();
	glColor4d(1.0, 1.0, 1.0, masterAlpha);
	
	ofFill();
		
	glColor4d(1.0, 1.0, 1.0, historyAlpha * masterAlpha);

	if (historyOffset > blobHistoryMatrixDisplayList.size()) {
		historyOffset = 0;
		historyPlayStep = 0;
	}
	
	if (historyOffset < 0) {
		historyOffset = 0;
		historyOffset = 0;
		historyPlayStep = 0;
	}
	
	if (blobHistoryMatrixDisplayList.size() > 0) {
		glCallList(blobHistoryMatrixDisplayList[(blobHistoryMatrixDisplayList.size()-1)-historyOffset]);
	}
	
																																				  
	for (int i=0; i < blobSnapshotMatrix.size(); i++) {
		for (int j=0; j < blobSnapshotMatrix[i].size(); j++) {
			glColor4d(1.0, 1.0, 1.0, snapshotAlpha * masterAlpha);

			ofxCvBlob b = blobSnapshotMatrix[i][j];
			if(fill)
				ofFill();
			else {
				ofNoFill();
			}

			ofBeginShape();
			for (int p = 0; p < b.nPts; p++) {
				
				ofxVec2f v = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b.pts[p].x, b.pts[p].y));
				ofVertex(v.x, v.y);
				
			}
			
			ofEndShape();
			
			
			ofBeginShape();
			
			ofNoFill();
			ofSetColor(255, 255, 255, 64 * snapshotAlpha * masterAlpha);
			ofSetLineWidth(3);
			
			for (int p = 0; p < b.nPts; p++) {
				
				ofxVec2f v = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b.pts[p].x, b.pts[p].y));
				ofVertex(v.x, v.y);
				
			}
			
			ofEndShape();
			
			ofBeginShape();
			
			ofNoFill();
			ofSetColor(255, 255, 255, 128 * snapshotAlpha * masterAlpha);
			ofSetLineWidth(2);
			
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
	motor.decreaseRate = decreaseValue*10.0;
	motor.expandRate = growthValue;
	if(clearFreeze){
		clearFreeze = false;
		cout<<"Clear freeze"<<endl;
		freezePoints.clear();
	}
	for(int i=0;i<blobSnapshotMatrix.size();i++){
		for(int u=0;u<blobSnapshotMatrix[i].size();u++){
			for(int v=0;v<blobSnapshotMatrix[i][u].nPts;v++){
				if(ofRandom(0, 1) < 0.01*freezeRate){
					
					ofxPoint2f proj = projection()->convertToCoordinate(projection()->getFloor(), blobSnapshotMatrix[i][u].pts[v]);
					freezePoints.push_back(proj);
				}
			}
		}
	}
	for(int i=0;i<freezePoints.size();i++){
		motor.addFreezePoint(freezePoints[i], freezeSpeed*0.01);
	}

	motor.update();
	if (getPlugin<Cameras*>(controller)->isFrameNew(cam) ) {

		historyOffset += round(historyPlayStep);
	
		if (bIsRecordingHistory) {

			vector<ofxCvBlob> blobList;
			
			GLuint index = glGenLists(1);
			
			if (index == 0) {
				ofLog(OF_LOG_ERROR, "BlobHistory has filled the OpenGL display lists at: " + ofToString(1.0*blobHistoryMatrixDisplayList.size()) );
			}
			
			blobHistoryMatrixDisplayList.push_back(index);
			ofFill();

			glNewList(blobHistoryMatrixDisplayList.back(), GL_COMPILE);

			for (int i=0; i < blob(cam)->numBlobs(); i++) {
				blobList.push_back(blob(cam)->getBlob(i));
				
				ofxCvBlob b = blobList.back();
				
				
				ofFill();
				ofSetColor(255, 255, 255);

				ofBeginShape();
				
				for (int p = 0; p < b.nPts; p++) {
					
					ofxVec2f v = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b.pts[p].x, b.pts[p].y));
					ofVertex(v.x, v.y);
					
				}
		
				ofEndShape();
				
				ofBeginShape();
	
				ofNoFill();
				ofSetColor(255, 255, 255, 64);
				ofSetLineWidth(3);
				
				for (int p = 0; p < b.nPts; p++) {
					
					ofxVec2f v = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b.pts[p].x, b.pts[p].y));
					ofVertex(v.x, v.y);
					
				}
				
				ofEndShape();
				
				ofBeginShape();
				
				ofNoFill();
				ofSetColor(255, 255, 255, 128);
				ofSetLineWidth(2);
				
				for (int p = 0; p < b.nPts; p++) {
					
					ofxVec2f v = projection()->convertToCoordinate(projection()->getFloor(), ofxVec2f(b.pts[p].x, b.pts[p].y));
					ofVertex(v.x, v.y);
					
				}
				
				ofEndShape();
				
			}

			glEndList();
			
			blobHistoryMatrix.push_back(blobList);

		}
	}
	if (bClearIce) {
		motor.setValueOnAll(1);
		bClearIce = false;
	}

	if (bClearHistory) {
		ofLog(OF_LOG_WARNING, "HISTORY CLEARING" );
		blobHistoryMatrix.clear();
		
		for (int i=0; i < blobHistoryMatrixDisplayList.size(); i++) {
			glDeleteLists(blobHistoryMatrixDisplayList[i], 1);
		}
		historyOffset = 0 ;
		historyPlayStep = 0;
		bClearHistory = false;
	}
	
	if (bTakeSnapshot) {
		ofLog(OF_LOG_NOTICE, "ADDING PRINT" );
		vector<ofxCvBlob> blobList;
		for (int i=0; i < blob(cam)->numBlobs(); i++) {
			blobList.push_back(blob(cam)->getBlob(i));
		}
		blobSnapshotMatrix.push_back(blobList);
		ofLog(OF_LOG_NOTICE, "  length " + ofToString(blobSnapshotMatrix.size(),1));
		bTakeSnapshot = false;
	}

	if (bRemoveOldestSnapshot) {
		ofLog(OF_LOG_NOTICE, "REMOVING PRINT" );
		if (blobSnapshotMatrix.size() > 0) {
			blobSnapshotMatrix.erase(blobSnapshotMatrix.begin());
		}
		ofLog(OF_LOG_NOTICE, "  length " + ofToString(blobSnapshotMatrix.size(),1));
		bRemoveOldestSnapshot = false;
	}

	if (bClearSnapshots) {
		ofLog(OF_LOG_NOTICE, "CLEARING PRINTS" );
		blobSnapshotMatrix.clear();
		bClearSnapshots = false;
	}
	
	
}