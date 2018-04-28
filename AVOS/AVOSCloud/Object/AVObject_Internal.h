//
//  AVObject_Internal.h
//  paas
//
//  Created by Zhu Zeng on 3/6/13.
//  Copyright (c) 2013 AVOS. All rights reserved.
//

#import "AVObject.h"
#import "AVRequestOperation.h"
#import "AVRequestManager.h"

FOUNDATION_EXPORT NSString *const internalIdTag;

NS_INLINE
NSString *request_path(NSDictionary *request) {
    return request[@"path"];
}

NS_INLINE
NSString *request_method(NSDictionary *request) {
    return request[@"method"];
}

NS_INLINE
NSString *request_internal_id(NSDictionary *request) {
    return request[@"body"][internalIdTag];
}

NS_INLINE
NSString *request_object_id(NSDictionary *request) {
    return request[@"body"][@"objectId"];
}

@interface AVObject ()

@property (nonatomic,   copy) NSString *objectId;
@property (nonatomic,   copy) NSString *className;
@property (nonatomic, strong) NSDate *updatedAt;
@property (nonatomic, strong) NSDate *createdAt;

@property (nonatomic, strong) NSMutableDictionary *localData;
@property (nonatomic, strong) NSMutableDictionary *estimatedData;
@property (nonatomic, strong) NSMutableDictionary *relationData;

@property (nonatomic,   copy, readonly) NSString *uuid;
@property (nonatomic, assign) BOOL isPointer;
@property (nonatomic, assign) BOOL running;
@property (nonatomic, strong) AVRequestOperationQueue *operationQueue;
@property (nonatomic, strong) AVRequestManager *requestManager;

@property (nonatomic, assign) BOOL submit;
@property (nonatomic, assign) BOOL inSetter;
@property (nonatomic, assign) BOOL hasDataForCloud;
@property (nonatomic, assign) BOOL hasDataForInitial;

- (NSDictionary *)localDataCopy;

- (void)updateLocalDataWithKey:(NSString *)key object:(id)object;

-(NSMutableArray *)findArrayForKey:(NSString *)key
                inDictionary:(NSMutableDictionary *)dict
                      create:(BOOL)create;

-(void)addRelation:(AVObject *)object
            forKey:(NSString *)key
            submit:(BOOL)submit;
-(void)removeRelation:(AVObject *)object
               forKey:(NSString *)key;

-(NSError *)preSave;
-(void)postSave;

- (void)postDelete;

// override to provide custom post data for create, put and post.
-(NSMutableDictionary *)postData;
-(NSMutableDictionary  *)initialBodyData;

-(void)refreshHasDataForCloud;
-(NSMutableArray *)dataForCloud;
-(NSMutableArray *)jsonDataForCloudWithClear:(BOOL)clear;
-(NSDictionary *)headerMap;

// override to provide custom classname for restful.
-(NSString *)internalClassName;
-(NSString *)childClassNameForRelation:(NSString *)key;
-(NSString *)myObjectPath;

- (NSMutableArray *) buildSaveRequests;

-(BOOL)hasValidObjectId;

- (void)setObject:(id)object
           forKey:(NSString *)key
           submit:(BOOL)submit;

-(void)addSetRequest:(NSString *)key
              object:(NSObject *)object;

- (BOOL)saveWithOption:(AVSaveOption *)option
            eventually:(BOOL)eventually
          verifyBefore:(BOOL)verifyBefore
                 error:(NSError **)error;

-(void)removeLocalData;
-(void)addInternalId:(NSMutableDictionary *)dict;
-(void)addDefaultACL:(NSMutableDictionary *)dict;

/* 针对 save 或 update 请求的 method 和 path */
- (NSString *)initialRequestMethod;
- (NSString *)initialRequestPath;

/**
 Post process batch requests.

 Subclass can override this method to add custom logic.
 */
- (void)postProcessBatchRequests:(NSMutableArray *)requests;

/*
 * Indicate that whether a batch saving should include update requests.
 * Update requests are generated by modifying object. Some objects may should not include
 * modification when saving because of some prerequests not satisfied.
 * For example, if an user is not login, update that user will failed.
 * So, we should not include update requests when sign up user.
 */
- (BOOL)shouldIncludeUpdateRequests;

/**
 * Get snapshot of object.
 *
 * @return The snapshot of object
 */
- (NSDictionary *)snapshot;

/**
 * Check wether an object is dirty.
 *
 * @return true if object is dirty, for example, has been modified or expired, false otherwise.
 */
- (BOOL)isDirty;

@end
