#import "GLee.h"

#import <Cocoa/Cocoa.h>
#include "testApp.h"
#include "CustomGLView.h"
#include "Plugin.h"
#include "PluginIncludes.h"


@interface ofPlugin : NSObject 
{
	NSString * name;
	NSNumber * enabled;
	NSNumber * header;
	
	FrostPlugin * plugin;
	
}
@property (retain, readwrite) NSString *name;
@property (retain, readwrite) NSNumber *enabled;
@property (retain, readwrite) NSNumber *header;
@property (assign, readwrite) NSNumber *settingsViewNumber;
@property (assign, readwrite)FrostPlugin * plugin;



@end


@interface ListView : NSTableView
{
}
- (void)drawRect:(NSRect)rect;
- (void)drawRow:(NSInteger)rowIndex clipRect:(NSRect)clipRect;
@end


@interface contentView : NSView
{
}
- (void)drawRect:(NSRect)rect;
@end




@interface OFGuiController : NSObject {
	
	testApp * ofApp;
	
	IBOutlet NSView * mainView;	
	IBOutlet contentView * contentArea;
	IBOutlet ListView * listView;
	
	NSMutableArray * viewItems;
	NSMutableArray * views;
	
	NSUserDefaults * userDefaults;
	
	
	/**
	 * Plugin View Outlets
	 **/
	
	IBOutlet NSView *cameraView;
	IBOutlet NSView *blobTrackingView;
	IBOutlet NSView *projectionSurfacesView;
	IBOutlet NSView *cameraKeystoneView;
	
	/**
	 * Moon Dust Outlets
	 **/
	IBOutlet NSView *moonDustView;
	
	IBOutlet NSSlider * MoonDustForce;
	IBOutlet NSSlider * MoonDustLength;
	IBOutlet NSSlider * MoonDustDensity;	
	IBOutlet NSSlider * MoonDustSize;
	IBOutlet NSButton * MoonDustDebug;
	
	/**
	 * La Linea Outlets
	 **/
	IBOutlet NSView *laLineaView;
	IBOutlet NSButton * LaLineaDebug;

	
	
	/**
	 * Blob Light Outlets
	 **/
	IBOutlet NSView *blobLightView;
	IBOutlet NSButton * BlobLightDebug;
	IBOutlet NSSlider * BlobLightBlur;
	IBOutlet NSSlider * BlobLightThreshold;
	IBOutlet NSSlider * BlobLightBlur2;
	IBOutlet NSSlider * BlobLightAlpha;
	IBOutlet NSSlider * BlobLightBeta;

	IBOutlet NSColorWell * BlobLightColor;
	
	/**
	 * Camera Outlets
	 **/
	
	IBOutlet NSView * cameraSetting;
	
	IBOutlet NSView * cameraSettings1;
	IBOutlet NSView * cameraSettings2;
	IBOutlet NSView * cameraSettings3;
	
	IBOutlet NSTextField * CameraGUID1;
	IBOutlet NSTextField * CameraGUID2;
	IBOutlet NSTextField * CameraGUID3;
	
	/**
	 * Projector Outlets
	 **/
	
	IBOutlet NSButton * ProjectorShowDebug;
	IBOutlet NSMatrix * ProjectorMatrix;
	IBOutlet NSSlider * ProjectorFloorAspect;
	IBOutlet NSTextField * ProjectorFloorAspectText;
	
	/**
	 * Blob Detection Outlets
	 **/
	
	IBOutlet NSSlider * BlobThreshold1;
	IBOutlet NSSlider * BlobBlur1;
	IBOutlet NSButton * BlobActive1;
	IBOutlet NSSlider * BlobThreshold2;
	IBOutlet NSSlider * BlobBlur2;
	IBOutlet NSButton * BlobActive2;
	IBOutlet NSSlider * BlobThreshold3;
	IBOutlet NSSlider * BlobBlur3;
	IBOutlet NSButton * BlobActive3;
	
	/**
	 * OpenGL Views
	 **/
	
	IBOutlet CustomGLView * camView;
	IBOutlet CustomGLView * projectorView;
	IBOutlet CustomGLView * cameraKeystoneOpenGlView;
	IBOutlet CustomGLView * blobView;
	IBOutlet CustomGLView * floorPreview;
	
	
}
@property (retain, readwrite) NSMutableArray *viewItems;
@property (retain, readwrite) NSMutableArray *views;


// ------------------------------------------------
- (id)tableView:(NSTableView *)aTableView;

-(IBAction)		toggleFullscreen:(id)sender;

-(void)			setFPS:(float)framesPerSecond;

-(IBAction)		cameraBindGuid1:(id)sender;
-(IBAction)		cameraBindGuid2:(id)sender;
-(IBAction)		cameraBindGuid3:(id)sender;

-(IBAction)		setMoonDustForce:(id)sender;
-(IBAction)		setMoonDustLength:(id)sender;
-(IBAction)		setMoonDustSize:(id)sender;
-(IBAction)		setMoonDustDensity:(id)sender;
-(IBAction)		setMoonDustDebug:(id)sender;

-(IBAction)		setLaLineaDebug:(id)sender;


-(IBAction)		setBlobLightDebug:(id)sender;
-(IBAction)		setBlobLightColor:(id)sender;
-(IBAction)		setBlobLightBlur:(id)sender;
-(IBAction)		setBlobLightThreshold:(id)sender;
-(IBAction)		setBlobLightBlur2:(id)sender;
-(IBAction)		setBlobLightAlpha:(id)sender;
-(IBAction)		setBlobLightBeta:(id)sender;


-(IBAction)		setProjectorShowDebug:(id)sender;
-(IBAction)		setProjectorMatrix:(id)sender;
-(IBAction)		setProjectorFloorAspect:(id)sender;

-(IBAction)		setCameraKeystoneShowDebug:(id)sender;
-(IBAction)		setCameraKeystoneMatrix:(id)sender;

-(IBAction)		setBlobThreshold1:(id)sender;
-(IBAction)		setBlobBlur1:(id)sender;
-(IBAction)		blobGrab1:(id)sender;
-(IBAction)		setBlobActive1:(id)sender;
-(IBAction)		setBlobThreshold2:(id)sender;
-(IBAction)		setBlobBlur2:(id)sender;
-(IBAction)		blobGrab2:(id)sender;
-(IBAction)		setBlobActive2:(id)sender;
-(IBAction)		setBlobThreshold3:(id)sender;
-(IBAction)		setBlobBlur3:(id)sender;
-(IBAction)		blobGrab3:(id)sender;
-(IBAction)		setBlobActive3:(id)sender;
-(IBAction)		drawBlobDebug:(id)sender;


-(void)			awakeFromNib;
-(void)			cameraUpdateGUIDs;

-(id)			init;

@end



extern OFGuiController * gui;