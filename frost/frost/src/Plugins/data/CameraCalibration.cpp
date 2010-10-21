
#include "CameraCalibration.h"
#include "ProjectionSurfaces.h"
#include "Cameras.h"
#include "PluginController.h"

#include "LaLinea.h"


int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
	int i, j, c = 0;
	for (i = 0, j = nvert-1; i < nvert; j = i++) {
		if ( ((verty[i]>testy) != (verty[j]>testy)) &&
			(testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
			c = !c;
	}
	return c;
}

CameraCalibration::CameraCalibration(){
	type = DATA;
	
	for(int cameraIterator=0;cameraIterator<4;cameraIterator++){
		cameras.push_back(new CameraCalibrationObject);
		cameras[cameraIterator]->warp = new Warp();
		cameras[cameraIterator]->coordWarp = new coordWarping;
		cameras[cameraIterator]->coordWarpCalibration = new coordWarping;
		
		cameras[cameraIterator]->warpLeftHalf = new Warp();
		cameras[cameraIterator]->coordWarpLeftHalf = new coordWarping;
		cameras[cameraIterator]->coordWarpCalibrationLeftHalf = new coordWarping;

		
		cameras[cameraIterator]->warpRightHalf = new Warp();
		cameras[cameraIterator]->coordWarpRightHalf = new coordWarping;
		cameras[cameraIterator]->coordWarpCalibrationRightHalf = new coordWarping;
		
		if(cameraIterator<3){
			cameras[cameraIterator]->name = "CAMERA "+ofToString(cameraIterator, 0);
		} else {
			cameras[cameraIterator]->name = "MOVIE FOR CAMERA 2";
		}
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
	
	w = 640;
	h = 480;
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
		for(int u=0;u<4;u++){
			keystoneXml->pushTag("camera", u);
			int numCorners = keystoneXml->getNumTags("corner");
			if(numCorners < 4){
			} else {
				for(int cameraIterator=0;cameraIterator<4;cameraIterator++){
					cameras[u]->calibHandles[cameraIterator] = ofxPoint2f(keystoneXml->getAttribute("corner", "x", 0.0, cameraIterator),  keystoneXml->getAttribute("corner", "y", 0.0, cameraIterator));
				}
				if(u>=2){
					for(int i=4;i<6;i++){
						cameras[u]->calibHandles[i] = ofxPoint2f(keystoneXml->getAttribute("corner", "x", 0.0, i),  keystoneXml->getAttribute("corner", "y", 0.0, i));
					}						
				}
			}
			keystoneXml->popTag();			
		}
		keystoneXml->popTag();
		
	}
	
	cameras[0]->calibPoints[0] = projection()->getFloor()->coordWarp->transform(0.2,0.2);
	cameras[0]->calibPoints[1] = projection()->getFloor()->coordWarp->transform(0.6,0.2);
	cameras[0]->calibPoints[2] = projection()->getFloor()->coordWarp->transform(0.6,0.8);
	cameras[0]->calibPoints[3] = projection()->getFloor()->coordWarp->transform(0.2,0.8);
	
	cameras[1]->calibPoints[0] = projection()->getFloor()->coordWarp->transform(0.35,0.4);
	cameras[1]->calibPoints[1] = projection()->getFloor()->coordWarp->transform(0.7,0.4);
	cameras[1]->calibPoints[2] = projection()->getFloor()->coordWarp->transform(0.7,0.7);
	cameras[1]->calibPoints[3] = projection()->getFloor()->coordWarp->transform(0.35,0.7);
	
	cameras[2]->calibPoints[0] = projection()->getWall()->coordWarp->transform(0.0,0.3);
	cameras[2]->calibPoints[1] = projection()->getWall()->coordWarp->transform(1,0.3);
	cameras[2]->calibPoints[2] = projection()->getWall()->coordWarp->transform(1,1);
	cameras[2]->calibPoints[3] = projection()->getWall()->coordWarp->transform(0,1);
	
	ofxVec2f v1 = projection()->getCurtain(3)->coordWarp->transform(0,1) - projection()->getCurtain(0)->coordWarp->transform(1,1);
	ofxVec2f v2 = projection()->getCurtain(5)->coordWarp->transform(0,1) - projection()->getCurtain(2)->coordWarp->transform(1,1);
	
	cameras[2]->calibPoints[4] = projection()->getCurtain(0)->coordWarp->transform(1,1) + v1*0.5;
	cameras[2]->calibPoints[5] = projection()->getCurtain(2)->coordWarp->transform(1,1) + v2*0.5;
	
	cameras[3]->calibPoints[0] = projection()->getWall()->coordWarp->transform(0.0,0.3);
	cameras[3]->calibPoints[1] = projection()->getWall()->coordWarp->transform(1,0.3);
	cameras[3]->calibPoints[2] = projection()->getWall()->coordWarp->transform(1,1);
	cameras[3]->calibPoints[3] = projection()->getWall()->coordWarp->transform(0,1);

	cameras[3]->calibPoints[4] = projection()->getCurtain(0)->coordWarp->transform(1,1) + v1*0.5;
	cameras[3]->calibPoints[5] = projection()->getCurtain(2)->coordWarp->transform(1,1) + v2*0.5;
	
	reCalibrate();
	
}

void CameraCalibration::reCalibrate(){
	for(int cameraIterator=0;cameraIterator<4;cameraIterator++){
		
		// normal cams
		
		ofxPoint2f a[4];
		a[0] = ofxPoint2f(0,0);
		a[1] = ofxPoint2f(1,0);
		a[2] = ofxPoint2f(1,1);
		a[3] = ofxPoint2f(0,1);
		
		cameras[cameraIterator]->coordWarpCalibration->calculateMatrix(cameras[cameraIterator]->calibHandles, cameras[cameraIterator]->calibPoints);	
		
		ofxPoint2f corners[4];
		for(int cornerIterator=0;cornerIterator<4;cornerIterator++){
			corners[cornerIterator] = cameras[cameraIterator]->coordWarpCalibration->transform(a[cornerIterator].x, a[cornerIterator].y);
			cameras[cameraIterator]->warp->SetCorner(cornerIterator,corners[cornerIterator].x,corners[cornerIterator].y);
		}
		
		cameras[cameraIterator]->warp->MatrixCalculate();
		cameras[cameraIterator]->coordWarp->calculateMatrix(a, cameras[cameraIterator]->warp->corners);		
		
		if(cameraIterator >= 2){
			
			{// folding wall left
				
				a[0] = ofxPoint2f(0,0);
				a[1] = ofxPoint2f(1,0);
				a[2] = ofxPoint2f(1,1);
				a[3] = ofxPoint2f(0,1);
				
				ofxPoint2f calibHandlesLeftHalf[4];
				ofxPoint2f calibPointsLeftHalf[4];
				
				calibHandlesLeftHalf[0] = cameras[cameraIterator]->calibHandles[0];
				calibHandlesLeftHalf[1] = cameras[cameraIterator]->calibHandles[4];
				calibHandlesLeftHalf[2] = cameras[cameraIterator]->calibHandles[5];
				calibHandlesLeftHalf[3] = cameras[cameraIterator]->calibHandles[3];
				
				calibPointsLeftHalf[0] = cameras[cameraIterator]->calibPoints[0];
				calibPointsLeftHalf[1] = cameras[cameraIterator]->calibPoints[4];
				calibPointsLeftHalf[2] = cameras[cameraIterator]->calibPoints[5];
				calibPointsLeftHalf[3] = cameras[cameraIterator]->calibPoints[3];
				
				cameras[cameraIterator]->coordWarpCalibrationLeftHalf->calculateMatrix(calibHandlesLeftHalf, calibPointsLeftHalf);	
				
				ofxPoint2f corners[4];
				for(int cornerIterator=0;cornerIterator<4;cornerIterator++){
					corners[cornerIterator] = cameras[cameraIterator]->coordWarpCalibrationLeftHalf->transform(a[cornerIterator].x, a[cornerIterator].y);
					cameras[cameraIterator]->warpLeftHalf->SetCorner(cornerIterator,corners[cornerIterator].x,corners[cornerIterator].y);
				}
				
				cameras[cameraIterator]->warpLeftHalf->MatrixCalculate();
				cameras[cameraIterator]->coordWarpLeftHalf->calculateMatrix(a, cameras[cameraIterator]->warpLeftHalf->corners);		
			}
			
			{// folding wall right
				
				a[0] = ofxPoint2f(0,0);
				a[1] = ofxPoint2f(1,0);
				a[2] = ofxPoint2f(1,1);
				a[3] = ofxPoint2f(0,1);
				
				ofxPoint2f calibHandlesRightHalf[4];
				ofxPoint2f calibPointsRightHalf[4];
				
				calibHandlesRightHalf[0] = cameras[cameraIterator]->calibHandles[4];
				calibHandlesRightHalf[1] = cameras[cameraIterator]->calibHandles[1];
				calibHandlesRightHalf[2] = cameras[cameraIterator]->calibHandles[2];
				calibHandlesRightHalf[3] = cameras[cameraIterator]->calibHandles[5];
				
				calibPointsRightHalf[0] = cameras[cameraIterator]->calibPoints[4];
				calibPointsRightHalf[1] = cameras[cameraIterator]->calibPoints[1];
				calibPointsRightHalf[2] = cameras[cameraIterator]->calibPoints[2];
				calibPointsRightHalf[3] = cameras[cameraIterator]->calibPoints[5];
				
				cameras[cameraIterator]->coordWarpCalibrationRightHalf->calculateMatrix(calibHandlesRightHalf, calibPointsRightHalf);	
				
				ofxPoint2f corners[4];
				for(int cornerIterator=0;cornerIterator<4;cornerIterator++){
					corners[cornerIterator] = cameras[cameraIterator]->coordWarpCalibrationRightHalf->transform(a[cornerIterator].x, a[cornerIterator].y);
					cameras[cameraIterator]->warpRightHalf->SetCorner(cornerIterator,corners[cornerIterator].x,corners[cornerIterator].y);
				}
				
				cameras[cameraIterator]->warpRightHalf->MatrixCalculate();
				cameras[cameraIterator]->coordWarpRightHalf->calculateMatrix(a, cameras[cameraIterator]->warpRightHalf->corners);		
				
			}
		}
		
		
	}
	
}
void CameraCalibration::update(){
	
}

void CameraCalibration::drawOnFloor(){
}
void CameraCalibration::draw(){
	ofDisableAlphaBlending();
	if(drawDebug){
		ofSetColor(255, 255, 255, 255);
		glPushMatrix();
		//applyWarp(selectedKeystoner);{
		glPushMatrix();{
			if(selectedKeystoner >= 2){
				
				glPopMatrix();
				//
				ofxVec2f calibHandlesInSpace[6];
				for(int i=0;i<6;i++){
					calibHandlesInSpace[i] = cameras[selectedKeystoner]->calibHandles[i];
				}
				
				
				float w = 640;
				float h = 480;
	
				getPlugin<Cameras*>(controller)->getVidGrabber(2)->getTextureReference().bind();				

				glBegin(GL_QUADS);{					
					//top left
					
					
					glTexCoord2f(calibHandlesInSpace[0].x*w, calibHandlesInSpace[0].y*h); 
					glVertex2f(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[0].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[0].y);
					
					//bottom left
					glTexCoord2f(calibHandlesInSpace[4].x*w, calibHandlesInSpace[4].y*h); 
					glVertex2f(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[4].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[4].y);
					
				
					glTexCoord2f(calibHandlesInSpace[5].x*w, calibHandlesInSpace[5].y*h); 
					glVertex2f(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[5].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[5].y);
					

					glTexCoord2f(calibHandlesInSpace[3].x*w, calibHandlesInSpace[3].y*h); 
					glVertex2f(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[3].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[3].y);
			
				}glEnd();
					
				glBegin(GL_QUADS);{					
					glTexCoord2f(calibHandlesInSpace[4].x*w, calibHandlesInSpace[4].y*h); 
					glVertex2f(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[4].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[4].y);
					
					//top right
					glTexCoord2f(calibHandlesInSpace[1].x*w, calibHandlesInSpace[1].y*h); 
					glVertex2f(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[1].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[1].y);
					
					//bottom right
					glTexCoord2f(calibHandlesInSpace[2].x*w, calibHandlesInSpace[2].y*h); 
					glVertex2f(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[2].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[2].y);

					glTexCoord2f(calibHandlesInSpace[5].x*w, calibHandlesInSpace[5].y*h); 
					glVertex2f(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[5].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[5].y);
					
					
				}glEnd();
				getPlugin<Cameras*>(controller)->getVidGrabber(2)->getTextureReference().unbind();
				
				
				glPushMatrix();
				
				//
			} else if(selectedKeystoner < 3)
				getPlugin<Cameras*>(controller)->draw(selectedKeystoner,0,0,1,1);
			else
				getPlugin<Cameras*>(controller)->draw(selectedKeystoner-1,0,0,1,1);
		}glPopMatrix();
		
		ofFill();
		ofSetColor(255, 0, 0);
		

		for(int i=0;i<4;i++){
			ofEllipse(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[i].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[i].y, 5, 5);			
		}
		if(selectedKeystoner >= 2){
			for(int i=4;i<6;i++){
				ofEllipse(ofGetWidth()*cameras[selectedKeystoner]->calibPoints[i].x, ofGetHeight()*cameras[selectedKeystoner]->calibPoints[i].y, 5, 5);			
			}			
		}
		
		/*		if(selectedKeystoner == 2){
		 applyWallProjection();
		 
		 glPopMatrix();
		 }
		 */
	}
	
	
	
}

void CameraCalibration::drawSettings(){
	ofPushStyle();
	
	ofFill();
	
	glPushMatrix();
	glPushMatrix();
	
	ofSetColor(255, 255, 255, 255);
	glPushMatrix();
	if(selectedKeystoner < 3)
		getPlugin<Cameras*>(controller)->draw(selectedKeystoner,0,0,w,h);
	else
		getPlugin<Cameras*>(controller)->draw(selectedKeystoner-1,0,0,w,h);
	
	glPopMatrix();
	glPopMatrix();
	

	
	ofNoFill();
	ofSetLineWidth(2);
	for(int cameraIterator=0;cameraIterator<4;cameraIterator++){
		ofSetColor(255, 0, 0, 225);
		ofEllipse(cameras[selectedKeystoner]->calibHandles[cameraIterator].x*w, cameras[selectedKeystoner]->calibHandles[cameraIterator].y*h, 15, 15);
	}
	if(selectedKeystoner >= 2){
		ofSetColor(255, 255, 0, 225);
		ofEllipse(cameras[selectedKeystoner]->calibHandles[4].x*w, cameras[selectedKeystoner]->calibHandles[4].y*h, 15, 15);
		ofEllipse(cameras[selectedKeystoner]->calibHandles[5].x*w, cameras[selectedKeystoner]->calibHandles[5].y*h, 15, 15);
	}
	ofFill();
	
	
	
	
	ofPopStyle();
	
}

void CameraCalibration::mousePressed(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/h);
	ofxPoint2f m = ofxPoint2f(curMouse.x, curMouse.y);
	//selectedCorner = cameras[selectedKeystoner]->warp->GetClosestCorner(curMouse.x, curMouse.y);'
	float closestDist = -1;
	for(int cameraIterator=0;cameraIterator<4;cameraIterator++){
		if(cameras[selectedKeystoner]->calibHandles[cameraIterator].distance(m) < closestDist || closestDist == -1){
			closestDist = cameras[selectedKeystoner]->calibHandles[cameraIterator].distance(m);
			selectedCorner = cameraIterator;
		}
	}
	if(selectedKeystoner >= 2){
		for(int i=4;i<6;i++){
			if(cameras[selectedKeystoner]->calibHandles[i].distance(m) < closestDist || closestDist == -1){
				closestDist = cameras[selectedKeystoner]->calibHandles[i].distance(m);
				selectedCorner = i;
			}
		}
	}
	lastMousePos = curMouse;
}

void CameraCalibration::mouseDragged(ofMouseEventArgs & args){
	ofxVec2f curMouse = ofxVec2f((float)(glDelegate->mouseX-offset)/w, (float)(glDelegate->mouseY-offset)/h);
	ofxVec2f newPos =  cameras[selectedKeystoner]->calibHandles[selectedCorner] + (curMouse-lastMousePos);
	cameras[selectedKeystoner]->calibHandles[selectedCorner] = newPos;
	lastMousePos = curMouse;
	reCalibrate();
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
	
	reCalibrate();
	
	saveXml();
}

void CameraCalibration::saveXml(){
	keystoneXml->pushTag("cameras", 0);
	for(int u=0;u<4;u++){
		keystoneXml->pushTag("camera", u);
		
		int numCorners = keystoneXml->getNumTags("corner");
		if(numCorners < 4){
			cout<<"====== ERROR: Wrong number of corners ======"<<endl; 
		} else {
			for(int cameraIterator=0;cameraIterator<4;cameraIterator++){
				keystoneXml->setAttribute("corner", "number", cameraIterator, cameraIterator);
				keystoneXml->setAttribute("corner", "x", cameras[u]->calibHandles[cameraIterator].x, cameraIterator);
				keystoneXml->setAttribute("corner", "y", cameras[u]->calibHandles[cameraIterator].y, cameraIterator);
				
			}
			if(u >= 2){
				for(int i=4;i<6;i++){
					keystoneXml->setAttribute("corner", "number", i, i);
					keystoneXml->setAttribute("corner", "x", cameras[u]->calibHandles[i].x, i);
					keystoneXml->setAttribute("corner", "y", cameras[u]->calibHandles[i].y, i);					
				}
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

ofxVec2f CameraCalibration::convertCoordinate(int cam, float x, float y){
	ofxVec2f v;
	ofxPoint2f p;
	if(cam < 2){
		p = cameras[cam]->coordWarp->transform(x,y);
	} else {
		
		float vertx[4];
		float verty[4];
		
		ofxVec2f midVec = cameras[cam]->calibHandles[5] - cameras[cam]->calibHandles[4];
		ofxVec2f point = ofxPoint2f(x,y) - cameras[cam]->calibHandles[4];
		float angle = midVec.angle(point);
		
		//Tjekker om spejlvendt (counterclockwise test) 
		ofxPoint2f * points = cameras[cam]->calibHandles;
		float cross = (points[1].x-points[0].x)*(points[2].y-points[1].y) - (points[1].y-points[0].y)*(points[2].x-points[1].x);
		if(cross >= 0) angle = -angle;
		
		if(angle < 0){
			p = cameras[cam]->coordWarpLeftHalf->transform(x,y);
		} else {
			p = cameras[cam]->coordWarpRightHalf->transform(x,y);
		}
		
	/*	//test if in left
		
		vertx[0] = cameras[cam]->calibHandles[0].x;
		vertx[1] = cameras[cam]->calibHandles[4].x;
		vertx[2] = cameras[cam]->calibHandles[5].x;
		vertx[3] = cameras[cam]->calibHandles[3].x;

		verty[0] = cameras[cam]->calibHandles[0].y;
		verty[1] = cameras[cam]->calibHandles[4].y;
		verty[2] = cameras[cam]->calibHandles[5].y;
		verty[3] = cameras[cam]->calibHandles[3].y;
		
		if (pnpoly(4, vertx, verty, x, y) > 0) {
			p = cameras[cam]->coordWarpLeftHalf->transform(x,y);
		}

		//test if in right
		
		vertx[0] = cameras[cam]->calibHandles[4].x;
		vertx[1] = cameras[cam]->calibHandles[1].x;
		vertx[2] = cameras[cam]->calibHandles[2].x;
		vertx[3] = cameras[cam]->calibHandles[5].x;
		
		verty[0] = cameras[cam]->calibHandles[4].y;
		verty[1] = cameras[cam]->calibHandles[1].y;
		verty[2] = cameras[cam]->calibHandles[2].y;
		verty[3] = cameras[cam]->calibHandles[5].y;
		
		if (pnpoly(4, vertx, verty, x, y) > 0) {
			p = cameras[cam]->coordWarpRightHalf->transform(x,y);
		}*/
		
		
		
	}
	v.x = p.x;
	v.y = p.y;
	return v;
	
}