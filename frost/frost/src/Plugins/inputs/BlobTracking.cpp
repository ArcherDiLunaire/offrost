#include "PluginController.h"
#include "ProjectionSurfaces.h"
#include "BlobTracking.h"
#include "Cameras.h"
#include "CameraCalibration.h"
#include "Frostscape.h"
#include "BlobLight.h"
#include "Frostscape.h"
#include "LaLineaFloor.h"

TrackerThread::TrackerThread(){
	updateContour = false;
}

void TrackerThread::start(){
	startThread(true, false);   // blocking, verbose
}

void TrackerThread::stop(){
	stopThread();
}

//--------------------------
void TrackerThread::threadedFunction(){
	
	while( isThreadRunning() != 0 ){
		if( lock() ){
			if(updateContour){
				contourFinder.findContours(grayDiff, 20, (w*h)/3, 10, false, true);	
				
				//(				opticalFlow.calc(grayLastImage,grayImage,3);
				
				updateContour = false;
			}
			unlock();
			ofSleepMillis(1 * 10);
		}
	}
}



unsigned long int PersistentBlob::idCounter = 0;




PersistentBlob::PersistentBlob(){
	id = PersistentBlob::idCounter++;
	timeoutCounter = 0;
}

ofxPoint2f PersistentBlob::getLowestPoint(){
	ofxPoint2f low;
	for(int i=0;i<blobs.size();i++){
		for(int u=0;u<blobs[i].nPts;u++){
			if(blobs[i].pts[u].y > low.y){
				low = blobs[i].pts[u];
			}
		}
	}
	return low;
}

Tracker::Tracker(){
	cw = 1280/2; 
	ch=960/2;
	//	int w = grabber->getWidth();//grabber->width; 
	//	int h = grabber->getHeight();//grabber->height;
	grayImageBlured.allocate(cw,ch);
	grayImage.allocate(cw,ch);
	grayLastImage.allocate(cw,ch);
	grayBg.allocate(cw,ch);
	grayBgMask.allocate(cw,ch);
	grayBgMask.set(255);
	grayDiff.allocate(cw,ch);
	bVideoPlayerWasActive = false;
	bLearnBakground = false;
	mouseBlob = false;
	postBlur = 0;
	bUseBgMask = false;
	postThreshold = 0;
}

void Tracker::setup(){
	
	thread.w = getPlugin<Cameras*>(controller)->getWidth();
	thread.h = getPlugin<Cameras*>(controller)->getHeight();	
	thread.grayDiff.setUseTexture(false);
	thread.grayDiff.allocate(cw,ch);
	thread.opticalFlow.allocate(cw,ch);
	thread.grayImage.allocate(cw,ch);
	thread.grayLastImage.allocate(cw,ch);
	
	thread.start();
}

void Tracker::update(){ 
	bool bNewFrame = false;
	
	
	if ((getPlugin<Cameras*>(controller)->isFrameNew(cameraId) && active) || mouseBlob){
		if (getPlugin<Cameras*>(controller)->isFrameNew(cameraId) && active){
			//	int t = ofGetElapsedTimeMillis();
			if(thread.lock()){
				
				grayImage.setFromPixels(getPlugin<Cameras*>(controller)->getPixels(cameraId), getPlugin<Cameras*>(controller)->getWidth(cameraId),getPlugin<Cameras*>(controller)->getHeight(cameraId));
				
				if(cameraId == 0){
					
					//Left Wall mask
					{
						ofxPoint2f wallLeft = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(0,1));
						ofxPoint2f wallRight = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(0,0));
						
						ofxPoint2f wallLeftCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(wallLeft.x, wallLeft.y);
						ofxPoint2f wallRightCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(wallRight.x, wallRight.y);
						
						ofxVec2f v = wallRightCam - wallLeftCam;
						ofxVec2f h = -ofxVec2f(-v.y, v.x)*0.3;
						
						wallLeftCam -= h * 0.15;
						wallRightCam -= h * 0.15;
						
						int nPoints = 4;
						CvPoint _cp[4]= {{wallLeftCam.x*cw,wallLeftCam.y*ch}, 
							{(wallLeftCam+h).x*cw,(wallLeftCam+h).y*ch},
							{(wallRightCam+h).x*cw,(wallRightCam+h).y*ch},
							{wallRightCam.x*cw,wallRightCam.y*ch}};			
						CvPoint* cp = _cp; cvFillPoly(grayImage.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
					}
					
					//Column mask
					{
						for(int c=0;c<3;c++){
							ofxPoint2f p[4];
							ofxVec2f v1 = *getPlugin<ProjectionSurfaces*>(controller)->getColumn(c)->corners[0] - *getPlugin<ProjectionSurfaces*>(controller)->getColumn(c)->corners[3];
							v1 = ofxVec2f(-v1.y,v1.x) * 0.05;
							for(int i=0;i<4;i++){
								ofxPoint2f point = *getPlugin<ProjectionSurfaces*>(controller)->getColumn(c)->corners[i];
								if(i == 0 || i == 3){
									point += v1;
								} else {
									point -= v1;
								}
								p[i] =  getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(point.x, point.y); 
							}
							
							int nPoints = 4;
							CvPoint _cp[4]= {{p[0].x*cw,p[0].y*ch}, 
								{(p[1]).x*cw,(p[1]).y*ch},
								{(p[2]).x*cw,(p[2]).y*ch},
								{p[3].x*cw,p[3].y*ch}};			
							CvPoint* cp = _cp; cvFillPoly(grayImage.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
						}
					}
				}
				
				if(cameraId == 2){
					CameraCalibrationObject * cam = getPlugin<CameraCalibration*>(controller)->cameras[2];
					 int nPoints = 4;
					 
					 float h = 10.0/(float)ch;
					 
					 ofxPoint2f points[4];
					
					
					if(cameraId == 2){
						grayImage.convertToRange(0,600);	
					}
					
					/* { //Top Left
					 points[0] = ofxPoint2f(cam->calibHandles[0].x, cam->calibHandles[0].y);
					 points[1] = ofxPoint2f(cam->calibHandles[4].x, cam->calibHandles[4].y);
					 points[2] = ofxPoint2f(cam->calibHandles[4].x, cam->calibHandles[4].y+h);
					 points[3] = ofxPoint2f(cam->calibHandles[0].x, cam->calibHandles[0].y+h);
					 
					 CvPoint _cp[4]= {{points[0].x*cw,points[0].y*ch}, 
					 {points[1].x*cw,points[1].y*ch},
					 {points[2].x*cw,points[2].y*ch},
					 {points[3].x*cw,points[3].y*ch}};			
					 CvPoint* cp = _cp; cvFillPoly(grayImage.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
					 
					 }
					 
					 { // Top Right
					 points[0] = ofxPoint2f(cam->calibHandles[4].x, cam->calibHandles[4].y);
					 points[1] = ofxPoint2f(cam->calibHandles[1].x, cam->calibHandles[1].y);
					 points[2] = ofxPoint2f(cam->calibHandles[1].x, cam->calibHandles[1].y+h);
					 points[3] = ofxPoint2f(cam->calibHandles[4].x, cam->calibHandles[4].y+h);
					 
					 CvPoint _cp[4]= {{points[0].x*cw,points[0].y*ch}, 
					 {points[1].x*cw,points[1].y*ch},
					 {points[2].x*cw,points[2].y*ch},
					 {points[3].x*cw,points[3].y*ch}};			
					 CvPoint* cp = _cp; cvFillPoly(grayImage.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
					 
					 }
					 
					 
					 { //Middle left
					 ofxVec2f vec1 = cam->calibHandles[3] - cam->calibHandles[0];
					 ofxVec2f vec2 = cam->calibHandles[5] - cam->calibHandles[4];
					 
					 ofxPoint2f p1 =  cam->calibHandles[0] + vec1 * 0.5;
					 ofxPoint2f p2 =  cam->calibHandles[4] + vec2 * 0.5;
					 
					 points[0] = ofxPoint2f(p1.x, p1.y);
					 points[1] = ofxPoint2f(p2.x, p2.y);
					 points[2] = ofxPoint2f(p2.x, p2.y-h);
					 points[3] = ofxPoint2f(p1.x, p1.y-h);
					 
					 CvPoint _cp[4]= {{points[0].x*cw,points[0].y*ch}, 
					 {points[1].x*cw,points[1].y*ch},
					 {points[2].x*cw,points[2].y*ch},
					 {points[3].x*cw,points[3].y*ch}};			
					 CvPoint* cp = _cp; cvFillPoly(grayImage.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
					 
					 }
					 
					 { //Middle right
					 ofxVec2f vec1 = cam->calibHandles[5] - cam->calibHandles[4];
					 ofxVec2f vec2 = cam->calibHandles[2] - cam->calibHandles[1];
					 
					 
					 ofxPoint2f p1 =  cam->calibHandles[4] + vec1 * 0.5;
					 ofxPoint2f p2 =  cam->calibHandles[1] + vec2 * 0.5;
					 
					 points[0] = ofxPoint2f(p1.x, p1.y);
					 points[1] = ofxPoint2f(p2.x, p2.y);
					 points[2] = ofxPoint2f(p2.x, p2.y-h);
					 points[3] = ofxPoint2f(p1.x, p1.y-h);
					 
					 CvPoint _cp[4]= {{points[0].x*cw,points[0].y*ch}, 
					 {points[1].x*cw,points[1].y*ch},
					 {points[2].x*cw,points[2].y*ch},
					 {points[3].x*cw,points[3].y*ch}};			
					 CvPoint* cp = _cp; cvFillPoly(grayImage.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
					 
					 }
					 */
					
					
					//TOP MASK
					
				/*	for (int i=0; i < 3; i++) {
						ofxVec2f vec1 = cam->calibHandles[3] - cam->calibHandles[0];
						ofxVec2f vec2 = cam->calibHandles[5] - cam->calibHandles[4];
						
						ofxPoint2f p1 =  cam->calibHandles[0] - vec1 * 0.5;
						ofxPoint2f p2 =  cam->calibHandles[4] - vec2 * 0.5;
						
						points[0] = ofxPoint2f(p1.x, p1.y);
						points[1] = ofxPoint2f(p2.x, p2.y);
						points[2] = ofxPoint2f(p2.x, p2.y+50);
						points[3] = ofxPoint2f(p1.x, p1.y+50);
						
						CvPoint _cp[4]= {{points[0].x*cw,points[0].y*ch}, 
							{points[1].x*cw,points[1].y*ch},
							{points[2].x*cw,points[2].y*ch},
							{points[3].x*cw,points[3].y*ch}};			
						CvPoint* cp = _cp; cvFillPoly(grayImage.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
						
						
					}*/
					
					
				}
				
				grayImageBlured = grayImage;
				grayImageBlured.blur(blur);
				
				if (!bVideoPlayerWasActive && getPlugin<Cameras*>(controller)->videoPlayerActive(cameraId) ) {
					bLearnBakground = true;
				}
				
				if (bVideoPlayerWasActive && !getPlugin<Cameras*>(controller)->videoPlayerActive(cameraId) ) {
					loadBackground();
				}
				
				if (bLearnBakground == true){
					if (bUseBgMask) {
						cvCopy(grayImageBlured.getCvImage(), grayBg.getCvImage(), grayBgMask.getCvImage());
						grayBg.flagImageChanged();
					} else {
						grayBg = grayImageBlured;
					}
					if (!getPlugin<Cameras*>(controller)->videoPlayerActive(cameraId)) {
						saveBackground();
					}
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
				
				
				if(cameraId == 0){
					//Mask
					if(!getPlugin<BlobLight*>(controller)->enabled){
						//Backwall mask
						{
							ofxPoint2f wallLeft = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(0,0));
							ofxPoint2f wallRight = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(1,0));
							
							ofxPoint2f wallLeftCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(wallLeft.x, wallLeft.y);
							ofxPoint2f wallRightCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(wallRight.x, wallRight.y);
							
							ofxVec2f v = wallRightCam - wallLeftCam;
							ofxVec2f h = -ofxVec2f(-v.y, v.x)*0.7;
							
							int nPoints = 4;
							CvPoint _cp[4]= {{wallLeftCam.x*cw,wallLeftCam.y*ch}, 
								{(wallLeftCam+h).x*cw,(wallLeftCam+h).y*ch},
								{(wallRightCam+h).x*cw,(wallRightCam+h).y*ch},
								{wallRightCam.x*cw,wallRightCam.y*ch}};			
							CvPoint* cp = _cp; cvFillPoly(grayDiff.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
						}
						
						//Right mask
						{
							ofxPoint2f wallLeft = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(1,0));
							ofxPoint2f wallRight = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(1,1));
							
							ofxPoint2f wallLeftCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(wallLeft.x, wallLeft.y);
							ofxPoint2f wallRightCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(wallRight.x, wallRight.y);
							
							ofxVec2f v = wallRightCam - wallLeftCam;
							ofxVec2f h = -ofxVec2f(-v.y, v.x)*0.7;
							
							int nPoints = 4;
							CvPoint _cp[4]= {{wallLeftCam.x*cw,wallLeftCam.y*ch}, 
								{(wallLeftCam+h).x*cw,(wallLeftCam+h).y*ch},
								{(wallRightCam+h).x*cw,(wallRightCam+h).y*ch},
								{wallRightCam.x*cw,wallRightCam.y*ch}};			
							CvPoint* cp = _cp; cvFillPoly(grayDiff.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
						}
						
					}
					
					// STAGE FRONT MASK (aber nicht ins frostscape)
					
					if(!getPlugin<Frostscape*>(controller)->enabled){
						ofxPoint2f frontEdgeLeft = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(0,0.85));
						ofxPoint2f frontEdgeRight = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(getPlugin<ProjectionSurfaces*>(controller)->getFloor()->aspect,0.85));
						ofxPoint2f frontEdgeLeftBack = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(0,1.3));
						ofxPoint2f frontEdgeRightBack = getPlugin<ProjectionSurfaces*>(controller)->convertToProjectionCoordinate(getPlugin<ProjectionSurfaces*>(controller)->getFloor(),ofxVec2f(getPlugin<ProjectionSurfaces*>(controller)->getFloor()->aspect,1.3));
						
						ofxPoint2f frontEdgeLeftCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(frontEdgeLeft.x, frontEdgeLeft.y);
						ofxPoint2f frontEdgeRightCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(frontEdgeRight.x, frontEdgeRight.y);
						ofxPoint2f frontEdgeLeftBackCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(frontEdgeLeftBack.x, frontEdgeLeftBack.y);
						ofxPoint2f frontEdgeRightBackCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(frontEdgeRightBack.x, frontEdgeRightBack.y);
						
						int nPoints = 4;
						CvPoint _cp[4]= {{frontEdgeLeftCam.x*cw,frontEdgeLeftCam.y*ch}, 
							{frontEdgeRightCam.x*cw,frontEdgeRightCam.y*ch},
							{frontEdgeRightBackCam.x*cw,frontEdgeRightBackCam.y*ch},
							{frontEdgeLeftBackCam.x*cw,frontEdgeLeftBackCam.y*ch}};			
						CvPoint* cp = _cp; cvFillPoly(grayDiff.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
					}
					
					//Wall mask
					if(getPlugin<LaLineaFloor*>(controller)->enabled){
						ofxPoint2f wallLeft = *getPlugin<ProjectionSurfaces*>(controller)->getColumn(0)->corners[3];
						ofxPoint2f wallRight = *getPlugin<ProjectionSurfaces*>(controller)->getColumn(2)->corners[2];
						
						ofxPoint2f wallLeftCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(wallLeft.x, wallLeft.y);
						ofxPoint2f wallRightCam = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(wallRight.x, wallRight.y);
						
						ofxVec2f v = wallRightCam - wallLeftCam;
						ofxVec2f h = -ofxVec2f(-v.y, v.x)*2;
						
						int nPoints = 4;
						CvPoint _cp[4]= {{wallLeftCam.x*cw,wallLeftCam.y*ch}, 
							{(wallLeftCam+h).x*cw,(wallLeftCam+h).y*ch},
							{(wallRightCam+h).x*cw,(wallRightCam+h).y*ch},
							{wallRightCam.x*cw,wallRightCam.y*ch}};			
						CvPoint* cp = _cp; cvFillPoly(grayDiff.getCvImage(), &cp, &nPoints, 1, cvScalar(0));
					}
				}
				
				
				
				
				// cout<<ofGetElapsedTimeMillis()-t<<endl;
				
				thread.grayDiff = grayDiff;
				thread.grayImage = grayImage;
				thread.grayLastImage = grayLastImage;
				thread.updateContour = true;
				thread.unlock();
				
				// contourFinder.findContours(grayDiff, 20, (getPlugin<Cameras*>(controller)->getWidth()*getPlugin<Cameras*>(controller)->getHeight())/3, 10, false, true);	
				
				
				grayLastImage = grayImage;
				
				
				postBlur = 0;
				postThreshold = 0; 
				
				bVideoPlayerWasActive = getPlugin<Cameras*>(controller)->videoPlayerActive(cameraId);
				
			}
		}
		for(int u=0;u<numPersistentBlobs();u++){
			ofxPoint2f p = persistentBlobs[u].centroid - persistentBlobs[u].lastcentroid;
			persistentBlobs[u].centroidV = ofxVec2f(p.x, p.y);
			persistentBlobs[u].lastcentroid = persistentBlobs[u].centroid ;
			persistentBlobs[u].blobs.clear();
		}
		
		for(int i=0;i<numBlobs();i++){
			bool blobFound = false;
			float shortestDist = 0;
			int bestId = -1;
			ofxPoint2f centroid = ofxPoint2f(getBlob(i).centroid.x, getBlob(i).centroid.y);
			
			//Går igennem alle grupper for at finde den nærmeste gruppe som blobben kan tilhøre
			//Magisk høj dist: 0.3
			for(int u=0;u<numPersistentBlobs();u++){
				float dist = centroid.distance(persistentBlobs[u].centroid);
				if(dist < 0.2 && (dist < shortestDist || bestId == -1)){
					bestId = u;
					shortestDist = dist;
					blobFound = true;
				}
			}
			
			if(blobFound){		
				//Fandt en gruppe som den her blob kan tilhøre.. Pusher blobben ind
				persistentBlobs[bestId].timeoutCounter = 0;
				persistentBlobs[bestId].blobs.push_back(getBlob(i));
				
				//regner centroid ud fra alle blobs i den
				persistentBlobs[bestId].centroid = ofxPoint2f();
				for(int g=0;g<persistentBlobs[bestId].blobs.size();g++){
					ofxPoint2f blobCentroid = ofxPoint2f(persistentBlobs[bestId].blobs[g].centroid.x, persistentBlobs[bestId].blobs[g].centroid.y);
					persistentBlobs[bestId].centroid += blobCentroid;					
				}
				persistentBlobs[bestId].centroid /= (float)persistentBlobs[bestId].blobs.size();
			}
			
			if(!blobFound){
				//Der var ingen gruppe til den her blob, så vi laver en
				PersistentBlob newB;
				newB.blobs.push_back(getBlob(i));
				newB.centroid = centroid;
				persistentBlobs.push_back(newB);		
			}
		}
		
		for(int u=0;u<numPersistentBlobs();u++){
			persistentBlobs[u].timeoutCounter ++;
			if(persistentBlobs[u].timeoutCounter > 10){
				deletePersistentBlobById(getPersistentBlobId(u));
			} else {
				
				
			}			
		}	
	}	
}

void Tracker::findContours(){
}

void Tracker::saveBackground(int num){
	ofLog(OF_LOG_NOTICE, "<<<<<<<< gemmer billede " + ofToString(cameraId));
	ofImage saveImg;
	saveImg.allocate(grayBg.getWidth(), grayBg.getHeight(), OF_IMAGE_GRAYSCALE);
	saveImg.setFromPixels(grayBg.getPixels(), grayBg.getWidth(), grayBg.getHeight(), false);
	saveImg.saveImage("blobTracker" +ofToString(cameraId)+"Background-" + ofToString(num) + ".png");
}

bool Tracker::loadBackground(int num){
	ofImage loadImg;
	if (loadImg.loadImage("blobTracker" +ofToString(cameraId)+"Background-" + ofToString(num) + ".png")) {
		grayBg.setFromPixels(loadImg.getPixels(), loadImg.getWidth(), loadImg.getHeight());
		return true;
	} else {
		return false;
	}
}

void Tracker::setBgMaskFromPixels(unsigned char* _pixels, int width, int height){
	grayBgMask.setFromPixels(_pixels, width, height);
}

int Tracker::numBlobs(){
	if(mouseBlob){
		return 1;
	}
	int r = 0;
	if(thread.lock()){
		r = thread.contourFinder.nBlobs;
		thread.unlock();
	}
	return r;
}

ofxCvBlob Tracker::getConvertedBlob(ofxCvBlob * blob, CameraCalibration * calibrator){
	ofxCvBlob deBarrelledBlob;
	
	int cameraCalibratorId = cameraId;
	if (bVideoPlayerWasActive && cameraId == 2) {
		cameraCalibratorId = 3;
	}
	
	//todo: correction of area and height ?
	deBarrelledBlob.area = blob->area;
	deBarrelledBlob.length = blob->length;
	
	
	//ofxVec2f dv = (getPlugin<Cameras*>(controller))->undistortPoint(cameraId, blob->centroid.x, blob->centroid.y);
	ofxVec2f dv = blob->centroid;
	
	deBarrelledBlob.centroid = ofPoint(dv.x,dv.y);
	
	deBarrelledBlob.boundingRect = blob->boundingRect;
	
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
	
	
	ofxVec2f boundingRectPoint = calibrator->convertCoordinate(cameraCalibratorId, deBarrelledBlob.boundingRect.x/getWidth(), deBarrelledBlob.boundingRect.y/getHeight());
	ofxVec2f boundingRectDimensions = calibrator->convertCoordinate(cameraCalibratorId, deBarrelledBlob.boundingRect.width/getWidth(), deBarrelledBlob.boundingRect.height/getHeight());
	
	b.boundingRect.x = boundingRectPoint.x;
	b.boundingRect.y = boundingRectPoint.y;
	b.boundingRect.width = boundingRectDimensions.x;
	b.boundingRect.height = boundingRectDimensions.y;
	
	ofxVec2f v = calibrator->convertCoordinate(cameraCalibratorId, deBarrelledBlob.centroid.x/getWidth(), deBarrelledBlob.centroid.y/getHeight());
	
	b.centroid = ofPoint(v.x, v.y);
	b.hole = blob->hole;
	for(int i=0;i<deBarrelledBlob.nPts;i++){
		ofxVec2f v = calibrator->convertCoordinate(cameraCalibratorId, deBarrelledBlob.pts[i].x/getWidth(), deBarrelledBlob.pts[i].y/getHeight());
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
	ofxCvBlob r;
	if(thread.lock()){
		r = getConvertedBlob(&thread.contourFinder.blobs[n], getPlugin<CameraCalibration*>(controller));
		thread.unlock();
	}
	
	return r;
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

vector<ofxCvBlob> Tracker::getBlobById(unsigned long int _id){
	for(int i=0;i<numPersistentBlobs();i++){
		if(persistentBlobs[i].id == _id){
			return persistentBlobs[i].blobs;
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
		mouseGeneratedBlob.boundingRect.x = x - 0.5;
		mouseGeneratedBlob.boundingRect.y = y - 0.5;
		mouseGeneratedBlob.boundingRect.width = 1.0;
		mouseGeneratedBlob.boundingRect.height = 1.0;
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
	for(int i=0;i<3;i++){
		bUseBgMask[i] = false;
	}	
}
void BlobTracking::setup(){
	for(int i=0;i<3;i++){
		trackers.push_back(new Tracker());
		trackers[i]->cameraId = i;
		trackers[i]->threshold = initThreshold[i];
		trackers[i]->blur = initBlur[i];
		trackers[i]->active = initActive[i];
		trackers[i]->controller = controller;
		trackers[i]->bLearnBakground != trackers[i]->loadBackground();
		trackers[i]->setup();
		trackers[i]->bUseBgMask = false;
	}
}


void BlobTracking::update(){
	//#pragma omp parallel for
	
	if (bUseBgMask[0]) {
		
		int nPoints = 4;
		
		ofxPoint2f wallMaskCorners[4];
		
		for(int i=0;i<4;i++){
			wallMaskCorners[i] = getPlugin<CameraCalibration*>(controller)->cameras[0]->coordWarp->inversetransform(projection()->getWall()->corners[i]->x, projection()->getWall()->corners[i]->y);
			wallMaskCorners[i].x *= trackers[0]->getWidth();
			wallMaskCorners[i].y *= trackers[0]->getHeight();
		}
		
		//padding
		
		int padding = 30;
		
		wallMaskCorners[0].x += padding;
		wallMaskCorners[0].y += padding;
		wallMaskCorners[1].x -= padding;
		wallMaskCorners[1].y += padding;
		wallMaskCorners[2].x -= padding;
		wallMaskCorners[2].y -= padding;
		wallMaskCorners[3].x += padding;
		wallMaskCorners[3].y -= padding;
		
		ofxCvGrayscaleImage theWallMask;
		
		theWallMask.allocate(trackers[0]->getWidth(),trackers[0]->getHeight());
		theWallMask.set(0);
		
		CvPoint _cp[4]= {{wallMaskCorners[0].x,wallMaskCorners[0].y}, {wallMaskCorners[1].x,wallMaskCorners[1].y},{wallMaskCorners[2].x,wallMaskCorners[2].y},{wallMaskCorners[3].x,wallMaskCorners[3].y}};			
		CvPoint* cp = _cp; cvFillPoly(theWallMask.getCvImage(), &cp, &nPoints, 1, cvScalar(255));
		
		trackers[0]->setBgMaskFromPixels(theWallMask.getPixels(), theWallMask.getWidth(), theWallMask.getHeight());
		
	}
	
	for(int i=0;i<trackers.size();i++){
		trackers[i]->bUseBgMask = bUseBgMask[i];
		trackers[i]->update();
	}	
	
	for(int i=0;i<trackers.size();i++){
		trackers[i]->findContours();	
	}
}

void BlobTracking::drawSettings(){
	
	for(int i=0;i<trackers.size();i++){
		ofSetColor(255, 255, 255);
		
		int w = 210;
		float a = 480.0/640.0;
		getPlugin<Cameras*>(controller)->draw(trackers[i]->cameraId,0,w*a*i,w,w*a);
		trackers[i]->grayImageBlured.draw(w,w*a*i,w,w*a);
		trackers[i]->grayBg.draw(w*2,w*a*i, w,w*a);
		//trackers[i]->grayBgMask.draw(w*2,w*a*i, w,w*a);
		trackers[i]->grayDiff.draw(w*3,w*a*i,w,w*a);
		if(trackers[i]->thread.lock()){
			trackers[i]->thread.contourFinder.draw(w*3,w*a*i,w,w*a);
			trackers[i]->thread.unlock();
		}
		
		// trackers[i]->opticalFlow.draw();
		// trackers[i]->simplifiedContourFinder.draw(w*3,w*a*i,w,w*a);
	}
}

void BlobTracking::draw(){
	if(drawDebug){
		ofSetColor(255, 255, 255);
		
		
		ofEnableAlphaBlending();
		glBlendFunc (GL_SRC_COLOR, GL_ONE);	
		
		for(int i=0;i<trackers.size();i++){
			ofDrawBitmapString(ofToString(trackers[i]->numBlobs(), 0), 10, 50*i+20);
			ofDrawBitmapString(ofToString(trackers[i]->numPersistentBlobs(), 0), 10, 50*i+30);
			
			ofSetColor(128, 128, 128);
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
				vector<ofxCvBlob> b = trackers[i]->persistentBlobs[u].blobs;
				ofSetColor(0, 255, 255);
				
				ofDrawBitmapString(ofToString(trackers[i]->persistentBlobs[u].id, 0), trackers[i]->persistentBlobs[u].centroid.x*ofGetWidth(), trackers[i]->persistentBlobs[u].centroid.y*ofGetHeight());
				
				for(int g=0;g<b.size();g++){
					ofSetColor(0, 0, 255);
					
					for(int x=0;x<b[g].nPts;x++){
						ofEllipse(b[g].pts[x].x*ofGetWidth(), b[g].pts[x].y*ofGetHeight(), 10, 10);
					}
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
	if(3 > n){
		trackers[n]->bLearnBakground = true;
	}
}
void BlobTracking::setActive(int n, bool b){
	if(trackers.size() < 1){
		initActive[n] = b;
	} else 
		trackers[n]->active = b;;
}


