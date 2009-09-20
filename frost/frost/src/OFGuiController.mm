

#import "OFGuiController.h"
#include "testApp.h"
#include "Plugin.h"
#include "PluginController.h"
#include "PluginIncludes.h"

OFGuiController * gui = NULL;



@implementation ofPlugin

@synthesize name, enabled, header, plugin;

- (id)init {
	return [super init];
}

- (void) setEnabled:(NSNumber *) n 
{
	enabled = n;
	if(plugin != nil){
	plugin->enabled = [n boolValue];
	}
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
	[camView setWindowId:1];
	[projectorView setWindowId:2];
	[blobView setWindowId:3];
	
	[floorPreview setWindowId:4];
	[floorPreview setDoDraw:TRUE];
	
	[ProjectorFloorAspectText setStringValue:[[NSNumber numberWithFloat:((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->floorAspect] stringValue]];

}

- (void)addObject:(NSString*)objname isheader:(bool)header plugin:(FrostPlugin*)p {
	ofPlugin * obj =  [[ofPlugin alloc]init];
	[obj setName:objname];
	[obj setHeader:[NSNumber numberWithBool:header]];
	[obj setPlugin:p];
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

		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->threshold = [userDefaults doubleForKey:@"blob.threshold1"];
		

		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->drawDebug = [userDefaults doubleForKey:@"projectionsurfaces.drawdebug"];		
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->floorAspect = [userDefaults doubleForKey:@"projectionsurfaces.flooraspect"];	

		printf("Aspect: %f",[userDefaults doubleForKey:@"projectionsurfaces.flooraspect"]);
		
		gui = self;
		
		viewItems = [[NSMutableArray alloc] init];			
		
		
		[self addObject:@"Inputs" isheader:TRUE plugin:nil];
		[self addObject:@"Cameras" isheader:FALSE plugin:getPlugin<Cameras*>(ofApp->pluginController)];
		[self addObject:@"Blob Tracking" isheader:FALSE plugin:getPlugin<BlobTracking*>(ofApp->pluginController)];
		
		[self addObject:@"Data" isheader:TRUE  plugin:nil];		
		[self addObject:@"Projection Surfaces" isheader:FALSE plugin:getPlugin<ProjectionSurfaces*>(ofApp->pluginController)];
		
		[self addObject:@"Outputs" isheader:TRUE  plugin:nil];		
		[self addObject:@"Moon Dust" isheader:FALSE plugin:getPlugin<MoonDust*>(ofApp->pluginController)];
		
		
		[blobTrackingView retain];
		[cameraView retain];
		[projectionSurfacesView retain];
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
	
	[camView setDoDraw:false];
	[projectorView setDoDraw:false];
	[blobView setDoDraw:false];
	
	id view;
	if(![(NSString*)[p name] compare:@"Cameras"]){
		view = cameraView;
		[camView setDoDraw:true];
	}
	if(![(NSString*)[p name] compare:@"Blob Tracking"]){
		view = blobTrackingView;
		[blobView setDoDraw:true];
	}
	if(![(NSString*)[p name] compare:@"Projection Surfaces"]){
		view = projectionSurfacesView;
		[projectorView setDoDraw:true];
		
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

-(IBAction)		setProjectorShowDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->drawDebug = b;
	}
}	

-(IBAction)		setProjectorMatrix:(id)sender {
	if(ofApp->setupCalled){
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->selectedKeystoner = [sender selectedRow];
		if([sender selectedRow] == 0){
			((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->w = ((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->glDelegate->m_Width/3.0;
			((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->h = ((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->glDelegate->m_Width/3.0;
		} else {
			((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->w = ((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->glDelegate->m_Width/1.50;
			((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->h = ((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->glDelegate->m_Width/1.5;
		}
	}
}


-(IBAction)		setProjectorFloorAspect:(id)sender{
	if(ofApp->setupCalled){
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->floorAspect = [sender doubleValue];
		[ProjectorFloorAspectText setStringValue:[sender stringValue]];
	}
}


-(IBAction)	setBlobThreshold:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->threshold = [sender doubleValue];
	}
}

@end




