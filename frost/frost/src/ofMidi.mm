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
	endpoint = [endpointArray lastObject];
	//		printf("&&&&&&& List MIDI: &&&&&&&\n");
  // while (endpoint = [enumerator nextObject]) {
        //[myPopUp addItemWithTitle:[endpoint displayName]];
//        [[myPopUp lastItem] setRepresentedObject:endpoint];
		NSLog([endpoint displayName]);

    //}
	printf("Is it in use? %d\n",[endpoint isInUse]);
	[endpoint addReceiver:self];

	
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
		int note = -1;
		int noteVel = -1;
		int controlNumber = -1;
		int controlValue = -1;
		
		if(packet->data[0] >= 144 && packet->data[0] <= 159){
			noteOn = true;
			channel = packet->data[0] - 143;
			note = packet->data[1];
			noteVel = packet->data[2];
		}
		if(packet->data[0] >= 128 && packet->data[0] <= 143){
			noteOff = true;
			channel = packet->data[0] - 127;
			note = packet->data[1];
			noteVel = packet->data[2];
		}
		if(packet->data[0] >= 176 && packet->data[0] <= 191){
			controlChange = true;
			channel = packet->data[0] - 175;
			controlNumber = packet->data[1];
			controlValue = packet->data[2];
		}
		
//		[gui->BlobLightThreshold setFloatValue:0 ] ;
		//printf("dasidj %f",[gui->BlobLightThreshold floatValue]);
		[gui doMidiStuff];
		
		
		packet = MIDIPacketNext(packet);
	}	
}
@end
