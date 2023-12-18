//
//  DotmetricsAVPlayerWrapper.h
//  Dotmetrics
//
//  Created by Denis Jakuš on 07.01.2022..
//  Copyright © 2022 Telekomunikacijske Usluge. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DotmetricsMediaItem.h"

@interface DotmetricsAVPlayerWrapper: NSObject <IDotmetricsMediaPlayer>

- (instancetype)initWithPlayer:(AVPlayer *)value;

@end
