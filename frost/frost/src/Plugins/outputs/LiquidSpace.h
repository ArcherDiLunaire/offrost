#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSAFluid.h"
#include "msaColor.h"

class ring {
public:
	float size;
	float age;
	ofxPoint2f center;
	float speed;
	float height ;
	int column;
	int birthMillis;
};


class LiquidSpace : public Output{
public:
	LiquidSpace();
	
	void setup();
		void draw();
	void drawOnFloor();
	void update();
	
	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true, msaColor color = msaColor(1.0,1.0,1.0));
	
	int					fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	bool				drawParticles;
	bool				renderUsingVA;
	float				colorMultiplier;
	bool				addingColor;
	float				forceMultiplier;
	bool				addingForce;
	
	ofxMSAFluidSolver	fluidSolver;
	ofxMSAFluidDrawer	fluidDrawer;	
	
	// ParticleSystem		particleSystem;
	
	float					pX, pY;
	
	// cache these for slightly better performance
	struct {
		int				width;
		int				height;
		float			invWidth;
		float			invHeight;
		float			aspectRatio;
		float			aspectRatio2;
	} window;
	
	void fill();
	
	bool debug;
	bool clear;
	
	int cam;
	
	msaColor dropColor;
	msaColor fillColor;
	
	vector<ring> rings;
	bool updateMotor;
	ofImage ringTexture;
	void addRing(int i);
	void addFixedRing(int i);
	
	float speed;
};