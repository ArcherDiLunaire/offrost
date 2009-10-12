/*
 *  MirrorBall.cpp
 *  openFrameworks
 *
 *  Created by frost on 06/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "PluginController.h"
#include "PluginIncludes.h"

#pragma mark Perlin Noise Functions

inline double findnoise2(double x,double y)
{
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
}

inline double interpolate(double a,double b,double x)
{
	double ft=x * 3.1415927;
	double f=(1.0-cos(ft))* 0.5;
	return a*(1.0-f)+b*f;
}

double noise(double x,double y)
{
	double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
	double floory=(double)((int)y);
	double s,t,u,v;//Integer declaration
	s=findnoise2(floorx,floory); 
	t=findnoise2(floorx+1,floory);
	u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
	v=findnoise2(floorx+1,floory+1);
	double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
	double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

MirrorBall::MirrorBall(){
	type = OUTPUT;
	cam = 0;
	
	rotation = 0;
	rotationDirection = 0.015;
	
	reflections1 = MirrorBallReflections();
	reflections2 = MirrorBallReflections();
}

#pragma mark Callback methods

void MirrorBall::setup(){
	mirrorBallImage.loadImage("spotlight.png");
	
	reflections1.setup(15, 0.04, 0.15, -0.0003, 0.0003, 0.01, 0.75, 0.0025, 0.003, 0.25);
	reflections2.setup(15, 0.04, 0.15, 0.0002, -0.0008, 0.01, 0.75, 0.0025, 0.005, 0.10);
	
}

void MirrorBall::draw(){
	
	
}

void MirrorBall::drawOnFloor(){
	
	
	ofPushStyle();
	
	ofEnableAlphaBlending();
	
	glPushMatrix();
	ofSetColor(255, 255, 255);
	
	glTranslated(0.3*projection()->getFloor()->aspect, 0.5, 0);
	
	ofSetColor(255,255,255,255*masterAlpha);
	reflections1.draw(rotation*360);
	
	ofSetColor(255,255,255,255*masterAlpha);
	reflections2.draw(rotation*360);
	
	glPopMatrix();
	ofPopStyle();
	
}

void MirrorBall::update(){
	if(bAddDot){
		bAddDot =false;
		reflections1.turnOnSomeDots(1);
		reflections2.turnOnSomeDots(0);
	}
	if(bClearDots){
		bClearDots =false;
		reflections1.clearReflections();
		reflections2.clearReflections();
	}
	
	rotation += rotationDirection*0.01;
	
	rotation = fmodf(rotation, 1.0);
	
	reflections1.update();
	reflections2.update();
	
}

#pragma mark Mirror Ball Ball

MirrorBallReflections::MirrorBallReflections(){
	
	numberOfCircles = 17;
	circlesSpacing = 0.04;
	circlesSpacingMultiplier = 0.15;
	
	circleOffsetX = -0.0003;
	circleOffsetY = 0.0003;
	
	dotSpacing = 0.75;
	
	dotPositionNoise = 0.003;
	dotAlphaNoise = 0.33;
	
	dotSize = 0.005;
	dotSizeDistanceMultiplier = 0.0025;
	
	center = ofxVec2f(0.3,0.5);
	
	nDots = getNumberOfDots(numberOfCircles, dotSpacing);
	nDotsTurnedOn = 1;
	
	fractionOfDotsTurnedOn = 1.0;
	
	alpha = 1.0;
	
	//	allReflections();
	clearReflections();
}

void MirrorBallReflections::setup(int _numberOfCircles, float _circlesSpacing, float _circlesSpacingMultiplier, float _circleOffsetX, float _circleOffsetY,
								  float _dotSize, float _dotSpacing, float _dotSizeDistanceMultiplier,
								  float _dotPositionNoise, float _dotAlphaNoise){
	numberOfCircles = _numberOfCircles;
	circlesSpacing = _circlesSpacing;
	circlesSpacingMultiplier = _circlesSpacingMultiplier;
	circleOffsetX = _circleOffsetX;
	circleOffsetY = _circleOffsetY;
	dotSize = _dotSize;
	dotSpacing = _dotSpacing;
	dotSizeDistanceMultiplier = _dotSizeDistanceMultiplier;
	dotPositionNoise = _dotPositionNoise;
	dotAlphaNoise = _dotAlphaNoise;
}

void MirrorBallReflections::update(){
	int nDotsThatShouldBeOn = fractionOfDotsShouldBeTurnedOn * nDots;
	if(nDotsTurnedOn < nDotsThatShouldBeOn){
		turnOnSomeDots( nDotsThatShouldBeOn - nDotsTurnedOn );
	} else if(nDotsTurnedOn > nDotsThatShouldBeOn){
		turnOffSomeDots(nDotsTurnedOn - nDotsThatShouldBeOn);
	}
	
}

void MirrorBallReflections::draw(float _angle){
	
	glPushMatrix();
	
	for (int theCircle=1; theCircle < numberOfCircles; theCircle++) {
		
		glTranslated(circleOffsetX*theCircle*theCircle, circleOffsetY*theCircle*theCircle, 0);
		
		int numDots = theCircle*(theCircle*dotSpacing);
		
		for (int theDot=0; theDot < numDots; theDot++) {
			glPushMatrix();
			
			// rotate the whole circle with our global mirrorball rotation
			glRotatef(_angle, 0, 0, 1.0);
			
			//rotate the dot to the angle relative to the dots index
			glRotatef((theDot*1.0/numDots)*360.0, 0, 0, 1.0);
			
			//translate the dot to the distance from the center
			
			glTranslated(0,((theCircle-1)*circlesSpacing)*(theCircle*circlesSpacingMultiplier), 0);
			
			//rotate the dot so it prolongs as a function of the distance to the center
			glRotatef((0.125 * ( theCircle*1.0/numberOfCircles ))*360.0, 0, 1.0, 0.0);
			
			ofColor theColor = ofGetStyle().color;
			
			ofPushStyle();
			ofEnableAlphaBlending();
			
			theColor.a *= alpha;
			
			ofSetColor(theColor.r, theColor.g, theColor.b, (
															(
															 ((theColor.a *1.0)- (dotAlphaNoise * theColor.a))
															 + ((0.5+(0.5*noise(theDot*109,(_angle/360)*17)))*(dotAlphaNoise * theColor.a)) // alpha noise gives flickering
															 )	
															* (1.0-sqrtf(theCircle*1.0/numberOfCircles))								 // distance fade
															)											
					   );
			
			float xNoised = sin(theDot*10020.6)*dotPositionNoise*((theCircle+2)-(numberOfCircles*1.0/theCircle));
			float yNoised = sin(theDot*10000.6)*dotPositionNoise*((theCircle+2)-(numberOfCircles*1.0/theCircle));
			
			if (dotStates[theCircle*(theDot+1)]) {
				ofRect(xNoised,yNoised , dotSize+(dotSizeDistanceMultiplier*theCircle), dotSize+(dotSizeDistanceMultiplier*theCircle));
			}
			
			//mirrorBallImage.draw(xNoised,yNoised , dotSize+(_dotSizeDistanceMultiplier*theCircle), dotSize+(_dotSizeDistanceMultiplier*theCircle));
			
			ofPopStyle();
			glPopMatrix();
			
		}
		
	}
	glPopMatrix();
}

int MirrorBallReflections::getNumberOfDots(int _numberOfCircles, float _dotSpacing){
	
	int retVal = 0;
	
	for (int theCircle=1; theCircle < _numberOfCircles; theCircle++) {
		retVal += theCircle*(theCircle*_dotSpacing);
	}
	
	return retVal *_numberOfCircles;
}

void MirrorBallReflections::setDotStates(int amountOfDots, bool isOn){
	fractionOfDotsTurnedOn = (nDotsTurnedOn * 1.0) / nDots;
	
	cout << "setDotStates" << amountOfDots << " to " << (isOn?"ON":"OFF") << endl;
	
	if(isOn && (nDots - nDotsTurnedOn) < amountOfDots) 
		amountOfDots = (nDots - nDotsTurnedOn);
	
	if(!isOn && (nDotsTurnedOn) > amountOfDots) 
		amountOfDots = nDotsTurnedOn;
	
	if((isOn && fractionOfDotsTurnedOn < 1.0) || (!isOn && fractionOfDotsTurnedOn > 0.0)){
		/*		for (int i = 0;i<dotStates.size() ; 0) {
		 if(dotStates[i] == isOn){
		 i += ofRandom(0,dotStates.size()/3.0);
		 } else {
		 dotStates[i] = isOn;
		 amountOfDots--;
		 if(isOn){
		 nDotsTurnedOn++;
		 } else {
		 nDotsTurnedOn--;
		 }
		 
		 if (amountOfDots < 0) {
		 break;
		 }
		 }
		 }*/
		int i= ofRandom(0,dotStates.size()-1);
		while(amountOfDots > 0){
			i++;
			if(i > dotStates.size()-1){
				i=0;	
			}
			if(dotStates[i] != isOn) {
				dotStates[i] = isOn;
				i= ofRandom(0,dotStates.size()-1);
				amountOfDots--;
			//	cout<<i<<endl;
				if(isOn){
					nDotsTurnedOn++;
				} else {
					nDotsTurnedOn--;
				}			
			}
		}
		fractionOfDotsTurnedOn = (nDotsTurnedOn * 1.0) / nDots;
	}
}


void MirrorBallReflections::turnOnSomeDots(int amountOfDots){
	setDotStates(amountOfDots, true);
}

void MirrorBallReflections::turnOffSomeDots(int amountOfDots){
	setDotStates(amountOfDots, false);
}

void MirrorBallReflections::clearReflections(){
	dotStates.assign(nDots, false);
	nDotsTurnedOn = 0;
	fractionOfDotsTurnedOn = 0.0;
	fractionOfDotsShouldBeTurnedOn = 0.0;
}

void MirrorBallReflections::allReflections(){
	dotStates.assign(nDots, true);
	nDotsTurnedOn = nDots;
	fractionOfDotsTurnedOn = 1.0;
	fractionOfDotsShouldBeTurnedOn = 1.0;
}