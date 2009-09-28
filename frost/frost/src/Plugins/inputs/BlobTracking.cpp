#include "PluginController.h"
#include "ProjectionSurfaces.h"
#include "BlobTracking.h"
#include "Cameras.h"
#include "CameraCalibration.h"

//--------------------------------------------------------------------------------
bool sort_carea_compare2( const CvSeq* a, const CvSeq* b) {
	// use opencv to calc size, then sort based on size
	float areaa = fabs(cvContourArea(a, CV_WHOLE_SEQ));
	float areab = fabs(cvContourArea(b, CV_WHOLE_SEQ));
	
    //return 0;
	return (areaa > areab);
}

int ofxExtendedBlobTracking::findSimplifiedContours( ofxCvGrayscaleImage&  input,
						   int minArea,
						   int maxArea,
						   int nConsidered,
						   bool bFindHoles,
						   bool bUseApproximation) {
	
	// get width/height disregarding ROI
	IplImage* ipltemp = input.getCvImage();
	_width = ipltemp->width;
	_height = ipltemp->height;
	
	reset();
	
	
	if( inputCopy.width == 0 ) {
		inputCopy.allocate( _width, _height );
	} else if( inputCopy.width != _width || inputCopy.height != _height ) {
		// reallocate to new size
		inputCopy.clear();
		inputCopy.allocate( _width, _height );
	}
	
	inputCopy = input;
	inputCopy.setROI( input.getROI() );
	
	CvSeq* contour_list = NULL;
	contour_storage = cvCreateMemStorage( 1000 );
	storage	= cvCreateMemStorage( 1000 );
	
	CvContourRetrievalMode  retrieve_mode
	= (bFindHoles) ? CV_RETR_LIST : CV_RETR_EXTERNAL;
	cvFindContours( inputCopy.getCvImage(), contour_storage, &contour_list,
				   sizeof(CvContour), retrieve_mode, bUseApproximation ? CV_CHAIN_APPROX_SIMPLE : CV_CHAIN_APPROX_NONE );
	CvSeq* contour_ptr = contour_list;
	
	CvSeq* c_new;
//	contour_storage = cvCreateMemStorage( 1000 );

	//c_new = cvApproxPoly(contour_ptr, sizeof(CvContour), contour_storage, CV_POLY_APPROX_DP, 2);
	// put the contours from the linked list, into an array for sorting
	while( (contour_ptr != NULL) ) {
		float area = fabs( cvContourArea(contour_ptr, CV_WHOLE_SEQ) );
		if( (area > minArea) && (area < maxArea) ) {
			cvSeqBlobs.push_back(contour_ptr);
		}
		contour_ptr = contour_ptr->h_next;
	}
	
	
	// sort the pointers based on size
	if( cvSeqBlobs.size() > 1 ) {
		sort( cvSeqBlobs.begin(), cvSeqBlobs.end(), sort_carea_compare2 );
	}
	
	
	// now, we have cvSeqBlobs.size() contours, sorted by size in the array
	// cvSeqBlobs let's get the data out and into our structures that we like
	for( int i = 0; i < MIN(nConsidered, (int)cvSeqBlobs.size()); i++ ) {
		blobs.push_back( ofxCvBlob() );
		float area = cvContourArea( cvSeqBlobs[i], CV_WHOLE_SEQ );
		CvRect rect	= cvBoundingRect( cvSeqBlobs[i], 0 );
		cvMoments( cvSeqBlobs[i], myMoments );
		
		blobs[i].area                     = fabs(area);
		blobs[i].hole                     = area < 0 ? true : false;
		blobs[i].length 			      = cvArcLength(cvSeqBlobs[i]);
		blobs[i].boundingRect.x           = rect.x;
		blobs[i].boundingRect.y           = rect.y;
		blobs[i].boundingRect.width       = rect.width;
		blobs[i].boundingRect.height      = rect.height;
		blobs[i].centroid.x 			  = (int) (myMoments->m10 / myMoments->m00);
		blobs[i].centroid.y 			  = (int) (myMoments->m01 / myMoments->m00);
		
		// get the points for the blob:
		CvPoint           pt;
		CvSeqReader       reader;
		cvStartReadSeq( cvSeqBlobs[i], &reader, 0 );
		
		for( int j=0; j < cvSeqBlobs[i]->total; j++ ) {
			CV_READ_SEQ_ELEM( pt, reader );
			blobs[i].pts.push_back( ofPoint((float)pt.x, (float)pt.y) );
		}
		blobs[i].nPts = blobs[i].pts.size();
		
	}
	
	nBlobs = blobs.size();
	
	// Free the storage memory.
	// Warning: do this inside this function otherwise a strange memory leak
	if( contour_storage != NULL ) { cvReleaseMemStorage(&contour_storage); }
	if( storage != NULL ) { cvReleaseMemStorage(&storage); }
	
	return nBlobs;
	
}



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
	ofxCvBlob b;
	float m = cw*ch;
	b.area = blob->area/m;
	b.length = blob->length/m;
	//bounding rect not defined
	
	ofxVec2f v = calibrator->convertCoordinate(cameraId, blob->centroid.x/getWidth(), blob->centroid.y/getHeight());
	
	b.centroid = ofPoint(v.x, v.y);
	b.hole = blob->hole;
	for(int i=0;i<blob->nPts;i++){
		ofxVec2f v = calibrator->convertCoordinate(cameraId, blob->pts[i].x/getWidth(), blob->pts[i].y/getHeight());
		b.pts.push_back(ofPoint(v.x, v.y));
	}
	b.nPts = blob->nPts;
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

void Tracker::smoothBlob(ofxCvBlob * blob, float smooth){
	contourSimp.smooth(blob->pts, blob->pts, smooth);

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


