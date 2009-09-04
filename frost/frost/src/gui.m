//
//  gui.m
//  openFrameworks
//
//  Created by Jonas Jongejan on 03/09/09.
//  Copyright 2009 HalfdanJ. All rights reserved.
//

#import "gui.h"

@implementation ListView



@end




@implementation contentView

- (void)drawRect:(NSRect)rect{
	NSBezierPath *bp = [NSBezierPath bezierPathWithRect:rect];
	NSColor *color = [NSColor  controlHighlightColor];
	[color set];
	[bp fill];
}


@end


@implementation gui
- (IBAction)doSomething:(id)sender{
	[contentArea addSubview:testView];
}



- (id)tableView:(NSTableView *)aTableView
objectValueForTableColumn:(NSTableColumn *)aTableColumn
			row:(int)rowIndex
{
    id theRecord, theValue;
	/*	
	 NSParameterAssert(rowIndex >= 0 && rowIndex < [records count]);
	 theRecord = [records objectAtIndex:rowIndex];
	 theValue = [theRecord objectForKey:[aTableColumn identifier]];*/
    return @"test";
}


- (void)tableView:(NSTableView *)aTableView
   setObjectValue:anObject
   forTableColumn:(NSTableColumn *)aTableColumn
			  row:(int)rowIndex
{
    id theRecord;
	/*
	 NSParameterAssert(rowIndex >= 0 && rowIndex < [records count]);
	 theRecord = [records objectAtIndex:rowIndex];
	 [theRecord setObject:anObject forKey:[aTableColumn identifier]];*/
    return;
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return 3;
}

@end
