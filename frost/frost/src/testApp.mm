#import "GLee.h"
#import <Cocoa/Cocoa.h>
#include "testApp.h"
#include "OFGuiController.h"


//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(255,0,255);	
	ofSetFrameRate(60);
	
	vidGrabber = new ofVideoGrabber();
	vidGrabber->initGrabber(640, 480);
	
	NSApplicationMain(0, NULL);
	
}



//--------------------------------------------------------------
void testApp::update(){
	vidGrabber->grabFrame();
}

//--------------------------------------------------------------
void testApp::draw(){
	vidGrabber->draw(0,0,100,100);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//------------- -------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	/*for (int i = 0; i < nCurveVertexes; i++){
		float diffx = x - curveVertices[i].x;
		float diffy = y - curveVertices[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < curveVertices[i].radius){
			curveVertices[i].bBeingDragged = true;
		} else {
			curveVertices[i].bBeingDragged = false;
		}	
	}
	
	printf("mouseX is %i mouseY is %i \n", x, y);

*/
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
/*
	for (int i = 0; i < nCurveVertexes; i++){
		curveVertices[i].bBeingDragged = false;	
	}*/
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}