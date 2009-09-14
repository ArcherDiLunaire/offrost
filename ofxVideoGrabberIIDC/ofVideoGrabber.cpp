#include "ofVideoGrabber.h"

#ifdef OF_VIDEO_CAPTURE_V4L
	#include "ofV4LUtils.h"
#endif


//--------------------------------------------------------------------
ofVideoGrabber::ofVideoGrabber(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		initializeQuicktime();
		bSgInited		= false;

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		bVerbose 			= false;
		bDoWeNeedToResize 	= false;

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------


		auxPixels			= NULL;

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L				// kept around if people have unicap issues...
	//--------------------------------

		bV4LGrabberInited 	= false;

	//---------------------------------
	#endif
	//---------------------------------

	// common
	bIsFrameNew				= false;
	bVerbose 				= false;
	bGrabberInited 			= false;
	bUseTexture				= true;
	bChooseDevice			= false;
	deviceID				= 0;
	width 					= 320;	// default setting
	height 					= 240;	// default setting
	pixels					= NULL;

	//by default, no features are available
	for (int index = 0; index < VGFN_NumFeatures; index++)
	{
		m_cameraFeatures[index] = NULL;
	}
	
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------
		//Enumerate cameras.  Assume user wants first camera, but another may be chosen before iniGrabber.  
		//This allows different grabbers to be used for different cameras.  
		bDoWeNeedToResize 	= false;
		m_driver = dc1394_new ();
		dc1394error_t err = dc1394_camera_enumerate (m_driver, &m_deviceList);
		DC1394_ERR(err,"Failed to enumerate cameras");

		if (m_deviceList->num == 0) {
			dc1394_log_error("No cameras found");
			return;
		}
		m_camera = NULL;

		//some features are created here as they affect camera creation.  Other ones created in initGrabber
		//!!!though maybe they should all be created here...
		m_cameraFeatures[VGFN_ColorMode]	= new CameraFeatureColorMode(VGFN_ColorMode, &m_camera, &width, &height, bVerbose);
		
	//---------------------------------
	#endif //OF_VIDEO_CAPTURE_LIBDC1394
	//---------------------------------

}


//--------------------------------------------------------------------
ofVideoGrabber::~ofVideoGrabber(){

	close();

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------
		if (offscreenGWorldPixels != NULL){
			delete offscreenGWorldPixels;
		}
	//---------------------------------
	#endif
	//---------------------------------
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------
		if(bGrabberInited)
			ucGrabber.close_unicap();

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------
		
		//free device enumeration
		dc1394_camera_free_list(m_deviceList);
		
		//free context
		dc1394_free(m_driver);
		
	//---------------------------------
	#endif //OF_VIDEO_CAPTURE_LIBDC1394
	//---------------------------------

	//delete all feature objects
	for (int index = 0; index < VGFN_NumFeatures; index++)
	{
		if (m_cameraFeatures[index] != NULL)
		{
			delete m_cameraFeatures[index];
			m_cameraFeatures[index] = NULL;
		}
	}

}

//--------------------------------------------------------------------
void ofVideoGrabber::listDevices(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		//initGrabbingComponent(vgd, true);		// if necessary, start the sg object

		if (!bSgInited){
			printf("error in list devices, couldn't allocate grabbing component\n");
			return;
		}
		printf("-------------------------------------\n");
		SGDeviceList deviceList;
		SGGetChannelDeviceList (gVideoChannel, sgDeviceListIncludeInputs, &deviceList);
		unsigned char pascalName[64];
		if(true)
		{
			printf("listing available capture devices\n");
			for(int i = 0 ; i < (*deviceList)->count ; ++i)
			{
				SGDeviceName nameRec;
				nameRec = (*deviceList)->entry[i];
				memcpy(pascalName, (*deviceList)->entry[i].name, sizeof(char) * 64);
				if(nameRec.flags != sgDeviceNameFlagDeviceUnavailable) printf("device[%i] %s \n",  i, p2cstr(pascalName) );
				else printf("(unavailable) device[%i] %s \n",  i, p2cstr(pascalName) );

			}
		}
		printf("-------------------------------------\n");

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------
		printf("---\n");
		VI.listDevices();
		printf("---\n");

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------


		ucGrabber.listUCDevices();

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------

	struct stat st;

	printf("listing available capture devices\n");
	printf("---\n");
	for (int i = 0; i < 8; i++)
	{
		sprintf(dev_name, "/dev/video%i", i);
		if (stat (dev_name, &st) == 0) {
			printf("Video device %i = /dev/video%i\n",i,i);
		} else {

		}
	}
	printf("---\n");

	//---------------------------------
	#endif
	//---------------------------------
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------
	printf("Listing available IIDC capture devices.\nUse unit number as unique ID for camera...\n");
	for (uint32_t index = 0; index < m_deviceList->num; index++)
	{
		//printf("Using camera with GUID %"PRIx64"\n", camera->guid);
		printf("Device[%d].unit = %x, GUID = %x\n", index, m_deviceList->ids[index].unit, m_deviceList->ids[index].guid);
	}
	//---------------------------------
	#endif //OF_VIDEO_CAPTURE_LIBDC1394
	//---------------------------------


}

//--------------------------------------------------------------------
void ofVideoGrabber::setVerbose(bool bTalkToMe){
	bVerbose = bTalkToMe;

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		ucGrabber.verbose=bVerbose;

	//---------------------------------
	#endif
	//---------------------------------
}


//--------------------------------------------------------------------
void ofVideoGrabber::setDeviceID(int _deviceID)
{
	deviceID		= _deviceID;
	bChooseDevice	= true;
}

//---------------------------------------------------------------------------
unsigned char * ofVideoGrabber::getPixels()
{
	return pixels;
}

//---------------------------------------------------------------------------
bool  ofVideoGrabber::isFrameNew()
{
	return bIsFrameNew;
}

//--------------------------------------------------------------------
void ofVideoGrabber::grabFrame(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		if (bGrabberInited == true){
			SGIdle(gSeqGrabber);
			// set the top pixel alpha = 0, so we can know if it
			// was a new frame or not..
			// or else we will process way more than necessary
			// (ie opengl is running at 60fps +, capture at 30fps)
			if (offscreenGWorldPixels[0] != 0x00){
				offscreenGWorldPixels[0] = 0x00;
				bHavePixelsChanged = true;
				convertPixels(offscreenGWorldPixels, pixels, width, height);
				if (bUseTexture){
					tex.loadData(pixels, width, height, GL_RGB);
				}
			}
		}

		// newness test for quicktime:
		if (bGrabberInited == true){
			bIsFrameNew = false;
			if (bHavePixelsChanged == true){
				bIsFrameNew = true;
				bHavePixelsChanged = false;
			}
		}


	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true){
			if (VI.isFrameNew(device)){

				bIsFrameNew = true;


				/*
				 	rescale --
				 	currently this is nearest neighbor scaling
				 	not the greatest, but fast
				 	this can be optimized too
				 	with pointers, etc

				 	better --
				 	make sure that you ask for a "good" size....

				*/

				unsigned char * viPixels = VI.getPixels(device, true, true);


				if (bDoWeNeedToResize == true){

					int inputW = VI.getWidth(device);
					int inputH = VI.getHeight(device);

					float scaleW =	(float)inputW / (float)width;
					float scaleH =	(float)inputH / (float)height;

					for(int i=0;i<width;i++){
						for(int j=0;j<height;j++){

							float posx = i * scaleW;
							float posy = j * scaleH;

							/*

							// start of calculating
							// for linear interpolation

							int xbase = (int)floor(posx);
							int xhigh = (int)ceil(posx);
							float pctx = (posx - xbase);

							int ybase = (int)floor(posy);
							int yhigh = (int)ceil(posy);
							float pcty = (posy - ybase);
							*/

							int posPix = (((int)posy * inputW * 3) + ((int)posx * 3));

							pixels[(j*width*3) + i*3    ] = viPixels[posPix  ];
							pixels[(j*width*3) + i*3 + 1] = viPixels[posPix+1];
							pixels[(j*width*3) + i*3 + 2] = viPixels[posPix+2];

						}
					}

				} else {

					q(pixels, viPixels, width*height*3);

				}

				if (bUseTexture){
					tex.loadData(pixels, width, height, GL_RGB);
				}
			}
		}

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		if (bGrabberInited){


			if (bDoWeNeedToResize == true){
				bIsFrameNew = ucGrabber.getFrameUC(&auxPixels);
				int inputW = ucGrabber.getUC_Width();
				int inputH = ucGrabber.getUC_Height();

				float scaleW =	(float)inputW / (float)width;
				float scaleH =	(float)inputH / (float)height;

				for(int i=0;i<width;i++){
					for(int j=0;j<height;j++){

						float posx = i * scaleW;
						float posy = j * scaleH;

						/*

						// start of calculating
						// for linear interpolation

						int xbase = (int)floor(posx);
						int xhigh = (int)ceil(posx);
						float pctx = (posx - xbase);

						int ybase = (int)floor(posy);
						int yhigh = (int)ceil(posy);
						float pcty = (posy - ybase);
						*/

						int posPix = (((int)posy * inputW * 3) + ((int)posx * 3));

						pixels[(j*width*3) + i*3    ] = auxPixels[posPix  ];
						pixels[(j*width*3) + i*3 + 1] = auxPixels[posPix+1];
						pixels[(j*width*3) + i*3 + 2] = auxPixels[posPix+2];

					}
				}

			} else {

				bIsFrameNew = ucGrabber.getFrameUC(&pixels);

			}
			if(bIsFrameNew) {
				if (bUseTexture){
					tex.loadData(pixels, width, height, GL_RGB);
				}
			}
		}

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------

		if (bV4LGrabberInited == true){
			bIsFrameNew = getFrameV4L(pixels);
			if(bIsFrameNew) {
				if (bUseTexture){
					tex.loadData(pixels, width, height, GL_RGB);
				}
			}
		}
		
	//---------------------------------
	#endif
	//---------------------------------
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------
		/*-----------------------------------------------------------------------
		 *  capture one frame (if ready)
		 *-----------------------------------------------------------------------*/
		if (m_camera != NULL)
		{
			dc1394video_frame_t *frame;
			
			//try to get a frame.  It may not be available if this routine called more frequently than frame rate
			dc1394error_t err = dc1394_capture_dequeue(m_camera, DC1394_CAPTURE_POLICY_POLL, &frame);
			if (frame == NULL)
			{															//no frame?
				bIsFrameNew = false;
				return;
			}
			DC1394_ERR_CLN(err ,cleanup_and_exit(m_camera),"Could not capture a frame");
			
			if (bDoWeNeedToResize)
			{															//do we need to resize image?
				DC1394_ERR_CLN(err ,cleanup_and_exit(m_camera),"Implement scaling please");
			}
			else
			{															//no resize needed
				unsigned char* source = frame->image;
				unsigned char* dest = pixels;
				size_t bytesToCopy = frame->stride < width * 3 ? frame->stride : width * 3;
				
				//covert (or copy) a line at a time (as stride may not equal width * depth)
				for (uint32_t line = 0; line < height; line++)
				{														//for each line
					//perform colour space conversions as may be needed
					unsigned char* sourcePixel;
					unsigned char* destPixel;
					
					//perform color space conversions as needed (or just copy)
					switch (frame->color_coding)
					{
						//Mono 8: set R,G,B to Y
						case DC1394_COLOR_CODING_MONO8:
							sourcePixel = source;
							destPixel = dest;
							
							for (uint32_t column = 0; column < width; column++)
							{
								*destPixel = *sourcePixel;
								destPixel++;
								*destPixel = *sourcePixel;
								destPixel++;
								*destPixel = *sourcePixel;
								destPixel++;
								sourcePixel++;
							}
							break;
						//MONO 16: set R,G,B to MSB(Y)
						case DC1394_COLOR_CODING_MONO16:
							sourcePixel = source;
							destPixel = dest;
							
							for (uint32_t column = 0; column < width; column++)
							{
								*destPixel = *sourcePixel;
								destPixel++;
								*destPixel = *sourcePixel;
								destPixel++;
								*destPixel = *sourcePixel;
								destPixel++;
								sourcePixel += 2;
							}
							break;
							
						//all other modes: straight copy, which may indeed mess up the signal
						//unimplemented: YUV modes, RGB16, Bayer etc.
						default:
							memcpy(dest, source, bytesToCopy);
					}
					source += frame->stride;
					dest += width * 3;
				}
			}
			
			//free frame so it can be used again
			err = dc1394_capture_enqueue(m_camera, frame);
			DC1394_ERR_CLN(err ,cleanup_and_exit(m_camera),"Frame could not be returned to the queue");
			
			bIsFrameNew = true;
			if (bUseTexture)
			{
				tex.loadData(pixels, width, height, GL_RGB);
			}
		}
		
	//---------------------------------
	#endif //OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------
	
}

//--------------------------------------------------------------------
void ofVideoGrabber::close(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		//thanks eloi!
		if (gSeqGrabber != 0L){
			CloseComponent (gSeqGrabber);
			gSeqGrabber = 0L;
		}

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------

		closeV4L();

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true){
			VI.stopDevice(device);
			bGrabberInited = false;
		}

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		if(bGrabberInited){
			ucGrabber.close_unicap();
			bGrabberInited 		= false;
			deviceID 			= 0;
			bIsFrameNew 		= false;
			bChooseDevice 		= false;
			if(auxPixels != NULL){
				delete auxPixels;
			}
		}

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------
    /*-----------------------------------------------------------------------
     *  close camera
     *-----------------------------------------------------------------------*/
		//stop an delete the camera.
		if (m_camera != NULL)
		{
			dc1394_video_set_transmission(m_camera, DC1394_OFF);
			dc1394_capture_stop(m_camera);
			dc1394_camera_free(m_camera);
		}
	//---------------------------------
	#endif //OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------

	if (pixels != NULL){
		delete pixels;
	}

	tex.clear();

}

//--------------------------------------------------------------------
void ofVideoGrabber::videoSettings(void)
{

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------

		Rect	curBounds, curVideoRect;
		ComponentResult	err;

		// Get our current state
		err = SGGetChannelBounds (gVideoChannel, &curBounds);
		if (err != noErr){
			printf("Error in SGGetChannelBounds \n");
			return;
		}
		err = SGGetVideoRect (gVideoChannel, &curVideoRect);
		if (err != noErr){
			printf("Error in SGGetVideoRect \n");
			return;
		}

		// Pause
		err = SGPause (gSeqGrabber, true);
		if (err != noErr){
			printf("Error in SGPause\n");
			return;
		}

		#ifdef TARGET_OSX
			//load any saved camera settings from file
			loadSettings();

			static SGModalFilterUPP gSeqGrabberModalFilterUPP = NewSGModalFilterUPP(SeqGrabberModalFilterUPP);
			ComponentResult result = SGSettingsDialog(gSeqGrabber,  gVideoChannel, 0, nil, seqGrabSettingsPreviewOnly, gSeqGrabberModalFilterUPP, nil);
			if (result != noErr){
				printf("error in  dialogue\n");
				return;
			}

			//save any changed settings to file
			saveSettings();
		#else
			SGSettingsDialog(gSeqGrabber, gVideoChannel, 0, nil, seqGrabSettingsPreviewOnly, NULL, 0);
		#endif

		SGSetChannelBounds(gVideoChannel, &videoRect);
		SGPause (gSeqGrabber, false);

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bGrabberInited == true) VI.showSettingsWindow(device);

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------

		ucGrabber.queryUC_imageProperties();

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------

		queryV4L_imageProperties();

	//---------------------------------
	#endif
	//---------------------------------

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------
	//To be implemented.  
	//---------------------------------
	#endif //OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------

}


//--------------------------------------------------------------------
#ifdef OF_VIDEO_CAPTURE_QUICKTIME
#ifdef TARGET_OSX 
//--------------------------------------------------------------------

//---------------------------------------------------------------------
bool ofVideoGrabber::saveSettings(){

		if (bGrabberInited != true) return false;
		ComponentResult	err;

		UserData mySGVideoSettings = NULL;
		// get the SGChannel settings cofigured by the user
		err = SGGetChannelSettings(gSeqGrabber, gVideoChannel, &mySGVideoSettings, 0);
		if ( err != noErr ){
			printf("Error getting camera settings %i\n",err);
			return false;
		}
		string pref = "ofVideoSettings-"+deviceName;
		CFStringRef cameraString = CFStringCreateWithCString(kCFAllocatorDefault,pref.c_str(),kCFStringEncodingMacRoman);

		//get the settings using the key "ofVideoSettings-the name of the device"
		SaveSettingsPreference( cameraString, mySGVideoSettings);
		DisposeUserData(mySGVideoSettings);
		return true;

}

//---------------------------------------------------------------------
bool ofVideoGrabber::loadSettings(){

   if (bGrabberInited != true) return false;

   ComponentResult   err;
   UserData mySGVideoSettings = NULL;

   // get the settings using the key "ofVideoSettings-the name of the device"
   string pref = "ofVideoSettings-"+deviceName;
   CFStringRef cameraString = CFStringCreateWithCString(kCFAllocatorDefault,pref.c_str(),kCFStringEncodingMacRoman);

   GetSettingsPreference(cameraString, &mySGVideoSettings);
   if (mySGVideoSettings) {

      Rect   curBounds, curVideoRect;
      ComponentResult   err;

      //we need to make sure the dimensions don't get effected
      //by our preferences

      // Get our current state
      err = SGGetChannelBounds (gVideoChannel, &curBounds);
      if (err != noErr){
         printf("Error in SGGetChannelBounds \n");
      }
      err = SGGetVideoRect (gVideoChannel, &curVideoRect);
      if (err != noErr){
         printf("Error in SGGetVideoRect \n");
      }

      // use the saved settings preference to configure the SGChannel
      err = SGSetChannelSettings(gSeqGrabber, gVideoChannel, mySGVideoSettings, 0);
      if ( err != noErr ) {
         printf("Error applying stored settings %i \n", err);
         return false;
      }
      DisposeUserData(mySGVideoSettings);

      // Pause
      err = SGPause (gSeqGrabber, true);
      if (err != noErr){
         printf("Error in SGPause\n");
      }
      SGSetChannelBounds(gVideoChannel, &videoRect);
      SGPause (gSeqGrabber, false);

   }else{
      printf("No camera settings to load \n");
      return false;
   }
   return true;
}


//---------------------------------
#endif //TARGET_OSX  
#endif //OF_VIDEO_CAPTURE_QUICKTIME
//---------------------------------


//--------------------------------------------------------------------
void ofVideoGrabber::initGrabber(int w, int h){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	//---------------------------------


		OSStatus err;

		///initGrabbingComponent(vgd, true);

		if (bSgInited != true){

			OSErr err = noErr;

			gSeqGrabber = OpenDefaultComponent(SeqGrabComponentType, 0);

			err = GetMoviesError ();
			if (gSeqGrabber == NULL || err) {
				printf("error: can't get default sequence grabber component");
				return;
			}

			err = SGInitialize(gSeqGrabber);
			if (err != noErr) {
				printf("error: can't initialize sequence grabber component");
				return;
			}

			err = SGSetDataRef(gSeqGrabber, 0, 0, seqGrabDontMakeMovie);
			if (err != noErr) {
				printf("error: can't set the destination data reference");
				return;
			}

			// windows crashes w/ out gworld, make a dummy for now...
			// this took a long time to figure out.
			err = SGSetGWorld(gSeqGrabber, 0, 0);
			if (err != noErr) {
				printf("error: setting up the gworld");
				return;
			}

			err = SGNewChannel(gSeqGrabber, VideoMediaType, &(gVideoChannel));
			if (err != noErr) {
				printf("error: creating a channel.  Check if you have any qt capable cameras attached\n");
				return;
			}

			bSgInited = true;

		}

		width 		= w;
		height 		= h;

		MacSetRect(&(videoRect),0, 0, width, height);

		//---------------------------------- buffer allocation
		// Create a buffer big enough to hold the video data,
		// make sure the pointer is 32-byte aligned.
		// also the rgb image that people will grab

		offscreenGWorldPixels 	= (unsigned char*)malloc(4 * width * height + 32);
		pixels					= new unsigned char[width*height*3];
		QTNewGWorldFromPtr (&videogworld, k32ARGBPixelFormat, &videoRect, NULL, NULL, 0, offscreenGWorldPixels, 4 * width);
		LockPixels(GetGWorldPixMap(videogworld));
		SetGWorld (videogworld, NULL);
		SGSetGWorld(gSeqGrabber, videogworld, nil);


		//---------------------------------- choose a specific device to setup
		bool didWeChooseADevice = false;

		if(!bChooseDevice) setDeviceID(0);
		else didWeChooseADevice = true;

		if(bChooseDevice)
		{

			//Generate a device list and enumerate
			//all devices availble to the channel
			SGDeviceList deviceList;
			SGGetChannelDeviceList (gVideoChannel, sgDeviceListIncludeInputs, &deviceList);

			int deviceCount = (*deviceList)->count;
			if(deviceCount == 0){
				printf("No catpure devices found\n");
				goto bail;
			}

			unsigned char pascalName[128];
			//List all available devices
			if(bVerbose && didWeChooseADevice)
			{
				printf("listing available capture devices\n");
				for(int i = 0 ; i < deviceCount; ++i){
					memcpy(pascalName, (*deviceList)->entry[i].name, sizeof(char) * 128);
					printf("device[%i] %s \n",  i, p2cstr(pascalName) );
				}
			}

			for(int i = 0 ; i < deviceCount; i++){

					if(deviceID >= deviceCount) deviceID -= deviceCount;

					SGDeviceName nameRec;
					nameRec = (*deviceList)->entry[deviceID];
					if(nameRec.flags != sgDeviceNameFlagDeviceUnavailable){

						memcpy(pascalName, (*deviceList)->entry[deviceID].name, sizeof(char) * 128);
						int len = strlen( (char *)p2cstr(pascalName) );

						if(len > 0){
							string str;
							str = (char *)p2cstr(pascalName);

							if(bVerbose)printf("attempting to setup device[%i] - %s \n",  deviceID, str.c_str());

							err = SGSetChannelDevice(gVideoChannel, pascalName);
							if ( err != noErr){
								if(bVerbose && didWeChooseADevice)printf("problems setting device[%i] %s *****\n", deviceID, str.c_str());
							}else{
								deviceName = str;
								if(bVerbose)printf("using device[%i] - %s\n", deviceID,  deviceName.c_str());
								break;
							}
						}
					} else {
						if(bVerbose && didWeChooseADevice)printf("device[%i] in use - using next default device\n", deviceID);
					}

				deviceID++;
			}
		}

	 	err = SGSetChannelUsage(gVideoChannel,seqGrabPreview);
		if ( err != noErr ) goto bail;

		err = SGSetChannelBounds(gVideoChannel, &videoRect);
		if ( err != noErr ) goto bail;

		err = SGPrepare(gSeqGrabber,  true, false); //theo swapped so preview is true and capture is false
		if ( err != noErr ) goto bail;

		err = SGStartPreview(gSeqGrabber);
		if ( err != noErr ) goto bail;

		bGrabberInited = true;
		loadSettings();

		if (bVerbose){
			printf ("end setup ofVideoGrabber\n");
			printf ("-------------------------------------\n");
			printf ("\n");
		}


		if (bUseTexture){
			// create the texture, set the pixels to black and
			// upload them to the texture (so at least we see nothing black the callback)
			tex.allocate(width,height,GL_RGB);
			memset(pixels, 0, width*height*3);
			tex.loadData(pixels, width, height, GL_RGB);
		}



		return;				// good catch eloi/theo

		//--------------------- (bail) something's wrong -----
		bail:

			if (bVerbose){
				printf ("***** ofVideoGrabber error *****\n");
				printf ("-------------------------------------\n");
				printf ("\n");
			}

			bGrabberInited = false;
			return;

	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	//---------------------------------

		if (bChooseDevice){
			device = deviceID;
			printf("choosing %i  \n", deviceID);
		} else {
			device = 0;
		}

		width = w;
		height = h;
		bGrabberInited = false;

		bool bOk = VI.setupDevice(device, width, height);

		int ourRequestedWidth = width;
		int ourRequestedHeight = height;

		if (bOk == true){
			bGrabberInited = true;
			width 	= VI.getWidth(device);
			height 	= VI.getHeight(device);

			if (width == ourRequestedWidth && height == ourRequestedHeight){
				bDoWeNeedToResize = false;
			} else {
				bDoWeNeedToResize = true;
				width = ourRequestedWidth;
				height = ourRequestedHeight;
			}


			pixels	= new unsigned char[width * height * 3];

			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				memset(pixels, 0, width*height*3);
				tex.loadData(pixels, width, height, GL_RGB);
			}
		} else {
			printf("error allocating a video device  \nplease check your camera with AMCAP or other software\n");
			bGrabberInited = false;
		}

	//---------------------------------
	#endif
	//---------------------------------



	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_UNICAP
	//--------------------------------
		if(!bGrabberInited){
			if (!bChooseDevice){
				deviceID = 0;
			}
			ucGrabber.open_device (deviceID);
			printf("choosing device %i: %s\n", deviceID,ucGrabber.device_identifier());
			ucGrabber.set_format(w,h);
			width 	= w;
			height 	= h;
			if(w==ucGrabber.getUC_Width() && h==ucGrabber.getUC_Height()){
				bDoWeNeedToResize=false;
			}else{
				bDoWeNeedToResize=true;
				auxPixels	= new unsigned char[ucGrabber.getUC_Width() * ucGrabber.getUC_Height() * 3];
			}
			pixels	= new unsigned char[width * height * 3];

			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				memset(pixels, 0, width*height*3);
				tex.loadData(pixels, width, height, GL_RGB);
			}
			ucGrabber.start_capture();
			bGrabberInited=true;
		}
	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_V4L
	//--------------------------------
		if (bChooseDevice){
			device = deviceID;
		} else {
			device = 0;
		}
		sprintf(dev_name, "/dev/video%i", device);
		printf("choosing device %s\n", dev_name);

		bool bOk = initV4L(w, h, dev_name);

		if (bOk == true){
			bV4LGrabberInited = true;
			width 	= getV4L_Width();
			height 	= getV4L_Height();
			pixels	= new unsigned char[width * height * 3];

			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				//memset(pixels, 0, width*height*3);
				//tex.loadData(pixels, width, height, GL_RGB);
			}
			
			printf("success allocating a video device \n");
		} else {
			printf("error allocating a video device \nplease check your camera and verify that your driver is correctly installed.\n");
		}	//---------------------------------


	//---------------------------------
	#endif
	//---------------------------------


	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------
		width = w;
		height = h;
		
		//first choose a device to use
		if (bChooseDevice)
		{
			bool bFound = false;
			for (uint32_t index = 0; index < m_deviceList->num; index++)
			{
				if (m_deviceList->ids[index].unit == deviceID)
				{
					bFound = true;
					m_deviceID = m_deviceList->ids[index].guid;
				}
			}
			
			if (!bFound)
			{
				printf("initGrabber warning: Device ID for unit %x not found, using first device\n", deviceID);
				m_deviceID = m_deviceList->ids[0].guid;
			}
		}
		else
		{
			m_deviceID = m_deviceList->ids[0].guid;
		}
		
		//now attempt to create a camera
		m_camera = dc1394_camera_new (m_driver, m_deviceID);
		if (!m_camera) 
		{
			dc1394_log_error("Failed to initialize camera with guid %x", m_deviceID);
			return;
		}
		
		//Create majority of features.  Some features may have been created in constructor.  
		//m_cameraFeatures[VGFN_VideoMode]		= new CameraFeatureGeneric(VGFN_VideoMode, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Brightness]	= new CameraFeatureGeneric(VGFN_Brightness, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Exposure]		= new CameraFeatureGeneric(VGFN_Exposure, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Sharpness]		= new CameraFeatureGeneric(VGFN_Sharpness, m_camera, bVerbose);
		m_cameraFeatures[VGFN_WhiteBalance]	= new CameraFeatureGeneric(VGFN_WhiteBalance, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Hue]			= new CameraFeatureGeneric(VGFN_Hue, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Saturation]	= new CameraFeatureGeneric(VGFN_Saturation, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Gamma]			= new CameraFeatureGeneric(VGFN_Gamma, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Shutter]		= new CameraFeatureGeneric(VGFN_Shutter, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Gain]			= new CameraFeatureGeneric(VGFN_Gain, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Iris]			= new CameraFeatureGeneric(VGFN_Iris, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Focus]			= new CameraFeatureGeneric(VGFN_Focus, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Temperature]	= new CameraFeatureGeneric(VGFN_Temperature, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Trigger]		= new CameraFeatureGeneric(VGFN_Trigger, m_camera, bVerbose);
		m_cameraFeatures[VGFN_TriggerDelay]	= new CameraFeatureGeneric(VGFN_TriggerDelay, m_camera, bVerbose);
		m_cameraFeatures[VGFN_WhiteShading]	= new CameraFeatureGeneric(VGFN_WhiteShading, m_camera, bVerbose);
		//m_cameraFeatures[VGFN_FrameRate]		= new CameraFeatureGeneric(VGFN_FrameRate, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Zoom]			= new CameraFeatureGeneric(VGFN_Zoom, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Pan]			= new CameraFeatureGeneric(VGFN_Pan, m_camera, bVerbose);
		m_cameraFeatures[VGFN_Tilt]			= new CameraFeatureGeneric(VGFN_Tilt, m_camera, bVerbose);
		m_cameraFeatures[VGFN_OpticalFilter] = new CameraFeatureGeneric(VGFN_OpticalFilter, m_camera, bVerbose);
		m_cameraFeatures[VGFN_CaptureSize]	= new CameraFeatureGeneric(VGFN_CaptureSize, m_camera, bVerbose);
		m_cameraFeatures[VGFN_CaptureQuality]= new CameraFeatureGeneric(VGFN_CaptureQuality, m_camera, bVerbose);

		/*-----------------------------------------------------------------------
		 *  get the best video mode that is equal to or better than requested 
		 *  resolution at highest framerate.
		 *-----------------------------------------------------------------------*/

		//set up bus mode and speed to maximum possible
		//set to maximum ISO speed bus will allow
		//n: ISO here refers to isosynchronous bus speed (1394a or 1394b), not ISO-euivalent (gain) as in the photography sense.
		//dc1394operation_mode_t operationMode;
	
	
	dc1394speed_t speedMode = DC1394_ISO_SPEED_800;
	
	/**

	 dc1394_video_set_mode(camera, DC1394_VIDEO_MODE_FORMAT7_0);
    err = dc1394_format7_set_roi(camera, DC1394_VIDEO_MODE_FORMAT7_0,
                                 DC1394_COLOR_CODING_MONO8,
                                 DC1394_USE_MAX_AVAIL, // use max packet size
                                 0, 0, // left, top
                                 WIDTH, HEIGHT);  // width, height
    	**/
		//!!! how do we know what mode to use?  This always succeeds, even with a legacy bus.
		dc1394error_t err = dc1394_video_set_operation_mode(m_camera, DC1394_OPERATION_MODE_1394B);
		if (err == DC1394_SUCCESS)
		{
			speedMode = DC1394_ISO_SPEED_800;
			cout << "1394B ISO SPEED 800" << endl;
		}
		else
		{
			cout << "1394a ISO SPEED 400" << endl;
			err = dc1394_video_set_operation_mode(m_camera, DC1394_OPERATION_MODE_LEGACY);
		}
		DC1394_ERR_CLN(err,cleanup_and_exit(m_camera),"Could not set operation mode");
		
		err = dc1394_video_set_iso_speed(m_camera, speedMode);
		DC1394_ERR_CLN(err,cleanup_and_exit(m_camera),"Could not set iso speed");
		
		//ready to set video mode parameters.  Use default value or value that may have been set before video turned on.
		m_cameraFeatures[VGFN_ColorMode]->setValue(VGFCM_DefaultColorMode);
		
		// get highest framerate at this resolution
		dc1394video_mode_t currentMode;
		err = dc1394_video_get_mode(m_camera, &currentMode);
		DC1394_ERR_CLN(err,cleanup_and_exit(m_camera),"Could not get video mode");
		
		//determine best framerate
		dc1394framerates_t framerates;
		err=dc1394_video_get_supported_framerates(m_camera, currentMode, &framerates);
		DC1394_ERR_CLN(err,cleanup_and_exit(m_camera),"Could not get framrates");
		dc1394framerate_t goodFrameRate;
		goodFrameRate = framerates.framerates[framerates.num-1];
		
		err=dc1394_video_set_framerate(m_camera, goodFrameRate);
		DC1394_ERR_CLN(err,cleanup_and_exit(m_camera),"Could not set framerate");
		
		
		/*-----------------------------------------------------------------------
		 *  setup capture
		 *-----------------------------------------------------------------------*/
		err = dc1394_capture_setup(m_camera, 4, DC1394_CAPTURE_FLAGS_DEFAULT);
		DC1394_ERR_CLN(err,cleanup_and_exit(m_camera),"Could not setup camera-\nmake sure that the video mode and framerate are\nsupported by your camera");
	
		/*-----------------------------------------------------------------------
		 *  have the camera start sending us data
		 *-----------------------------------------------------------------------*/
		err = dc1394_video_set_transmission(m_camera, DC1394_ON);
		DC1394_ERR_CLN(err,cleanup_and_exit(m_camera),"Could not start camera iso transmission");

		//create pixel buffer in RGB8 (default) format
		//todo: create it in native format and only convert if textures are enabled.  
		pixels	= new unsigned char[width * height * 3];
		
		//create texture
		if (bUseTexture)
		{
			tex.allocate(width,height,GL_RGB);
			memset(pixels, 0, width*height*3);
			tex.loadData(pixels, width, height, GL_RGB);
		}
		
	//---------------------------------
	#endif //OF_VIDEO_CAPTURE_LIBDC1394
	//--------------------------------

}

//---------------------------------
#ifdef OF_VIDEO_CAPTURE_LIBDC1394
//--------------------------------

//--------------------------------
// Function:	ofVideoGrabber::cleanup_and_exit
// Description:	Error-handling helper function for camera initialization.  Deletes a camera before exiting
// Parameters:	camera - camera to delete (if not NULL)
// Return:		-
void ofVideoGrabber::cleanup_and_exit(dc1394camera_t *camera)
{
	if (camera != NULL)
	{
		dc1394_video_set_transmission(camera, DC1394_OFF);
		dc1394_capture_stop(camera);
		dc1394_camera_free(camera);
	}
    exit(1);
}
//---------------------------------
#endif //OF_VIDEO_CAPTURE_LIBDC1394
//--------------------------------


//------------------------------------
void ofVideoGrabber::setUseTexture(bool bUse)
{
	bUseTexture = bUse;
}


//------------------------------------
void ofVideoGrabber::draw(float _x, float _y, float _w, float _h)
{
	if (bUseTexture)
	{
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y)
{
	draw(_x, _y, width, height);
}

//--------------------------------
// Function:	ofVideoGrabber::hasFeature
// Description:	Check if a camera has a particular feature
// Parameters:	feature - feature to check
// Return:		true if feature available
//				false if:
//					- feature not available
//					- implementation does not support features
#define ASSERT_FEATURE(f)	assert((feature) >= VGFN_Width && (feature) < VGFN_NumFeatures);
bool ofVideoGrabber::hasFeature(t_cameraFeature feature)									
{
	ASSERT_FEATURE(feature); 
	if (m_cameraFeatures[feature] == NULL)
	{
		return(false);
	}
	return(m_cameraFeatures[feature]->hasFeature());
}

//--------------------------------
// Function:	ofVideoGrabber::setValue
// Description:	Set value of a particular feature
// Parameters:	feature - feature to set value of
//				value - value to set it to.  Will be clamped to valid range.  
// Return:		true if value set.
//				false if:
//					- feature not available
//					- implementation does not support features
//					- value set to auto control
bool ofVideoGrabber::setValue(t_cameraFeature feature, float value)					
{
	ASSERT_FEATURE(feature); 
	if (m_cameraFeatures[feature] == NULL)
	{
		return(false);
	}
	return(m_cameraFeatures[feature]->setValue(value));
}

//--------------------------------
// Function:	ofVideoGrabber::getValue
// Description:	Get value of a particular feature
// Parameters:	feature - feature to check.  
//				value - where to store the current value.  Only valid on success. 
// Return:		true if value read OK.
//				false if:
//					- feature not available
//					- implementation does not support features
//					- value cannot be read
bool ofVideoGrabber::getValue(t_cameraFeature feature, float* value)						
{
	ASSERT_FEATURE(feature); 
	if (m_cameraFeatures[feature] == NULL)
	{
		return(false);
	}
	return(m_cameraFeatures[feature]->getValue(value));
}

//--------------------------------
// Function:	ofVideoGrabber::getValueRange
// Description:	Get allowable range of a value
// Parameters:	feature - feature to get range of
//				lower, upper - bounds of range.  Only valid on success.  
// Return:		true if value range read OK.
//				false if:
//					- feature not available
//					- implementation does not support features
//					- range cannot be read
bool ofVideoGrabber::getValueRange(t_cameraFeature feature, float* lower, float* upper)	
{
	ASSERT_FEATURE(feature); 
	if (m_cameraFeatures[feature] == NULL)
	{
		return(false);
	}
	return(m_cameraFeatures[feature]->getValueRange(lower, upper));
}

//--------------------------------
// Function:	ofVideoGrabber::getAuto
// Description:	See if a value is set to auto control
// Parameters:	feature - feature to check
//				autoMode - where to store result.  Only valid on success.
// Return:		true if value status read OK.
//				false if:
//					- feature not available
//					- implementation does not support features
//					- auto status cannot be read
bool ofVideoGrabber::getAuto(t_cameraFeature feature, bool* autoMode)
{
	ASSERT_FEATURE(feature); 
	if (m_cameraFeatures[feature] == NULL)
	{
		return(false);
	}
	return(m_cameraFeatures[feature]->getAuto(autoMode));
}

//--------------------------------
// Function:	ofVideoGrabber::setAuto
// Description:	Sets a feature to auto control.  
// Parameters:	feature - feature to check
//				autoMode - true for automatic control, false for manual
// Return:		true if value status read OK.
//				false if:
//					- feature not available
//					- implementation does not support features
//					- feature does not support auto or manual mode (as requested)
bool ofVideoGrabber::setAuto(t_cameraFeature feature, bool autoMode)
{
	ASSERT_FEATURE(feature); 
	if (m_cameraFeatures[feature] == NULL)
	{
		return(false);
	}
	return(m_cameraFeatures[feature]->setAuto(autoMode));
}

//---------------------------------
#ifdef OF_VIDEO_CAPTURE_LIBDC1394
//--------------------------------
//--------------------------------
// Function:	ofVideoGrabber::CameraFeature::CameraFeature
// Description:	Constructor of empty generic (pure virtual) CameraFeature.  
// Parameters:	Feature ID
// Return:		
ofVideoGrabber::CameraFeature::CameraFeature(t_cameraFeature feature)
{
	ASSERT_FEATURE(feature);
}

ofVideoGrabber::CameraFeature::~CameraFeature()
{
}

//--------------------------------
// Function:	CameraFeatureGeneric::CameraFeatureGeneric
// Description:	Create a generic feature.  Queries the camera for properties of said feature.  
//	todo: modify so that feature is created in ofVideoGrabber constructor so all features can be set before camera initialization.
// Parameters:	feature - feature ID
//				camera - pointer to camera (must not be NULL)
//				bVerbose - should we be verbose?
// Return:		-
ofVideoGrabber::CameraFeatureGeneric::CameraFeatureGeneric(t_cameraFeature feature, dc1394camera_t* camera, bool bVerbose)
 : ofVideoGrabber::CameraFeature::CameraFeature(feature)
{
	assert(camera != NULL);
	
	//save camera pointer, will be needed for subsequent feature calls
	m_camera = camera;
	
	//map camera feature to libdc1394 camera feature
	switch (feature)
	{
		case VGFN_Brightness:
			m_dc1394FeatureID = DC1394_FEATURE_BRIGHTNESS;
			break;
		case VGFN_Exposure:
			m_dc1394FeatureID = DC1394_FEATURE_EXPOSURE;
			break;
		case VGFN_Sharpness:
			m_dc1394FeatureID = DC1394_FEATURE_SHARPNESS;
			break;
		case VGFN_WhiteBalance:
			m_dc1394FeatureID = DC1394_FEATURE_WHITE_BALANCE;
			break;
		case VGFN_Hue:
			m_dc1394FeatureID = DC1394_FEATURE_HUE;
			break;
		case VGFN_Saturation:
			m_dc1394FeatureID = DC1394_FEATURE_SATURATION;
			break;
		case VGFN_Gamma:
			m_dc1394FeatureID = DC1394_FEATURE_GAMMA;
			break;
		case VGFN_Shutter:
			m_dc1394FeatureID = DC1394_FEATURE_SHUTTER;
			break;
		case VGFN_Gain:
			m_dc1394FeatureID = DC1394_FEATURE_GAIN;
			break;
		case VGFN_Iris:
			m_dc1394FeatureID = DC1394_FEATURE_IRIS;
			break;
		case VGFN_Focus:
			m_dc1394FeatureID = DC1394_FEATURE_FOCUS;
			break;
		case VGFN_Temperature:
			m_dc1394FeatureID = DC1394_FEATURE_TEMPERATURE;
			break;
		case VGFN_Trigger:
			m_dc1394FeatureID = DC1394_FEATURE_TRIGGER;
			break;
		case VGFN_TriggerDelay:
			m_dc1394FeatureID = DC1394_FEATURE_TRIGGER_DELAY;
			break;
		case VGFN_WhiteShading:
			m_dc1394FeatureID = DC1394_FEATURE_WHITE_SHADING;
			break;
		case VGFN_FrameRate:
			m_dc1394FeatureID = DC1394_FEATURE_FRAME_RATE;
			break;
		case VGFN_Zoom:
			m_dc1394FeatureID = DC1394_FEATURE_ZOOM;
			break;
		case VGFN_Pan:
			m_dc1394FeatureID = DC1394_FEATURE_PAN;
			break;
		case VGFN_Tilt:
			m_dc1394FeatureID = DC1394_FEATURE_TILT;
			break;
		case VGFN_OpticalFilter:
			m_dc1394FeatureID = DC1394_FEATURE_OPTICAL_FILTER;
			break;
		case VGFN_CaptureSize:
			m_dc1394FeatureID = DC1394_FEATURE_CAPTURE_SIZE;
			break;
		case VGFN_CaptureQuality:
			m_dc1394FeatureID = DC1394_FEATURE_CAPTURE_QUALITY;
			break;

		default:
			dc1394error_t err = DC1394_FUNCTION_NOT_SUPPORTED;
			DC1394_ERR(err, "Camera feature not supported");
	}
	
	//now get feature data
	m_dc1394FeatureData.id = m_dc1394FeatureID;
	dc1394error_t err = dc1394_feature_get(m_camera, &m_dc1394FeatureData);
	
	//print out a list of feature and it's capabilities and modes
	if (bVerbose)
	{
		dc1394_feature_print(&m_dc1394FeatureData, stdout);
	}
}

ofVideoGrabber::CameraFeatureGeneric::~CameraFeatureGeneric()
{
}

//--------------------------------
// Function:	CameraFeatureGeneric::hasFeature
// Description:	See if a given feature is available
// Parameters:	-
// Return:		true if feature available
bool ofVideoGrabber::CameraFeatureGeneric::hasFeature(void)
{
	return(m_dc1394FeatureData.available == DC1394_TRUE);
}

//--------------------------------
// Function:	CameraFeatureGeneric::setValue
// Description:	Set a generic feature
// Parameters:	Value - value to set feature to.  Will be clamped to feature bounds (getValueRange).
//						Value and range are system and camera-specific.
// Return:		true if value set.  May fail (false) for the following reasons:
//				- feature on auto mode
//				- feature not available (see hasFeature)
//				- system or camera-specific error
bool ofVideoGrabber::CameraFeatureGeneric::setValue(float value)
{
	if (m_dc1394FeatureData.available == DC1394_TRUE)
	{
		//don't set mode if feature set to auto
		if (m_dc1394FeatureData.current_mode == DC1394_FEATURE_MODE_AUTO)
		{
			//...log message?
			return(false);
		}
		//clamp to range
		uint32_t intValue = (uint32_t)value;
		if (intValue < m_dc1394FeatureData.min)
		{
			intValue = m_dc1394FeatureData.min;
		}
		else if (intValue > m_dc1394FeatureData.max)
		{
			intValue = m_dc1394FeatureData.max;
		}
		
		//set the feature
		dc1394error_t err = dc1394_feature_set_value(m_camera, m_dc1394FeatureID, intValue);
		m_dc1394FeatureData.value = intValue;
		
		return (err == DC1394_SUCCESS);
	}
	else
	{
		return false;												//not available
	}
}

//--------------------------------
// Function:	CameraFeatureGeneric::getValue
// Description:	Get a generic feature value.
// Parameters:	value - where to store value.  Invalid (unset) on failure.
// Return:		true on success (value is meaningful).  May fail for following reasons:
//				- feature not available
//				- feature cannont be read from camera
bool ofVideoGrabber::CameraFeatureGeneric::getValue(float* value)
{
	if (m_dc1394FeatureData.available == DC1394_TRUE)
	{
		if (m_dc1394FeatureData.readout_capable != DC1394_TRUE)
		{
			return false;
		}
		//get the value from camera
		uint32_t	intValue;
		dc1394error_t err = dc1394_feature_get_value(m_camera, m_dc1394FeatureID, &intValue);
		m_dc1394FeatureData.value = intValue;
		*value = (float)intValue;
		
		return (err == DC1394_SUCCESS);
	}
	else
	{
		return false;												//not available
	}
}

//--------------------------------
// Function:	CameraFeatureGeneric::getValueRange
// Description:	Get range of allowable values
// Parameters:	lower - where to store lower bounds
//				upper - where to store upper bounds
//				nb: these values may encapsulate an integer range
// Return:		true on success.  false if feature not available
bool ofVideoGrabber::CameraFeatureGeneric::getValueRange(float* lower, float* upper)
{
	if (m_dc1394FeatureData.available == DC1394_TRUE)
	{
		*lower = (float)m_dc1394FeatureData.min;
		*upper = (float)m_dc1394FeatureData.max;
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------
// Function:	CameraFeatureGeneric::getAuto
// Description:	Is feature automatically set?
// Parameters:	autoMode - where to store the mode.  true if in auto mode
// Return:		true if we could find out.  False if feature not available.
bool ofVideoGrabber::CameraFeatureGeneric::getAuto(bool* autoMode)
{
	if (m_dc1394FeatureData.available == DC1394_TRUE)
	{
		*autoMode = m_dc1394FeatureData.current_mode == DC1394_FEATURE_MODE_AUTO;
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------
// Function:	CameraFeatureGeneric::setAuto
// Description:	Set feature into auto or manual mode.
// Parameters:	autoMode - true means auto mode, false means manual
// Return:		true of mode set or already set to specified mode.  
//				false if feature not available or cannot be set to automatic.  
bool ofVideoGrabber::CameraFeatureGeneric::setAuto(bool autoMode)
{
	if (m_dc1394FeatureData.available == DC1394_TRUE)
	{
		if (m_dc1394FeatureData.current_mode == DC1394_FEATURE_MODE_AUTO && autoMode == false)
		{														//switch off
			return(setMode(DC1394_FEATURE_MODE_MANUAL));
		}
		else if (m_dc1394FeatureData.current_mode == DC1394_FEATURE_MODE_MANUAL && autoMode == true)
		{														//switch on
			return(setMode(DC1394_FEATURE_MODE_AUTO));
		}
		return true;											//no need to switch
	}
	else
	{
		return false;
	}
}

//--------------------------------
// Function:	CameraFeatureGeneric::setMode
// Description:	Helper function for ::setAuto
// Parameters:	mode - DC1394_FEATURE_MODE_AUTO or DC1394_FEATURE_MODE_MANUAL
// Return:		true if mode set as desired
bool ofVideoGrabber::CameraFeatureGeneric::setMode(dc1394feature_mode_t mode)
{
	//find desired mode
	bool bFoundMode = false;
	
	for (uint32_t index = 0; index < DC1394_FEATURE_MODE_NUM; index++)
	{
		if (m_dc1394FeatureData.modes.modes[index] == mode)
		{
			bFoundMode = true;
			break;
		}
	}
	if (bFoundMode)
	{
		dc1394error_t err = dc1394_feature_set_mode(m_camera, m_dc1394FeatureID, mode);
		if (err == DC1394_SUCCESS)
		{
			m_dc1394FeatureData.current_mode = mode;
			return true;
		}
	}
	return false;
}

//--------------------------------
// Function:	CameraFeatureColorMode::CameraFeatureColorMode
// Description:	ColorMode feature constructor.  Called during ofVideoGrabber constructor so it just stores pointers to grabber parameters
// Parameters:	feature - feature ID
//				camera - where camera will be stored.  
//				width - where width will be stored.
//				height - where height will be stored.
//				bVerbose - verbose?
// Return:		-
ofVideoGrabber::CameraFeatureColorMode::CameraFeatureColorMode(t_cameraFeature feature, dc1394camera_t** camera, const int* width, const int* height, bool bVerbose)
 : ofVideoGrabber::CameraFeature::CameraFeature(feature)
{
	//save camera pointer, will be needed for subsequent feature calls
	m_camera = camera;	
	
	//Set default coding.  Client may change coding before camera is created, in which case that coding will be used.  
	//If changed after creation, coding will be changed.  
	//If camera is not capable of this default mode, another mode will be chosen as default.  
	m_desiredColorCoding = DC1394_COLOR_CODING_RGB8;
	
	//no modes known until camera is created
	m_availableModes.num = 0;
	
	//set default color coding to that from first mode.  Not a very good one.
	//dc1394_get_color_coding_from_video_mode(m_camera, m_availableModes.modes[0], &m_colorCoding);
	assert(DC1394_COLOR_CODING_MIN != 0);
	m_colorCoding = (dc1394color_coding_t)0;									//set to a nonsense value to ensure the first call to setValue will result in a mode change
	
	//save the width and height, as it will be needed before the camera is capturing and modes are set
	m_currentWidth = width;
	m_currentHeight = height;
}

//--------------------------------
// Function:	CameraFeatureColorMode::~CameraFeatureColorMode
// Description:	Destructor
// Parameters:	
// Return:		
ofVideoGrabber::CameraFeatureColorMode::~CameraFeatureColorMode()
{
}

//--------------------------------
// Function:	CameraFeatureColorMode::hasFeature
// Description:	Has feature
// Parameters:	
// Return:		true 
bool ofVideoGrabber::CameraFeatureColorMode::hasFeature(void)
{
	return true;
}

//--------------------------------
// Function:	CameraFeatureColorMode::setValue
// Description:	  Sets ColorMode.  
// Parameters:	value - vaue representing color mode.  See VGFCM_xxx.  
//				When setting the value of color mode, uses the next mode up or down (depending on current mode) that works without changing resolution.
//				Mode selected may be different than what was requested.
//				When called before camera created, desired mode will be saved for later.
//				When called during camera initialization (value = VGFCM_DefaultColorMode), previously-set mode, or current mode will be set.  
// Return:		true if mode changed or no change needed.
//				false if mode not changed:
//					- no camera yet (does not indicate an error condition)
//					- no video modes known or available
//					- video mode reportedly available, but could not be set
bool ofVideoGrabber::CameraFeatureColorMode::setValue(float value)
{
	dc1394color_coding_t desired = (dc1394color_coding_t)value;
	
	//if default value passed in, set default color mode
	if (value == VGFCM_DefaultColorMode)
	{
		desired = m_desiredColorCoding;
	}
	
	//if camera not yet created, remember mode for later setting
	if (*m_camera == NULL)
	{
		m_desiredColorCoding = desired;
		return false;
	}
	
	//clamp to bounds
	if (desired < DC1394_COLOR_CODING_MIN)
	{
		desired = DC1394_COLOR_CODING_MIN;
	}
	if (desired > DC1394_COLOR_CODING_MAX)
	{
		desired = DC1394_COLOR_CODING_MAX;
	}
	
	//already in this mode?
	if (desired == m_colorCoding)
	{												
		return true;								//nothing to do
	}
	
	//if we don't yet know modes
	if (m_availableModes.num == 0)
	{
		dc1394error_t err = dc1394_video_get_supported_modes(*m_camera, &m_availableModes);
		if (err != DC1394_SUCCESS)
		{
			return false;
		}
	}
	
	//Now make an array of all the modes we might like to try.  Specified mode goes first.  
	//Later when looking through available video modes, the first possible coding in this array will be chosen as best.
	//This algorithm assumes that all the color codings enumerations are sequential.
	int nPreferredCodings;
	dc1394color_coding_t preferredModes[DC1394_COLOR_CODING_NUM];
	if (desired < m_colorCoding)
	{																			//going to nominally lower coding
		for (nPreferredCodings = 0; nPreferredCodings < DC1394_COLOR_CODING_NUM; nPreferredCodings++)
		{
			if (desired + nPreferredCodings > DC1394_COLOR_CODING_MAX)
			{
				break;
			}
			if (desired + nPreferredCodings >= m_colorCoding)
			{																	//no longer less than current
				break;
			}
			preferredModes[nPreferredCodings] = (dc1394color_coding_t)(desired + nPreferredCodings);
		}
		//and add in a bunch more codings just in case...
		for (int nMoreCodings = 1; nPreferredCodings < DC1394_COLOR_CODING_NUM; nMoreCodings++, nPreferredCodings++)
		{		
			if (desired - nMoreCodings < DC1394_COLOR_CODING_MIN)
			{
				break;
			}
			preferredModes[nPreferredCodings] = (dc1394color_coding_t)(desired - nMoreCodings);
		}
	}
	else
	{																			//going to nominally greater coding
		for (nPreferredCodings = 0; nPreferredCodings < DC1394_COLOR_CODING_NUM; nPreferredCodings++)
		{		
			if (desired - nPreferredCodings < DC1394_COLOR_CODING_MIN)
			{
				break;
			}
			if (desired - nPreferredCodings <= m_colorCoding)
			{																	//no longer greater than current
				break;
			}
			preferredModes[nPreferredCodings] = (dc1394color_coding_t)(desired - nPreferredCodings);
		}
		//and add in a bunch more codings just in case...
		for (int nMoreCodings = 1; nPreferredCodings < DC1394_COLOR_CODING_NUM; nMoreCodings++, nPreferredCodings++)
		{		
			if (desired + nMoreCodings > DC1394_COLOR_CODING_MAX)
			{
				break;
			}
			preferredModes[nPreferredCodings] = (dc1394color_coding_t)(desired + nMoreCodings);
		}
	}
	
	//now go through all available modes looking for a mode that works at our current resolution
	int iBestCoding = DC1394_COLOR_CODING_NUM;
	dc1394video_mode_t bestVideoMode;
	dc1394color_coding_t bestCoding;
	for (int index = m_availableModes.num - 1; index >= 0; index--) 
	{
		//first get parameters of current mode
		uint32_t modeWidth, modeHeight;

		dc1394error_t err = dc1394_get_image_size_from_video_mode(*m_camera, m_availableModes.modes[index], &modeWidth, &modeHeight);
		if (err == DC1394_SUCCESS)
		{
			if (modeWidth <= *m_currentWidth && modeHeight <= *m_currentHeight)
			{																		//if same resolution as current
				if (!dc1394_is_video_mode_scalable(m_availableModes.modes[index]))	//??? what is this scalable all about?
				{																	//and scalable
					dc1394color_coding_t modeCoding;
					err = dc1394_get_color_coding_from_video_mode(*m_camera, m_availableModes.modes[index], &modeCoding);
					if (err == DC1394_SUCCESS)
					{
						//find color coding in list of desired codings
						
						for (int iCoding = 0; iCoding < nPreferredCodings; iCoding++)
						{															//for each preferred coding
							if (modeCoding == preferredModes[iCoding])
							{														//if this coding found
								if (iCoding < iBestCoding)
								{													//if color coding closest to current found thus far
									iBestCoding = iCoding;
									bestVideoMode = m_availableModes.modes[index];
									bestCoding = modeCoding;
								}
								break;
							}
						}
					}
				}
			}
		}
	}

	//after that loop, we know a video mode that matches current resolution, and is closest, up or down, to current mode
	if (iBestCoding < nPreferredCodings)
	{
		m_colorCoding = bestCoding;
		//!!!need to tell the video mode feature of change of mode?
		
		dc1394error_t err = dc1394_capture_stop(*m_camera);
		//!!! hack to not restart the first time through
		//!!! is there a way to tell if capture is running?
		static bool bRestartCapture = false;//!!!err != DC1394_CAPTURE_IS_NOT_SET;
		
		err = dc1394_video_set_mode(*m_camera, bestVideoMode);
		if (err != DC1394_SUCCESS)
		{
			return false;
		}
		
		if (bRestartCapture)
		{
			err = dc1394_capture_setup(*m_camera, 4, DC1394_CAPTURE_FLAGS_DEFAULT);
			if (err != DC1394_SUCCESS)
			{
				return false;
			}
		}
		bRestartCapture = true;
	}

	return true;
}

//--------------------------------
// Function:	CameraFeatureColorMode::getValue
// Description:	return current mode
// Parameters:	-
// Return:		current video mode (see VGFCM_xxxx) Undefined if capture not running.
bool ofVideoGrabber::CameraFeatureColorMode::getValue(float* value)
{
	*value = (float)m_colorCoding;
	return true;
}

//--------------------------------
// Function:	CameraFeatureColorMode::getValueRange
// Description:	Get the range of this feature is simply all possible color modes that can be set on this camera.
// Parameters:	lower, upper - where to store the range.
//				May include modes not available at current resolution (or future resolution).
// Return:		
//the range of this feature is simply all possible color modes that can be set on this camera
bool ofVideoGrabber::CameraFeatureColorMode::getValueRange(float* lower, float* upper)
{
	dc1394color_coding_t lowMode, highMode;
	
	if (*m_camera == NULL)
	{
		return false;
	}
	
	//if we don't yet know modes
	if (m_availableModes.num == 0)
	{
		dc1394error_t err = dc1394_video_get_supported_modes(*m_camera, &m_availableModes);
		if (err != DC1394_SUCCESS)
		{
			return false;
		}
	}
	
	lowMode = DC1394_COLOR_CODING_MAX;
	highMode = DC1394_COLOR_CODING_MIN;
	
	for (uint32_t index = 0; index < m_availableModes.num; index++)
	{
		dc1394color_coding_t localMode;
		dc1394_get_color_coding_from_video_mode(*m_camera, m_availableModes.modes[index], &localMode);
		
		if (localMode > highMode)
		{
			highMode = localMode;
		}
		if (localMode < lowMode)
		{
			lowMode = localMode;
		}
	}
	if (lowMode > highMode)
	{												//didn't find any modes
		return false;
	}
	
	*lower = (float)lowMode;
	*upper = (float)highMode;
	return true;
}

//--------------------------------
// Function:	CameraFeatureColorMode::getAuto
// Description:	Auto color mode?  Doesn't make sense, but there is a default.  
// Parameters:	automode - false
// Return:		true
bool ofVideoGrabber::CameraFeatureColorMode::getAuto(bool* autoMode)
{
	*autoMode = false;
	return true;
}

//--------------------------------
// Function:	CameraFeatureColorMode::setAuto
// Description:	Auto color mode?  Doesn't make sense, but there is a default.   
// Parameters:	automode - ignored
// Return:		false if you try to set auto mode.  
bool ofVideoGrabber::CameraFeatureColorMode::setAuto(bool autoMode)
{
	return(!autoMode);
}


//---------------------------------
#endif //OF_VIDEO_CAPTURE_LIBDC1394
//--------------------------------
