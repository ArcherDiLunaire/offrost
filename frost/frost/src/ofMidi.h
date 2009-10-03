//
//  ofMidi.h
//  openFrameworks
//
//  Created by Jonas Jongejan on 02/10/09.
//  Copyright 2009 HalfdanJ. All rights reserved.
//
#import "GLee.h"


#import <Cocoa/Cocoa.h>

#include "PYMIDI.h"
#include "OFGuiController.h"

@interface ofMidi : NSObject {
	PYMIDIManager* manager;
	PYMIDIEndpoint * endpoint;
	IBOutlet OFGuiController * gui;
}

-(void) awakeFromNib;

@end
