//
//  ofMidi.m
//  openFrameworks
//
//  Created by Jonas Jongejan on 02/10/09.
//  Copyright 2009 HalfdanJ. All rights reserved.
//

#import "ofMidi.h"


@implementation ofMidi
-(void) awakeFromNib{
	printf("\n\n\n&&&&&&& MIDI WAKE UP &&&&&&&\n");
	
	manager = [PYMIDIManager sharedInstance];
	endpoint = new PYMIDIRealEndpoint;
    NSArray* endpointArray = [manager realSources];
    
    NSEnumerator* enumerator = [endpointArray objectEnumerator];
	endpoint = [endpointArray objectAtIndex:1];
	NSLog([endpoint displayName]);
	printf("Is it in use? %d\n",[endpoint isInUse]);
	[endpoint addReceiver:self];
	
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
	
	[self hookupSlider:gui->BlobLightBlobAlpha onChannel:7 onNumber:2 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->BlobLightBlur onChannel:7 onNumber:3 controlChanges:true noteChanges:false scale:1.0/127.0];	
	[self hookupSlider:gui->BlobLightThreshold onChannel:7 onNumber:4 controlChanges:true noteChanges:false scale:1.0/127.0];	
	[self hookupSlider:gui->BlobLightBlur2 onChannel:7 onNumber:2 controlChanges:true noteChanges:false scale:1.0/127.0];	
	[self hookupSlider:gui->BlobLightHistoryAlpha onChannel:7 onNumber:5 controlChanges:true noteChanges:false scale:1.0/127.0];	
	[self hookupSlider:gui->BlobLightAlpha onChannel:7 onNumber:6 controlChanges:true noteChanges:false scale:1.0/127.0];		
		[self hookupSlider:gui->BlobLightBeta onChannel:7 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];		
		[self hookupSlider:gui->BlobLightAddBackground onChannel:7 onNumber:8 controlChanges:true noteChanges:false scale:1.0/127.0];		
#pragma mark Folding Woman
	
	// channel 8
	
	[self hookupSlider:gui->foldingHistoryAddMultiplier onChannel:8 onNumber:2 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingHistoryPush1 onChannel:8 onNumber:3 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingHistoryPush2 onChannel:8 onNumber:4 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingHistoryPush3 onChannel:8 onNumber:5 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingFloorbox onChannel:8 onNumber:7 controlChanges:true noteChanges:false scale:1.0/127.0];
	[self hookupSlider:gui->foldingFishAlpha onChannel:8 onNumber:8 controlChanges:true noteChanges:false scale:1.0/127.0];
	
	
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
	
#pragma mark Liquid Space
	
	// channel 10
	[self hookupButton:gui->liquidUpdateMotor			onChannel:10 onNumber:1 controlChanges:true noteChanges:false];
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
	
#pragma mark LED Grid

	// channel 14
	
	
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
- (void)processMIDIPacketList:(MIDIPacketList*)packetList sender:(id)sender
{
	MIDIPacket *packet = &packetList->packet[0];
	for (int i = 0; i < packetList->numPackets; ++i) {
		printf("Midi: %i\n",packet->data[0]);
		bool noteOn = false;
		bool noteOff = false;
		bool controlChange;
		int channel = -1;
		int number = -1;
		int value = -1;
		
		if(packet->data[0] >= 144 && packet->data[0] <= 159){
			noteOn = true;
			channel = packet->data[0] - 143;
			number = packet->data[1];
			value = packet->data[2];
		}
		if(packet->data[0] >= 128 && packet->data[0] <= 143){
			noteOff = true;
			channel = packet->data[0] - 127;
			number = packet->data[1];
			value = packet->data[2];
		}
		if(packet->data[0] >= 176 && packet->data[0] <= 191){
			controlChange = true;
			channel = packet->data[0] - 175;
			number = packet->data[1];
			value = packet->data[2];
		}
		if([midiActive state] ==  NSOnState){
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
			
			//LaLineaFloor
			/*if(controlChange && channel == 6){
				if(number == 6){
					[gui modifyLaLineaFloorReset:self];
				}
			}*/
			
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
			
			
			
		}
		
		packet = &packetList->packet[i];
	}	
}
@end
