/*
 *  LiquidSpace.cpp
 *  openFrameworks
 *
 *  Created by frost on 30/09/09.
 *
 */

#include "PluginController.h"
#include "PluginIncludes.h"
#include "msaColor.h"

#pragma mark Custom methods

void fadeToColor(float r, float g, float b, float speed) {
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, speed);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1.0, 0);
    glVertex2f(1.0, 1.0);
    glVertex2f(0, 1.0);
    glEnd();
}


// add force and dye to fluid, and create particles
void LiquidSpace::addToFluid(float x, float y, float dx, float dy, bool addColor, bool addForce) {
    float speed = dx * dx  + dy * dy / window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
	printf("%f, %f, %f\n", dx, dy, speed);
    if(speed > 0) {
        if(x<0) x = 0; 
        else if(x>1) x = 1;
        if(y<0) y = 0; 
        else if(y>1) y = 1;
		
        float colorMult = 50;
        float velocityMult = 30;
		
        int index = fluidSolver.getIndexForNormalizedPosition(x, y);
		
		if(addColor) {
			msaColor drawColor;
			int hue = lroundf((x + y) * 180 + ofGetFrameNum()) % 360;
			drawColor.setHSV(hue, 1, 1);
			
			fluidSolver.r[index]  += drawColor.r * colorMult;
			fluidSolver.g[index]  += drawColor.g * colorMult;
			fluidSolver.b[index]  += drawColor.b * colorMult;			
		}
		
		if(addForce) {
			fluidSolver.u[index] += dx * velocityMult;
			fluidSolver.v[index] += dy * velocityMult;
		}
		
		if(!drawFluid && ofGetFrameNum()%5 ==0) fadeToColor(0, 0, 0, 0.1);
    }
}




LiquidSpace::LiquidSpace(){
	type = OUTPUT;
	cam = 1;
}

void LiquidSpace::setup(){

	window.width		= 1.0;
	window.height		= 1.0;
	
	window.invWidth		= 1.0;
	window.invHeight	= 1.0;
	window.aspectRatio	= projection()->getFloor()->aspect;
	window.aspectRatio2 = window.aspectRatio / window.aspectRatio;
	
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(false).setFadeSpeed(0.000000005).setDeltaT(0.1).setVisc(0.000085).setColorDiffusion(0.00002);
	fluidDrawer.setup(&fluidSolver);
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	renderUsingVA		= true;
	resizeFluid			= true;
	
}

void LiquidSpace::update(){
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX * projection()->getFloor()->aspect);
		cout << projection()->getFloor()->aspect << endl;
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	
	fluidSolver.update();
	if (blob(0)->numBlobs() > 0) {
		float _x;
		float _y;
		
		_x = blob(0)->getLargestBlob().centroid.x;
		_y = blob(0)->getLargestBlob().centroid.y;
		
		cout << _x << ", " << _y << endl;
		
		addToFluid(_x, _y, _x-pX, _y-pY, true);

		pX = _x;
		pY = _y;
	}
}

void LiquidSpace::drawOnFloor(){
	if(drawFluid) {
		glColor3f(1, 1, 1);
		fluidDrawer.draw(0, 0, 1.0, 1.0);
	}
}