#import "GLee.h"

#import <Cocoa/Cocoa.h>
#include "testApp.h"
#include "CustomGLView.h"
#include "Plugin.h"
#include "PluginIncludes.h"

#include "FrostGuiObjects.h"

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
@property (assign, readwrite) FrostPlugin * plugin;



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
	
#pragma mark Plugin View Outlets
	
	IBOutlet NSView *cameraView;
	IBOutlet NSView *blobTrackingView;
	IBOutlet NSView *projectionSurfacesView;
	IBOutlet NSView *cameraKeystoneView;
	
#pragma mark Moon Dust Outlets

	IBOutlet NSView *moonDustView;
	
	IBOutlet frostSlider * MoonDustForce;
	IBOutlet frostSlider * MoonDustLength;
	IBOutlet frostSlider * MoonDustDensity;	
	IBOutlet frostSlider * MoonDustSize;
	IBOutlet NSButton * MoonDustDebug;
	IBOutlet frostSlider * MoonDustMasterAlpha;
	IBOutlet frostSlider * MoonDustColumnAlpha;	

#pragma mark La Linea Outlets
	
	IBOutlet NSView *laLineaView;
	IBOutlet NSButton * LaLineaDebug;
	IBOutlet NSButton * LaLineaLockOffsetSliders;
	IBOutlet frostSlider * LaLineaMasterAlpha;
	IBOutlet frostCheckbox *LaLineaUseFilm;
	IBOutlet frostSlider *LaLineaOffsetX1;
	IBOutlet frostSlider *LaLineaOffsetY1;
	IBOutlet frostSlider *LaLineaOffsetX2;
	IBOutlet frostSlider *LaLineaOffsetY2;
	IBOutlet frostCheckbox * LaLineaTrackingActive;

#pragma mark La Linea Floor Outlets

	IBOutlet frostSlider * LaLineaFloorMasterAlpha;
	IBOutlet NSView *laLineaFloorView;
	IBOutlet frostSlider * LaLineaFloorSpeed;
	IBOutlet frostSlider * LaLineaFloorWidth;	
	IBOutlet frostSlider * LaLineaFloorDirSpeed;
	IBOutlet frostSlider * LaLineaFloorCurl;
	IBOutlet NSButton * LaLineaFloorReset;

#pragma mark Blob Light Outlets

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
	
#pragma mark Blob History outlets
	
	IBOutlet NSView *BlobHistoryView;
	IBOutlet frostCheckbox *BlobHistoryIsRecording;
	IBOutlet frostSlider *BlobHistoryPlayDirection;
	IBOutlet frostSlider *BlobHistoryAlpha;
	IBOutlet frostSlider *BlobHistoryPrintsAlpha;
	
	IBOutlet frostSlider * BlobHistoryGrow;
	IBOutlet frostSlider * BlobHistoryDecrease;
	IBOutlet frostSlider * BlobHistoryFreezeSpeed;
	IBOutlet frostSlider * BlobHistoryFreezeStrength;
		
#pragma mark Spotlight outlets
	
	IBOutlet NSView *SpotlightView;
	IBOutlet frostSlider * SpotlightMasterAlpha;
	IBOutlet frostSlider * SpotlightRadiusMultiplier;
	
#pragma mark MirrorBall outlets
	
	IBOutlet NSView *MirrorBallView;
	
	IBOutlet frostSlider * MirrorBallMasterAlpha;
	
	IBOutlet frostCheckbox * MirrorBallReflection1DotAdd;
	IBOutlet frostCheckbox * MirrorBallReflection1DotRemove;
	IBOutlet frostCheckbox * MirrorBallReflection1Clear;

	IBOutlet frostSlider * MirrorBallReflection1OnFraction;
	IBOutlet frostSlider * MirrorBallReflection1AlphaFraction;
	
	IBOutlet frostCheckbox * MirrorBallReflection2DotAdd;
	IBOutlet frostCheckbox * MirrorBallReflection2DotRemove;
	IBOutlet frostCheckbox * MirrorBallReflection2Clear;
	
	IBOutlet frostSlider * MirrorBallReflection2OnFraction;
	IBOutlet frostSlider * MirrorBallReflection2AlphaFraction;
	
	IBOutlet frostSlider * MirrorBallRotationFactor;
	
#pragma mark LEDGrid outlets
	
	IBOutlet NSView *LEDGridlView;
	
#pragma mark Folding outlets
	
	IBOutlet NSView *foldingView;
	IBOutlet frostSlider * foldingHistoryAddMultiplier;
	
#pragma mark Frostscape outlets
	
	IBOutlet NSView *frostscapeView;
	IBOutlet frostSlider * FrostScapeSlider1;
	IBOutlet frostSlider * FrostScapeSlider2;
	IBOutlet frostSlider * FrostScapeSlider3;
	IBOutlet frostSlider * FrostScapeSlider4;
	IBOutlet frostSlider * FrostScapeSlider5;
	IBOutlet frostSlider * FrostScapeSlider6;
	
	IBOutlet frostSlider * FrostscapeSideFreeze;
	IBOutlet frostSlider * FrostscapeColumnFreeze;
	IBOutlet frostSlider * FrostscapeWhiteBackground;

	IBOutlet frostCheckbox * FrostscapeInvert;
	IBOutlet frostSlider * FrostscapeMasterAlpha;


#pragma mark LiquidSpace outlets
	
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
	
#pragma mark Camera Outlets
	
	IBOutlet NSView * cameraSetting;
	
	IBOutlet NSView * cameraSettings1;
	IBOutlet NSView * cameraSettings2;
	IBOutlet NSView * cameraSettings3;
	
	IBOutlet NSTextField * CameraGUID1;
	IBOutlet NSTextField * CameraGUID2;
	IBOutlet NSTextField * CameraGUID3;
	
#pragma mark Projector Outlets
	
	IBOutlet NSButton * ProjectorShowDebug;
	IBOutlet NSMatrix * ProjectorMatrix;
	IBOutlet NSSlider * ProjectorFloorAspect;
	IBOutlet NSButton * ProjectorLockFloorAspect;
	IBOutlet NSTextField * ProjectorFloorAspectText;
	
#pragma mark Blob Tracking Outlets
	
	IBOutlet NSSlider * BlobThreshold1;
	IBOutlet NSSlider * BlobBlur1;
	IBOutlet frostCheckbox * BlobActive1;
	IBOutlet NSSlider * BlobThreshold2;
	IBOutlet NSSlider * BlobBlur2;
	IBOutlet frostCheckbox * BlobActive2;
	IBOutlet NSSlider * BlobThreshold3;
	IBOutlet NSSlider * BlobBlur3;
	IBOutlet frostCheckbox * BlobActive3;

#pragma mark OpenGL Views
	
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

-(void) changePluginEnabled:(int)n enable:(bool)enable;
-(void) changeView:(int)n;

#pragma mark Camera Actions

-(IBAction)		cameraBindGuid1:(id)sender;
-(IBAction)		cameraBindGuid2:(id)sender;
-(IBAction)		cameraBindGuid3:(id)sender;

#pragma mark MoonDust Actions

-(IBAction)		modifyMoonDustForce:(id)sender;
-(IBAction)		modifyMoonDustLength:(id)sender;
-(IBAction)		modifyMoonDustSize:(id)sender;
-(IBAction)		modifyMoonDustDensity:(id)sender;
-(IBAction)		modifyMoonDustDebug:(id)sender;

#pragma mark La Linea Actions

-(IBAction)		modifyLaLineaDebug:(id)sender;

-(IBAction)		modifyLaLineaFloorWidth:(id)sender;
-(IBAction)		modifyLaLineaFloorSpeed:(id)sender;
-(IBAction)		modifyLaLineaFloorDirSpeed:(id)sender;
-(IBAction)		modifyLaLineaFloorReset:(id)sender;

-(IBAction)		modifyLaLineaLockOffsetSliders:(id)sender;
-(IBAction)		modifyLaLineaUseFilm:(id)sender;
-(IBAction)		modifyLaLineaTrackingActive:(id)sender;

#pragma mark Blob Light Actions

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

#pragma mark Frostscape Actions

-(IBAction)		modifyFrostscapeSlider1:(id)sender;
-(IBAction)		modifyFrostscapeSlider2:(id)sender;
-(IBAction)		modifyFrostscapeSlider3:(id)sender;
-(IBAction)		modifyFrostscapeSlider4:(id)sender;
-(IBAction)		modifyFrostscapeSlider5:(id)sender;
-(IBAction)		modifyFrostscapeSlider6:(id)sender;
-(IBAction)		frostscapeFillIce:(id)sender;
-(IBAction)		frostscapeEmptyIce:(id)sender;

#pragma mark Folding Actions

-(IBAction)		modifyFoldingHistoryAddMultiplier:(id)sender;

#pragma mark LiquidSpace Actions

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

#pragma mark Projector Surfaces Actions

-(IBAction)		modifyProjectorShowDebug:(id)sender;
-(IBAction)		modifyProjectorMatrix:(id)sender;
-(IBAction)		modifyProjectorFloorAspect:(id)sender;
-(IBAction)		modifyProjectorLockFloorAspect:(id)sender;


#pragma mark Camera Keystone Actions

-(IBAction)		modifyCameraKeystoneShowDebug:(id)sender;
-(IBAction)		modifyCameraKeystoneMatrix:(id)sender;

#pragma mark Blob Tracker Actions

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

#pragma mark Blob History Actions

-(IBAction) modifyBlobHistoryIsRecording:(id)sender;
-(IBAction) modifyBlobHistoryClearHistory:(id)sender;
-(IBAction) modifyBlobHistoryAddSnapshot:(id)sender;
-(IBAction) modifyBlobHistoryRemoveSnapshot:(id)sender;
-(IBAction) modifyBlobHistoryClearSnapshots:(id)sender;

#pragma mark MirrorBall Actions

-(IBAction) modifyMirrorBall1AddReflection:(id)sender;
-(IBAction) modifyMirrorBall1RemoveReflection:(id)sender;
-(IBAction) modifyMirrorBall1ClearReflections:(id)sender;

-(IBAction) modifyMirrorBall2AddReflection:(id)sender;
-(IBAction) modifyMirrorBall2RemoveReflection:(id)sender;
-(IBAction) modifyMirrorBall2ClearReflections:(id)sender;

# pragma mark Callbacks

-(void)			awakeFromNib;
-(void)			cameraUpdateGUIDs;

-(id)			init;


@end



extern OFGuiController * gui;