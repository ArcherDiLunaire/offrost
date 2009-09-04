//
//  SharedContextGLView.h
//  openFrameworks
//
//  Created by Edvin Besic on 7/13/09.
//  Copyright 2009 Daddy. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>


@interface SharedContextGLView : NSOpenGLView
{
	NSOpenGLContext*	mySharedOpenGLContext;
}

+(NSOpenGLPixelFormat*)basicPixelFormat;
-(void)awakeFromNib;

@end
