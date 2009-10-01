#pragma once
#include "ofMain.h"
#include "Output.h"
#include "ofxVectorMath.h"
#include "ofxMSAFluid.h"

class LiquidSpace : public Output{
public:
	LiquidSpace();
	
	void setup();
	void drawOnFloor();
	void update();
	
	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);
	
	int					fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	bool				drawParticles;
	bool				renderUsingVA;
	bool				addingForce;
	bool				addingColor;
	
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
	int cam;
	
	msaColor dropColor;
	msaColor fillColor;
	
};