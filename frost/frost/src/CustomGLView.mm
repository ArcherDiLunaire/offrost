#import "GLee.h"
#import "CustomGLView.h"
#import <OpenGL/gl.h>
#import <stdlib.h>

// ===================================

@implementation CustomGLView

// pixel format definition - prob could be tweaked a lot
+ (NSOpenGLPixelFormat*) basicPixelFormat
{
	return [super basicPixelFormat];
}



#pragma mark ---- IB Actions ----

-(IBAction) animate: (id) sender
{
	fAnimate = 1 - fAnimate;
	if (fAnimate)
		[animateMenuItem setState: NSOnState];
	else 
		[animateMenuItem setState: NSOffState];
}

// ---------------------------------

-(IBAction) info: (id) sender
{
	fInfo = 1 - fInfo;
	if (fInfo)
		[infoMenuItem setState: NSOnState];
	else
		[infoMenuItem setState: NSOffState];
	[self setNeedsDisplay: YES];
}

#pragma mark ---- Method Overrides ----

-(void)keyDown:(NSEvent *)theEvent
{
    NSString *characters = [theEvent characters];
    if ([characters length]) {
        unichar character = [characters characterAtIndex:0];
		delegate->handleKeyDown( character );
	}
}

// ---------------------------------

- (void)mouseDown:(NSEvent *)theEvent 
{
    if ([theEvent modifierFlags] & NSControlKeyMask) 
		[self rightMouseDown:theEvent];
	else if ([theEvent modifierFlags] & NSAlternateKeyMask) 
		[self otherMouseDown:theEvent];
	else {
		NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
		delegate->handleMouseDown( location.x, location.y, 0 );
	}
}

// ---------------------------------

- (void)rightMouseDown:(NSEvent *)theEvent
{
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	delegate->handleMouseDown( location.x, location.y, 1 );
}

// ---------------------------------

- (void)otherMouseDown:(NSEvent *)theEvent
{
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];	
	delegate->handleMouseDown( location.x, location.y, 2 );
}

- (void)mouseMoved:(NSEvent *)theEvent{
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];	
	delegate->handleMouseMove( location.x, location.y );
}


// ---------------------------------

- (void)mouseUp:(NSEvent *)theEvent
{
	delegate->handleMouseUp( 0 );
}

// ---------------------------------

- (void)rightMouseUp:(NSEvent *)theEvent
{
	delegate->handleMouseUp( 1 );
}

// ---------------------------------

- (void)otherMouseUp:(NSEvent *)theEvent
{
	delegate->handleMouseUp( 2 );
}

// ---------------------------------

- (void)mouseDragged:(NSEvent *)theEvent
{
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	delegate->handleMouseDragged( location.x, location.y, 0 );
}

// ---------------------------------

- (void)scrollWheel:(NSEvent *)theEvent
{
}

// ---------------------------------

- (void)rightMouseDragged:(NSEvent *)theEvent
{
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	delegate->handleMouseDragged( location.x, location.y, 1 );
}

// ---------------------------------

- (void)otherMouseDragged:(NSEvent *)theEvent
{
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	delegate->handleMouseDragged( location.x, location.y, 2 );
	
}

- (void)animationTimer:(NSTimer *)timer
{
	
/*
	Update window params here
*/
	if( delegate->m_NewTitle )
	{
		NSString *stringFromUTFString = [[NSString alloc] initWithUTF8String:delegate->getTitle().c_str() ];
		[ [self window] setTitle: stringFromUTFString];
		delegate->m_NewTitle = false;
	}
	
	if( delegate->m_NewSize )
	{
		NSRect newFrame  = [[self window] frame];
		NSRect currFrame = [self frame];
		
		newFrame.size = NSMakeSize(delegate->getWidth(), delegate->getHeight());
		newFrame.origin.y += currFrame.size.height - delegate->getHeight() + 40; //the 40 takes into account the top and bottom bars
		
		[[self window] setFrame:newFrame display:NO];
		delegate->m_NewSize = false;
	}
	
	if( delegate->m_NewPosition )
	{
		NSRect screenRect = [[NSScreen mainScreen] frame];
		
		NSPoint point;
		point.x = delegate->getPositionX();
		point.y = screenRect.size.height - (delegate->getPositionX()+20); //takes into account the 20 pixels of the menubar
		
		[[self window] setFrameTopLeftPoint:point];
		
		delegate->m_NewPosition = false;
	}
	
	if( delegate->m_NewMode && delegate->isFullScreen() )
	{
		[self goFullscreen];
		delegate->m_NewMode = false;
	}
	else if( delegate->m_NewMode && !delegate->isFullScreen() )
	{
		[self goWindow];
		delegate->m_NewMode = false;
	}
	
	delegate->update();
	
	[self drawRect:[self bounds]]; // redraw now instead dirty to enable updates during live resize
}

// ---------------------------------

- (void) drawRect:(NSRect)rect
{		
    [[self openGLContext] makeCurrentContext];
	NSRect rectView = [self bounds];
	
	delegate->render( rectView.size.width, rectView.size.height );
	
	[[self openGLContext] flushBuffer];
	
}

// ---------------------------------

- (void) update // window resizes, moves and display changes (resize, depth and display config change)
{
}

// ---------------------------------

-(id) initWithFrame: (NSRect) frameRect
{
	//creation of the GL  context
	NSOpenGLPixelFormat * pf = [CustomGLView basicPixelFormat];
	self = [super initWithFrame: frameRect pixelFormat: pf];
	
    return self;
}

// ---------------------------------

- (BOOL)acceptsFirstResponder
{
	return YES;
}

// ---------------------------------

- (BOOL)becomeFirstResponder
{
	return  YES;
}

// ---------------------------------

- (BOOL)resignFirstResponder
{
	return YES;
}

// ---------------------------------
- (void) goFullscreen{
	if([self respondsToSelector:@selector(isInFullScreenMode)]){
		if(![self isInFullScreenMode]){
			[self enterFullScreenMode:[NSScreen mainScreen]
						  withOptions:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:NO] forKey:NSFullScreenModeAllScreens] ];
		}
	}
}

// ---------------------------------
- (void) goWindow{
	if([self respondsToSelector:@selector(isInFullScreenMode)]){
		if([self isInFullScreenMode]){
			[self exitFullScreenModeWithOptions:nil];
		}
	}
}

// ---------------------------------
- (void) toggleFullscreen{
	if([self respondsToSelector:@selector(isInFullScreenMode)])
	{
		if([self isInFullScreenMode])
		{
			[self exitFullScreenModeWithOptions:nil];
		}
		else
		{
			[self enterFullScreenMode:[NSScreen mainScreen]
						  withOptions:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:NO] forKey:NSFullScreenModeAllScreens] ];
		}
	}
}

// ---------------------------------

- (void) awakeFromNib
{
	[super awakeFromNib];
	
	fInfo = 1;
	fAnimate = 1;
	time = CFAbsoluteTimeGetCurrent ();  // set animation time start time
	fDrawHelp = 1;

/*
	Create our custom delegate
*/
	delegate = new CustomGLViewDelegate( );
	
//lets set the window size to the requested size - this is duplicated in update - so maybe making some nice functions could be a good idea
	NSRect currFrame = [self frame];
	NSRect newFrame = [[self window] frame];
	newFrame.size = NSMakeSize( delegate->getWidth(), delegate->getHeight() );
	newFrame.origin.y += currFrame.size.height - delegate->getHeight() + 40; //the 40 takes into account the top and bottom bars
	[[self window] setFrame:newFrame display:NO];
	
	//need this to get mouse moved events
	[[self window] setAcceptsMouseMovedEvents:YES]; 
	
	/*
		Our custom view does not have to have the same FPS as main view.
	*/
	timer = [NSTimer timerWithTimeInterval:(1.0f/30.0f) target:self selector:@selector(animationTimer:) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode]; // ensure timer fires during resize
	
	/// register delegate with main application
	delegate->registerWithMainApplication();
}


@end
