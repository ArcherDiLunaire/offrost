/*
 *  LiquidSpace.cpp
 *  openFrameworks
 *
 *  Created by frost on 30/09/09.
 *
 */

#include "PluginController.h"
#include "PluginIncludes.h"

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

void LiquidSpace::fill(){
	//not implemented
	;
}

// add force and dye to fluid, and create particles
void LiquidSpace::addToFluid(float x, float y, float dx, float dy, bool addColor, bool addForce, msaColor color) {
    float speed = dx * dx  + dy * dy / window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
	
    if(speed > 0) {
        if(x<0) x = 0; 
        else if(x>1) x = 1;
        if(y<0) y = 0; 
        else if(y>1) y = 1;
		
        float colorMult = colorMultiplier;
        float velocityMult = forceMultiplier;
		
        int index = fluidSolver.getIndexForNormalizedPosition(x, y);
		
		if(addColor) {
			msaColor drawColor;
			int hue = lroundf((x + y) * 180 + ofGetFrameNum()) % 360;
			drawColor.setHSV(hue, 1, 1);						
			fluidSolver.r[index]  += color.r * colorMult;
			fluidSolver.g[index]  += color.g * colorMult;
			fluidSolver.b[index]  += color.b * colorMult;			
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
	dropColor.set(1.0,1.0,1.0);
	fillColor.set(0.0,0.0,0.0);
	
}

#pragma mark Callback methods

void LiquidSpace::setup(){
	ringTexture.loadImage("waterRingTexture1.png");

	window.width		= projection()->getFloor()->aspect;
	window.height		= 1.0;
	
	window.invWidth		= 1.0/projection()->getFloor()->aspect;
	window.invHeight	= 1.0;
	window.aspectRatio	= projection()->getFloor()->aspect;
	window.aspectRatio2 = window.aspectRatio / window.aspectRatio;
	
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.00000001).setDeltaT(0.5).setVisc(0.0000001).setColorDiffusion(0.00000002);
	fluidDrawer.setup(&fluidSolver);
	
	fluidCellsX			= 120;
	
	drawFluid			= true;
	renderUsingVA		= true;
	resizeFluid			= true;
	
}

void LiquidSpace::update(){
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX * projection()->getFloor()->aspect);
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
	
	//	cout<<updateMotor<<endl;
	if(updateMotor){
		fluidSolver.update();
		
		if (blob(0)->numBlobs() > 0) {
			
			float _x;
			float _y;
			
			for(int i =0;i<blob(0)->numBlobs();i++){
				ofxCvBlob b = blob(0)->getBlob(i);
				ofxVec2f r = projection()->convertToFloorCoordinate(ofxVec2f(blob(0)->getLargestBlob().centroid.x, blob(0)->getLargestBlob().centroid.y));			
				
				for(int j =0;j<b.nPts;j+=10){
					ofxVec2f p = projection()->convertToFloorCoordinate(ofxVec2f(b.pts[j].x, b.pts[j].y));			
					
					
					_x = r.x;
					_y = r.y;
					
					//				cout << _x << ", " << _y << endl;
					
					int index = fluidSolver.getIndexForNormalizedPosition(p.x / projection()->getFloor()->aspect, p.y);
					fluidSolver.r[index]  += dropColor.r * colorMultiplier * 0.5;
					fluidSolver.g[index]  += dropColor.g * colorMultiplier * 0.5;
					fluidSolver.b[index]  += dropColor.b * colorMultiplier * 0.5;	
					
					//addToFluid(_x / projection()->getFloor()->aspect, _y, ofRandom(-0.02, 0.02), ofRandom(-0.02, 0.02), addingColor, addingForce, dropColor);
				}
			}
			
			ofxVec2f r = projection()->convertToFloorCoordinate(ofxVec2f(blob(0)->getLargestBlob().centroid.x, blob(0)->getLargestBlob().centroid.y));			
			
			_x = r.x;
			_y = r.y;
			
			
			addToFluid(_x / projection()->getFloor()->aspect, _y, _x / projection()->getFloor()->aspect - pX / projection()->getFloor()->aspect, _y-pY, addingColor, addingForce, dropColor);
			
			
			pX = _x;
			pY = _y;
		}
	}
	for(int i=0;i<rings.size();i++){
		if(rings[i].height > 0){
			rings[i].height -= rings[i].speed*60.0/ofGetFrameRate();
		} else {
			rings[i].size += rings[i].speed*0.4*30.0/ofGetFrameRate();
			rings[i].speed *= 0.99;
			
		}
	}
}

void LiquidSpace::drawOnFloor(){
	ofEnableAlphaBlending();
	glBlendFunc (GL_SRC_COLOR, GL_ONE);	
	
	if(drawFluid && updateMotor) {
		glColor3f(1, 1, 1);
		fluidDrawer.draw(0, 0, window.aspectRatio, 1.0);
	}
	
	
}

void LiquidSpace::draw(){
	
	
	for(int i=0;i<rings.size();i++){
		
		ofSetColor(255, 255, 255);
		if(rings[i].height > 0){
			projection()->applyProjection(projection()->getColumn(rings[i].column));
			ofEllipse(projection()->getColumn(rings[i].column)->aspect/2.0, 1.0-rings[i].height, projection()->getColumn(rings[i].column)->aspect, projection()->getColumn(rings[i].column)->aspect);
			
			glPopMatrix();
		} else {
			ofEnableAlphaBlending();
			projection()->applyProjection(projection()->getFloor());
			ofNoFill();
			glTranslated(rings[i].center.x, rings[i].center.y, 0);
			int n = 200;
			ringTexture.getTextureReference().bind();
			glBegin(GL_QUAD_STRIP);
			for(int u=0;u<n;u++){
				glTexCoord2f(0.0f, 0.0f);    
				glVertex2f(cos(TWO_PI*(float)u/n)*rings[i].size, sin(TWO_PI*(float)u/n)*rings[i].size);
				glTexCoord2f(50, 0.0f);    

				glVertex2f(cos(TWO_PI*(float)u/n)*(rings[i].size+0.01), sin(TWO_PI*(float)u/n)*(rings[i].size+0.01));
			}	
			glEnd();
			ringTexture.getTextureReference().unbind();
			ofFill();
			glPopMatrix();
			
		}
	}
}
void LiquidSpace::addRing(int i){
	ring r;
	r.center = projection()->getColumnCoordinate(i);
	r.size = 0;
	r.speed = ofRandom(0.005, 0.02);
	r.height = 1.0;
	r.column = i;
	rings.push_back(r);
}