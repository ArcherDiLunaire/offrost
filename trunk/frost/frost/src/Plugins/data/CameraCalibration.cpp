
#include "CameraCalibration.h"
#include "ProjectionSurfaces.h"
#include "Cameras.h"
#include "PluginController.h"


CameraCalibration::CameraCalibration(){
	type = DATA;

	for(int i=0;i<3;i++){
		cameras.push_back(new CameraCalibrationObject);
		cameras[i]->warp = new Warp();
		cameras[i]->floorWarp = new Warp();
		cameras[i]->coordWarp = new coordWarping;
		cameras[i]->name = "CAMERA "+ofToString(i, 0);
	}
		
	drawDebug = false;
	selectedCorner = 0;
	selectedKeystoner = 0;
	offset = 0;
	
}

void CameraCalibration::guiWakeup(){
	ofAddListener(glDelegate->mousePressed,this, &CameraCalibration::mousePressed);
	ofAddListener(glDelegate->mouseDragged,this, &CameraCalibration::mouseDragged);
	ofAddListener(glDelegate->keyPressed,this, &CameraCalibration::keyPressed);
	
	w = glDelegate->m_Width;
	h = glDelegate->m_Width;
}


void CameraCalibration::setup(){
	verdana.loadFont("verdana.ttf",40);
	
	keystoneXml = new ofxXmlSettings;
	keystoneXml->loadFile("keystoneSettings.xml");
	int numFloor = keystoneXml->getNumTags("cameras");
	if(numFloor != 1){
		cout<<"====== ERROR: No cameras in keystone xml ======"<<endl;
	} else {

		keystoneXml->pushTag("cameras", 0);
		for(int u=0;u<3;u++){
			keystoneXml->pushTag("camera", u);
			int numCorners = keystoneXml->getNumTags("corner");
			if(numCorners != 4){
			} else {
				for(int i=0;i<4;i++){
					cameras[u]->warp->SetCorner( keystoneXml->getAttribute("corner", "number", 0, i) ,  keystoneXml->getAttribute("corner", "x", 0.0, i),  keystoneXml->getAttribute("corner", "y", 0.0, i));
				}
			}
			keystoneXml->popTag();			
		}
		keystoneXml->popTag();
				
	}
	
	for(int i=0;i<3;i++){
		cameras[i]->warp->MatrixCalculate();
		ofxPoint2f a[4];
		a[0] = ofxPoint2f(0,0);
		a[1] = ofxPoint2f(1,0);
		a[2] = ofxPoint2f(1,1);
		a[3] = ofxPoint2f(0,1);
		cameras[i]->coordWarp->calculateMatrix(a, cameras[i]->warp->corners);
		
		for(int u=0;u<4;u++){
//			cameras[i]->warp->SetCorner();	
		}
		
	}
	
	
}
void CameraCalibration::update(){
	if(selectedKeystoner == 0){
		offset = 50;
		w = glDelegate->m_Width-100;
		h = glDelegate->m_Width*0.75-100;
	}
}

void CameraCalibration::drawOnFloor(){
}
void CameraCalibration::draw(){
/*	if(drawDebug){
		for(int i=0;i<10;i++){
			float a = 0.3;
			
			if(selectedKeystoner == i){
				a = 1.0;
			}
			applyProjection(objects[i]);					
			if(i>0&&i<4){
				drawGrid(objects[i]->name, (objects[i]->aspect), 1.0/ (objects[i]->aspect), false, a, 0.02);	
				
			} else {
				drawGrid(objects[i]->name, (objects[i]->aspect), 10, (i==0)?true : false, a, 1.0);	
			}
			
			glPopMatrix();
			
		}
	}
	ofSetColor(255, 255, 0);
*/
}

void CameraCalibration::drawSettings(){
	glPushMatrix();
	glTranslated(offset, offset, 0);
	glPushMatrix();

	ofSetColor(255, 255, 255, 255);
	glPushMatrix();
	applyWarp(0,w,h);
	getPlugin<Cameras*>(controller)->vidGrabber[0]->draw(0,0,1,1);
	glPopMatrix();
	
	for(int i=0;i<4;i++){
		ofSetColor(255,0, 0,255);
		if(selectedCorner == i){
			ofSetColor(255,255, 0,255);
		}
		ofxVec2f v = cameras[selectedKeystoner]->warp->corners[i];

		ofEllipse(v.x*w, v.y*h, 10, 10);
	}		
	
	
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255,40);
	ofRect(0, 0, w, h);
		
	ofSetColor(255, 255, 255, 255);
	
	
	if(selectedKeystoner == 0){
		//Cam 0
		glPushMatrix();
		glScaled(0.3, 0.3, 1.0);
		verdana.drawString("PROJECTION", 0, 40);
		glPopMatrix();
		
		projection()->applyFloorProjection(w, h);
		ofSetColor(255, 255, 255, 40);
		ofRect(0, 0, 1*projection()->getFloor()->aspect, 1);
		glPopMatrix();

		
		
		

		
	}
		
	glPopMatrix();
	glPopMatrix();
	
/*	glPushMatrix();
	glTranslated(offset, offset, 0);
	glPushMatrix();
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255,70);
	ofRect(0, 0, w, h);
	
	glScaled(w, h, 1.0);
	for(int i=0;i<4;i++){
		ofSetColor(255,0, 0);
		if(selectedCorner == i){
			ofSetColor(255,255, 0);
		}
		ofxVec2f v = objects[selectedKeystoner]->warp->corners[i];
		ofEllipse(v.x, v.y, 0.05, 0.05);
	}	
	glPopMatrix();
	
	//	drawDebugGrids(w,h);
	
	for(int i=0;i<10;i++){
		float a = 0.3;
		if(selectedKeystoner == i){
			a = 1.0;
		}		
		applyProjection(objects[i], w, h);	
		
		if(i>0&&i<4){
			drawGrid(objects[i]->name, (objects[i]->aspect), 1.0/ (objects[i]->aspect), false, a, 0.02);	
			
		} else {
			drawGrid(objects[i]->name, (objects[i]->aspect), 10, (i==0)?true : false, a, 1.0);	
		}
		
		glPopMatrix();
		
	}
	
	
	glPopMatrix();*/
	
}

void CameraCalibration::mousePressed(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/w);
	selectedCorner = cameras[selectedKeystoner]->warp->GetClosestCorner(curMouse.x, curMouse.y);
	lastMousePos = curMouse;
}

void CameraCalibration::mouseDragged(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/w);
	ofxVec2f newPos =  cameras[selectedKeystoner]->warp->corners[selectedCorner] + (curMouse-lastMousePos);
	cameras[selectedKeystoner]->warp->SetCorner(selectedCorner, newPos.x, newPos.y);
	lastMousePos = curMouse;
	cameras[selectedKeystoner]->warp->MatrixCalculate();
	saveXml();
}

void CameraCalibration::keyPressed(ofKeyEventArgs & args){
	ofxVec2f newPos =  cameras[selectedKeystoner]->warp->corners[selectedCorner] ;
	
	if(args.key == 63233){
		newPos -= ofxVec2f(0,-0.001);
	}
	if(args.key == 63232){
		newPos += ofxVec2f(0,-0.001);
	}
	if(args.key == 63234){
		newPos += ofxVec2f(-0.001,0);
	}
	if(args.key == 63235){
		newPos -= ofxVec2f(-0.001,0);
	}
	cameras[selectedKeystoner]->warp->SetCorner(selectedCorner, newPos.x, newPos.y);
	cameras[selectedKeystoner]->warp->MatrixCalculate();
	
	saveXml();
}

void CameraCalibration::saveXml(){
	
	keystoneXml->pushTag("cameras", 0);
	for(int u=0;u<3;u++){
		keystoneXml->pushTag("camera", u);
		
		int numCorners = keystoneXml->getNumTags("corner");
		if(numCorners != 4){
			cout<<"====== ERROR: Wrong number of corners ======"<<endl;
		} else {
			for(int i=0;i<4;i++){
				keystoneXml->setAttribute("corner", "number", i, i);
				keystoneXml->setAttribute("corner", "x", cameras[u]->warp->corners[i].x, i);
				keystoneXml->setAttribute("corner", "y", cameras[u]->warp->corners[i].y, i);
			}
		}
		keystoneXml->popTag();
		
	}
	keystoneXml->popTag();
	
	keystoneXml->saveFile("keystoneSettings.xml");
	
	
}

void CameraCalibration::applyWarp(int cam, float _w, float _h){
	glPushMatrix();
	float setW = 1.0;
	float setH = 1.0;
	
	glScaled(_w, _h, 1.0);
	cameras[cam]->warp->MatrixMultiply();
	glScaled(setW, setH, 1.0);
}

