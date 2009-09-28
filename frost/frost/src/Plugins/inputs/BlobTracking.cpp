#include "PluginController.h"
#include "ProjectionSurfaces.h"
#include "BlobTracking.h"
#include "Cameras.h"
#include "CameraCalibration.h"

Tracker::Tracker(){
	cw = 1024; 
	ch=768;
	//	int w = grabber->getWidth();//grabber->width; 
	//	int h = grabber->getHeight();//grabber->height;
	grayImageBlured.allocate(cw,ch);
	grayImage.allocate(cw,ch);
	grayBg.allocate(cw,ch);
	grayDiff.allocate(cw,ch);
	bLearnBakground = true;
	mouseBlob = false;
}

void Tracker::update(){ 
	bool bNewFrame = false;
	if(getPlugin<Cameras*>(controller)->isReady(cameraId)){
		ofxVideoGrabber * grabber = getPlugin<Cameras*>(controller)->getVidGrabber(cameraId);
		bNewFrame = grabber->isFrameNew();
		if (bNewFrame && active){
			
			grayImage.setFromPixels(grabber->getPixels(), grabber->getWidth(),grabber->getHeight());
			grayImageBlured = grayImage;
			grayImageBlured.blur(blur);
			
			if (bLearnBakground == true){
				grayBg = grayImageBlured;		
				bLearnBakground = false;
			}
			
			grayDiff.absDiff(grayBg, grayImageBlured);
			grayDiff.threshold(threshold);
			
			contourFinder.findContours(grayDiff, 20, (grabber->getWidth()*grabber->getHeight())/3, 10, false, true);	
//			simplifiedContourFinder.findSimplifiedContours(grayDiff, 20, (grabber->getWidth()*grabber->getHeight())/3, 10, false, true);	
		}
	}
}

void Tracker::findContours(){
}


int Tracker::numBlobs(){
	if(mouseBlob){
		return 1;
	}
	return contourFinder.nBlobs;
}

ofxCvBlob Tracker::getConvertedBlob(ofxCvBlob * blob, CameraCalibration * calibrator){
	ofxCvBlob deBarrelledBlob;

	//todo: correction of area and height ?
	deBarrelledBlob.area = blob->area;
	deBarrelledBlob.length = blob->length;
	
	ofxVec2f dv = (getPlugin<Cameras*>(controller))->undistortPoint(cameraId, blob->centroid.x, blob->centroid.y);

	deBarrelledBlob.centroid = ofPoint(dv.x,dv.y);
	
	for(int i=0;i<blob->nPts;i++){
		ofxVec2f v = (getPlugin<Cameras*>(controller))->undistortPoint(cameraId, blob->pts[i].x, blob->pts[i].y);
		deBarrelledBlob.pts.push_back(ofPoint(v.x, v.y));
	}

	deBarrelledBlob.nPts = blob->nPts;

	ofxCvBlob b;
	float m = cw*ch;
	b.area = deBarrelledBlob.area/m;
	b.length = deBarrelledBlob.length/m;
	
	//bounding rect ignored
	
	ofxVec2f v = calibrator->convertCoordinate(cameraId, deBarrelledBlob.centroid.x/getWidth(), deBarrelledBlob.centroid.y/getHeight());
	
	b.centroid = ofPoint(v.x, v.y);
		
	for(int i=0;i<deBarrelledBlob.nPts;i++){
		ofxVec2f v = calibrator->convertCoordinate(cameraId, deBarrelledBlob.pts[i].x/getWidth(), deBarrelledBlob.pts[i].y/getHeight());
		b.pts.push_back(ofPoint(v.x, v.y));
	}

	b.nPts = deBarrelledBlob.nPts;
	b.hole = blob->hole;
	
	return b;
}

ofxCvBlob Tracker::getBlob(int n){
	if(mouseBlob){
		return mouseGeneratedBlob;
	}
	return getConvertedBlob(&contourFinder.blobs[n], getPlugin<CameraCalibration*>(controller));
}
ofxCvBlob Tracker::getLargestBlob(){
	float largets = 0;
	ofxCvBlob b;
	for(int i=0;i<numBlobs();i++){
		if(getBlob(i).area > largets){
			largets = getBlob(i).area;
			b = getBlob(i);
		}
	}
	return b;
}

void Tracker::updateMouseBlob(float x, float y, int button){
	if(button != -1){
		mouseBlob = true;
		mouseGeneratedBlob = ofxCvBlob();
		mouseGeneratedBlob.area = 1;
		mouseGeneratedBlob.length = 1;
		mouseGeneratedBlob.centroid = ofPoint(x,y);
		int n= 300;
		for(int i=n;i>=0;i--){
			float p = TWO_PI*i/(float)n;
			mouseGeneratedBlob.pts.push_back(ofPoint(x+cos(p)*0.06, y+sin(p)*0.06));
		}
		mouseGeneratedBlob.nPts = n;
	} else {
		mouseBlob = false;
	}
}


ofxCvBlob Tracker::smoothBlob(ofxCvBlob blob, float smooth){
	ofxCvBlob n = blob;
	vector<ofxVec2f> p;
//	contourSimp.smooth(blob.pts, p, smooth);
	for(int i=0;i<p.size();i++){
		n.pts[i] = p[i];
	}
	return n;

}
void Tracker::extrudeBlob(ofxCvBlob * blob, float value){
	
}



int Tracker::getWidth(){
	return cw;
}
int Tracker::getHeight(){
	return ch;
}


BlobTracking::BlobTracking(){
	type = INPUT;
	drawDebug = false;
}
void BlobTracking::setup(){
	for(int i=0;i<3;i++){
		trackers.push_back(new Tracker());
		trackers[i]->cameraId = i;
		trackers[i]->threshold = initThreshold[i];
		trackers[i]->blur = initBlur[i];
		trackers[i]->active = initActive[i];
		trackers[i]->controller = controller;
	}
}
void BlobTracking::update(){
	//#pragma omp parallel for
	for(int i=0;i<trackers.size();i++){
		trackers[i]->update();		
	}	
	for(int i=0;i<trackers.size();i++){
		trackers[i]->findContours();	
	}	
	
}

void BlobTracking::drawSettings(){
	
	for(int i=0;i<trackers.size();i++){
		ofSetColor(255, 255, 255);
		
		int w = 250;
		float a = 480.0/640.0;
		ofxVideoGrabber * grabber = getPlugin<Cameras*>(controller)->getVidGrabber(trackers[i]->cameraId);
		if(grabber != NULL){
			grabber->draw(0,w*a*i,w,w*a);
		}
		trackers[i]->grayImageBlured.draw(w,w*a*i,w,w*a);
		trackers[i]->grayBg.draw(w*2,w*a*i, w,w*a);
		trackers[i]->grayDiff.draw(w*3,w*a*i,w,w*a);
		trackers[i]->contourFinder.draw(w*3,w*a*i,w,w*a);
//		trackers[i]->simplifiedContourFinder.draw(w*3,w*a*i,w,w*a);
	}
}

void BlobTracking::draw(){
	if(drawDebug){
		ofEnableAlphaBlending();
		glBlendFunc (GL_SRC_COLOR, GL_ONE);	
		
		for(int i=0;i<trackers.size();i++){
			ofSetColor(255, 255, 255);
			glPushMatrix();
			getPlugin<CameraCalibration*>(controller)->applyWarp(i);
			trackers[i]->grayDiff.draw(0,0,1,1);
			
			glPopMatrix();
			
			for(int u =0;u<trackers[i]->numBlobs();u++){
				ofxCvBlob b = trackers[i]->getBlob(u);
				ofSetColor(255, 0, 255);
				
				for(int x=0;x<b.nPts;x++){
					ofEllipse(b.pts[x].x*ofGetWidth(), b.pts[x].y*ofGetHeight(), 5, 5);
				}
			}
		}
		glPopMatrix();
		
	}
}

void BlobTracking::setThreshold(int n, float v){
	if(trackers.size() < 1){
		initThreshold[n] = v;
	} else 
		trackers[n]->threshold = v;;
	
}
void BlobTracking::setBlur(int n, int v){
	if(v % 2 == 0){
		v += 1;
	}
	if(trackers.size() < 1){
		initBlur[n] = v;
	} else { 		
		trackers[n]->blur = v;;
	}
	
}

void setActive(int n, bool b);

void BlobTracking::grab(int n){
	if(trackers.size() > n){
		trackers[n]->bLearnBakground = true;
	}
}
void BlobTracking::setActive(int n, bool b){
	if(trackers.size() < 1){
		initActive[n] = b;
	} else 
		trackers[n]->active = b;;
}


