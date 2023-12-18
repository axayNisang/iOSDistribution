//
//  DotmetricsYTPlayerViewWrapper.h
//  Dotmetrics
//
//  Created by Denis Jakuš on 25.01.2022..
//  Copyright © 2022 Telekomunikacijske Usluge. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "YTPlayerView.h"
#import "DotmetricsMediaItem.h"

@interface DotmetricsYTPlayerViewWrapper: NSObject <IDotmetricsMediaPlayer, YTPlayerViewDelegate>

- (instancetype)initWithPlayer:(YTPlayerView *)value;

@end
