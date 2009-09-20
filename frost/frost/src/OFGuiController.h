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
//	IBOutlet NSOpenGLView * camView;
	
	IBOutlet NSView *cameraView;
	IBOutlet NSView *blobTrackingView;
	IBOutlet NSView *projectionSurfacesView;

	IBOutlet NSView *moonDustView;
	
	NSUserDefaults * userDefaults;
	
	IBOutlet NSSlider * MoonDustForce;
	IBOutlet NSSlider * MoonDustDamp;
	
	IBOutlet NSButton * ProjectorShowDebug;
	IBOutlet NSMatrix * ProjectorMatrix;
	IBOutlet NSSlider * ProjectorFloorAspect;
	IBOutlet NSTextField * ProjectorFloorAspectText;

	IBOutlet NSSlider * BlobThreshold;



	
	NSMutableArray * viewItems;
	NSMutableArray * views;
	
	IBOutlet CustomGLView * camView;
	IBOutlet CustomGLView * projectorView;
	IBOutlet CustomGLView * blobView;

	IBOutlet CustomGLView * floorPreview;
	

}
@property (retain, readwrite) NSMutableArray *viewItems;
@property (retain, readwrite) NSMutableArray *views;


// ------------------------------------------------
- (id)tableView:(NSTableView *)aTableView;

-(IBAction)		toggleFullscreen:(id)sender;

-(void)			setFPS:(float)framesPerSecond;

-(IBAction)		setMoonDustForce:(id)sender;
-(IBAction)		setMoonDustDamp:(id)sender;

-(IBAction)		setProjectorShowDebug:(id)sender;
-(IBAction)		setProjectorMatrix:(id)sender;
-(IBAction)		setProjectorFloorAspect:(id)sender;

-(IBAction)		setBlobThreshold:(id)sender;


-(void)			awakeFromNib;
-(id)			init;
@end




extern OFGuiController * gui;