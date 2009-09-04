//
//  gui.h
//  openFrameworks
//
//  Created by Jonas Jongejan on 03/09/09.
//  Copyright 2009 HalfdanJ. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface ListView : NSTableView
{
}

@end


@interface contentView : NSView
{
}
- (void)drawRect:(NSRect)rect;


@end



@interface gui : NSObject {
	IBOutlet NSView * mainView;	
	IBOutlet contentView * contentArea;
	IBOutlet ListView * listView;
	
	IBOutlet NSView *testView;

}

- (id)tableView:(NSTableView *)aTableView;


- (IBAction)doSomething:(id)sender;
@end
