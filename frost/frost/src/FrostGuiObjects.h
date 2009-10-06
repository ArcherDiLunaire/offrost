//
//  FrostGuiObjects.h
//  openFrameworks
//
//  Created by frost on 06/10/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface frostCheckbox : NSButton
{
	NSButton * valButton;
	SEL myAction;
	NSObject * myTarget;
	
	int midiChannel;
	int midiNumber;
	bool midiControlHookup;
	bool midiNoteHookup;
	
	bool hookedUpToBool;
	bool * hookedUpBool;
}

- (void) receiveMidiOnChannel:(int)channel number:(int)number control:(bool)control noteOn:(bool)noteOn noteOff:(bool)noteOff value:(int)value;
- (void) setMidiChannel:(int)channel number:(int)number control:(bool)control note:(bool)note;
- (id) initWithFrame:(NSRect)frame;
- (void) awakeFromNib;
- (void) changeValueFromControl:(id)sender;
- (void) hookUpBool:(BOOL*)f;
- (BOOL) boolValue;

@end



@interface frostSlider : NSSlider
{
	NSSlider * valSlider;
	NSTextField * valTextfield;
	SEL myAction;
	NSObject * myTarget;
	
	int midiChannel;
	int midiNumber;
	bool midiControlHookup;
	bool midiNoteHookup;
	float midiScaleFactor;
	
	bool hookedUpToFloat;
	float * hookedUpFloat;
}

- (void) receiveMidiOnChannel:(int)channel number:(int)number control:(bool)control noteOn:(bool)noteOn noteOff:(bool)noteOff value:(int)value;

- (void) setMidiChannel:(int)channel number:(int)number control:(bool)control note:(bool)note scale:(float)scale;
- (id) initWithFrame:(NSRect)frame;
- (void) awakeFromNib;
- (void) changeValueFromControl:(id)sender;
- (void) changeValueFromControlMidi:(id)sender;
- (void) hookUpFloat:(float*)f;
- (float) convertToMidiValue:(float)f;
- (float) convertFromMidiValue:(float)f;
- (void) modifyEnabled:(id)sender;

@end
