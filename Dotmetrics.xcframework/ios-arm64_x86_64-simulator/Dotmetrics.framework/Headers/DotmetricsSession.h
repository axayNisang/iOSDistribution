//  DotmetricsSession.h
//  Copyright (C) 2012 Char Software Inc., DBA Localytics
//
//  This code is provided under the Localytics Modified BSD License.
//  A copy of this license has been distributed in a file called LICENSE
//  with this source code.
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "DotmetricsMediaSession.h"

#define CLIENT_VERSION              @"1.7.5"      // The version of this library

/*!
 @class DotmetricsSession
 @discussion The class which manages creating, collecting, & uploading a Dotmetrics session.
 Please see the following guides for information on how to best use this
 
 <strong>Best Practices</strong>
 <ul>
 <li>Instantiate the DotmetricsSession object in applicationDidFinishLaunching.</li>
 <li>Open your session and begin your uploads in applicationDidFinishLaunching. This way the
 upload has time to complete and it all happens before your users have a
 chance to begin any data intensive actions of their own.</li>
 <li>Close the session in applicationWillTerminate, and in applicationDidEnterBackground.</li>
 <li>Resume the session in applicationWillEnterForeground.</li>
 <li>Do not call any Dotmetrics functions inside a loop.  Instead, calls
 such as <code>tagEvent</code> should follow user actions.  This limits the
 amount of data which is stored and uploaded.</li>
 <li>Do not use multiple LocalticsSession objects to upload data with
 multiple application keys.  This can cause invalid state.</li>
 </ul>
 
 @author Dotmetrics
 */

// Optional delegate. Set this delegate if you require the following callbacks

@protocol DotmetricsSessionDelegate <NSObject>
@optional

/*!
 @method DotmetricsResumedSession
 @abstract Register for this callback to be notified when Dotmetrics has either 
 resumed a previous session or created a new one. See the on the 'resume' method 
 for additional details.
 @param didResumeExistingSession This flag will indicate if Dotmetrics restored an existing
 session or started a new one. 
 */
- (void)DotmetricsResumedSession:(BOOL)didResumeExistingSession;

@end

@interface DotmetricsSession : NSObject {
    
	BOOL _hasInitialized;               // Whether or not the session object has been initialized.
	BOOL _isSessionOpen;                // Whether or not this session has been opened.
    float _backgroundSessionTimeout;    // If an App stays in the background for more
    // than this many seconds, start a new session
    // when it returns to foreground.
@private
#pragma mark Member Variables
    dispatch_queue_t _queue;                // Queue of Dotmetrics block objects.
    dispatch_group_t _criticalGroup;        // Group of blocks the must complete before backgrounding.
	NSString *_applicationKey;              // Unique identifier for the instrumented application
    NSTimeInterval _lastSessionStartTimestamp;  // The start time of the most recent session.
    long _sessionBackgroundTime;   //Time spend in background in last session
    NSDate *_sessionCloseTime;              // Time session was closed.
    NSMutableString *_unstagedFlowEvents;        // Comma-delimited list of app screens and events tagged during this
    // session that have NOT been staged for upload.
    NSMutableString *_stagedFlowEvents;        // App screens and events tagged during this session that HAVE been staged
    // for upload.
    NSMutableString *_screens;              // Comma-delimited list of screens tagged during this session.
	BOOL _sessionHasBeenOpen;               // Whether or not this session has ever been open.
}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
@property (nonatomic,readonly) dispatch_queue_t queue;
@property (nonatomic,readonly) dispatch_group_t criticalGroup;
@property (atomic) BOOL isSessionOpen;
@property (atomic) BOOL hasInitialized;
@property (atomic) float backgroundSessionTimeout;
@property (nonatomic, assign, readonly) NSTimeInterval lastSessionStartTimestamp;
@property (nonatomic, assign, readonly) long sessionBackgroundTime;
@property (nonatomic, assign, readonly) NSInteger sessionNumber;
@property (nonatomic, assign) id<DotmetricsSessionDelegate> DotmetricsDelegate;

@property (nonnull, retain) NSString *callbackScheme;
@property (nonatomic, assign) NSTimeInterval cookieCheckInterval;
@property (nonatomic, assign) NSTimeInterval cookieCheckInitialDelay;
@property (nonatomic, assign) NSTimeInterval openBrowserTimeout;

@property (nonatomic, retain) NSString *cookieURL;
@property (nonatomic, retain) NSString *cookieName;
@property (nonatomic, retain) NSString *cookieUploadURL;
@property (nonatomic, retain) NSString *cookieDownloadURL;
@property (nonatomic, assign) BOOL cookieFlowDisabled;
@property (nonatomic, retain) NSString *cookieDomain;
@property (nonatomic, retain) NSURL *url;
@property (nonatomic, assign) NSMutableArray* mediaSessions;

+ (void)logMessage:(NSString *)message;

/*!
 @property enableHTTPS
 @abstract Determines whether or not HTTPS is used when calling the Dotmetrics
 post URL. The default is NO.
 */
@property (nonatomic, assign) BOOL enableHTTPS;

/*!
 @property loggingEnabled
 @abstract Determines whether or Dotmetrics debugging information is shown
 to the console. The default is NO
 */
@property (nonatomic, assign) BOOL loggingEnabled;

#pragma mark Public Methods
// Configures and starts a session from the given URL. Calls completionHandler when done, with error
- (void)configureAndStartWithConfigurationURL:(NSURL *)url completionHandler:(void (^)(NSError *))completionHandler;
// Switches to new ApiKey, and starts new session. Calls completionHandler when done, with error
- (void)switchToNewEndpointWithConfigurationURL:(NSURL *)url completionHandler:(void (^)(NSError *))completionHandler;

/*!
 @method sharedDotmetricsSession
 @abstract Accesses the Session object.  This is a Singleton class which maintains
 a single session throughout your application.  It is possible to manage your own
 session, but this is the easiest way to access the Dotmetrics object throughout your code.
 The class is accessed within the code using the following syntax:
 [[DotmetricsSession sharedDotmetricsSession] functionHere]
 So, to tag an event, all that is necessary, anywhere in the code is:
 [[DotmetricsSession sharedDotmetricsSession] tagEvent:@"MY_EVENT"];
 */
+ (DotmetricsSession *)sharedDotmetricsSession;

/*!
 @method DotmetricsSession
 @abstract Initializes the Dotmetrics Object.  Not necessary if you choose to use startSession.
 @param applicationKey The key unique for each application
 */
- (void)DotmetricsSession:(NSString *)appKey;

/*!
 @method startSession
 @abstract An optional convenience initialize method that also calls the DotmetricsSession, open &
 upload methods. Best Practice is to call open & upload immediately after Dotmetrics Session when loading an app,
 this method fascilitates that behavior.
 It is recommended that this call be placed in <code>applicationDidFinishLaunching</code>.
 @param applicationKey The key unique for each application
 */
- (void)startSession:(NSString *)appKey serviceURL:(NSString *)serviceURL;

/*!
 @method setOptIn
 @abstract (OPTIONAL) Allows the application to control whether or not it will collect user data.
 Even if this call is used, it is necessary to continue calling upload().  No new data will be
 collected, so nothing new will be uploaded but it is necessary to upload an event telling the
 server this user has opted out.
 @param optedIn True if the user is opted in, false otherwise.
 */
- (void)setOptIn:(BOOL)optedIn;

/*!
 @method tagEvent
 @abstract Allows a session to tag a particular event as having occurred.  For
 example, if a view has three buttons, it might make sense to tag
 each button click with the name of the button which was clicked.
 For another example, in a game with many levels it might be valuable
 to create a new tag every time the user gets to a new level in order
 to determine how far the average user is progressing in the game.
 <br>
 <strong>Tagging Best Practices</strong>
 <ul>
 <li>DO NOT use tags to record personally identifiable information.</li>
 <li>The best way to use tags is to create all the tag strings as predefined
 constants and only use those.  This is more efficient and removes the risk of
 collecting personal information.</li>
 <li>Do not set tags inside loops or any other place which gets called
 frequently.  This can cause a lot of data to be stored and uploaded.</li>
 </ul>
 <br>
 @param event The name of the event which occurred.
 */
- (void)tagEvent:(NSString *)event;

- (void)tagEvent:(NSString *)event
      attributes:(NSDictionary *)attributes;

- (void)tagEvent:(NSString *)event
      attributes:(NSDictionary *)attributes
customerValueIncrease:(NSNumber *)customerValueIncrease;

- (void)tagEvent:(NSString *)event
      attributes:(NSDictionary *)attributes
reportAttributes:(NSDictionary *)reportAttributes;

- (void)tagEvent:(NSString *)event
      attributes:(NSDictionary *)attributes
reportAttributes:(NSDictionary *)reportAttributes
customerValueIncrease:(NSNumber *)customerValueIncrease;


/*!
 @method tagScreen
 @abstract Allows tagging the flow of screens encountered during the session.
 @param screen The name of the screen
 */
- (void)tagScreen:(NSString *)screen;

/*!
 @method upload
 @abstract Creates a low priority thread which uploads any Dotmetrics data already stored
 on the device.  This should be done early in the process life in order to
 guarantee as much time as possible for slow connections to complete.  It is also reasonable
 to upload again when the application is exiting because if the upload is cancelled the data
 will just get uploaded the next time the app comes up.
 */
- (void)upload:(void (^)(NSError *))completionBlock;


/*!
 @method close
 @abstract Closes the Dotmetrics session.  This should be called in
 <code>applicationWillTerminate</code>.
 <br>
 If close is not called, the session will still be uploaded but no
 events will be processed and the session time will not appear. This is
 because the session is not yet closed so it should not be used in
 comparison with sessions which are closed.
 */
- (void)close;

/*!
 @method setLocation
 @abstract Stores the user's location.  This will be used in all event and session calls.
 If your application has already collected the user's location, it may be passed to Dotmetrics
 via this function.  This will cause all events and the session close to include the locatin
 information.  It is not required that you call this function.
 @param deviceLocation The user's location.
 */
- (void)setLocation:(CLLocationCoordinate2D)deviceLocation;


/*!
 @method setCustomDimension
 @abstract (ENTERPRISE ONLY) Sets the value of a custom dimension. Custom dimensions are dimensions
 which contain user defined data unlike the predefined dimensions such as carrier, model, and country.
 Once a value for a custom dimension is set, the device it was set on will continue to upload that value
 until the value is changed. To clear a value pass nil as the value.
 The proper use of custom dimensions involves defining a dimension with less than ten distinct possible
 values and assigning it to one of the four available custom dimensions. Once assigned this definition should
 never be changed without changing the App Key otherwise old installs of the application will pollute new data.
 */
- (void)setCustomDimension:(int)dimension value:(NSString *)value;

/*!
 @method setValueForIdentifier
 @abstract (ENTERPRISE ONLY) Sets the value of a custom identifier. Identifiers are a form of key/value storage
 which contain custom user data. Identifiers might include things like email addresses, customer IDs, twitter
 handles, and facebook IDs.
 Once a value is set, the device it was set on will continue to upload that value until the value is changed.
 To delete a property, pass in nil as the value.
 */
- (void)setValueForIdentifier:(NSString *)identifierName value:(NSString *)value;

/*!
 @method setCustomerName
 @abstract (ENTERPRISE ONLY) Record the customer name
 Once this value is set, the device it was set on will continue to upload that value until the value is changed.
 To delete the value, pass in nil.
 */
- (void)setCustomerName:(NSString *)email;

/*!
 @method setCustomerId
 @abstract (ENTERPRISE ONLY) Record your custom customer identifier
 Once this value is set, the device it was set on will continue to upload that value until the value is changed.
 To delete the value, pass in nil.
 */
- (void)setCustomerId:(NSString *)customerId;

/*!
 @method setCustomerId
 @abstract (ENTERPRISE ONLY) Record the customer's email address
 Once this value is set, the device it was set on will continue to upload that value until the value is changed.
 To delete the value, pass in nil.
 */
- (void)setCustomerEmail:(NSString *)email;

- (void)parseCallbackURL:(NSURL *)url;
#pragma clang diagnostic pop

- (void)sendFingerprint;    //@TODO

// Private methods
- (void)ll_open;

- (BOOL)ll_isOptedIn;
    
- (NSString *_Nullable)identifierForVendor;

- (void)applyTCF:(BOOL)value;

- (void)saveMediaData:(NSObject *_Nullable)model;

- (void)saveMediaDataAsync:(NSObject *_Nullable)model;

- (DotmetricsMediaSession*_Nonnull)openMediaSession;

- (void)shouldUseIDFA:(BOOL)value;

// Internal logging macros. You should not need to modify this
#define Dotmetrics_LOGGING_ENABLED [[DotmetricsSession sharedDotmetricsSession] loggingEnabled]
#define DotmetricsLog(message, ...)if([[DotmetricsSession sharedDotmetricsSession] loggingEnabled]) \
[DotmetricsSession logMessage:[NSString stringWithFormat:@"%s:\n + " message "\n\n", __PRETTY_FUNCTION__, ##__VA_ARGS__]]

@end
