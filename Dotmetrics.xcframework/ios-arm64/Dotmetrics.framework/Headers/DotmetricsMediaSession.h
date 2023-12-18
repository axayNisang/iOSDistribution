//
//  DotmetricsMediaSession.h
//  Dotmetrics
//
//  Created by Denis Jakuš on 10.01.2022..
//  Copyright © 2022 Telekomunikacijske Usluge. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "DotmetricsMediaItem.h"

@interface DotmetricsMediaSession: NSObject

+ (DotmetricsMediaSession*) openNewSession;
- (void) addMediaItem:(DotmetricsMediaItem*) item;
- (void) attachPlayer:(id<IDotmetricsMediaPlayer>) item;
- (void) close;
- (void) saveCurrentState;

@end
