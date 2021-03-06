//
//  DNCommunicationsAPI.h
//  DoubleNode.com
//
//  Derived from work originally created by Darren Ehlers
//  Portions Copyright (c) 2012 DoubleNode.com and Darren Ehlers.
//  All rights reserved.
//

#import <Foundation/Foundation.h>

#import "DNCommunicationDetails.h"
#import "DNCommunicationPageDetails.h"

#import "DNUtilities.h"

typedef NSArray*(^DNCommunicationsAPIIncomingHandlerBlock)(DNCommunicationDetails* commDetails, DNCommunicationPageDetails* pageDetails, NSDictionary* response, NSDictionary* headers);
typedef void(^DNCommunicationsAPICompletionHandlerBlock)(DNCommunicationDetails* commDetails, DNCommunicationPageDetails* pageDetails, NSArray* objects);
typedef void(^DNCommunicationsAPIErrorHandlerBlock)(DNCommunicationDetails* commDetails, DNCommunicationPageDetails* pageDetails, NSInteger responseCode, NSError* error, NSTimeInterval retryRecommendation);

typedef BOOL(^APIProcessingNowBlock)(NSArray* objects);

@interface DNCommunicationsAPIQueued : NSObject

@property (nonatomic, copy) BOOL        (^filterHandler)(id item);
@property (nonatomic, copy) NSArray*    (^incomingHandler)(DNCommunicationDetails* commDetails, DNCommunicationPageDetails* pageDetails, NSDictionary* response, NSDictionary* headers);
@property (nonatomic, copy) void        (^completionHandler)(DNCommunicationDetails* commDetails, DNCommunicationPageDetails* pageDetails, NSArray* objects);
@property (nonatomic, copy) void        (^errorHandler)(DNCommunicationDetails* commDetails, DNCommunicationPageDetails* pageDetails, NSInteger responseCode, NSError* error, NSTimeInterval retryRecommendation);

@end

@interface DNCommunicationsAPI : NSObject

+ (id)manager;
+ (void)clearCache;

- (id)init;

- (NSURLRequest*)addAuthorizationHeader:(NSURLRequest*)request
                            commDetails:(DNCommunicationDetails*)commDetails;

- (void)reauthorizeWithCommDetails:(DNCommunicationDetails*)commDetails
                           success:(void (^)(void))success
                           failure:(void (^)(void))failure;

- (NSString*)cacheKey:(DNCommunicationDetails*)commDetails
      withPageDetails:(DNCommunicationPageDetails*)pageDetails;

- (BOOL)isCacheExpired:(DNCommunicationDetails*)commDetails
       withPageDetails:(DNCommunicationPageDetails*)pageDetails
               withTTL:(NSUInteger)ttl;

- (void)markCacheUpdated:(DNCommunicationDetails*)commDetails
         withPageDetails:(DNCommunicationPageDetails*)pageDetails;

- (void)markCacheExpired:(DNCommunicationDetails*)commDetails
         withPageDetails:(DNCommunicationPageDetails*)pageDetails;

- (NSString*)getFirstPartMethod:(NSString*)methodName;

- (NSInteger)apiPageSizeRetrieve:(NSString*)apikey;
- (NSInteger)apiPageSizeRetrieve:(NSString*)apikey
                         default:(NSInteger)defaultPageSize;
- (NSInteger)apiTTLRetrieve:(NSString*)apikey;
- (NSInteger)apiTTLRetrieve:(NSString*)apikey
                    default:(NSInteger)defaultTTL;
- (void)resetRetryRecommendation:(NSString*)apikey;
- (NSTimeInterval)retryRecommendation:(NSString*)apikey;

- (BOOL)processNow:(DNCommunicationDetails*)commDetails
               now:(void(^)(DNCommunicationDetails* commDetails, NSArray* objects, BOOL isExpired))nowHandler;

- (BOOL)processNow:(DNCommunicationDetails*)commDetails
           objects:(NSArray*)objects
            filter:(BOOL(^)(id object))filterHandler
               now:(void(^)(DNCommunicationDetails* commDetails, NSArray* objects, BOOL isExpired))nowHandler;

- (void)processGet:(DNCommunicationDetails*)commDetails
            filter:(BOOL(^)(id object))filterHandler
          incoming:(DNCommunicationsAPIIncomingHandlerBlock)incomingHandler
        completion:(DNCommunicationsAPICompletionHandlerBlock)completionHandler
             error:(DNCommunicationsAPIErrorHandlerBlock)errorHandler;

- (void)processPost:(DNCommunicationDetails*)commDetails
             filter:(BOOL(^)(id object))filterHandler
           incoming:(DNCommunicationsAPIIncomingHandlerBlock)incomingHandler
         completion:(DNCommunicationsAPICompletionHandlerBlock)completionHandler
              error:(DNCommunicationsAPIErrorHandlerBlock)errorHandler;

- (void)processPut:(DNCommunicationDetails*)commDetails
            filter:(BOOL(^)(id object))filterHandler
          incoming:(DNCommunicationsAPIIncomingHandlerBlock)incomingHandler
        completion:(DNCommunicationsAPICompletionHandlerBlock)completionHandler
             error:(DNCommunicationsAPIErrorHandlerBlock)errorHandler;

- (void)processDelete:(DNCommunicationDetails*)commDetails
               filter:(BOOL(^)(id object))filterHandler
             incoming:(DNCommunicationsAPIIncomingHandlerBlock)incomingHandler
           completion:(DNCommunicationsAPICompletionHandlerBlock)completionHandler
                error:(DNCommunicationsAPIErrorHandlerBlock)errorHandler;

- (void)subProcessResponse:(NSHTTPURLResponse*)httpResponse
               commDetails:(DNCommunicationDetails*)commDetails
               pageDetails:(DNCommunicationPageDetails*)pageDetails
                 errorCode:(NSInteger)errorCode
                   request:(NSURLRequest*)request
                      data:(NSData*)data
                     retry:(void(^)(DNCommunicationDetails* commDetails, DNCommunicationPageDetails* pageDetails))retryHandler
                    filter:(BOOL(^)(id object))filterHandler
                  incoming:(DNCommunicationsAPIIncomingHandlerBlock)incomingHandler
                completion:(DNCommunicationsAPICompletionHandlerBlock)completionHandler
                     error:(DNCommunicationsAPIErrorHandlerBlock)errorHandler;

- (void)subProcessRequest:(NSURLRequest*)request
              commDetails:(DNCommunicationDetails*)commDetails
              pageDetails:(DNCommunicationPageDetails*)pageDetails
                   filter:(BOOL(^)(id object))filterHandler
                 incoming:(DNCommunicationsAPIIncomingHandlerBlock)incomingHandler
               completion:(DNCommunicationsAPICompletionHandlerBlock)completionHandler
                    error:(DNCommunicationsAPIErrorHandlerBlock)errorHandler;

- (BOOL)queueProcess:(DNCommunicationDetails*)commDetails
         pageDetails:(DNCommunicationPageDetails*)pageDetails
              filter:(BOOL(^)(id object))filterHandler
            incoming:(DNCommunicationsAPIIncomingHandlerBlock)incomingHandler
          completion:(DNCommunicationsAPICompletionHandlerBlock)completionHandler
               error:(DNCommunicationsAPIErrorHandlerBlock)errorHandler;

/*
- (void)processPut:(NSString*)apikey
        withRouter:(id)router
            withID:(id)idValue
        withParams:(NSDictionary*)params
        completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
             error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;

- (void)processPut:(NSString*)apikey
        withRouter:(id)router
            withID:(id)idValue
        withParams:(NSDictionary*)params
         withFiles:(NSArray*)files
        completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
             error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;

- (void)processPost:(NSString*)apikey
         withRouter:(id)router
         withParams:(NSDictionary*)params
         completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
              error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;

- (void)processPost:(NSString*)apikey
         withRouter:(id)router
             withID:(id)idValue
         withParams:(NSDictionary*)params
         completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
              error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;

- (void)processPost:(NSString*)apikey
         withRouter:(id)router
         withParams:(NSDictionary*)params
          withFiles:(NSArray*)files
         completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
              error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;

 - (void)processRequest:(NSString*)apikey
            withRouter:(id)router
                offset:(NSUInteger)offset
                 count:(NSUInteger)count
            completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
                 error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;

- (void)processRequest:(NSString*)apikey
            withRouter:(id)router
                withID:(id)idValue
                offset:(NSUInteger)offset
                 count:(NSUInteger)count
            completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
                 error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;

- (void)processRequest:(NSString*)apikey
            withRouter:(id)router
       withParamString:(NSString*)params
                offset:(NSUInteger)offset
                 count:(NSUInteger)count
            completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
                 error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;

- (void)processRequest:(NSString*)apikey
            withRouter:(id)router
                withID:(id)idValue
       withParamString:(NSString*)params
                offset:(NSUInteger)offset
                 count:(NSUInteger)count
            completion:(void(^)(NSDictionary* response, NSDictionary* headers))completionHandler
                 error:(void(^)(NSInteger responseCode, NSError* error, NSString* url, NSTimeInterval retryRecommendation))errorHandler;
*/

/*
- (void)processingCompletionBlock:(NSString*)apikey
                       withRouter:(id)router
                           withID:(id)idValue
                          objects:(NSArray*)objects
                           filter:(BOOL(^)(id object))filterHandler
                       completion:(void(^)(NSArray* speakers))completionHandler;

- (void)processingCompletionBlock:(NSString*)apikey
                       withRouter:(id)router
                           withID:(id)idValue
                  withParamString:(NSString*)params
                          objects:(NSArray*)objects
                           filter:(BOOL(^)(id object))filterHandler
                       completion:(void(^)(NSArray* speakers))completionHandler;

- (void)processingQueueCompletionBlock:(NSString*)apikey
                            withRouter:(id)router
                               objects:(NSArray*)objects;

- (void)processingQueueErrorBlock:(NSString*)apikey
                       withRouter:(id)router
                     responseCode:(NSInteger)responseCode
                            error:(NSError*)error
                              url:(NSString*)url
              retryRecommendation:(NSTimeInterval)retryRecommendation;
*/

@end
