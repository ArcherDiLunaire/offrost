/*
 *  Spotlight.h
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

class Spotlight: public Output{

public:

	Spotlight();
	
	void setup();
	void drawOnFloor();
	void update();
	
	int cam;
	
};