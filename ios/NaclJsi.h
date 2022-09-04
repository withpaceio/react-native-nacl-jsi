#import <React/RCTBridgeModule.h>

#import "react-native-nacl-jsi.h"
#import "sodium.h"

@interface NaclJsi : NSObject <RCTBridgeModule>
@property (nonatomic, assign) BOOL setBridgeOnMainQueue;
@end
