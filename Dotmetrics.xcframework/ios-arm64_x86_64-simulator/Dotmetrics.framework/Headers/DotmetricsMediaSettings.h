//
//  DotmetricsMediaSettings.h
//  Dotmetrics
//
//  Created by Denis Jakuš on 05.01.2022..
//  Copyright © 2022 Telekomunikacijske Usluge. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum {
    VIDEO_ON_DEMAND = 0,
    VIDEO_LIVE,
    AUDIO_ON_DEMAND,
    AUDIO_LIVE,
    AD
} MediaType;

@interface DotmetricsMediaSettings: NSObject

@property (nonatomic, copy) NSString *name;
@property (nonatomic) long duration;
@property (nonatomic, copy) NSString *playerName;
@property (nonatomic, copy) NSString *playerID;
@property (nonatomic, copy) NSString *category;
@property (nonatomic, copy) NSString *format;
@property (nonatomic) MediaType type;

- (instancetype)initWithName:(NSString *)name duration:(long)duration playerName:(NSString *)playerName playerID:(NSString *)playerID category:(NSString *)category format:(NSString *)format type:(MediaType)type;

- (NSString*)mediaTypeToString;


@end
