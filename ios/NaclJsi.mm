#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>

#import "NaclJsi.h"

@implementation NaclJsi

@synthesize bridge = _bridge;
@synthesize methodQueue = _methodQueue;

RCT_EXPORT_MODULE()

+ (BOOL)requiresMainQueueSetup {
  return YES;
}

- (void)setBridge:(RCTBridge *)bridge {
  _bridge = bridge;
  _setBridgeOnMainQueue = RCTIsMainQueue();
  
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)self.bridge;
  if (!cxxBridge.runtime) {
    return;
  }
  
  react_native_nacl::install(*(facebook::jsi::Runtime *)cxxBridge.runtime);
}

- (void)invalidate {
  react_native_nacl::cleanup();
}

@end
