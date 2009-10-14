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

- (void) setEnabled:(NSNumber *) n {
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
								  // initWithColorsAndLocations:[NSColor colorWithCalibratedRed:89 green:153 blue:229 alpha:1.0], (CGFloat)0.0,
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

-(void) awakeFromNib {
	
	NSLog(@"--- wake from nib ---\n");
	
	[camView setWindowId:1];
	[projectorView setWindowId:2];
	[cameraKeystoneOpenGlView setWindowId:5];
	[blobView setWindowId:3];
	
	[floorPreview setWindowId:4];
	[floorPreview setDoDraw:TRUE];
	
	[ProjectorFloorAspectText setStringValue:[[NSNumber numberWithFloat:((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->objects[0]->aspect] stringValue]];
	
	
	[FrostscapeSideFreeze hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->sideFreeze];
	[FrostscapeColumnFreeze1 hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->columnFreeze[0]];
	[FrostscapeColumnFreeze2 hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->columnFreeze[1]];
	[FrostscapeColumnFreeze3 hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->columnFreeze[2]];
	[FrostscapeFreezeColumns hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->freezeColumns];
	[FrostscapeInvert hookUpBool:&getPlugin<Frostscape*>(ofApp->pluginController)->invert];
	[FrostscapeMasterAlpha hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->masterAlpha];
	[FrostscapeWhiteBackground hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->whiteBackground];
	[FrostscapeAddLines hookUpBool:&getPlugin<Frostscape*>(ofApp->pluginController)->addingLines];
	[FrostscapeLinesAlpha hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->linesAlpha];
	[FrostscapeLinesAddSpeed hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->linesSpeed];
	[FrostscapeResetLines hookUpBool:&getPlugin<Frostscape*>(ofApp->pluginController)->resetLines];
	[FrostscapeFreezeLines hookUpFloat:&getPlugin<Frostscape*>(ofApp->pluginController)->freezeLines];

	[liquidSpaceMasterAlpha hookUpFloat:&getPlugin<LiquidSpace*>(ofApp->pluginController)->masterAlpha];

	[liquidUpdateMotor hookUpBool:&getPlugin<LiquidSpace*>(ofApp->pluginController)->updateMotor];
	[liquidSpaceClear hookUpBool:&getPlugin<LiquidSpace*>(ofApp->pluginController)->clear];
	
	[LEDRadius hookUpFloat:&getPlugin<LEDGrid*>(ofApp->pluginController)->radius];
	
	[MoonDustMasterAlpha hookUpFloat:&getPlugin<MoonDust*>(ofApp->pluginController)->masterAlpha];
	[MoonDustColumnAlpha hookUpFloat:&getPlugin<MoonDust*>(ofApp->pluginController)->columnAlpha];
	
	[LaLineaMasterAlpha hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->masterAlpha];
	[LaLineaMaskAlpha hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->maskAlpha];
	
	[LaLineaOffsetX1 hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->offsetPoint.x];
	[LaLineaOffsetY1 hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->offsetPoint.y];
	[LaLineaOffsetX2 hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->offsetPoint2.x];
	[LaLineaOffsetY2 hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->offsetPoint2.y];
	[LaLineaNoise1 hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->noise1];
	[LaLineaNoise2 hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->noise2];
	[LaLineaNoise3 hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->noise3];
	[LaLineaDancer hookUpFloat:&getPlugin<LaLinea*>(ofApp->pluginController)->dancer];

	[LaLineaFloorWidth hookUpFloat:&getPlugin<LaLineaFloor*>(ofApp->pluginController)->width];
	[LaLineaFloorSpeed hookUpFloat:&getPlugin<LaLineaFloor*>(ofApp->pluginController)->speed];
	[LaLineaFloorDirSpeed hookUpFloat:&getPlugin<LaLineaFloor*>(ofApp->pluginController)->dirSpeed];
	[LaLineaFloorMasterAlpha hookUpFloat:&getPlugin<LaLineaFloor*>(ofApp->pluginController)->masterAlpha];
	[LaLineaFloorCurl hookUpFloat:&getPlugin<LaLineaFloor*>(ofApp->pluginController)->curlValue];
	[LaLineaFloorReset hookUpBool:&getPlugin<LaLineaFloor*>(ofApp->pluginController)->bReset];
	
	[SpotlightMasterAlpha hookUpFloat:&getPlugin<Spotlight*>(ofApp->pluginController)->masterAlpha];
	[SpotlightRadiusMultiplier hookUpFloat:&getPlugin<Spotlight*>(ofApp->pluginController)->radiusMultiplier];

	[BlobHistoryMasterAlpha hookUpFloat:&getPlugin<BlobHistory*>(ofApp->pluginController)->masterAlpha];
	[BlobHistoryAlpha hookUpFloat:&getPlugin<BlobHistory*>(ofApp->pluginController)->historyAlpha];
	[BlobHistoryPrintsAlpha hookUpFloat:&getPlugin<BlobHistory*>(ofApp->pluginController)->snapshotAlpha];
	[BlobHistoryPlayDirection hookUpFloat:&getPlugin<BlobHistory*>(ofApp->pluginController)->historyPlayStep];
	
	[BlobHistoryGrow hookUpFloat:&getPlugin<BlobHistory*>(ofApp->pluginController)->growthValue];
	[BlobHistoryDecrease hookUpFloat:&getPlugin<BlobHistory*>(ofApp->pluginController)->decreaseValue];
	[BlobHistoryFreezeSpeed hookUpFloat:&getPlugin<BlobHistory*>(ofApp->pluginController)->freezeSpeed];
	[BlobHistoryFreezeStrength hookUpFloat:&getPlugin<BlobHistory*>(ofApp->pluginController)->freezeRate];
	[BlobHistoryClearFreeze hookUpBool:&getPlugin<BlobHistory*>(ofApp->pluginController)->clearFreeze];
	[BlobHistoryIsRecording hookUpBool:&getPlugin<BlobHistory*>(ofApp->pluginController)->bIsRecordingHistory];
	[BlobHistoryClearHistory hookUpBool:&getPlugin<BlobHistory*>(ofApp->pluginController)->bClearHistory];
	[BlobHistoryAddPrint hookUpBool:&getPlugin<BlobHistory*>(ofApp->pluginController)->bTakeSnapshot];
	[BlobHistoryRemovePrint hookUpBool:&getPlugin<BlobHistory*>(ofApp->pluginController)->bRemoveOldestSnapshot];
	[BlobHistoryRemoveAllPrints hookUpBool:&getPlugin<BlobHistory*>(ofApp->pluginController)->bClearSnapshots];
	[BlobHistoryClearIce hookUpBool:&getPlugin<BlobHistory*>(ofApp->pluginController)->bClearIce];
	[BlobHistoryFill hookUpBool:&getPlugin<BlobHistory*>(ofApp->pluginController)->fill];
	
	[Blob1OnlyCurtain hookUpBool:&getPlugin<BlobTracking*>(ofApp->pluginController)->bUseBgMask[0]];

	[MirrorBallMasterAlpha hookUpFloat:&getPlugin<MirrorBall*>(ofApp->pluginController)->masterAlpha];
	[MirrorBallReflection1OnFraction hookUpFloat:&getPlugin<MirrorBall*>(ofApp->pluginController)->reflections1.fractionOfDotsShouldBeTurnedOn];
	[MirrorBallReflection1AlphaFraction hookUpFloat:&getPlugin<MirrorBall*>(ofApp->pluginController)->reflections1.alpha];
	[MirrorBallReflection2OnFraction hookUpFloat:&getPlugin<MirrorBall*>(ofApp->pluginController)->reflections2.fractionOfDotsShouldBeTurnedOn];
	[MirrorBallReflection2AlphaFraction hookUpFloat:&getPlugin<MirrorBall*>(ofApp->pluginController)->reflections2.alpha];
	[MirrorBallRotationFactor hookUpFloat:&getPlugin<MirrorBall*>(ofApp->pluginController)->rotationDirection];
	[MirrorBallReflection1DotAdd hookUpBool:&getPlugin<MirrorBall*>(ofApp->pluginController)->bAddDot];
	[MirrorBallReflection1Clear hookUpBool:&getPlugin<MirrorBall*>(ofApp->pluginController)->bClearDots];
	[MirrorBallReflection1DotSize hookUpFloat:&getPlugin<MirrorBall*>(ofApp->pluginController)->reflections1.dotSizeMultiplier];
	[MirrorBallReflection2DotSize hookUpFloat:&getPlugin<MirrorBall*>(ofApp->pluginController)->reflections2.dotSizeMultiplier];

	[liquidSpaceSpeed hookUpFloat:&getPlugin<LiquidSpace*>(ofApp->pluginController)->speed];
	
	[foldingAlpha hookUpFloat:&getPlugin<Folding*>(ofApp->pluginController)->masterAlpha];
	[foldingHistoryPush1 hookUpFloat:&getPlugin<Folding*>(ofApp->pluginController)->push1];
	[foldingHistoryPush2 hookUpFloat:&getPlugin<Folding*>(ofApp->pluginController)->push2];
	[foldingHistoryPush3 hookUpFloat:&getPlugin<Folding*>(ofApp->pluginController)->push3];
	[foldingFishAlpha hookUpFloat:&getPlugin<Folding*>(ofApp->pluginController)->fishAlpha];
	[foldingFloorbox hookUpFloat:&getPlugin<Folding*>(ofApp->pluginController)->foldingFloorbox];	 
	[foldingReset hookUpBool:&getPlugin<Folding*>(ofApp->pluginController)->reset];	 
	[foldingUpdateHistoryFromBlob hookUpBool:&getPlugin<Folding*>(ofApp->pluginController)->updateHistoryFromBlob];	
	[foldingHistoryAlpha hookUpFloat:&getPlugin<Folding*>(ofApp->pluginController)->historyAlpha];
	[foldingHistoryAddMultiplier hookUpFloat:&getPlugin<Folding*>(ofApp->pluginController)->historyAddMultiplier];
	
	[cameraBrightness1 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraBrightness[0]];
	[cameraBrightness2 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraBrightness[1]];
	[cameraBrightness3 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraBrightness[2]];
	
	[cameraGain1 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraGain[0]];
	[cameraGain2 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraGain[1]];
	[cameraGain3 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraGain[2]];
	
	[cameraShutter1 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraShutter[0]];
	[cameraShutter2 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraShutter[1]];
	[cameraShutter3 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraShutter[2]];
	
	[cameraExposure1 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraExposure[0]];
	[cameraExposure2 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraExposure[1]];
	[cameraExposure3 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraExposure[2]];

	[cameraGamma1 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraGamma[0]];
	[cameraGamma2 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraGamma[1]];
	[cameraGamma3 hookUpFloat:&getPlugin<Cameras*>(ofApp->pluginController)->cameraGamma[2]];

}

- (void)addObject:(NSString*)objname isheader:(bool)header plugin:(FrostPlugin*)p {
	ofPlugin * obj =  [[ofPlugin alloc]init];
	[obj setName:objname];
	[obj setHeader:[NSNumber numberWithBool:header]];
	[obj setPlugin:p];
	[obj setEnabled:[NSNumber numberWithBool:TRUE]];
	[viewItems addObject:obj];
}

- (id)init {
	NSLog(@"--- init ---\n");	
	
	if(self = [super init]) {
		
		userDefaults = [[NSUserDefaults standardUserDefaults] retain];
		
		ofApp = (testApp*)ofGetAppPtr();
		
		(getPlugin<MoonDust*>(ofApp->pluginController))->density = [userDefaults doubleForKey:@"moondust.density"];
		(getPlugin<MoonDust*>(ofApp->pluginController))->force = [userDefaults doubleForKey:@"moondust.force"];
		(getPlugin<MoonDust*>(ofApp->pluginController))->size = [userDefaults doubleForKey:@"moondust.size"];
		(getPlugin<MoonDust*>(ofApp->pluginController))->length = [userDefaults doubleForKey:@"moondust.length"];
		(getPlugin<MoonDust*>(ofApp->pluginController))->debug = [userDefaults boolForKey:@"moondust.debug"];
		
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
		
		(getPlugin<Folding*>(ofApp->pluginController))->historyAddMultiplier = [userDefaults doubleForKey:@"folding.historyAddMultiplier"];
		
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
		
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fillColor.set(0.0,0.0,0.0);
		
		/**
		 (getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidDrawer.getFluidSolver()->setFadeSpeed(0.00005 * [userDefaults doubleForKey:@"liquidSpace.fadeSpeed"]);
		 (getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidDrawer.getFluidSolver()->setVisc(0.0000001 * [userDefaults doubleForKey:@"liquidSpace.viscosity"]);
		 (getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidDrawer.getFluidSolver()->setColorDiffusion(0.0000001 * [userDefaults doubleForKey:@"liquidSpace.diffusion"]);
		 (getPlugin<LiquidSpace*>(ofApp->pluginController))->dropColor.set(1.0,1.0,1.0);
		 (getPlugin<LiquidSpace*>(ofApp->pluginController))->addingColor = [userDefaults boolForKey:@"liquidSpace.addingColor"];
		 (getPlugin<LiquidSpace*>(ofApp->pluginController))->colorMultiplier = 0.05 *  [userDefaults boolForKey:@"liquidSpace.colorMultiplier"];
		 (getPlugin<LiquidSpace*>(ofApp->pluginController))->addingForce = [userDefaults boolForKey:@"liquidSpace.addingForce"];
		 (getPlugin<LiquidSpace*>(ofApp->pluginController))->forceMultiplier = 0.05 * [userDefaults boolForKey:@"liquidSpace.forceMultiplier"];
		 **/
		
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
		[self addObject:@"La Linea Floor Line" isheader:FALSE plugin:getPlugin<LaLineaFloor*>(ofApp->pluginController)];
		[self addObject:@"Blob light" isheader:FALSE plugin:getPlugin<BlobLight*>(ofApp->pluginController)];
		[self addObject:@"Folding / Goldfish" isheader:FALSE plugin:getPlugin<Folding*>(ofApp->pluginController)];
		[self addObject:@"Frostscape" isheader:FALSE plugin:getPlugin<Frostscape*>(ofApp->pluginController)];
		[self addObject:@"Liquid Space" isheader:FALSE plugin:getPlugin<LiquidSpace*>(ofApp->pluginController)];
		[self addObject:@"Blob History" isheader:FALSE plugin:getPlugin<BlobHistory*>(ofApp->pluginController)];
		[self addObject:@"Spotlight" isheader:FALSE plugin:getPlugin<Spotlight*>(ofApp->pluginController)];
		[self addObject:@"Mirror Ball" isheader:FALSE plugin:getPlugin<MirrorBall*>(ofApp->pluginController)];
		[self addObject:@"LED Grid" isheader:FALSE plugin:getPlugin<LEDGrid*>(ofApp->pluginController)];
		
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
		[laLineaFloorView retain];
		[blobLightView retain];
		[frostscapeView retain];
		[liquidSpaceView retain];
		[BlobHistoryView retain];
		[SpotlightView retain];
		[MirrorBallView retain];
		
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
		
		ofApp->cameraGUIDs[0] = (uint64_t)guidVal[0];
		ofApp->cameraGUIDs[1] = (uint64_t)guidVal[1];
		ofApp->cameraGUIDs[2] = (uint64_t)guidVal[2];
		
		
		(getPlugin<Cameras*>(ofApp->pluginController))->setGUIDs((uint64_t)guidVal[0],(uint64_t)guidVal[1],(uint64_t)guidVal[2]);
		
		[self cameraUpdateGUIDs];
		
    }
	return self;
}


-(void) changeView:(int)n{
	ofPlugin * p = [viewItems objectAtIndex:n];
	int row = n;
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
	
	id view = nil;
	
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
		[ProjectorLockFloorAspect setState:NSOnState];
		[ProjectorFloorAspect setEnabledWithButton:ProjectorLockFloorAspect];
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
		[LaLineaLockOffsetSliders setState:NSOnState];
		[LaLineaOffsetX1 setEnabledWithButton:LaLineaLockOffsetSliders];
		[LaLineaOffsetY1 setEnabledWithButton:LaLineaLockOffsetSliders];
		[LaLineaOffsetX2 setEnabledWithButton:LaLineaLockOffsetSliders];
		[LaLineaOffsetY2 setEnabledWithButton:LaLineaLockOffsetSliders];
	}	
	
	if(![(NSString*)[p name] compare:@"La Linea Floor Line"]){
		view = laLineaFloorView;
	}	
	
	if(![(NSString*)[p name] compare:@"Blob light"]){
		view = blobLightView;
	}	
	
	if(![(NSString*)[p name] compare:@"Folding / Goldfish"]){
		view = foldingView;
	}	
	
	if(![(NSString*)[p name] compare:@"Frostscape"]){
		view = frostscapeView;
	}	
	
	if(![(NSString*)[p name] compare:@"Liquid Space"]){
		view = liquidSpaceView;
	}
	
	if(![(NSString*)[p name] compare:@"Blob History"]){
		view = BlobHistoryView;
	}
	
	if(![(NSString*)[p name] compare:@"Spotlight"]){
		view = SpotlightView;
	}
	
	if(![(NSString*)[p name] compare:@"Mirror Ball"]){
		view = MirrorBallView;
	}
	
	if(![(NSString*)[p name] compare:@"LED Grid"]){
		view = LEDGridlView;
	}
	if(view != nil){
		[contentArea addSubview:view];
		NSRect currFrame = [contentArea frame];
		CGFloat h = currFrame.size.height;
		
		NSRect currFrame2 = [view frame];
		CGFloat h2 = currFrame2.size.height;
		
		[view setFrameOrigin:NSMakePoint(0,h-h2)]; 
	}
}

-(IBAction) setListViewRow:(id)sender {
	[self changeView:[sender selectedRow]];
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

-(void) changePluginEnabled:(int)n enable:(bool)enable{
	NSMutableArray * array;
	int i;
	if(n < [viewItems count]){
		array = viewItems;
		i = n;
	}
	i ++;
	if(i >= 3){
		i++;	
	}	
	if(i >= 6){
		i++;	
	}
	
	
	ofPlugin * p = [array objectAtIndex:i];
	[p setEnabled:[NSNumber numberWithBool:enable]];	
	[userDefaults setValue:[p enabled] forKey:[NSString stringWithFormat:@"plugins.enable%d",i]];
	[listView setNeedsDisplay:TRUE];

	[self changeView:i];
	
}


- (int)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return [viewItems count];
}

#pragma mark Cameras

-(IBAction)		cameraBindGuid1:(id)sender{
	if(ofApp->setupCalled){
		uint64_t guidVal;
		sscanf([[CameraGUID1 stringValue] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal);
		(getPlugin<Cameras*>(ofApp->pluginController))->setGUID(0, (uint64_t)guidVal);
		ofApp->cameraGUIDs[0] = (uint64_t)guidVal;
		[self cameraUpdateGUIDs];
	}
}

-(IBAction)		cameraBindGuid2:(id)sender{
	if(ofApp->setupCalled){
		uint64_t guidVal;
		sscanf([[CameraGUID2 stringValue] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal);
		(getPlugin<Cameras*>(ofApp->pluginController))->setGUID(1, (uint64_t)guidVal);
		ofApp->cameraGUIDs[1] = (uint64_t)guidVal;
		[self cameraUpdateGUIDs];
	}
}

-(IBAction)	cameraBindGuid3:(id)sender{
	if(ofApp->setupCalled){
		uint64_t guidVal;
		sscanf([[CameraGUID3 stringValue] cStringUsingEncoding:NSUTF8StringEncoding], "%llx", &guidVal);
		(getPlugin<Cameras*>(ofApp->pluginController))->setGUID(2, (uint64_t)guidVal);
		ofApp->cameraGUIDs[2] = (uint64_t)guidVal;
		[self cameraUpdateGUIDs];
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



#pragma mark Moon Dust


-(IBAction)	modifyMoonDustForce:(id)sender{
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->force = [sender doubleValue];
	}
}

-(IBAction)		modifyMoonDustLength:(id)sender {
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->length = [sender doubleValue];
	}
}
-(IBAction)		modifyMoonDustSize:(id)sender {
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->size = [sender doubleValue];
	}
}
-(IBAction)		modifyMoonDustDensity:(id)sender {
	if(ofApp->setupCalled){
		((MoonDust*)getPlugin<MoonDust*>(ofApp->pluginController))->density = [sender doubleValue];
	}
}
-(IBAction)		modifyMoonDustDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<MoonDust*>(ofApp->pluginController))->debug = b;
	}
}	

#pragma mark La Linea

-(IBAction)		modifyLaLineaDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<LaLinea*>(ofApp->pluginController))->debug = b;
	}
}

-(IBAction)		modifyLaLineaLockOffsetSliders:(id)sender{
	[LaLineaOffsetX1 setEnabledWithButton:sender];
	[LaLineaOffsetY1 setEnabledWithButton:sender];
	[LaLineaOffsetX2 setEnabledWithButton:sender];
	[LaLineaOffsetY2 setEnabledWithButton:sender];
}

-(IBAction)		modifyLaLineaUseFilm:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<LaLinea*>(ofApp->pluginController))->bHasToUseFilm = b;
	}
}

-(IBAction)		modifyLaLineaTrackingActive:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<LaLinea*>(ofApp->pluginController))->tracking = b;
	}
}



#pragma mark Blob Light

-(IBAction)		modifyBlobLightDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<BlobLight*>(ofApp->pluginController))->debug = b;
	}
}

-(IBAction)		modifyBlobLightColor:(id)sender{	
	if(ofApp->setupCalled){
		
		(getPlugin<BlobLight*>(ofApp->pluginController))->r  = [[sender color] redComponent]*255;
		(getPlugin<BlobLight*>(ofApp->pluginController))->g  = [[sender color] greenComponent]*255;
		(getPlugin<BlobLight*>(ofApp->pluginController))->b  = [[sender color] blueComponent]*255;
	}
}
-(IBAction)		modifyBlobLightColor2:(id)sender{	
	if(ofApp->setupCalled){
		
		(getPlugin<BlobLight*>(ofApp->pluginController))->r2  = [[sender color] redComponent]*255;
		(getPlugin<BlobLight*>(ofApp->pluginController))->g2  = [[sender color] greenComponent]*255;
		(getPlugin<BlobLight*>(ofApp->pluginController))->b2  = [[sender color] blueComponent]*255;
	}
}
-(IBAction)		modifyBlobLightBlur:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->blur = [sender doubleValue];
	}
}

-(IBAction)		modifyBlobLightThreshold:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->threshold = [sender doubleValue];
	}
}

-(IBAction)		modifyBlobLightBlur2:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->blur2 = [sender doubleValue];
	}
}
-(IBAction)		modifyBlobLightAlpha:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->alpha = [sender doubleValue];
	}
}
-(IBAction)		modifyBlobLightBeta:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->beta = [sender doubleValue];
	}
}
-(IBAction)		modifyBlobLightHistoryAlpha:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->historyalpha = [sender doubleValue];
	}
}

-(IBAction)		modifyBlobLightBlobAlpha:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->blobalpha = [sender doubleValue];
	}
}
-(IBAction)		modifyBlobLightBackgroundAdd:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<BlobLight*>(ofApp->pluginController))->addblack = [sender doubleValue];
	}
}
-(IBAction)		modifyBlobLightBackgroundClear:(id)sender{
	(getPlugin<BlobLight*>(ofApp->pluginController))->history.set(0);
}

#pragma mark Folding

-(IBAction)		modifyFoldingHistoryAddMultiplier:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Folding*>(ofApp->pluginController))->historyAddMultiplier = [sender doubleValue];
	}
}

#pragma mark Frostscape

-(IBAction)		modifyFrostscapeSlider1:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider1([sender doubleValue]);
	}
}

-(IBAction)		modifyFrostscapeSlider2:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider2([sender doubleValue]);
	}
}
-(IBAction)		modifyFrostscapeSlider3:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider3([sender doubleValue]);
	}
}
-(IBAction)		modifyFrostscapeSlider4:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider4([sender doubleValue]);
	}
}
-(IBAction)		modifyFrostscapeSlider5:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider5([sender doubleValue]);
	}
}
-(IBAction)		modifyFrostscapeSlider6:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->setslider6([sender doubleValue]);
	}
}
-(IBAction)		frostscapeFillIce:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->fillIce();
	}
}
-(IBAction)		frostscapeEmptyIce:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<Frostscape*>(ofApp->pluginController))->emptyIce();
	}
}

#pragma mark Liquid Space

-(IBAction)		modifyLiquidSpaceFillColor:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fillColor.set([[sender color] redComponent],[[sender color] greenComponent], [[sender color] blueComponent]);
	}
}

-(IBAction)		fillLiquidSpace:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fill();
	}
}

-(IBAction)		modifyLiquidSpaceFadeSpeed:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidDrawer.getFluidSolver()->setFadeSpeed(0.00005 * [sender doubleValue]);
	}
}

-(IBAction)		modifyLiquidSpaceViscosity:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidDrawer.getFluidSolver()->setVisc(0.0000001 * [sender doubleValue]);
	}
}

-(IBAction)		modifyLiquidSpaceDiffusion:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->fluidDrawer.getFluidSolver()->setColorDiffusion(0.0000001 * [sender doubleValue]);
	}
}

-(IBAction)		modifyLiquidSpaceDropColor:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->dropColor.set([[sender color] redComponent],[[sender color] greenComponent], [[sender color] blueComponent]);
	}
}

-(IBAction)		modifyLiquidSpaceAddingColor:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->addingColor = b;
	}
}

-(IBAction)		modifyLiquidSpaceColorMultiplier:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->colorMultiplier = 0.05 * [sender doubleValue];
	}
}

-(IBAction)		modifyLiquidSpaceAddingForce:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->addingForce = b;
	}
}

-(IBAction)		modifyLiquidSpaceForceMultiplier:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->forceMultiplier = 0.05 * [sender doubleValue];
	}
}

-(IBAction)		liquidAddRing1:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->addRing(0);
	}
}

-(IBAction)		liquidAddRing2:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->addRing(1);
	}
}
-(IBAction)		liquidAddRing3:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->addRing(2);
	}
}

-(IBAction)		liquidAddFixedRing2:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LiquidSpace*>(ofApp->pluginController))->addFixedRing(1);
	}
}



#pragma mark Projector calibration

-(IBAction)		modifyProjectorShowDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->drawDebug = b;
	}
}	

-(IBAction)		modifyProjectorMatrix:(id)sender {
	if(ofApp->setupCalled){
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->selectedKeystoner = [sender selectedRow];
		if([sender selectedRow] == 0){
			((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->w = ((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->glDelegate->m_Width/3.0;
			((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->h = ((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->glDelegate->m_Width/3.0;
		} else {
			((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->w = ((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->glDelegate->m_Width/1.50;
			((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->h = ((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->glDelegate->m_Width/1.5;
		}
		[ProjectorFloorAspect setFloatValue:((getPlugin<ProjectionSurfaces*>(ofApp->pluginController)->objects[[sender selectedRow]]->aspect))];
		//		[ProjectorFloorAspect setMinValue:0.5];
		[ProjectorFloorAspectText setDoubleValue:[ProjectorFloorAspect doubleValue]];
		
	}
}


-(IBAction)		modifyProjectorFloorAspect:(id)sender{
	if(ofApp->setupCalled){
		(((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->objects[((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->selectedKeystoner]->aspect) = [sender doubleValue];
		[ProjectorFloorAspectText setStringValue:[sender stringValue]];
		((ProjectionSurfaces*)getPlugin<ProjectionSurfaces*>(ofApp->pluginController))->saveXml();
		
	}
}

-(IBAction)		modifyProjectorLockFloorAspect:(id)sender{
	[ProjectorFloorAspect setEnabledWithButton:sender];
}

#pragma mark Camera calibration


-(IBAction)		modifyCameraKeystoneShowDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<CameraCalibration*>(ofApp->pluginController))->drawDebug = b;
	}
}
-(IBAction)		modifyCameraKeystoneMatrix:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<CameraCalibration*>(ofApp->pluginController))->selectedKeystoner = [sender selectedRow];
	}
}

#pragma mark Blob Tracker

-(IBAction)		drawBlobDebug:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		(getPlugin<BlobTracking*>(ofApp->pluginController))->drawDebug = b;
	}
}

-(IBAction)	modifyBlobThreshold1:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(0,[sender doubleValue]);
	}
}	

-(IBAction)	modifyBlobBlur1:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(0,[sender doubleValue]);
	}
}	

-(IBAction)	blobGrab1:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->grab(0);
	}
}
-(IBAction)	modifyBlobActive1:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(0,b);
	}
}

-(IBAction)	modifyBlobThreshold2:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(1,[sender doubleValue]);
	}
}	

-(IBAction)	modifyBlobBlur2:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(1,[sender doubleValue]);
	}
}	

-(IBAction)		blobGrab2:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->grab(1);
	}
}

-(IBAction)		modifyBlobActive2:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(1,b);
	}	
}

-(IBAction)	modifyBlobThreshold3:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setThreshold(2,[sender doubleValue]);
	}
}	

-(IBAction)	modifyBlobBlur3:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setBlur(2,[sender doubleValue]);
	}
}	

-(IBAction)		blobGrab3:(id)sender{
	if(ofApp->setupCalled){
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->grab(2);
	}
}

-(IBAction)		modifyBlobActive3:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		((BlobTracking*)getPlugin<BlobTracking*>(ofApp->pluginController))->setActive(2,b);
	}
}

/*#pragma mark La Linea Floor
-(IBAction)		modifyLaLineaFloorReset:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LaLineaFloor*>(ofApp->pluginController))->reset();
	}
}*/

#pragma mark Blob History

-(IBAction) modifyBlobHistoryIsRecording:(id)sender{
	if(ofApp->setupCalled){
		bool b = false;
		if([sender state] ==  NSOnState ){
			b = true;	
		}
		((BlobHistory*)getPlugin<BlobHistory*>(ofApp->pluginController))->bIsRecordingHistory = b;
	}
}

-(IBAction) modifyBlobHistoryClearHistory:(id)sender{
	if(ofApp->setupCalled){
		((BlobHistory*)getPlugin<BlobHistory*>(ofApp->pluginController))->bClearHistory = true;
	}
}

-(IBAction) modifyBlobHistoryAddSnapshot:(id)sender{
	if(ofApp->setupCalled){
		((BlobHistory*)getPlugin<BlobHistory*>(ofApp->pluginController))->bTakeSnapshot = true;
	}
}


-(IBAction) modifyBlobHistoryRemoveSnapshot:(id)sender{
	if(ofApp->setupCalled){
		((BlobHistory*)getPlugin<BlobHistory*>(ofApp->pluginController))->bRemoveOldestSnapshot = true;
	}
}


-(IBAction) modifyBlobHistoryClearSnapshots:(id)sender{
	if(ofApp->setupCalled){
		((BlobHistory*)getPlugin<BlobHistory*>(ofApp->pluginController))->bClearSnapshots = true;
	}
}

-(IBAction) ledSetBlobColor:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LEDGrid*>(ofApp->pluginController))->r  = [[sender color] redComponent]*255;
		(getPlugin<LEDGrid*>(ofApp->pluginController))->g  = [[sender color] greenComponent]*255;
		(getPlugin<LEDGrid*>(ofApp->pluginController))->b  = [[sender color] blueComponent]*255;
	}
}
-(IBAction) ledSetBackgroundColor:(id)sender{
	if(ofApp->setupCalled){
		(getPlugin<LEDGrid*>(ofApp->pluginController))->r2  = [[sender color] redComponent]*255;
		(getPlugin<LEDGrid*>(ofApp->pluginController))->g2  = [[sender color] greenComponent]*255;
		(getPlugin<LEDGrid*>(ofApp->pluginController))->b2  = [[sender color] blueComponent]*255;
	}
}


@end




