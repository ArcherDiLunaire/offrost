#include "testApp.h"
#include "CustomGLViewDelegate.h"

//#include "OFGuiController.h"


//--------------------------------------------------------------

testApp::testApp(): otherWindow(), projectionSurfaceWindow(), blobWindow(), floorPreview(), cameraCalibrationWindow(), ofBaseApp() {
	setupCalled = false;
	pluginController = new PluginController;
	pluginController->addPlugin(new Cameras);
	pluginController->addPlugin(new BlobTracking);
	
	pluginController->addPlugin(new ProjectionSurfaces);
	pluginController->addPlugin(new CameraCalibration);
	
	pluginController->addPlugin(new MoonDust);

	pluginController->addPlugin(new BlobHistory);
	pluginController->addPlugin(new LaLinea);
	pluginController->addPlugin(new LaLineaFloor);
	pluginController->addPlugin(new Folding);
	pluginController->addPlugin(new Frostscape);
	pluginController->addPlugin(new LiquidSpace);
	pluginController->addPlugin(new BlobLight);
	
	pluginController->addPlugin(new Spotlight);
	pluginController->addPlugin(new MirrorBall);
	pluginController->addPlugin(new LEDGrid);

	
}

void testApp::setup(){	
	cout<<"--- Testapp setup"<<endl;
	ofSetDataPathRoot("data/");
	ofEnableAlphaBlending();
	ofBackground(0,0,0);	
	
//	ofSetLogLevel(OF_LOG_VERBOSE);
	
	lucidaGrande.loadFont("LucidaGrande.ttc",22, false, true);
	
	pluginController->setup();
	
	//pluginByType <int> obj;
	//obj.get(pluginController);
	setupCalled = true;
	
	glEnable (GL_MULTISAMPLE_ARB);
    glHint (GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	
	midiOut = new ofxMidiOut;
	midiOut->listPorts();
	midiOut->openPort(0);
	
	cameraThreadTimer = -500;
	
}

void testApp::setReferenceToOtherWindow( CustomGLViewDelegate* delegate, int i )
{
	cout<<"Setup called: "<<setupCalled<< "   "<<i<<endl;
	if(i == 0){
		cout<<"---------- ERROR: Window_id is 0 ------------"<<endl;
	}	
	if(i == 1){
		otherWindow = delegate;
		otherWindow->setup(&testApp::drawCameraView);
	}
	if(i == 2){
		projectionSurfaceWindow = delegate;
		getPlugin<ProjectionSurfaces*>(pluginController)->glDelegate = delegate;
		projectionSurfaceWindow->setup(&testApp::drawProjectionSurfaceView);
		getPlugin<ProjectionSurfaces*>(pluginController)->guiWakeup();
	}
	if(i == 3){
		blobWindow = delegate;
		blobWindow->setup(&testApp::drawBlobWindow);
	}
	
	if(i == 4){
		floorPreview = delegate;
		floorPreview->setup(&testApp::drawFloorPreview);
	}
	
	if(i == 5){
		cameraCalibrationWindow = delegate;
		getPlugin<CameraCalibration*>(pluginController)->glDelegate = delegate;
		cameraCalibrationWindow->setup(&testApp::drawCameraCalibrationView);
		getPlugin<CameraCalibration*>(pluginController)->guiWakeup();
	}
	
}


//--------------------------------------------------------------
void testApp::update()
{
	float mousex = (float)mouseX/ofGetWidth();
	float mousey = (float)mouseY/ofGetHeight();
	
	if(mousex < 0.5){
		getPlugin<MoonDust*>(pluginController)->min = mousex-0.5;
		getPlugin<MoonDust*>(pluginController)->max = 0;
	} else {
		getPlugin<MoonDust*>(pluginController)->max = mousex-0.5;
		getPlugin<MoonDust*>(pluginController)->min = 0;
		
	}
	
	pluginController->update(mousex, mousey);
	
	if(cameraThreadTimer > 100){
		cout<<endl<<"ERROR: DEAD CAMERA"<<endl;
		
		vector<FrostPlugin*>::iterator it;
		for(it=pluginController->plugins.begin(); it!=pluginController->plugins.end(); it++){
			if(dynamic_cast<Cameras*> ((*it)) != NULL){
				delete * it;  
                pluginController->plugins.erase(it);
				break;
			}
		}
		
		Cameras * c = new Cameras();
		c->setGUIDs(cameraGUIDs[0], cameraGUIDs[1], cameraGUIDs[2]);
		c->setup();
		//	cout<<"GUIDS: "<<cameraGUIDs[0]<<"  "<<cameraGUIDs[1]<<"  "<<cameraGUIDs[2]<<endl;
		pluginController->addPlugin(c);
		ofSleepMillis(3500);
		cameraThreadTimer = 0;
	}
	
	for(int i=0;i<3;i++){
		if(getPlugin<Cameras*>(pluginController)->isRunning(i)){			
			if(((Libdc1394Grabber*) getPlugin<Cameras*>(pluginController)->getVidGrabber(i)->videoGrabber)->lock()){
				if(((Libdc1394Grabber*) getPlugin<Cameras*>(pluginController)->getVidGrabber(i)->videoGrabber)->blinkCounter != cameraLastBlinkCount[i]){
					cameraThreadTimer = 0;
				} else {
					cameraThreadTimer ++;
				}
				cameraLastBlinkCount[i] = ((Libdc1394Grabber*) getPlugin<Cameras*>(pluginController)->getVidGrabber(i)->videoGrabber)->blinkCounter;
				((Libdc1394Grabber*) getPlugin<Cameras*>(pluginController)->getVidGrabber(i)->videoGrabber)->unlock();
			}
		} else {
			if(getPlugin<Cameras*>(pluginController)->hasCameras)
				cameraThreadTimer ++;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	//	ofDrawBitmapString(ofToString(ofGetFrameRate(), 0), 10, 20);
	
	pluginController->draw();
	pluginController->drawFloor();
	pluginController->drawWall();
	
	fps = ofGetFrameRate();
}

void testApp::drawCameraView(){
	ofFill();
	ofSetColor(164,164, 164);
	ofRect(0, 0, otherWindow->getWidth(), otherWindow->getHeight());
	ofSetColor(196, 196, 100);
	for (float i = 0.0; i < otherWindow->getWidth()+otherWindow->getHeight(); i+=20.0) {
		ofBeginShape();
		ofVertex(i,0);
		ofVertex(i+10, 0);
		ofVertex((i-otherWindow->getHeight())+11.0, otherWindow->getHeight());
		ofVertex(i-otherWindow->getHeight()+1, otherWindow->getHeight());
		ofEndShape(true);
	}
	
	for (int i=0; i<3; i++) {
		if(getPlugin<Cameras*>(pluginController)->isRunning(i)){
			ofSetColor(255,255, 255);
			getPlugin<Cameras*>(pluginController)->getVidGrabber(i)->draw((otherWindow->getWidth()/3.0)*i,0,otherWindow->getWidth()/3.0,otherWindow->getHeight());
			if(((Libdc1394Grabber*) getPlugin<Cameras*>(pluginController)->getVidGrabber(i)->videoGrabber)->lock()){
				if(((Libdc1394Grabber*) getPlugin<Cameras*>(pluginController)->getVidGrabber(i)->videoGrabber)->blinkCounter % 50 < 25){
					ofSetColor(255, 0, 0);
					ofEllipse((otherWindow->getWidth()/3.0)*i+10, 10, 10, 10);
				}
				((Libdc1394Grabber*) getPlugin<Cameras*>(pluginController)->getVidGrabber(i)->videoGrabber)->unlock();
			}
		} else {
			ofEnableAlphaBlending();
			ofSetColor(255,255,255,(((sinf(ofGetElapsedTimef()*5.0)/2.0)+0.5)*255));
			lucidaGrande.drawString("camera offline",(45+((otherWindow->getWidth()/3.0)*i)),(otherWindow->getHeight()/2)+10);
		}
	}
	 
}

void testApp::drawProjectionSurfaceView(){
	getPlugin<ProjectionSurfaces*>(pluginController)->drawSettings();
}

void testApp::drawCameraCalibrationView(){
	getPlugin<CameraCalibration*>(pluginController)->drawSettings();	
}

void testApp::drawBlobWindow(){
	ofSetColor(255,255, 255);
	getPlugin<BlobTracking*>(pluginController)->drawSettings();
	
}

void testApp::drawFloorPreview(){
	glPushMatrix();
	glScaled(1.0, 0.5, 1.0);
	glScaled(floorPreview->m_Width/getPlugin<ProjectionSurfaces*>(pluginController)->getFloor()->aspect, floorPreview->m_Height, 1.0);
	for(int i=0;i<pluginController->plugins.size();i++){
		FrostPlugin* plugin = pluginController->plugins[i];
		if(plugin->enabled){
			plugin->drawOnFloor(); 
		}
	}
	
	glPopMatrix();
	
	ofSetColor(255, 255, 255);
	ofLine(0, floorPreview->m_Height/2.0, floorPreview->m_Width, floorPreview->m_Height/2.0);
	
	glPushMatrix();
	
	glTranslated(0.5, floorPreview->m_Height/2.0, 0);
	glScaled(1.0, 0.5, 1.0);
	glScaled(floorPreview->m_Width/getPlugin<ProjectionSurfaces*>(pluginController)->getWall()->aspect, floorPreview->m_Height, 1.0);
	for(int i=0;i<pluginController->plugins.size();i++){
		FrostPlugin* plugin = pluginController->plugins[i];
		if(plugin->enabled){
			plugin->drawOnWall(); 
		}
	}
	glPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'f'){
		ofToggleFullscreen();
	}
	if(key == 'c'){
		//getPlugin<Cameras*>(pluginController)->vidGrabber->videoSettings();
	}
	if( key == ' ' ) {
		if(getPlugin<Cameras*>(pluginController)->calibAddSnapshot(getPlugin<Cameras*>(pluginController)->getGUID(0))){
			cout << "CALIB: Added snapshot image..." << endl;
		} else {
			cout << "CALIB: **** ERROR *** Could not add snapshot image..." << endl;
		}
	}
	if( key == '.' ) {
		cout << "Calibrating..." << endl;
		getPlugin<Cameras*>(pluginController)->calibrate(getPlugin<Cameras*>(pluginController)->getGUID(0));
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
	BlobTracking * t = getPlugin<BlobTracking*>(pluginController);
	for(int i=0;i<t->trackers.size();i++){
		t->trackers[i]->updateMouseBlob((float)x/ofGetWidth(), (float)y/ofGetHeight(), button);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	BlobTracking * t = getPlugin<BlobTracking*>(pluginController);
	for(int i=0;i<t->trackers.size();i++){
		t->trackers[i]->updateMouseBlob((float)x/ofGetWidth(), (float)y/ofGetHeight(), -1);
	}
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}
