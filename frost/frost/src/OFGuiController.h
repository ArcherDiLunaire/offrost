#import "GLee.h"

#import <Cocoa/Cocoa.h>
#include "testApp.h"
#include "CustomGLView.h"



@interface ofPlugin : NSObject 
{
	NSString * name;
	NSNumber * enabled;
	NSNumber * header;

}
@property (retain, readwrite) NSString *name;
@property (retain, readwrite) NSNumber *enabled;
@property (retain, readwrite) NSNumber *header;
@property (assign, readwrite) NSNumber *settingsViewNumber;



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
	
	NSMutableArray * viewItems;
	NSMutableArray * views;
	
	IBOutlet CustomGLView * camView;
	IBOutlet CustomGLView * projectorView;

}
@property (retain, readwrite) NSMutableArray *viewItems;
@property (retain, readwrite) NSMutableArray *views;


// ------------------------------------------------
- (id)tableView:(NSTableView *)aTableView;

-(IBAction)		toggleFullscreen:(id)sender;

-(void)			setFPS:(float)framesPerSecond;

-(IBAction)		setMoonDustForce:(id)sender;
-(IBAction)		setMoonDustDamp:(id)sender;

-(void)			awakeFromNib;
-(id)			init;
@end




extern OFGuiController * gui;