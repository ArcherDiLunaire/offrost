//
//  SharedContextGLView.m
//  openFrameworks
//
//  Created by Edvin Besic on 7/13/09.
//  Copyright 2009 Daddy. All rights reserved.
//

#import "SharedContextGLView.h"

// reference to the first context created, used by all others
// as a shareContext reference
NSOpenGLContext* __sharedOpenGLContext;

@implementation SharedContextGLView


+(NSOpenGLPixelFormat*)basicPixelFormat
{
	NSOpenGLPixelFormatAttribute attributes [] = {
		NSOpenGLPFAWindow,
		NSOpenGLPFADoubleBuffer,	// double buffered
		NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)16, // 16 bit depth buffer
		(NSOpenGLPixelFormatAttribute)nil
	};
	return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

-(void)awakeFromNib
{
	printf( "SharedOpenGLContext::awakeFromNib\n" );

/// first time
	if( __sharedOpenGLContext == nil )
	{
		__sharedOpenGLContext = mySharedOpenGLContext = [[NSOpenGLContext alloc] initWithFormat:[SharedContextGLView basicPixelFormat] shareContext:nil];
		printf( "SharedOpenGLContext: Created main context\n" );
	}
	else
	{
		mySharedOpenGLContext = [[NSOpenGLContext alloc] initWithFormat:[SharedContextGLView basicPixelFormat] shareContext:__sharedOpenGLContext];
		printf( "SharedOpenGLContext: Created secondary context\n" );
	}

/// set and make current
	[self setOpenGLContext:mySharedOpenGLContext];
	[mySharedOpenGLContext makeCurrentContext];
}

@end
