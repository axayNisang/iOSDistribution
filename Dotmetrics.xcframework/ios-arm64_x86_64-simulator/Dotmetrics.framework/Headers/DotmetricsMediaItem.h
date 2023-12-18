//
//  DotmetricsMediaItem.h
//  Dotmetrics
//
//  Created by Denis Jakuš on 03.01.2022..
//  Copyright © 2022 Telekomunikacijske Usluge. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "DotmetricsMediaSettings.h"

@protocol IDotmetricsMediaPlayer

@required
- (void)saveCurrentState;
- (void)close;
@end

@interface DotmetricsMediaItem: NSObject <IDotmetricsMediaPlayer>

+ (DotmetricsMediaItem*)createWithSettings:(DotmetricsMediaSettings *)settings;
- (void)setSettings:(DotmetricsMediaSettings *)value;
- (void)setDummyPlaybackPosition:(BOOL)flag;
- (void)mediaPlay:(long)offset;
- (void)mediaStop:(long)offset;
- (void)mediaForceStop;
- (void)mediaStartBuffering:(long)offset;
- (void)mediaStopBuffering;
- (void)saveCurrentState;
- (void)close;

@end
