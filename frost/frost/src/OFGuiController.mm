

#import "OFGuiController.h"
#include "testApp.h"

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
	if([p header]){
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



@implementation OFGuiController

@synthesize viewItems,views;



// --------------------------------------------------- awake from the nib file
-(void) awakeFromNib {
	printf("--- wake from nib ---\n");
}

// --------------------------------------------------- init
- (id)init {
	printf("--- init ---\n");	

    if(self = [super init]) {
		
		ofApp = (testApp*)ofGetAppPtr();
		gui = self;
		
		viewItems = [[NSMutableArray alloc] init];			
		
		ofPlugin * inputsHeader =  [[ofPlugin alloc]init];
		[inputsHeader setName:@"Inputs"];
		[inputsHeader setHeader:[NSNumber numberWithBool:TRUE]];
		[viewItems addObject:inputsHeader];

		
		ofPlugin * camera =  [[ofPlugin alloc]init];
		[camera setName:@"Cameras"];
		[camera setEnabled:[NSNumber numberWithBool:TRUE]];
		[viewItems addObject:camera];

		
		ofPlugin * dataHeader =  [[ofPlugin alloc]init];
		[dataHeader setName:@"Cameras"];
		[dataHeader setHeader:[NSNumber numberWithBool:TRUE]];
		[viewItems addObject:dataHeader];

		ofPlugin * outputHeader =  [[ofPlugin alloc]init];
		[outputHeader setName:@"Outputs"];
		[outputHeader setHeader:[NSNumber numberWithBool:TRUE]];
		[viewItems addObject:outputHeader];
		
		
		NSColor * color = [theColor color];
		float r = [color redComponent];
		float g = [color greenComponent];
		float b = [color blueComponent];
		if(ofApp) {
		}
    }
	
    return self;
}


-(IBAction) setListViewRow:(id)sender {
	ofPlugin * p = [viewItems objectAtIndex:[sender selectedRow]];
	int row = [sender selectedRow];
	if(![(NSString*)[p name] compare:@"Cameras"]){
		[contentArea addSubview:cameraView];
	}
}

// --------------------------------------------------- set FPS
-(void) setFPS:(float)framesPerSecond {
	[fpsText setFloatValue:(float)framesPerSecond];
}

// --------------------------------------------------- set the worm mode
-(IBAction) setWormMode:(id)sender {
	///	ofApp->bWorm = [sender state];
}
// --------------------------------------------------- clear the points
-(IBAction) clearDrawing:(id)sender {
	//ofApp->pnts.clear();
}

// --------------------------------------------------- set the line thickness
-(IBAction) setLineThickness:(id)sender {
	//ofApp->lineThickness = [lineThicknessSlider floatValue];	
}

// --------------------------------------------------- set the color
-(IBAction) setTheColor:(id)sender {
	
	NSColor * color = [sender color];
	
	float r = [color redComponent];
	float g = [color greenComponent];
	float b = [color blueComponent];
	
	if(ofApp) {
		//	ofApp->color.r = r * 255.0;
		//	ofApp->color.g = g * 255.0;
		//	ofApp->color.b = b * 255.0;
	}
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

