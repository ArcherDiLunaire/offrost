#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "PluginIncludes.h"
#include "PluginController.h"
#include "ofxMidi.h"
//#include "ofxOsc.h"
//#include "guiController.h"


class CustomGLViewDelegate;


typedef struct
{
	float 	x;
	float 	y;
	bool 	bBeingDragged;
	bool 	bOver;
	float 	radius;
	
}	draggableVertex;




class testApp : public ofBaseApp
{
	
private:
	
	CustomGLViewDelegate* otherWindow;
	CustomGLViewDelegate* projectionSurfaceWindow;
	CustomGLViewDelegate* cameraCalibrationWindow;

	CustomGLViewDelegate* blobWindow;

	CustomGLViewDelegate* floorPreview;

	
public:
	

	testApp( ) ;	
	
	void setup();
	
	void update();
	void draw();
	
	void drawCameraView();
	void drawProjectionSurfaceView();
	void drawCameraCalibrationView();
	void drawBlobWindow();
	void drawFloorPreview();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	PluginController * pluginController;

	void setReferenceToOtherWindow( CustomGLViewDelegate* delegate, int i );
	
	ofTrueTypeFont	lucidaGrande;
	
	bool setupCalled;
	int fps;
	
	int cameraThreadTimer;
	int cameraTimer;
	int numCameras;
	unsigned long int cameraLastBlinkCount[3];
	
	uint64_t cameraGUIDs[3];
	
	enum camera_states {
		camera_state_running,
		camera_state_closing,
		camera_state_starting
	};
	
	int camera_state;
	
	float cameraBrightness[3];
	float cameraExposure[3];
	float cameraShutter[3];
	float cameraGamma[3];
	float cameraGain[3];
	
	Cameras * c;
	
};

#endif

