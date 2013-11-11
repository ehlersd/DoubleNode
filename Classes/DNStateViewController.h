//
//  DNStateViewController.h
//  Phoenix
//
//  Created by Darren Ehlers on 11/9/13.
//  Copyright (c) 2013 Table Project. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "DNStateOptions.h"

@interface DNStateViewController : UIViewController
{
    NSString*   currentViewState;
}

- (void)changeToViewState:(NSString*)newViewState
                 animated:(BOOL)animated;

- (void)changeFromCurrentState:(NSString*)currentState
                    toNewState:(NSString*)newState
                      animated:(BOOL)animated;

- (void)performViewStateSelector:(SEL)aSelector
                        animated:(BOOL)animated;

@end
