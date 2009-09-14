#define OF_VIDEO_CAPTURE_LIBDC1394

#include "testApp.h"




//--------------------------------------------------------------
void testApp::setup()
{
	camWidth 		= 1280;	// try to grab at this size.
	camHeight 		= 960;

	vidGrabber.listDevices();
	vidGrabber.setVerbose(true);
	vidGrabber.setValue(VGFN_Width, (float)camWidth);
	vidGrabber.setValue(VGFN_Height, (float)camHeight);
	vidGrabber.setValue(VGFN_ColorMode, VGFCM_FORMAT7_0);
	vidGrabber.setUseTexture(true);
	
	vidGrabber.initGrabber(camWidth,camHeight);

	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	
}


//--------------------------------------------------------------
void testApp::update(){

	ofBackground(100,100,100);

	vidGrabber.grabFrame();
	vidGrabber.

	if (vidGrabber.isFrameNew()){
		int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = vidGrabber.getPixels();
		for (int i = 0; i < totalPixels; i++){
			videoInverted[i] = 255 - pixels[i];
		}
		//videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	vidGrabber.draw(0,0);
	//videoTexture.draw(20+camWidth,20,camWidth,camHeight);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	// in fullscreen mode, on a pc at least, the
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...

	switch (key)
	{
	/*
		case 's':
		case 'S':
			vidGrabber.videoSettings();
			break;
	*/		
		//gain tweakage
		case 'q':
		case 'Q':
			printf("Gain up: ");
			featureAdjust(VGFN_Gain, 0.1f);
			break;
		case 'a':
		case 'A':
			printf("Gain down: ");
			featureAdjust(VGFN_Gain, -0.1f);
			break;
		case 'z':
		case 'Z':
			printf("Gain auto: ");
			featureAuto(VGFN_Gain);
			break;

		//exposure tweakage
		case 'w':
		case 'W':
			printf("Exposure up: ");
			featureAdjust(VGFN_Exposure, 0.1f);
			break;
		case 's':
		case 'S':
			printf("Exposure down: ");
			featureAdjust(VGFN_Exposure, -0.1f);
			break;
		case 'x':
		case 'X':
			printf("Exposure auto: ");
			featureAuto(VGFN_Exposure);
			break;

		//shutter
		case 'e':
		case 'E':
			printf("Shutter up: ");
			featureAdjust(VGFN_Shutter, 0.1f);
			break;
		case 'd':
		case 'D':
			printf("Shutter down: ");
			featureAdjust(VGFN_Shutter, -0.1f);
			break;
		case 'c':
		case 'C':
			printf("Shutter auto: ");
			featureAuto(VGFN_Shutter);
			break;
			
		//Brightness
		case 'r':
		case 'R':
			printf("Brightness up: ");
			featureAdjust(VGFN_Brightness, 0.1f);
			break;
		case 'f':
		case 'F':
			printf("Brightness down: ");
			featureAdjust(VGFN_Brightness, -0.1f);
			break;
		case 'v':
		case 'V':
			printf("Brightness auto: ");
			featureAuto(VGFN_Brightness);
			break;
			
		//video mode
		case '2':
			printf("ColorMode up: ");
			featureAdjust(VGFN_ColorMode, 1.0f);
			break;
		case '1':
			printf("ColorMode down: ");
			featureAdjust(VGFN_ColorMode, -1.0f);
			break;

		//color coding
		case 'o':
		case 'O':
			printf("Gamma up: ");
			featureAdjust(VGFN_Gamma, 0.1f);
			break;
		case 'l':
		case 'L':
			printf("Gamma down: ");
			featureAdjust(VGFN_Gamma, -0.1f);
			break;
		case '.':
		case '>':
			printf("Gamma auto: ");
			featureAuto(VGFN_Gamma);
			break;

	}


}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}

//Adjust gain by range * factor
void testApp::featureAdjust(t_cameraFeature feature, float factor)
{
	float upper, lower, currentValue, newValue;
	bool currentAuto;
	
	if (vidGrabber.getAuto(feature, &currentAuto))
	{
		if (currentAuto)
		{
			if (vidGrabber.setAuto(feature, false))
			{
				printf("...auto off...");
			}
		}
	}
	if (vidGrabber.getValueRange(feature, &lower, &upper))
	{
		if (upper > lower)
		{
			if (vidGrabber.getValue(feature, &currentValue))
			{
				float targetValue;
				if (factor == 1.0f || factor == -1.0f)
				{					//increment
					targetValue = currentValue + factor;
				}
				else
				{
					targetValue = currentValue + factor * (upper - lower);
				}
				if (vidGrabber.setValue(feature, targetValue))
				{
					if (vidGrabber.getValue(feature, &newValue))
					{
						float factorPercent = factor * 100.0f;
						printf("%.2f + %.2f%% = %.2f\n", currentValue, factor, newValue);
						return;
					}
				}
			}
		}
	}
	printf("failed\n");
}

void testApp::featureAuto(t_cameraFeature feature)
{
	bool currentAuto;
	
	if (vidGrabber.getAuto(feature, &currentAuto))
	{
		if (!currentAuto)
		{
			if (vidGrabber.setAuto(feature, true))
			{
				printf("on.\n");
				return;
			}
		}
		else
		{
			printf("already on.\n");
			return;
		}
	}
	printf("failed\n");
}
