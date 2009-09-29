#pragma once
#include "ofMain.h"
#include "Input.h"
#include "ofxVectorMath.h"
#include "Warp.h"

#include "ofxVideoGrabber.h"
#include "ofxVideoGrabberFeature.h"
#include "ofxVideoGrabberSettings.h"
#include "ofxVideoGrabberSDK.h"
#include "ofCvCameraCalibration.h"

/* SDKs */
#include "Libdc1394Grabber.h"


class Cameras : public Input{
public:
	Cameras();
	
	void setup();
	void update();

	void draw(int _grabberIndex, float _x, float _y, float _w, float _h);
	void draw(int _grabberIndex, float _x, float _y);
	void draw();
	
	void videoPlayerActivate(int _grabberIndex);
	void videoPlayerDeactivate(int _grabberIndex);
	
	bool videoPlayerActive(int _grabberIndex);
	void videoPlayerPlay(int _grabberIndex);
	void videoPlayerStop(int _grabberIndex);
	void videoPlayerSetLoopState(int _grabberIndex, int _state);
	bool videoPlayerLoadUrl(int _grabberIndex, string url);
		
	virtual unsigned char* getPixels(int _grabberIndex);

	bool calibAddSnapshot(uint64_t _cameraGUID);
	bool calibrate(uint64_t _cameraGUID);

	ofPoint undistortPoint(int _grabberIndex, float _PixelX, float _PixelY);
	ofPoint distortPoint(int _grabberIndex, float _PixelX, float _PixelY);
	
	void initGrabber(int _grabber, uint64_t _cameraGUID = 0x0ll);
	void initCameraCalibration(uint64_t _cameraGUID);

	void setGUIDs(uint64_t _cameraGUID1, uint64_t _cameraGUID2, uint64_t _cameraGUID3);
	bool cameraGUIDexists(uint64_t _cameraGUID);

	ofxVideoGrabber * getVidGrabber(int _cameraIndex);
	
	ofCvCameraCalibration calib[3];
	ofxCvGrayscaleImage calibImage[3];
	CvSize csize;
	
	void setCameraCalibration(uint64_t _cameraGUID, float _k1, float _k2, float _c1, float _c2, double fx, double cx, double fy, double cy);

	bool isReady(int _cameraIndex);
	
	bool setGUID(int _grabber, uint64_t _cameraGUID);
	uint64_t getGUID(int _grabber);
	int getGrabberIndexFromGUID(uint64_t _cameraGUID);
	
	u_int64_t cameraGUIDs[3];
	
	int camWidth;
	int camHeight;
	
	bool cameraInited[3];

private:

	ofxVideoGrabber * vidGrabber[3];
	
	ofVideoPlayer videoPlayer[3];
	bool videoPlayerActivated[3];

};