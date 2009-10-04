#import "GLee.h"

#import <Cocoa/Cocoa.h>
#include "testApp.h"
#include "CustomGLView.h"
#include "Plugin.h"
#include "PluginIncludes.h"


@interface frostSlider : NSSlider
{
	NSSlider * valSlider;
	NSTextField * valTextfield;
	SEL myAction;
	NSObject * myTarget;
	
	int midiChannel;
	int midiNumber;
	bool midiControlHookup;
	bool midiNoteHookup;
	float midiScaleFactor;
}

- (void) receiveMidiOnChannel:(int)channel number:(int)number control:(bool)control noteOn:(bool)noteOn noteOff:(bool)noteOff value:(int)value;

- (void) setMidiChannel:(int)channel number:(int)number control:(bool)control note:(bool)note scale:(float)scale;
- (id) initWithFrame:(NSRect)frame;
- (void) awakeFromNib;
- (void) changeValueFromControl:(id)sender;

@end


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
@public
	
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
	
	IBOutlet NSSlider * BlobLightHistoryAlpha;
	IBOutlet NSSlider * BlobLightBlobAlpha;
	IBOutlet NSSlider * BlobLightAddBackground;
	
	IBOutlet NSColorWell * BlobLightColor;
	
	/**
	 * Folding outlets
	 **/
	
	IBOutlet NSView *foldingView;
	
	/**
	 * Frostscape outlets
	 **/
	
	IBOutlet NSView *frostscapeView;
	IBOutlet frostSlider * FrostScapeSlider1;
	IBOutlet frostSlider * FrostScapeSlider2;
	IBOutlet frostSlider * FrostScapeSlider3;
	IBOutlet frostSlider * FrostScapeSlider4;
	IBOutlet frostSlider * FrostScapeSlider5;
	IBOutlet frostSlider * FrostScapeSlider6;
	
	/**
	 * LiquidSpace outlets
	 **/
	
	IBOutlet NSView *liquidSpaceView;
	
	IBOutlet NSButton * liquidSpaceFIll;
	IBOutlet NSColorWell * liquidSpaceFillColor;
	IBOutlet NSSlider * liquidSpaceFadeSpeed;
	IBOutlet NSSlider * liquidSpaceViscosity;
	IBOutlet NSSlider * liquidSpaceDiffusion;
	IBOutlet NSColorWell * liquidSpaceDropColor;
	IBOutlet NSButton * liquidSpaceAddingColor;
	IBOutlet NSSlider * liquidSpaceColorMultiplier;
	IBOutlet NSButton * liquidSpaceAddingForce;
	IBOutlet NSSlider * liquidSpaceForceMultiplier;
	
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

-(IBAction) setListViewRow:(id)sender;


// ------------------------------------------------
- (id)tableView:(NSTableView *)aTableView;

-(IBAction)		toggleFullscreen:(id)sender;

-(void)			setFPS:(float)framesPerSecond;

-(IBAction)		cameraBindGuid1:(id)sender;
-(IBAction)		cameraBindGuid2:(id)sender;
-(IBAction)		cameraBindGuid3:(id)sender;

-(IBAction)		modifyMoonDustForce:(id)sender;
-(IBAction)		modifyMoonDustLength:(id)sender;
-(IBAction)		modifyMoonDustSize:(id)sender;
-(IBAction)		modifyMoonDustDensity:(id)sender;
-(IBAction)		modifyMoonDustDebug:(id)sender;

-(IBAction)		modifyLaLineaDebug:(id)sender;


-(IBAction)		modifyBlobLightDebug:(id)sender;
-(IBAction)		modifyBlobLightColor:(id)sender;
-(IBAction)		modifyBlobLightColor2:(id)sender;
-(IBAction)		modifyBlobLightBlur:(id)sender;
-(IBAction)		modifyBlobLightThreshold:(id)sender;
-(IBAction)		modifyBlobLightBlur2:(id)sender;
-(IBAction)		modifyBlobLightAlpha:(id)sender;
-(IBAction)		modifyBlobLightBeta:(id)sender;
-(IBAction)		modifyBlobLightHistoryAlpha:(id)sender;
-(IBAction)		modifyBlobLightBlobAlpha:(id)sender;
-(IBAction)		modifyBlobLightBackgroundAdd:(id)sender;
-(IBAction)		modifyBlobLightBackgroundClear:(id)sender;

-(IBAction)		modifyFrostscapeSlider1:(id)sender;
-(IBAction)		modifyFrostscapeSlider2:(id)sender;
-(IBAction)		modifyFrostscapeSlider3:(id)sender;
-(IBAction)		modifyFrostscapeSlider4:(id)sender;
-(IBAction)		modifyFrostscapeSlider5:(id)sender;
-(IBAction)		modifyFrostscapeSlider6:(id)sender;

-(IBAction)		modifyLiquidSpaceFillColor:(id)sender;
-(IBAction)		modifylLiquidSpace:(id)sender;
-(IBAction)		modifyLiquidSpaceFadeSpeed:(id)sender;
-(IBAction)		modifyLiquidSpaceViscosity:(id)sender;
-(IBAction)		modifyLiquidSpaceDiffusion:(id)sender;
-(IBAction)		modifyLiquidSpaceDropColor:(id)sender;
-(IBAction)		modifyLiquidSpaceAddingColor:(id)sender;
-(IBAction)		modifyLiquidSpaceColorMultiplier:(id)sender;
-(IBAction)		modifyLiquidSpaceAddingForce:(id)sender;
-(IBAction)		modifyLiquidSpaceForceMultiplier:(id)sender;

-(IBAction)		modifyProjectorShowDebug:(id)sender;
-(IBAction)		modifyProjectorMatrix:(id)sender;
-(IBAction)		modifyProjectorFloorAspect:(id)sender;

-(IBAction)		modifyCameraKeystoneShowDebug:(id)sender;
-(IBAction)		modifyCameraKeystoneMatrix:(id)sender;

-(IBAction)		modifyBlobThreshold1:(id)sender;
-(IBAction)		modifyBlobBlur1:(id)sender;
-(IBAction)		blobGrab1:(id)sender;
-(IBAction)		modifyBlobActive1:(id)sender;
-(IBAction)		modifyBlobThreshold2:(id)sender;
-(IBAction)		modifyBlobBlur2:(id)sender;
-(IBAction)		blobGrab2:(id)sender;
-(IBAction)		modifyBlobActive2:(id)sender;
-(IBAction)		modifyBlobThreshold3:(id)sender;
-(IBAction)		modifyBlobBlur3:(id)sender;
-(IBAction)		blobGrab3:(id)sender;
-(IBAction)		modifyBlobActive3:(id)sender;
-(IBAction)		drawBlobDebug:(id)sender;


-(void)			awakeFromNib;
-(void)			cameraUpdateGUIDs;

-(id)			init;


@end



extern OFGuiController * gui;