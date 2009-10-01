#include "PluginController.h"
#include "ProjectionSurfaces.h"
#include "BlobTracking.h"
#include "Cameras.h"
#include "CameraCalibration.h"

unsigned long int PersistentBlob::idCounter = 0;

PersistentBlob::PersistentBlob(){
	id = PersistentBlob::idCounter++;
	timeoutCounter = 0;
}

Tracker::Tracker(){
	cw = 1280/2; 
	ch=960/2;
	//	int w = grabber->getWidth();//grabber->width; 
	//	int h = grabber->getHeight();//grabber->height;
	grayImageBlured.allocate(cw,ch);
	grayImage.allocate(cw,ch);
	grayBg.allocate(cw,ch);
	grayDiff.allocate(cw,ch);
	bLearnBakground = false;
	mouseBlob = false;
	postBlur = 0;
	postThreshold = 0;
}

void Tracker::update(){ 
	bool bNewFrame = false;
	
	
	if ((getPlugin<Cameras*>(controller)->isFrameNew(cameraId) && active) || mouseBlob){
		if (getPlugin<Cameras*>(controller)->isFrameNew(cameraId) && active){
			grayImage.setFromPixels(getPlugin<Cameras*>(controller)->getPixels(cameraId), getPlugin<Cameras*>(controller)->getWidth(cameraId),getPlugin<Cameras*>(controller)->getHeight(cameraId));
			grayImageBlured = grayImage;
			grayImageBlured.blur(blur);
			
			if (bLearnBakground == true){
				grayBg = grayImageBlured;
				ofImage saveImg;
				saveImg.allocate(grayBg.getWidth(), grayBg.getHeight(), OF_IMAGE_GRAYSCALE);
				saveImg.setFromPixels(grayBg.getPixels(), grayBg.getWidth(), grayBg.getHeight(), false);
				saveImg.saveImage("blobtrackerBackground"+ofToString(cameraId)+".png");
				bLearnBakground = false;
			}
			
			grayDiff.absDiff(grayBg, grayImageBlured);
			grayDiff.threshold(threshold);
			
			if(postBlur > 0){
				grayDiff.blur(postBlur);
				if(postThreshold > 0){
					grayDiff.threshold(postThreshold, false);
				}
				
			}
			
			contourFinder.findContours(grayDiff, 20, (getPlugin<Cameras*>(controller)->getWidth()*getPlugin<Cameras*>(controller)->getHeight())/3, 10, false, true);	
			
			postBlur = 0;
			postThreshold = 0;
		}
		for(int u=0;u<numPersistentBlobs();u++){
			ofxPoint2f p = persistentBlobs[u].centroid - persistentBlobs[u].lastcentroid;
			persistentBlobs[u].centroidV = ofxVec2f(p.x, p.y);
			persistentBlobs[u].lastcentroid = persistentBlobs[u].centroid ;
			
		}
		
		for(int i=0;i<numBlobs();i++){
			bool blobFound = false;
			for(int u=0;u<numPersistentBlobs();u++){
				ofxPoint2f centroid = ofxPoint2f(getBlob(i).centroid.x, getBlob(i).centroid.y);
				if(centroid.distance(getBlobById(getPersistentBlobId(u)).centroid) < 0.09){
					blobFound = true;
					
					persistentBlobs[u].centroid = centroid;
					persistentBlobs[u].timeoutCounter = 0;
					persistentBlobs[u].blob = getBlob(i);
					break;
				}
			}
			if(!blobFound){
				ofxPoint2f centroid = ofxPoint2f(getBlob(i).centroid.x, getBlob(i).centroid.y);
				
				PersistentBlob newB;
				newB.blob = getBlob(i);
				newB.centroid = centroid;
				persistentBlobs.push_back(newB);
				
			}
		}
		
		for(int u=0;u<numPersistentBlobs();u++){
			persistentBlobs[u].timeoutCounter ++;
			if(persistentBlobs[u].timeoutCounter > 10){
				deletePersistentBlobById(getPersistentBlobId(u));
			}
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
	
	//ofxVec2f dv = (getPlugin<Cameras*>(controller))->undistortPoint(cameraId, blob->centroid.x, blob->centroid.y);
	ofxVec2f dv = blob->centroid;
	
	deBarrelledBlob.centroid = ofPoint(dv.x,dv.y);
	
	/*for(int i=0;i<blob->nPts;i++){
	 ofxVec2f v = (getPlugin<Cameras*>(controller))->undistortPoint(cameraId, blob->pts[i].x, blob->pts[i].y);
	 deBarrelledBlob.pts.push_back(ofPoint(v.x, v.y));
	 }*/
	
	for(int i=0;i<blob->nPts;i++){
		ofxVec2f v = blob->pts[i];
		deBarrelledBlob.pts.push_back(ofPoint(v.x, v.y));
	}
	
	
	
	deBarrelledBlob.nPts = blob->nPts;
	
	
	
	ofxCvBlob b;
	float m = cw*ch;
	b.area = deBarrelledBlob.area/m;
	b.length = deBarrelledBlob.length/m;
	//bounding rect not defined
	
	ofxVec2f v = calibrator->convertCoordinate(cameraId, deBarrelledBlob.centroid.x/getWidth(), deBarrelledBlob.centroid.y/getHeight());
	
	b.centroid = ofPoint(v.x, v.y);
	b.hole = blob->hole;
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

ofxCvBlob Tracker::getBlobById(unsigned long int _id){
	for(int i=0;i<numPersistentBlobs();i++){
		if(persistentBlobs[i].id == _id){
			return persistentBlobs[i].blob;
		}
	}
}
int Tracker::numPersistentBlobs(){
	return persistentBlobs.size();
}
bool Tracker::persistentBlobExist(unsigned long int _id){
	
}
unsigned long int Tracker::getPersistentBlobId(int n){
	return persistentBlobs[n].id;
}
void Tracker::deletePersistentBlobById(unsigned long int id){
	for(int i=0;i<numPersistentBlobs();i++){
		if(persistentBlobs[i].id == id){
			persistentBlobs.erase(persistentBlobs.begin()+i);
			break;
		}
	}
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
			mouseGeneratedBlob.pts.push_back(ofPoint(x+cos(p)*0.12*(sin(p*5)+1.5)/3.0, y+sin(p)*0.12*(sin(p*5)+1.5)/3.0));
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
		ofImage loadImg;
		if (loadImg.loadImage("blobtrackerBackground"+ofToString(i)+".png")) {
			trackers[i]->grayBg.setFromPixels(loadImg.getPixels(), loadImg.getWidth(), loadImg.getHeight());
		} else {
			trackers[i]->bLearnBakground = true;
		}
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
		getPlugin<Cameras*>(controller)->draw(trackers[i]->cameraId,0,w*a*i,w,w*a);
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
			
			for(int u =0;u<trackers[i]->numPersistentBlobs();u++){
				ofxCvBlob b = trackers[i]->persistentBlobs[u].blob;
				ofSetColor(255, 0, 255);
				ofDrawBitmapString(ofToString(trackers[i]->persistentBlobs[u].id, 0), b.centroid.x*ofGetWidth(), b.centroid.y*ofGetHeight());
				ofSetColor(0, 0, 255);
				
				for(int x=0;x<b.nPts;x++){
					ofEllipse(b.pts[x].x*ofGetWidth(), b.pts[x].y*ofGetHeight(), 10, 10);
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


