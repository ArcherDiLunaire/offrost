#include "testApp.h"
#include "CustomGLViewDelegate.h"

//#include "OFGuiController.h"


//--------------------------------------------------------------

testApp::testApp(): otherWindow(), projectionSurfaceWindow(), ofBaseApp() {
	setupCalled = false;
	pluginController = new PluginController;
	pluginController->addPlugin(new ProjectionSurfaces);
	pluginController->addPlugin(new MoonDust);

	
	
}

void testApp::setup(){	
	cout<<"--- Testapp setup"<<endl;
	ofSetDataPathRoot("data/");
	ofEnableAlphaBlending();
	ofBackground(0,0,0);	
	
	vidGrabber = new ofVideoGrabber();
	vidGrabber->initGrabber(640, 480);
	
	
	pluginController->setup();

	//pluginByType <int> obj;
	//obj.get(pluginController);
	setupCalled = true;
	
}

void testApp::setReferenceToOtherWindow( CustomGLViewDelegate* delegate, int i )
{
	cout<<endl<<"Reference called "<<i<<endl;
	if(i == 0){
		cout<<"---------- ERROR: Window_id is 0 ------------"<<endl;
	}	
	if(i == 1){
		cout<<"--- 1 OK --"<<endl;
		otherWindow = delegate;
		otherWindow->setup(&testApp::drawCameraView);
	}
	if(i == 2){
		cout<<"--- 2 OK --"<<endl;
		projectionSurfaceWindow = delegate;
		projectionSurfaceWindow->setup(&testApp::drawProjectionSurfaceView);
	}
}


//--------------------------------------------------------------
void testApp::update()
{
	float mousex = (float)mouseX/ofGetWidth();
	if(mousex < 0.5){
		getPlugin<MoonDust*>(pluginController)->min = mousex;
		getPlugin<MoonDust*>(pluginController)->max = 0.5;
	} else {
		getPlugin<MoonDust*>(pluginController)->max = mousex;
		getPlugin<MoonDust*>(pluginController)->min = 0.5;
	
	}
	
	vidGrabber->grabFrame();
	pluginController->update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 0), 10, 20);
	
	pluginController->draw();
}

void testApp::drawCameraView(){
	vidGrabber->draw(0,0,otherWindow->getWidth(),otherWindow->getHeight());
}

void testApp::drawProjectionSurfaceView(){
	ofSetColor(255,0, 0);
	ofRect(0, 0, 10, 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'f'){
		ofToggleFullscreen();
	}
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
