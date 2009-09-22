#include "PluginController.h"
#include "ProjectionSurfaces.h"
#include "BlobTracking.h"
#include "Cameras.h"


Tracker::Tracker(ofxVideoGrabber * _grabber){
	grabber = _grabber;		
	int w = 640;//grabber->width; 
	int h = 480;//grabber->height;
	colorImg.allocate(w,h);
	grayImage.allocate(w,h);
	grayBg.allocate(w,h);
	grayDiff.allocate(w,h);
	bLearnBakground = true;
}

void Tracker::update(){ 
	/*bool bNewFrame = false;
	bNewFrame = grabber->isFrameNew();
	if (bNewFrame){
		
		colorImg.setFromPixels(grabber->getPixels(), 640,480);

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (640*480)/3, 10, true);	// find holes
	}*/
}


BlobTracking::BlobTracking(){
	type = INPUT;
}
void BlobTracking::setup(){
	trackers.push_back(new Tracker(getPlugin<Cameras*>(controller)->vidGrabber[0]));
}
void BlobTracking::update(){
	for(int i=0;i<trackers.size();i++){
		trackers[i]->update();
		trackers[i]->threshold = threshold;
	}	
}
void BlobTracking::draw(){
	/*for(int i=0;i<trackers.size();i++){
		trackers[i]->grabber->draw(0,0,100,100);
		trackers[i]->grayBg.draw(100,0,100,100);
		trackers[i]->grayDiff.draw(200,0,100,100);
		
	}	*/
}

