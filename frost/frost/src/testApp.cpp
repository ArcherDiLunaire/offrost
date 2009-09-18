#include "testApp.h"
#include "CustomGLViewDelegate.h"

//#include "OFGuiController.h"


//--------------------------------------------------------------

testApp::testApp(): otherWindow(), projectionSurfaceWindow(), blobWindow(), ofBaseApp() {
	setupCalled = false;
	pluginController = new PluginController;
	pluginController->addPlugin(new Cameras);
	pluginController->addPlugin(new BlobTracking);

	pluginController->addPlugin(new ProjectionSurfaces);
	
	pluginController->addPlugin(new MoonDust);

	
	
}

void testApp::setup(){	
	cout<<"--- Testapp setup"<<endl;
	ofSetDataPathRoot("data/");
	ofEnableAlphaBlending();
	ofBackground(0,0,0);	

	
	
	pluginController->setup();

	//pluginByType <int> obj;
	//obj.get(pluginController);
	setupCalled = true;
	
}

void testApp::setReferenceToOtherWindow( CustomGLViewDelegate* delegate, int i )
{
	if(i == 0){
		cout<<"---------- ERROR: Window_id is 0 ------------"<<endl;
	}	
	if(i == 1){
		otherWindow = delegate;
		otherWindow->setup(&testApp::drawCameraView);
	}
	if(i == 2){
		projectionSurfaceWindow = delegate;
		projectionSurfaceWindow->setup(&testApp::drawProjectionSurfaceView);
		getPlugin<ProjectionSurfaces*>(pluginController)->glDelegate = delegate;
	}
	if(i == 3){
		blobWindow = delegate;
		blobWindow->setup(&testApp::drawBlobWindow);
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
	
	pluginController->update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 0), 10, 20);
	
	pluginController->draw();
	fps = ofGetFrameRate();
}

void testApp::drawCameraView(){
	getPlugin<Cameras*>(pluginController)->vidGrabber->draw(0,0,otherWindow->getWidth(),otherWindow->getHeight());
}

void testApp::drawProjectionSurfaceView(){
	getPlugin<ProjectionSurfaces*>(pluginController)->drawSettings();
}

void testApp::drawBlobWindow(){
	ofSetColor(255,255, 255);
	getPlugin<BlobTracking*>(pluginController)->draw();

}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'f'){
		ofToggleFullscreen();
	}
	if(key == 'c'){
		getPlugin<Cameras*>(pluginController)->vidGrabber->videoSettings();
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

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}
