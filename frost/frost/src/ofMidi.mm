//
//  ofMidi.m
//  openFrameworks
//
//  Created by Jonas Jongejan on 02/10/09.
//  Copyright 2009 HalfdanJ. All rights reserved.
//

#import "ofMidi.h"

#include "testApp.h"
#include "Plugin.h"
#include "PluginController.h"
#include "PluginIncludes.h"

@implementation ofMidi
-(void) awakeFromNib{
	printf("\n\n\n&&&&&&& MIDI WAKE UP &&&&&&&\n");
	
	manager = [PYMIDIManager sharedInstance];
	endpoint = new PYMIDIRealEndpoint;
	
	[endpoint retain];
	
    NSArray* endpointArray = [manager realSources];
    NSEnumerator* enumerator = [endpointArray objectEnumerator];
	
	id anObject;
	
	while (anObject = [enumerator nextObject]) {
		if ([[anObject displayName] isEqualToString:@"IAC-driver Frost Video"]) {
			endpoint = anObject;
			NSLog([endpoint displayName]);
			printf("Is it in use? %d\n",[endpoint isInUse]);
			[endpoint addReceiver:self];
		}
		NSLog([anObject displayName]);
	}
	
	frostSliderHookups  = [[NSMutableArray alloc] initWithCapacity:10];
	
#pragma mark Blob Tracking
	
	[self hookupButton:gui->Blob1OnlyCurtain onChannel:1 onNumber:6 controlChanges:true noteChanges:false];
	
#pragma mark Moon Dust
	
	// channel 4
	
	[self hookupSlider:gui->MoonDustMasterAlpha onChannel:4 onNumber:1 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->MoonDustForce onChannel:4 onNumber:2 controlChanges:true noteChanges:false scale:3.0/127.0];
	[self hookupSlider:gui->MoonDustLength onChannel:4 onNumber:3 controlChanges:true noteChanges:false scale:3000.0/127.0];
	[self hookupSlider:gui->MoonDustDensity onChannel:4 onNumber:4 controlChanges:true noteChanges:false scale:10000.0/127.0];
	[self hookupSlider:gui->MoonDustSize onChannel:4 onNumber:5 controlChanges:true noteChanges:false scale:0.1/127.0];
	[self hookupSlider:gui->MoonDustColumnAlpha onChannel:4 onNumber:6 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->MoonDustColumnMaskAlpha onChannel:4 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];
	
#pragma mark La Linea
	
	// channel 5
	
	[self hookupSlider:gui->LaLineaMasterAlpha onChannel:5 onNumber:1 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaMaskAlpha onChannel:5 onNumber:4 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaNoise1 onChannel:5 onNumber:5 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaNoise2 onChannel:5 onNumber:6 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaNoise2 onChannel:5 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaDancer onChannel:5 onNumber:8 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaNoise3 onChannel:5 onNumber:9 controlChanges:true noteChanges:false scale:1.0/127.0];
	
#pragma mark La Linea Floor
	
	// channel 6
	
	[self hookupSlider:gui->LaLineaFloorMasterAlpha onChannel:6 onNumber:1 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaFloorSpeed onChannel:6 onNumber:2 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaFloorDirSpeed onChannel:6 onNumber:3 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaFloorWidth onChannel:6 onNumber:4 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LaLineaFloorCurl onChannel:6 onNumber:5 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupButton:gui->LaLineaFloorReset onChannel:6 onNumber:6 controlChanges:true noteChanges:false];
	
#pragma mark Blob Light
	
	// channel 7
	
	[self hookupSlider:gui->BlobLightBlobAlpha		onChannel:7 onNumber:2 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->BlobLightBlur			onChannel:7 onNumber:3 controlChanges:true noteChanges:false scale:1.0/127.0];	
	[self hookupSlider:gui->BlobLightThreshold		onChannel:7 onNumber:4 controlChanges:true noteChanges:false scale:1.0/127.0];	
	[self hookupSlider:gui->BlobLightBlur2			onChannel:7 onNumber:5 controlChanges:true noteChanges:false scale:300.0/127.0];	
	[self hookupSlider:gui->BlobLightHistoryAlpha	onChannel:7 onNumber:6 controlChanges:true noteChanges:false scale:1.0/127.0];	
	[self hookupSlider:gui->BlobLightAlpha			onChannel:7 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];		
	[self hookupSlider:gui->BlobLightBeta			onChannel:7 onNumber:8 controlChanges:true noteChanges:false scale:1.0/127.0];		
	[self hookupSlider:gui->BlobLightAddBackground	onChannel:7 onNumber:9 controlChanges:true noteChanges:false scale:1.0/127.0];		
	
#pragma mark Folding Woman
	
	// channel 8
	
	[self hookupSlider:gui->foldingAlpha					onChannel:8 onNumber:1 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingHistoryAddMultiplier		onChannel:8 onNumber:2 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingHistoryPush1				onChannel:8 onNumber:3 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingHistoryPush2				onChannel:8 onNumber:4 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingHistoryPush3				onChannel:8 onNumber:5 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingFloorbox					onChannel:8 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingFishAlpha				onChannel:8 onNumber:8 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupButton:gui->foldingReset					onChannel:8 onNumber:9 controlChanges:true noteChanges:false];
	[self hookupButton:gui->foldingUpdateHistoryFromBlob	onChannel:8 onNumber:10 controlChanges:true noteChanges:false];
	[self hookupSlider:gui->foldingHistoryAlpha				onChannel:8 onNumber:11 controlChanges:true noteChanges:false scale:1.0/127.0];
	
	[self hookupSlider:gui->foldingFloorboxAlpha			onChannel:8 onNumber:12 controlChanges:true noteChanges:false scale:1.0/127.0];
	
	
	[self hookupSlider:gui->foldingFloorboxAlpha			onChannel:8 onNumber:12 controlChanges:true noteChanges:false scale:1.0/127.0];
	
	
#pragma mark Frostscape
	
	// channel 9
	
	[self hookupSlider:gui->FrostscapeMasterAlpha onChannel:9 onNumber:1 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostScapeSlider1 onChannel:9 onNumber:2 controlChanges:true noteChanges:false scale:10.0/127.0];
	[self hookupSlider:gui->FrostScapeSlider2 onChannel:9 onNumber:3 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostScapeSlider3 onChannel:9 onNumber:4 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostScapeSlider5 onChannel:9 onNumber:5 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostScapeSlider6 onChannel:9 onNumber:6 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostscapeSideFreeze onChannel:9 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostscapeWhiteBackground onChannel:9 onNumber:8 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostScapeSlider4 onChannel:9 onNumber:9 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupButton:gui->FrostscapeInvert onChannel:9 onNumber:12 controlChanges:true noteChanges:false];
	[self hookupSlider:gui->FrostscapeColumnFreeze1 onChannel:9 onNumber:13 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostscapeColumnFreeze2 onChannel:9 onNumber:14 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostscapeColumnFreeze3 onChannel:9 onNumber:15 controlChanges:true noteChanges:false scale:1.0/127.0];
	
	[self hookupSlider:gui->FrostscapeLinesAlpha onChannel:9 onNumber:16 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->FrostscapeLinesAddSpeed onChannel:9 onNumber:17 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupButton:gui->FrostscapeResetLines onChannel:9 onNumber:18 controlChanges:true noteChanges:false];
	
	
#pragma mark Liquid Space
	
	// channel 10
	[self hookupSlider:gui->liquidSpaceMasterAlpha		onChannel:10 onNumber:1 controlChanges:true noteChanges:false scale:1.0/127.0];
	
	[self hookupSlider:gui->liquidSpaceFadeSpeed		onChannel:10 onNumber:2 controlChanges:true noteChanges:false scale:100.0/127.0];
	[self hookupSlider:gui->liquidSpaceViscosity		onChannel:10 onNumber:3 controlChanges:true noteChanges:false scale:100.0/127.0];
	[self hookupSlider:gui->liquidSpaceDiffusion		onChannel:10 onNumber:4 controlChanges:true noteChanges:false scale:100.0/127.0];
	[self hookupSlider:gui->liquidSpaceSpeed			onChannel:10 onNumber:5 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupButton:gui->liquidSpaceClear			onChannel:10 onNumber:6 controlChanges:true noteChanges:false];
	[self hookupButton:gui->liquidSpaceAddingColor		onChannel:10 onNumber:7 controlChanges:true noteChanges:false];
	[self hookupSlider:gui->liquidSpaceColorMultiplier	onChannel:10 onNumber:8 controlChanges:true noteChanges:false scale:100.0/127.0];
	[self hookupButton:gui->liquidSpaceAddingForce		onChannel:10 onNumber:9 controlChanges:true noteChanges:false];
	[self hookupSlider:gui->liquidSpaceForceMultiplier	onChannel:10 onNumber:10 controlChanges:true noteChanges:false scale:100.0/127.0];
	//add fixed ring 1 reserved
	[self hookupButton:gui->liquidSpaceAddFixedRing2	onChannel:10 onNumber:12 controlChanges:true noteChanges:false];
	//add fixed ring 2 reserved
	[self hookupButton:gui->liquidSpaceAddRing1			onChannel:10 onNumber:14 controlChanges:true noteChanges:false];
	[self hookupButton:gui->liquidSpaceAddRing2			onChannel:10 onNumber:15 controlChanges:true noteChanges:false];
	[self hookupButton:gui->liquidSpaceAddRing3			onChannel:10 onNumber:16 controlChanges:true noteChanges:false];
	[self hookupButton:gui->liquidUpdateMotor			onChannel:10 onNumber:17 controlChanges:true noteChanges:false];
	[self hookupSlider:gui->liquidSpaceWhiteBlue		onChannel:10 onNumber:30 controlChanges:true noteChanges:false scale:1.0/127.0];

	
#pragma mark Blob History
	
	// channel 11
	
	[self hookupSlider:gui->BlobHistoryMasterAlpha onChannel:11 onNumber:1 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->BlobHistoryAlpha onChannel:11 onNumber:2 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->BlobHistoryPlayDirection onChannel:11 onNumber:3 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupButton:gui->BlobHistoryIsRecording onChannel:11 onNumber:4 controlChanges:true noteChanges:false];
	[self hookupButton:gui->BlobHistoryClearHistory onChannel:11 onNumber:5 controlChanges:true noteChanges:false];
	[self hookupSlider:gui->BlobHistoryGrow onChannel:11 onNumber:6 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->BlobHistoryDecrease onChannel:11 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->BlobHistoryFreezeSpeed onChannel:11 onNumber:8 controlChanges:true noteChanges:false scale:1.0/127.0];	
	[self hookupSlider:gui->BlobHistoryFreezeStrength onChannel:11 onNumber:9 controlChanges:true noteChanges:false scale:1.0/127.0];		
	[self hookupButton:gui->BlobHistoryClearFreeze onChannel:11 onNumber:10 controlChanges:true noteChanges:false];
	[self hookupSlider:gui->BlobHistoryPrintsAlpha onChannel:11 onNumber:11 controlChanges:true noteChanges:false scale:1.0/127.0];		
	[self hookupButton:gui->BlobHistoryAddPrint onChannel:11 onNumber:12 controlChanges:true noteChanges:false];
	[self hookupButton:gui->BlobHistoryRemovePrint onChannel:11 onNumber:13 controlChanges:true noteChanges:false];
	[self hookupButton:gui->BlobHistoryRemoveAllPrints onChannel:11 onNumber:14 controlChanges:true noteChanges:false];
	[self hookupButton:gui->BlobHistoryClearIce onChannel:11 onNumber:15 controlChanges:true noteChanges:false];
	[self hookupButton:gui->BlobHistoryFill onChannel:11 onNumber:16 controlChanges:true noteChanges:false];
	
	
#pragma mark Spotlight
	
	// channel 12
	
#pragma mark Mirror Ball
	
	// channel 13
	
	[self hookupSlider:gui->MirrorBallMasterAlpha				onChannel:13 onNumber:1 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->MirrorBallReflection1AlphaFraction	onChannel:13 onNumber:2 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->MirrorBallReflection1OnFraction		onChannel:13 onNumber:3 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->MirrorBallReflection1DotSize		onChannel:13 onNumber:4 controlChanges:true noteChanges:false scale:3.0/127.0];
	[self hookupSlider:gui->MirrorBallReflection2AlphaFraction	onChannel:13 onNumber:5 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->MirrorBallReflection2OnFraction		onChannel:13 onNumber:6 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->MirrorBallReflection2DotSize		onChannel:13 onNumber:7 controlChanges:true noteChanges:false scale:3.0/127.0];
	[self hookupSlider:gui->MirrorBallRotationFactor			onChannel:13 onNumber:8 controlChanges:true noteChanges:false scale:0.3/127.0];
	[self hookupButton:gui->MirrorBallReflection1Clear			onChannel:13 onNumber:9 controlChanges:true noteChanges:false];
	
#pragma mark LED Grid
	
	// channel 14
	
	[self hookupSlider:gui->LEDRadius							onChannel:14 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->LEDMasterFader						onChannel:14 onNumber:9 controlChanges:true noteChanges:false scale:1.0/127.0];
	
	// set gui info for booleans
	
	[gui->BlobActive1 setMidiChannel:1 number:1 control:true note:false];
	[gui->BlobActive2 setMidiChannel:1 number:2 control:true note:false];
	[gui->BlobActive3 setMidiChannel:1 number:3 control:true note:false];
	
	[gui->LaLineaTrackingActive setMidiChannel:5 number:2 control:true note:false];
	[gui->LaLineaUseFilm setMidiChannel:5 number:3 control:true note:false];
	
}

-(void) hookupSlider:(frostSlider*)slider onChannel:(int)channel onNumber:(int)number controlChanges:(bool)control noteChanges:(bool)note scale:(float)scale{
	[frostSliderHookups addObject:slider];
	
	[slider setMidiChannel:channel number:number control:control note:note scale:scale];
}

-(void) hookupButton:(frostCheckbox*)slider onChannel:(int)channel onNumber:(int)number controlChanges:(bool)control noteChanges:(bool)note {
	[frostSliderHookups addObject:slider];
	[slider setMidiChannel:channel number:number control:control note:note];
}

BOOL isDataByte (Byte b)		{ return b < 0x80; }
BOOL isStatusByte (Byte b)		{ return b >= 0x80 && b < 0xF8; }
BOOL isRealtimeByte (Byte b)	{ return b >= 0xF8; }

- (void)processMIDIPacketList:(MIDIPacketList*)packetList sender:(id)sender {
	
	MIDIPacket * packet = &packetList->packet[0];
	
	for (int i = 0; i < packetList->numPackets; i++) {
		
		for (int j = 0; j < packet->length; j+=3) {
			
			bool noteOn = false;
			bool noteOff = false;
			bool controlChange = false;
			int channel = -1;
			int number = -1;
			int value = -1;
			
			if(packet->data[0+j] >= 144 && packet->data[0+j] <= 159){
				noteOn = true;
				channel = packet->data[0+j] - 143;
				number = packet->data[1+j];
				value = packet->data[2+j];
			}
			if(packet->data[0+j] >= 128 && packet->data[0+j] <= 143){
				noteOff = true;
				channel = packet->data[0+j] - 127;
				number = packet->data[1+j];
				value = 0; // packet->data[2+j];
				
			}
			if(packet->data[0+j] >= 176 && packet->data[0+j] <= 191){
				controlChange = true;
				channel = packet->data[0+j] - 175;
				number = packet->data[1+j];
				value = packet->data[2+j];
			}
			
			if (channel == 5 && number == 5 ){
				// la linea noise hack
				if ( (noteOff || noteOn) && value == 0 ) { 
					if ([gui->LaLineaNoise1 floatValue] < 0.125) {
						value = 0;
					} else {
						value = [gui->LaLineaNoise1 floatValue] * 127.0;
					}
				}
				controlChange = true;
				noteOn = false;
				noteOff = false;
			}			
			
			
			if([midiActive state] ==  NSOnState){
				
				[gui->midiStatusText setStringValue:[NSString stringWithFormat:@"Channel: %d,  Number: %d,  Value: %d",channel, number, value]];
				
				if (controlChange && number == 7) {
					//printf("Midi: %i\n",value);
				}
				
				for(int i=0;i<[frostSliderHookups count];i++){
					[[frostSliderHookups objectAtIndex:i] receiveMidiOnChannel:channel number:number control:controlChange noteOn:noteOn noteOff:noteOff value:value];
				}
				
				//packet = MIDIPacketNext(packet);
				if(controlChange && number == 0){
					[gui changePluginEnabled:channel enable:(value ==0)?false:true];
				}	
				
				//Blob tracker
				if(controlChange && channel == 1){
					if(number == 1){
						if(value == 0){
							[gui->BlobActive1 setState:NSOffState];
						}
						if(value > 0){
							[gui->BlobActive1 setState:NSOnState];
						}
						[gui modifyBlobActive1:gui->BlobActive1];
						
					}
					
					if(number == 2){
						if(value == 0){
							[gui->BlobActive2 setState:NSOffState];
						}
						if(value > 0){
							[gui->BlobActive2 setState:NSOnState];
						}
						[gui modifyBlobActive2:gui->BlobActive2];
					}
					
					if(number == 3){
						if(value == 0){
							[gui->BlobActive3 setState:NSOffState];
						}
						if(value > 0){
							[gui->BlobActive3 setState:NSOnState];
						}
						[gui modifyBlobActive3:gui->BlobActive3];
					}
					
					if(number == 4){
						printf("Grab %i",value);
						if(value == 0){
							[gui blobGrab1:self];
						}
						if(value == 1){
							[gui blobGrab2:self];
						}
						if(value == 2){
							[gui blobGrab3:self];
						}
					}
				}
				
				if(controlChange && channel == 5){
					if(number == 2){
						if(value == 0){
							[gui->LaLineaTrackingActive setState:NSOffState];
						}
						if(value > 0){
							[gui->LaLineaTrackingActive setState:NSOnState];
						}
						[gui modifyLaLineaTrackingActive:gui->LaLineaTrackingActive];
						
					}
					if(number == 3){
						if(value == 0){
							[gui->LaLineaUseFilm setState:NSOffState];
						}
						if(value > 0){
							[gui->LaLineaUseFilm setState:NSOnState];
						}
						[gui modifyLaLineaUseFilm:gui->LaLineaUseFilm];
						
					}
				}
				
				if(controlChange && channel == 7){
					if(number == 10){
						getPlugin<BlobLight*>(gui->ofApp->pluginController)->r = value*2.0;
					}
					if(number == 11){
						getPlugin<BlobLight*>(gui->ofApp->pluginController)->g = value*2.0;
					}
					if(number == 12){
						getPlugin<BlobLight*>(gui->ofApp->pluginController)->b = value*2.0;
					}
					if(number == 13){
						getPlugin<BlobLight*>(gui->ofApp->pluginController)->r2 = value*2.0;
					}
					if(number == 14){
						getPlugin<BlobLight*>(gui->ofApp->pluginController)->g2 = value*2.0;
					}
					if(number == 15){
						getPlugin<BlobLight*>(gui->ofApp->pluginController)->b2 = value*2.0;
					}
					
					CGFloat r = getPlugin<BlobLight*>(gui->ofApp->pluginController)->r / 255.0;
					CGFloat g = getPlugin<BlobLight*>(gui->ofApp->pluginController)->g / 255.0;
					CGFloat b = getPlugin<BlobLight*>(gui->ofApp->pluginController)->b / 255.0;
					
					[gui->BlobLightColor setColor:[NSColor colorWithCalibratedRed:r green:g blue:b alpha:1.0 ]];
					
					
					CGFloat r2 = getPlugin<BlobLight*>(gui->ofApp->pluginController)->r2 / 255.0;
					CGFloat g2 = getPlugin<BlobLight*>(gui->ofApp->pluginController)->g2 / 255.0;
					CGFloat b2 = getPlugin<BlobLight*>(gui->ofApp->pluginController)->b2 / 255.0;
					
					[gui->BlobLightHistoryColor setColor:[NSColor colorWithCalibratedRed:r2 green:g2 blue:b2 alpha:1.0 ]];
					
				}
				
				//Frostscape
				if(controlChange && channel == 9){
					if(number == 10 && value > 0){
						[gui frostscapeEmptyIce:self];
						cout<<"EMPTY ICE"<<endl;
					}
					
					if(number == 11 && value > 0){
						[gui frostscapeFillIce:self];
					}
				}
				
				if(controlChange && channel == 10){
					
					if(number == 20){
						getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.r = value/127.0;
					}
					if(number == 21){
						getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.g = value/127.0;
					}
					if(number == 22){
						getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.b = value/127.0;
					}
					
					if(number == 30){
						getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.r = 1.0-(value/127.0);
						getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.g = 1.0-(value/127.0);
						getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.b = 127.0;
					}
					
					CGFloat r = getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.r;
					CGFloat g = getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.g;
					CGFloat b = getPlugin<LiquidSpace*>(gui->ofApp->pluginController)->dropColor.b;
					
					[gui->liquidSpaceDropColor setColor:[NSColor colorWithCalibratedRed:r green:g blue:b alpha:1.0 ]];
					
				}				
				
				if(controlChange && channel == 14){
					if(number == 1){
						getPlugin<LEDGrid*>(gui->ofApp->pluginController)->r = value*2.0;
					}
					if(number == 2){
						getPlugin<LEDGrid*>(gui->ofApp->pluginController)->g = value*2.0;
					}
					if(number == 3){
						getPlugin<LEDGrid*>(gui->ofApp->pluginController)->b = value*2.0;
					}
					if(number == 4){
						getPlugin<LEDGrid*>(gui->ofApp->pluginController)->r2 = value*2.0;
					}
					if(number == 5){
						getPlugin<LEDGrid*>(gui->ofApp->pluginController)->g2 = value*2.0;
					}
					if(number == 6){
						getPlugin<LEDGrid*>(gui->ofApp->pluginController)->b2 = value*2.0;
					}
					
					CGFloat r = getPlugin<LEDGrid*>(gui->ofApp->pluginController)->r / 255.0;
					CGFloat g = getPlugin<LEDGrid*>(gui->ofApp->pluginController)->g / 255.0;
					CGFloat b = getPlugin<LEDGrid*>(gui->ofApp->pluginController)->b / 255.0;
					
					[gui->LEDblobColor setColor:[NSColor colorWithCalibratedRed:r green:g blue:b alpha:1.0 ]];
					
					CGFloat r2 = getPlugin<LEDGrid*>(gui->ofApp->pluginController)->r2 / 255.0;
					CGFloat g2 = getPlugin<LEDGrid*>(gui->ofApp->pluginController)->g2 / 255.0;
					CGFloat b2 = getPlugin<LEDGrid*>(gui->ofApp->pluginController)->b2 / 255.0;
					
					[gui->LEDbackgroundColor setColor:[NSColor colorWithCalibratedRed:r2 green:g2 blue:b2 alpha:1.0 ]];
					
				}
			}
			
		}	
		packet = MIDIPacketNext (packet);
	}
}
	@end
