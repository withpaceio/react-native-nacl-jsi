#import <React/RCTBridgeModule.h>

#ifdef __cplusplus
#import "react-native-nacl-jsi.h"
#endif

@interface NaclJsi : NSObject <RCTBridgeModule>
@property (nonatomic, assign) BOOL setBridgeOnMainQueue;
@end
