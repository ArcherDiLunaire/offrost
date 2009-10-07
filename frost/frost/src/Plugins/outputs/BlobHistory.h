/*
 *  BlobHistory.h
 *  openFrameworks
 *
 *  Created by frost on 06/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "FrostMotor.h"

class BlobHistory : public Output{

public:

	// the weird spaces below are important, or the 2d vector will not work !!!
	
	vector<vector<ofxCvBlob> > blobHistoryMatrix;
	vector<GLuint> blobHistoryMatrixDisplayList;
	
	vector<vector<ofxCvBlob> > blobSnapshotMatrix;
	vector<GLuint> blobSnapshotMatrixDisplayList;
		
	BlobHistory();
	
	void setup();
	void drawOnFloor();
	void draw();
	void update();

	
	FrostMotor motor;
	
	int cam;

	bool bIsRecordingHistory;
	bool bClearHistory;

	bool bTakeSnapshot;
	bool bRemoveOldestSnapshot;
	bool bClearSnapshots;
	
	float historyPlayStep;
	float historyAlpha;
	
	int historyOffset;

	float snapshotAlpha;
	
	vector<ofxPoint2f> freezePoints;

};