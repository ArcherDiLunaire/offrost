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
	IBOutlet NSView *moonDustView;
	
	/**
	 * Moon Dust Outlets
	 **/
		
	IBOutlet NSSlider * MoonDustForce;
	IBOutlet NSSlider * MoonDustDamp;
	
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
	
	IBOutlet NSSlider * BlobThreshold;

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
-(IBAction)		setMoonDustDamp:(id)sender;

-(IBAction)		setProjectorShowDebug:(id)sender;
-(IBAction)		setProjectorMatrix:(id)sender;
-(IBAction)		setProjectorFloorAspect:(id)sender;

-(IBAction)		setCameraKeystoneShowDebug:(id)sender;
-(IBAction)		setCameraKeystoneMatrix:(id)sender;

-(IBAction)		setBlobThreshold:(id)sender;


-(void)			awakeFromNib;
-(void)			cameraUpdateGUIDs;

-(id)			init;

@end



extern OFGuiController * gui;