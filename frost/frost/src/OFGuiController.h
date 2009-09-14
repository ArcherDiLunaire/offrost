#import "GLee.h"

#import <Cocoa/Cocoa.h>
#include "testApp.h"




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
	IBOutlet NSOpenGLView * camView;
	
	IBOutlet NSView *cameraView;
	IBOutlet NSView *blobTrackingView;
	IBOutlet NSView *moonDustView;
	
	IBOutlet NSTextField *	fpsText;
	
	NSMutableArray * viewItems;
	NSMutableArray * views;

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


@interface CameraGlView : NSOpenGLView
{
	testApp * ofApp;
	NSTimer * animationTimer;
}

- (void) drawRect: (NSRect) bounds ;

@end


extern OFGuiController * gui;