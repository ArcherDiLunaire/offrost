

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
	[cameraKeystoneOpenGlView setWindowId:5];
	[blobView setWindowId:3];
	
	[floorPreview setWindowId:4];
	[floorPreview setDoDraw:TRUE];
	
	[ProjectorFloorAspectText setStringValue:[[NSNumber numberWithFloat:((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->objects[0]->aspect] stringValue]];
	
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
		
		(getPlugin<MoonDust*>(ofApp->pluginController))->density = [userDefaults doubleForKey:@"moondust.density"];
		(getPlugin<MoonDust*>(ofApp->pluginController))->force = [userDefaults doubleForKey:@"moondust.force"];
		(getPlugin<MoonDust*>(ofApp->pluginController))->size = [userDefaults doubleForKey:@"moondust.size"];
		(getPlugin<MoonDust*>(ofApp->pluginController))->length = [userDefaults doubleForKey:@"moondust.length"];
		(getPlugin<MoonDust*>(ofApp->pluginController))->debug = [userDefaults boolForKey:@"moondust.density"];
		
		(getPlugin<LaLinea*>(ofApp->pluginController))->debug = [userDefaults boolForKey:@"lalinea.density"];
		
		
		(getPlugin<BlobLight*>(ofApp->pluginController))->debug = [userDefaults boolForKey:@"bloblight.density"];
		(getPlugin<BlobLight*>(ofApp->pluginController))->blur = [userDefaults doubleForKey:@"bloblight.blur"];
		(getPlugin<BlobLight*>(ofApp->pluginController))->threshold = [userDefaults doubleForKey:@"bloblight.threshold"];
		(getPlugin<BlobLight*>(ofApp->pluginController))->blur2 = [userDefaults doubleForKey:@"bloblight.blur2"];
		(getPlugin<BlobLight*>(ofApp->pluginController))->alpha = [userDefaults doubleForKey:@"bloblight.alpha"];
		(getPlugin<BlobLight*>(ofApp->pluginController))->beta = [userDefaults doubleForKey:@"bloblight.beta"];
		(getPlugin<BlobLight*>(ofApp->pluginController))->historyalpha = [userDefaults doubleForKey:@"bloblight.historya"];
		(getPlugin<BlobLight*>(ofApp->pluginController))->blobalpha = [userDefaults doubleForKey:@"bloblight.bloba"];
		(getPlugin<BlobLight*>(ofApp->pluginController))->addblack = [userDefaults doubleForKey:@"bloblight.addblack"];
		
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider1([userDefaults doubleForKey:@"frostscape.val1"]);
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider2([userDefaults doubleForKey:@"frostscape.val2"]);
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider3([userDefaults doubleForKey:@"frostscape.val3"]);
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider4([userDefaults doubleForKey:@"frostscape.val4"]);
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider5([userDefaults doubleForKey:@"frostscape.val5"]);
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider6([userDefaults doubleForKey:@"frostscape.val6"]);
		
		
		(getPlugin<BlobTracking*>(ofApp->pluginController))->drawDebug = [userDefaults doubleForKey:@"blob.drawdebug"];	
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(0,[userDefaults doubleForKey:@"blob.threshold1"]);
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(0,[userDefaults doubleForKey:@"blob.blur1"]);
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(0,[userDefaults boolForKey:@"blob.active1"]);
		
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(1,[userDefaults doubleForKey:@"blob.threshold2"]);
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(1,[userDefaults doubleForKey:@"blob.blur2"]);
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(1,[userDefaults boolForKey:@"blob.active2"]);
		
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(2,[userDefaults doubleForKey:@"blob.threshold3"]);
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(2,[userDefaults doubleForKey:@"blob.blur3"]);
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(2,[userDefaults boolForKey:@"blob.active3"]);
		
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->drawDebug = [userDefaults doubleForKey:@"projectionsurfaces.drawdebug"];		
		
		
		
		gui = self;
		
		viewItems = [[NSMutableArray alloc] init];			
		
		
		[self addObject:@"Inputs" isheader:TRUE plugin:nil];
		[self addObject:@"Cameras" isheader:FALSE plugin:getPlugin<Cameras*>(ofApp->pluginController)];
		[self addObject:@"Blob Tracking" isheader:FALSE plugin:getPlugin<BlobTracking*>(ofApp->pluginController)];
		
		[self addObject:@"Data" isheader:TRUE  plugin:nil];		
		[self addObject:@"Projection Surfaces" isheader:FALSE plugin:getPlugin<ProjectionSurfaces*>(ofApp->pluginController)];
		[self addObject:@"Camera Calibration" isheader:FALSE plugin:getPlugin<CameraCalibration*>(ofApp->pluginController)];
		
		[self addObject:@"Outputs" isheader:TRUE  plugin:nil];		
		[self addObject:@"Moon Dust" isheader:FALSE plugin:getPlugin<MoonDust*>(ofApp->pluginController)];
		[self addObject:@"La Linea" isheader:FALSE plugin:getPlugin<LaLinea*>(ofApp->pluginController)];
		[self addObject:@"Blob light" isheader:FALSE plugin:getPlugin<BlobLight*>(ofApp->pluginController)];
		[self addObject:@"Folding" isheader:FALSE plugin:getPlugin<Folding*>(ofApp->pluginController)];
		[self addObject:@"Frostscape" isheader:FALSE plugin:getPlugin<Frostscape*>(ofApp->pluginController)];
		[self addObject:@"Liquid Space" isheader:FALSE plugin:getPlugin<LiquidSpace*>(ofApp->pluginController)];
		
		
		NSMutableArray * array;
		array = viewItems;
		for(int i=0;i<[viewItems count];i++){
			ofPlugin * p = [array objectAtIndex:i];
			[p setEnabled:[NSNumber numberWithBool:[userDefaults boolForKey:[NSString stringWithFormat:@"plugins.enable%d",i]]]];
		}
		
		
		[blobTrackingView retain];
		[cameraView retain];
		[projectionSurfacesView retain];
		[cameraKeystoneView retain];
		[moonDustView retain];
		[laLineaView retain];
		[blobLightView retain];
		[frostscapeView retain];
		[liquidSpaceView retain];
		
		uint64_t guidVal[3];
		
		for (int i=0; i<3; i++) {
			guidVal[i] = 0x0ll;
		}
		
		if ([userDefaults stringForKey:@"camera.1.guid"] != nil) {
			sscanf([[userDefaults stringForKey:@"camera.1.guid"] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal[0]);
		}
		
		if ([userDefaults stringForKey:@"camera.2.guid"] != nil) {
			sscanf([[userDefaults stringForKey:@"camera.2.guid"] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal[1]);
		}
		
		if ([userDefaults stringForKey:@"camera.3.guid"] != nil) {
			sscanf([[userDefaults stringForKey:@"camera.3.guid"] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal[2]);
		}
		
		(getPlugin<Cameras*>(ofApp->pluginController))->setGUIDs((uint64_t)guidVal[0],(uint64_t)guidVal[1],(uint64_t)guidVal[2]);
		
		[self cameraUpdateGUIDs];
		
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
	[cameraKeystoneOpenGlView setDoDraw:false];
	[cameraKeystoneOpenGlView setDoDraw:false];
	
	[blobView setDoDraw:false];
	
	id view;
	if(![(NSString*)[p name] compare:@"Cameras"]){
		view = cameraView;
		
		NSView * cameraSettingEx1 = cameraSetting;
		[cameraSettingEx1 setFrameOrigin: NSPointFromString(@"{10, 50}")];
		[cameraSettings1 addSubview:cameraSettingEx1];
		
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
	if(![(NSString*)[p name] compare:@"Camera Calibration"]){
		view =cameraKeystoneView;
		[cameraKeystoneOpenGlView setDoDraw:true];
		
	}	
	
	if(![(NSString*)[p name] compare:@"Moon Dust"]){
		view = moonDustView;
	}	
	if(![(NSString*)[p name] compare:@"La Linea"]){
		view = laLineaView;
	}	
	if(![(NSString*)[p name] compare:@"Blob light"]){
		view = blobLightView;
	}	
	
	if(![(NSString*)[p name] compare:@"Folding"]){
		view = foldingView;
	}	
	
	if(![(NSString*)[p name] compare:@"Frostscape"]){
		view = frostscapeView;
	}	
	
	if(![(NSString*)[p name] compare:@"Liquid Space"]){
		view = liquidSpaceView;
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
		[userDefaults setValue:[p enabled] forKey:[NSString stringWithFormat:@"plugins.enable%d",i]];
		
		
	}  
	return;
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return [viewItems count];
}

-(IBAction)		cameraBindGuid1:(id)sender{
	if(ofApp->setupCalled){
		uint64_t guidVal;
		sscanf([[CameraGUID1 stringValue] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal);
		(getPlugin<Cameras*>(ofApp->pluginController))->setGUID(0, (uint64_t)guidVal);
		[self cameraUpdateGUIDs];
	}
}

-(IBAction)		cameraBindGuid2:(id)sender{
	if(ofApp->setupCalled){
		uint64_t guidVal;
		sscanf([[CameraGUID2 stringValue] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal);
		(getPlugin<Cameras*>(ofApp->pluginController))->setGUID(1, (uint64_t)guidVal);
		[self cameraUpdateGUIDs];
	}
}

-(IBAction)		cameraBindGuid3:(id)sender{
	if(ofApp->setupCalled){
		uint64_t guidVal;
		sscanf([[CameraGUID3 stringValue] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal);
		(getPlugin<Cameras*>(ofApp->pluginController))->setGUID(2, (uint64_t)guidVal);
		[self cameraUpdateGUIDs];
	}
}



-(IBAction)	setMoonDustForce:(id)sender{
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->force = [sender doubleValue];
	}
}

-(IBAction)		setMoonDustLength:(id)sender {
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->length = [sender doubleValue];
	}
}
-(IBAction)		setMoonDustSize:(id)sender {
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->size = [sender doubleValue];
	}
}
-(IBAction)		setMoonDustDensity:(id)sender {
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->density = [sender doubleValue];
	}
}
-(IBAction)		setMoonDustDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<MoonDust*>(ofApp->pluginController))->debug = b;
	}
}	

-(IBAction)		setLaLineaDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<LaLinea*>(ofApp->pluginController))->debug = b;
	}
}


-(IBAction)		setBlobLightDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<BlobLight*>(ofApp->pluginController))->debug = b;
	}
}

-(IBAction)		setBlobLightColor:(id)sender{	
	if(ofApp->setupCalled){
		
		(getPlugin<BlobLight*>(ofApp->pluginController))->r  = [[sender color] redComponent]*255;
		(getPlugin<BlobLight*>(ofApp->pluginController))->g  = [[sender color] greenComponent]*255;
		(getPlugin<BlobLight*>(ofApp->pluginController))->b  = [[sender color] blueComponent]*255;
	}
}
-(IBAction)		setBlobLightColor2:(id)sender{	
	if(ofApp->setupCalled){
		
		(getPlugin<BlobLight*>(ofApp->pluginController))->r2  = [[sender color] redComponent]*255;
		(getPlugin<BlobLight*>(ofApp->pluginController))->g2  = [[sender color] greenComponent]*255;
		(getPlugin<BlobLight*>(ofApp->pluginController))->b2  = [[sender color] blueComponent]*255;
	}
}
-(IBAction)		setBlobLightBlur:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->blur = [sender doubleValue];
	}
}

-(IBAction)		setBlobLightThreshold:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->threshold = [sender doubleValue];
	}
}

-(IBAction)		setBlobLightBlur2:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->blur2 = [sender doubleValue];
	}
}
-(IBAction)		setBlobLightAlpha:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->alpha = [sender doubleValue];
	}
}
-(IBAction)		setBlobLightBeta:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->beta = [sender doubleValue];
	}
}
-(IBAction)		setBlobLightHistoryAlpha:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->historyalpha = [sender doubleValue];
	}
}

-(IBAction)		setBlobLightBlobAlpha:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->blobalpha = [sender doubleValue];
	}
}
-(IBAction)		setBlobLightBackgroundAdd:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->addblack = [sender doubleValue];
	}
}
-(IBAction)		setBlobLightBackgroundClear:(id)sender{
	(getPlugin<BlobLight*>(ofApp->pluginController))->history.set(0);
}

-(IBAction)		setFrostscapeSlider1:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider1([sender doubleValue]);
	}
}

-(IBAction)		setFrostscapeSlider2:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider2([sender doubleValue]);
	}
}
-(IBAction)		setFrostscapeSlider3:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider3([sender doubleValue]);
	}
}
-(IBAction)		setFrostscapeSlider4:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider4([sender doubleValue]);
	}
}
-(IBAction)		setFrostscapeSlider5:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider5([sender doubleValue]);
	}
}
-(IBAction)		setFrostscapeSlider6:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider6([sender doubleValue]);
	}
}



-(IBAction)		setLiquidSpaceFillColor:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fillColor.set([[sender color] redComponent],[[sender color] greenComponent], [[sender color] blueComponent]);
	}
}

-(IBAction)		fillLiquidSpace:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fill();
	}
}

-(IBAction)		setLiquidSpaceFadeSpeed:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidSolver.setFadeSpeed(0.00000001 * [sender doubleValue]);
	}
}

-(IBAction)		setLiquidSpaceViscosity:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidSolver.setVisc(0.00000002 * [sender doubleValue]);
	}
}

-(IBAction)		setLiquidSpaceDiffusion:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidSolver.setColorDiffusion(0.00000002 * [sender doubleValue]);
	}
}

-(IBAction)		setLiquidSpaceDropColor:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->dropColor.set([[sender color] redComponent],[[sender color] greenComponent], [[sender color] blueComponent]);
	}
}

-(IBAction)		setLiquidSpaceAddingColor:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->addingColor = ;
	}
}

-(IBAction)		setLiquidSpaceAddingColor:(id)sender;
-(IBAction)		setLiquidSpaceColorMultiplier:(id)sender;
-(IBAction)		setLiquidSpaceAddingForce:(id)sender;
-(IBAction)		setLiquidSpaceForceMultiplier:(id)sender;



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
		//		[ProjectorFloorAspect setFloatValue:((*getPlugin<ProjectionSurfaces*>(ofApp->pluginController)->objects[[sender selectedRow]]->aspect))];
		[ProjectorFloorAspect setMinValue:0.5];
		[ProjectorFloorAspectText setDoubleValue:[ProjectorFloorAspect doubleValue]];
		
	}
}


-(IBAction)		setProjectorFloorAspect:(id)sender{
	if(ofApp->setupCalled){
		(((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->objects[((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->selectedKeystoner]->aspect) = [sender doubleValue];
		[ProjectorFloorAspectText setStringValue:[sender stringValue]];
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->saveXml();
		
	}
}

-(IBAction)		setCameraKeystoneShowDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<CameraCalibration*>(ofApp->pluginController))->drawDebug = b;
	}
}
-(IBAction)		setCameraKeystoneMatrix:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<CameraCalibration*>(ofApp->pluginController))->selectedKeystoner = [sender selectedRow];
	}
}


-(IBAction)		drawBlobDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<BlobTracking*>(ofApp->pluginController))->drawDebug = b;
	}
}

-(IBAction)	setBlobThreshold1:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(0,[sender doubleValue]);
	}
}	

-(IBAction)	setBlobBlur1:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(0,[sender doubleValue]);
	}
}	

-(IBAction)		blobGrab1:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->grab(0);
	}
}
-(IBAction)		setBlobActive1:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(0,b);
	}
	
}

-(IBAction)	setBlobThreshold2:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(1,[sender doubleValue]);
	}
}	

-(IBAction)	setBlobBlur2:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(1,[sender doubleValue]);
	}
}	

-(IBAction)		blobGrab2:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->grab(1);
	}
}
-(IBAction)		setBlobActive2:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(1,b);
	}
	
}

-(IBAction)	setBlobThreshold3:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(2,[sender doubleValue]);
	}
}	

-(IBAction)	setBlobBlur3:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(2,[sender doubleValue]);
	}
}	

-(IBAction)		blobGrab3:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->grab(2);
	}
}
-(IBAction)		setBlobActive3:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(2,b);
	}
	
}

-(void) cameraUpdateGUIDs{
	if((getPlugin<Cameras*>(ofApp->pluginController))->getGUID(0) != 0x0ll){
		[CameraGUID1 setStringValue:[NSString stringWithFormat:@"%llx",(getPlugin<Cameras*>(ofApp->pluginController))->getGUID(0)]];
	}
	if((getPlugin<Cameras*>(ofApp->pluginController))->getGUID(1) != 0x0ll){
		[CameraGUID2 setStringValue:[NSString stringWithFormat:@"%llx",(getPlugin<Cameras*>(ofApp->pluginController))->getGUID(1)]];
	}
	if((getPlugin<Cameras*>(ofApp->pluginController))->getGUID(2) != 0x0ll){
		[CameraGUID3 setStringValue:[NSString stringWithFormat:@"%llx",(getPlugin<Cameras*>(ofApp->pluginController))->getGUID(2)]];
	}
}
@end




