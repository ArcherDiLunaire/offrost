#ifndef _OF_VIDEO_GRABBER
#define _OF_VIDEO_GRABBER

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"


#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	#include "ofQtUtils.h"
#endif

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include "videoInput.h"
#endif

#ifdef OF_VIDEO_CAPTURE_LIBDC1394
	#include <stdint.h>
	#include <dc1394/dc1394.h>
	#include <inttypes.h>
#endif

// todo:
// 		QT - callback, via SGSetDataProc - couldn't get this to work yet
// 		image decompress options ala mung...

//definitions for camera capabilities (abstractions of capabilities reported by libdc1394)
enum t_cameraFeature
{
	VGFN_Width = 0,					//resolution: reflects captured size, which may differ from ofVideoGrabber (behavior will vary between implementations)
	VGFN_Height,
	//VGFN_VideoMode,
	VGFN_ColorMode,					//Color mode: color monochrome etc. see values below.  Conversions may be made in capture routine.
	VGFN_Brightness,				//Brightness: histogram bias
	VGFN_Exposure,					//Exposure: gain * shutter, affects both.
	VGFN_Sharpness,
	VGFN_WhiteBalance,
	VGFN_Hue,
	VGFN_Saturation,
	VGFN_Gamma,						
	VGFN_Shutter,
	VGFN_Gain,						//Gain: ISO equivalency on cameras.  
	VGFN_Iris,
	VGFN_Focus,
	VGFN_Temperature,
	VGFN_Trigger,
	VGFN_TriggerDelay,
	VGFN_WhiteShading,
	VGFN_FrameRate,
	VGFN_Zoom,
	VGFN_Pan,
	VGFN_Tilt,
	VGFN_OpticalFilter,
	VGFN_CaptureSize,
	VGFN_CaptureQuality,
	
	VGFN_NumFeatures
};

//--------------------------------
//definitions of video modes that wrap up underlying video input settings
#ifdef OF_VIDEO_CAPTURE_LIBDC1394

//definitions of color modes for VGFN_ColorMode
#define	VGFCM_DefaultColorMode		-1.0f
#define	VGFCM_RGB					((float)DC1394_COLOR_CODING_RGB8)
#define	VGFCM_MONO					((float)DC1394_COLOR_CODING_MONO8)
#define	VGFCM_YUV411				((float)DC1394_COLOR_CODING_YUV411)
#define	VGFCM_YUV422				((float)DC1394_COLOR_CODING_YUV422)
#define VGFCM_YUV444				((float)DC1394_COLOR_CODING_YUV444)
#define VGFCM_FORMAT7_0				((float)DC1394_VIDEO_MODE_FORMAT7_0)

#else // OF_VIDEO_CAPTURE_LIBDC1394

//nonsense values for other implementations that do no support this feature
#define	VGFCM_DefaultColorMode		-1.0f
#define	VGFCM_RGB					1.0f
#define	VGFCM_MONO					2.0f
#define	VGFCM_YUV411				3.0f
#define	VGFCM_YUV422				4.0f
#define VGFCM_YUV444				5.0f

#endif // OF_VIDEO_CAPTURE_LIBDC1394


class ofVideoGrabber{

	public :

		ofVideoGrabber();
		~ofVideoGrabber();

		void 			listDevices();
		bool 			isFrameNew();
		void			grabFrame();
		void			close();
		void 			initGrabber(int w, int h);
		void			initGrabber(int w, int h, bool bTexture);
		void			videoSettings();
		unsigned char 	* getPixels();
		void 			setVerbose(bool bTalkToMe);
		void			setDeviceID(int _deviceID);
		void 			setUseTexture(bool bUse);
		void 			draw(float x, float y, float w, float h);
		void 			draw(float x, float y);
		int 			width, height;
		
		//--------------------------------
		// These functions abstract the capabilities of the camera and allow client apps to access them.
		// Underlying features may or may not be implemented on all implementations of ofVideoCapture; check return values
		// See ofVideoGrabber.cpp for full descriptions.
		bool			hasFeature(t_cameraFeature feature);
		bool			setValue(t_cameraFeature feature, float value);
		bool 			getValue(t_cameraFeature feature, float* value);
		bool			getValueRange(t_cameraFeature feature, float* lower, float* upper);
		bool			getAuto(t_cameraFeature feature, bool* autoMode);
		bool			setAuto(t_cameraFeature feature, bool autoMode);
		
	protected:

		//--------------------------------
		// Class:		CameraFeature
		// Description: Pure virtual base class to abstract underlying camera features.  
		//				Instantiate for different grabber types/feature types
		// Inherits:	-
		class CameraFeature
		{
			public:
			
			CameraFeature(t_cameraFeature feature);
			virtual ~CameraFeature();
			
			//virtual interface: inherit for implementation-specific feature classes
			virtual bool hasFeature(void) = 0;
			virtual bool setValue(float value) = 0;
			virtual bool getValue(float* value) = 0;
			virtual bool getValueRange(float* lower, float* upper) = 0;
			virtual bool getAuto(bool* autoMode) = 0;
			virtual bool setAuto(bool autoMode) = 0;
			
			protected:
			private:
		};
		

		bool					bChooseDevice;
		int						deviceID;
		bool					bUseTexture;
		ofTexture 				tex;
		bool 					bVerbose;
		bool 					bGrabberInited;
	    unsigned char * 		pixels;
		bool 					bIsFrameNew;

		//array of pointers CameraFeature objects.  Feature, and implementation-specific.
		CameraFeature*			m_cameraFeatures[VGFN_NumFeatures];
		
		//--------------------------------- quicktime
		#ifdef OF_VIDEO_CAPTURE_QUICKTIME


			unsigned char * 	offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)
			int 				w,h;
			bool 				bHavePixelsChanged;
			GWorldPtr 			videogworld;
			SeqGrabComponent	gSeqGrabber;
			SGChannel 			gVideoChannel;
			Rect				videoRect;
			bool 				bSgInited;
			string				deviceName;

			//--------------------------------------------------------------------
			#ifdef TARGET_OSX
			//--------------------------------------------------------------------

			bool					saveSettings();
			bool					loadSettings();

			//--------------------------------------------------------------------
			#endif
			//--------------------------------------------------------------------


		#endif

		//--------------------------------- directshow
		#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			int 					device;
			videoInput 				VI;

			bool 					bDoWeNeedToResize;

		#endif

		//--------------------------------- linux unicap
		#ifdef OF_VIDEO_CAPTURE_UNICAP
			bool 					bDoWeNeedToResize;
			unsigned char *			auxPixels;
			ofUCUtils				ucGrabber;
		#endif


		//--------------------------------- linux V4L
		// if unicap doesn't work, we keep linux v4l in here....
		// so folks can switch, in ofConstants.h
		#ifdef OF_VIDEO_CAPTURE_V4L
			int 					device;
			char 					dev_name[80];
			bool					bV4LGrabberInited;
		#endif

		//--------------------------------- dc1394: true IIDC cameras
		#ifdef OF_VIDEO_CAPTURE_LIBDC1394
			bool 					bDoWeNeedToResize;
			dc1394_t*				m_driver;
			dc1394camera_list_t*	m_deviceList;
			uint64_t				m_deviceID;
			dc1394camera_t*			m_camera;
			
			static void cleanup_and_exit(dc1394camera_t *camera);

			//--------------------------------
			// Class:		CameraFeatureGeneric
			// Description: Generic feature: supports most libdc1394 camera features.  
			// Inherits:	CameraFeature - virtual interface
			class CameraFeatureGeneric : public CameraFeature
			{
				public:
				
				CameraFeatureGeneric(t_cameraFeature feature, dc1394camera_t* camera, bool bVerbose);
				virtual ~CameraFeatureGeneric();
				
				virtual bool hasFeature(void);
				virtual bool setValue(float value);
				virtual bool getValue(float *value);
				virtual bool getValueRange(float* lower, float* upper);
				virtual bool getAuto(bool* autoMode);
				virtual bool setAuto(bool autoMode);

				private:
				
				bool setMode(dc1394feature_mode_t mode);
				
				dc1394camera_t*			m_camera;							//camera pointer
				dc1394feature_t			m_dc1394FeatureID;					//mapping from t_cameraFeature to implementation-specific feature ID
				dc1394feature_info_t    m_dc1394FeatureData;				//info on the feature (range, status etc). 
			};


			//--------------------------------
			// Class:		CameraFeatureColorMode
			// Description: Special feature for colour modes. 
			// Inherits:	CameraFeature - virtual interface
			class CameraFeatureColorMode : public CameraFeature
			{
				public:
				CameraFeatureColorMode(t_cameraFeature feature, dc1394camera_t** camera, const int *width, const int *height, bool bVerbose);
				virtual ~CameraFeatureColorMode();
				
				virtual bool hasFeature(void);
				virtual bool setValue(float value);
				virtual bool getValue(float *value);
				virtual bool getValueRange(float* lower, float* upper);
				virtual bool getAuto(bool* autoMode);
				virtual bool setAuto(bool autoMode);

				private:
				
				dc1394camera_t**		m_camera;							//camera pointer
				dc1394color_coding_t	m_colorCoding;						//current coding mode
				dc1394color_coding_t	m_desiredColorCoding;				//what coding we will set to once camera initialized
				dc1394video_modes_t		m_availableModes;					//available video modes
				
				const int				*m_currentWidth;					//current resolution of grabber
				const int				*m_currentHeight;
			};
		#endif //OF_VIDEO_CAPTURE_LIBDC1394

};




#endif

