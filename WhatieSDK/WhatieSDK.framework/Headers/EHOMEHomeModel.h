//
//  EHOMEHomeModel.h
//  WhatieSDK
//
//  Created by clj on 2018/6/30.
//  Copyright © 2018年 IIDreams. All rights reserved.
//

#import "EHOMEBaseObject.h"

@class host;
@interface EHOMEHomeModel : EHOMEBaseObject

#pragma mark - Property of HomeModel

/**
 The unique ID of home, called homeId.
 */
@property (nonatomic, assign) int id;

/** Name of home. */
@property (nonatomic, copy) NSString *name;

/**
 host of home.
 Get the hostID of home by host.id
 */
@property(nonatomic, strong) host *host;

#pragma mark - Functions
/**
 addHomeWithName
 
 add a home with name.
 
 @param name : new home name.
 
 */
-(void)addNewHomeWithName:(NSString *)name
                  success:(successBlock)success
                  failure:(failBlock)failure;


/**
 UpdateHomeName
 
 update home name with name.
 
 @param name : new home name.
 
 */
-(void)updateHomeName:(NSString *)name
              success:(successBlock)success
              failure:(failBlock)failure;


/**
 transferHome
 
 */
-(void)transferHomeWithtransferEmail:(NSString *)email
                             success:(successBlock)success
                             failure:(failBlock)failure;


/**
 exitHome
 
 */
-(void)exitHome:(successBlock)success
        failure:(failBlock)failure;


/**
 transferAdmin
 
 */
-(void)transferAdminWithmemberId:(int)memberId
                         success:(successBlock)success
                         failure:(failBlock)failure;


/**
 inviteHomeMemberWithEmail
 invite member of home with home email
 @param email : new member email.
 
 */
-(void)inviteHomeMemberWithEmail:(NSString *)email
                         success:(successBlock)success
                         failure:(failBlock)failure;

@end


@interface host : EHOMEBaseObject
@property (nonatomic, assign) int id;
@end


@class customer;
@interface EHhomeMemberModel : EHOMEBaseObject
@property (nonatomic, assign) BOOL host;
@property (nonatomic, strong) customer *customer;

/**
 getMemberList
 
 */
-(void)getMemberWithHomeId:(int)homeId
                      List:(successBlock)success
                   failure:(failBlock)failure;

/**
 deleteMember
 
 */
-(void)deleteMemberWithHomeId:(int)homeId
                       hostId:(int)hostId
                     memberId:(int)memberId
                      success:(successBlock)success
            failure:(failBlock)failure;

@end


@class userPhoto;
@interface customer : EHOMEBaseObject
@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *email;
@property (nonatomic, assign) int id;
@property (nonatomic, strong) userPhoto *portrait;
@property (nonatomic, strong) userPhoto *portraitThumb;

@end

@interface userPhoto:EHOMEBaseObject

@property (nonatomic, copy) NSString *path;

@end
