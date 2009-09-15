

#import "OFGuiController.h"
#include "testApp.h"
#include "Plugin.h"
#include "PluginController.h"
#include "PluginIncludes.h"

OFGuiController * gui = NULL;

@implementation ofPlugin

@synthesize name, enabled, header;

- (id)init {
	return [super init];
}

@end



@implementation ListView

- (void)drawRect:(NSRect)rect{
	[super drawRect:rect];	
}

- (void)drawRow:(NSInteger)rowIndex clipRect:(NSRect)clipRect{
	ofPlugin * p = [[[self dataSource] viewItems] objectAtIndex:rowIndex];
	if([[p header] isEqualToNumber:[NSNumber numberWithBool:TRUE]]){
		NSRect bounds = [self rectOfRow:rowIndex];
				
		NSBezierPath*    clipShape = [NSBezierPath bezierPathWithRect:bounds];

		
		
		NSGradient* aGradient = [[[NSGradient alloc]
//								  initWithColorsAndLocations:[NSColor colorWithCalibratedRed:89 green:153 blue:229 alpha:1.0], (CGFloat)0.0,
								initWithColorsAndLocations:[NSColor colorWithCalibratedHue:0.59 saturation:0.61 brightness:0.90 alpha:1.0], (CGFloat)0.0,
								  [NSColor colorWithCalibratedHue:0.608 saturation:0.85 brightness:0.81 alpha:1.0], (CGFloat)1.0,
								  nil] autorelease];
		
		[aGradient drawInBezierPath:clipShape angle:90.0];
		
		
		NSMutableParagraphStyle *paragraphStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
		[paragraphStyle setAlignment:NSCenterTextAlignment];

		
		NSDictionary *textAttribs;
		textAttribs = [NSDictionary dictionaryWithObjectsAndKeys: [NSFont fontWithName:@"Lucida Grande" size:12],
					   NSFontAttributeName, [NSColor whiteColor],NSForegroundColorAttributeName,  paragraphStyle, NSParagraphStyleAttributeName, nil];
		
		[[p name] drawInRect:bounds withAttributes:textAttribs];
	
	} else {
		[super drawRow:rowIndex clipRect:clipRect];
	}
}

@end


@implementation contentView : NSView
- (void)drawRect:(NSRect)rect{
	[super drawRect:rect];	
}
@end



@implementation OFGuiController

@synthesize viewItems,views;




// --------------------------------------------------- awake from the nib file
-(void) awakeFromNib {
	printf("--- wake from nib ---\n");

}

- (void)addObject:(NSString*)objname isheader:(bool)header {
	ofPlugin * obj =  [[ofPlugin alloc]init];
	[obj setName:objname];
	[obj setHeader:[NSNumber numberWithBool:header]];
	[obj setEnabled:[NSNumber numberWithBool:TRUE]];
	[viewItems addObject:obj];
	
}

// --------------------------------------------------- init
- (id)init {
	printf("--- init ---\n");	

    if(self = [super init]) {
		userDefaults = [[NSUserDefaults standardUserDefaults] retain];
		

		
		ofApp = (testApp*)ofGetAppPtr();
		
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->damp = [userDefaults doubleForKey:@"moondust.damp"];
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->force = [userDefaults doubleForKey:@"moondust.force"];

		gui = self;
		
		viewItems = [[NSMutableArray alloc] init];			
		
		
		[self addObject:@"Inputs" isheader:TRUE];
		[self addObject:@"Cameras" isheader:FALSE];
		[self addObject:@"Blob Tracking" isheader:FALSE];

		[self addObject:@"Inputs" isheader:TRUE];		
		
		[self addObject:@"Outputs" isheader:TRUE];		
		[self addObject:@"Moon Dust" isheader:FALSE];

		
		[blobTrackingView retain];
		[cameraView retain];
		[moonDustView retain];
		
    }
	
    return self;
}


-(IBAction) setListViewRow:(id)sender {
	ofPlugin * p = [viewItems objectAtIndex:[sender selectedRow]];
	int row = [sender selectedRow];
	NSEnumerator *enumerator = [[contentArea subviews] objectEnumerator];
	id anObject;
	
	while (anObject = [enumerator nextObject]) {
		[anObject retain];
		[anObject removeFromSuperview];
	}
	
	id view;
	if(![(NSString*)[p name] compare:@"Cameras"]){
		view = cameraView;
	}
	if(![(NSString*)[p name] compare:@"Blob Tracking"]){
		view = blobTrackingView;
	}
	if(![(NSString*)[p name] compare:@"Moon Dust"]){
		view = moonDustView;
	}	
	

	
	[contentArea addSubview:view];
	NSRect currFrame = [contentArea frame];
	CGFloat h = currFrame.size.height;

	NSRect currFrame2 = [view frame];
	CGFloat h2 = currFrame2.size.height;
	
	[view setFrameOrigin:NSMakePoint(0,h-h2)]; 
	
}
	


-(IBAction)		toggleFullscreen:(id)sender{
	ofToggleFullscreen();
}


- (id)tableView:(NSTableView *)aTableView
objectValueForTableColumn:(NSTableColumn *)aTableColumn
			row:(int)rowIndex
{
	NSMutableArray * array;
	int i;
	if(rowIndex < [viewItems count]){
		array = viewItems;
		i = rowIndex;
	}
	   
	ofPlugin * p = [array objectAtIndex:i];
	if(![(NSString*)[aTableColumn identifier] compare:@"name"]){
		return [p name];		
	} else if(![(NSString*)[aTableColumn identifier] compare:@"enable"]){
		return [p enabled];
	} else {
		return @"hmm?";
	}

}


- (void)tableView:(NSTableView *)aTableView
   setObjectValue:anObject
   forTableColumn:(NSTableColumn *)aTableColumn
			  row:(int)rowIndex
{
	NSMutableArray * array;
	int i;
	if(rowIndex < [viewItems count]){
		array = viewItems;
		i = rowIndex;
	}
	
	ofPlugin * p = [array objectAtIndex:i];
	
	if(![(NSString*)[aTableColumn identifier] compare:@"name"]){
	} else if(![(NSString*)[aTableColumn identifier] compare:@"enable"]){
		[p setEnabled:anObject];		

	}  
	return;
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return [viewItems count];
}

-(IBAction)	setMoonDustForce:(id)sender{
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->force = [sender doubleValue];
	}
}

-(IBAction)	setMoonDustDamp:(id)sender{
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->damp = [sender doubleValue];
	}
}

@end


@implementation CameraGlView
- (void) awakeFromNib {
	ofApp = (testApp*)ofGetAppPtr();
	
	CGLGetPixelFormat(CGLGetCurrentContext());
	
	
	
}

- (void) drawRect: (NSRect) bounds{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	unsigned char * p  = ofApp->vidGrabber->getPixels();
//	ofApp->vidGrabber->draw(0, 0);
	glFlush();
	
	if ( ! animationTimer )
		animationTimer=[ [ NSTimer scheduledTimerWithTimeInterval:0.17 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES ] retain ] ;

}

/**
 * Service the animation timer.
 */
- (void) animationTimerFired: (NSTimer *) timer
{
	[ self setNeedsDisplay: YES ] ;
}



@end

