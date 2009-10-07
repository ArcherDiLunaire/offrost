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

class BlobHistory : public Output{

public:

	// the weird spaces below are important, or the 2d vector will not work !!!
	
	vector<vector<ofxCvBlob> > blobHistoryMatrix;
	
	BlobHistory();
	
	void setup();
	void drawOnFloor();
	void draw();
	void update();

	int cam;

	bool bIsRecording;
	
};