
![](https://img.shields.io/badge/platform-iOS-red.svg) ![](https://img.shields.io/badge/language-Objective--C-orange.svg)   ![](https://img.shields.io/badge/license-MIT%20License-brightgreen.svg)  

## WahtieSDK Version 1.1.1 updated at 2018-06-04

```
What's new:
All APIs for electrical outlets. SDKs for bulbs will be provided about June 6 or 7, 2018. 
Note: it's better to review all details on June 7. 
```

WhatieSDK is an SDK provided by ATI TECHNOLOGY (WUHAN) CO.,LTD. for the 3rd party accessing to our IOT cloud platform easily and quickly. Using this SDK, developers can do almost all funcation points on electrical outlets and RGBW bulbs, such as user registration/login, smart configration, add/share/unbind/delete devices, device control, timing countdown, timer, etc. 

[![](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/1small.PNG)](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/1.PNG)
[![](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/2small.PNG)](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/2.PNG)
[![](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/3small.PNG)](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/3.PNG)
[![](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/4small.PNG)](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/4.PNG)
[![](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/5small.PNG)](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/5.PNG)
[![](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/6small.PNG)](https://github.com/ATI-Wuhan/WhatieSDK_iOS/blob/master/images/6.PNG)


**Note:** For all function points, no any backend development is needed for integrating the SDK into your APP. You just do all of your work in your APP side. 


[中文文档/Chinese](https://www.jianshu.com/p/9afa0004a772). 


## Requirements
* iOS 8 or later
* Xcode 9 or later

## Installation
### Manual Install
Firstly, add the "WhatieSDK.framework" into your project,
And then,
`#import <WhatieSDK/WhatieSDK.h>`

## Usage
### 1. SDK Init
The SDK should be init before any APIs in SDK used. This init operation is recommended to be done in the function `didFinishLaunching...` with accessId and accessKey.  
**Note:** To use the SDK, you should contact us to apply for accessId and accessKey.
Example:
```objc
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    //your code...

    //Init WhatieSDK   

    [[EHOMESDK shareInstance] startWithAccessId:AccessId andAccessKey:AccessKey];

    //your code...

}
```
### 2. User Management

The SDK provides user management functions, such as user login, user logout, login password update. The only information you should give to SDK is: (1) the login email and (2) the encrypted password (**Note:** the SDK does not need the original password characters, it only needs ciphertext, like MD5 text). Here, the login email is the email used for logining into your APP, like Vivitar APP. The encrypted password is the ciphertext of the original password, generated in your APP. 

**Note:** (1) all other information on user management procedure is not needed for SDK. (2) The user email and ciphertext will be also stored in our cloud paltform. No any backend development is needed for integrating the SDK into your APP.

#### 2.1 Login
Login with user email and encrypted password.
```objc
[EHOMEUserModel loginWithEmail:email password:password startBlock:^{   
    //Start login...
} successBlock:^(id responseObject) {
    //Login success
} failBlock:^(NSError *error) {
    //Login failed
}];
```
#### 2.2 updateLoginPassword
Update login password with encrypted newPassword and encrypted oldPassword.
```objc
[EHOMEUserModel updateLoginPasswordOldPasswordMD5:oldPasswordMD5 newPasswordMD5:newPasswordMD5 startBlock:^{
    NSLog(@"Start Updating Login Password");
} successBlock:^(id responseObject) {
    NSLog(@"Update Login Password Success = %@", responseObject);
} failBlock:^(NSError *error) {
    NSLog(@"Update Login Password Failed = %@", error);
}];
```
#### 2.3 Logout
```objc
[EHOMEUserModel logoutWithStartBlock:^{
    NSLog(@"logout...");
} successBlock:^(id responseObject) {
    NSLog(@"logout success = %@", responseObject);
} failBlock:^(NSError *error) {
    NSLog(@"logout failed = %@", error);
}];
```

### 3. SmartConfig and Device Init
#### 3.1 SmartConfig
The device uses the EHOMESmartConfig singleton function to perform network distribution, and calls the smartConfigWithWifiPasseord function to get wifi password connected to the mobile phone.
Just tell us the password of your wifi.
```objc
[[EHOMESmartConfig shareInstance] smartConfigWithWifiPassword:_wifiPassword startBlock:^{
    NSLog(@"Start to smart config...");
} progressBlock:^(NSProgress *progress) {
    NSLog(@"smart config progress = %@", progress);
} successBlock:^(id responseObject) {
    NSLog(@"Smart config success = %@", responseObject);
    NSInteger protocol = [[responseObject objectForKey:@"protocol"] integerValue];
    //if protocol is equal to 9,smart config success.else,the device is other's.
    if (protocol == 9) {
        //success
    }else{
        //the device is other's,get the owner's email
        NSString *email = [[responseObject objectForKey:@"data"] objectForKey:@"email"];
    }
} failBlock:^(NSError *error) {
    NSLog(@"Smart config failed = %@", error);
}];
```
#### 3.2 Device Init
After finishing the SmartConfig procedure, the device should be init with devId and device name.
```objc
[EHOMEDeviceModel getStartedWithDevId:devId deviceName:deviceName startBlock:^{       

    //init...

} successBlock:^(id responseObject) {

    //init success

} failBlock:^(NSError *error) {

    //init failed

}];
```
### 4. Device Controlling
#### 4.1 Get My Devices List
Get the device list and return an array of `<EHOMEDeviceModel * >`. The device properties are in `EHOMEDeviceModel.h`.
```objc
[EHOMEDeviceModel getMyDeviceListWithStartBlock:^{
    //Start getting my devices...
} successBlock:^(id responseObject) {
    //Return the device array, which is encapsulated by EHOMEDeviceModel.
} failBlock:^(NSError *error) {
    //Get my devices failed
}];
```
#### 4.2 Operate Device
Just tell us which device and the device status you want it to be.
```objc
[EHOMEDeviceModel switchDeviceStatusWithDeviceModel:deviceModel toStatus:isOn startBlock:^{
    //controlling...
} successBlock:^(id responseObject) {
    //control success
} failBlock:^(NSError *error) {
    //control failed
}];
```
#### 4.3 Edit the device name

```objc
[EHOMEDeviceModel updateDeviceNameWithDeviceModel:deviceModel name:@"newName" startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```
#### 4.4 Unbind device

```objc
[EHOMEDeviceModel unBindDeviceWithDeviceModel:deviceModel startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```
#### 4.5 Share device by QR code
The sharing device is coordinated by the device owner and the shared person. This scheme is to achieve the purpose of sharing devices by scanning the QR Code. First of all, the owner of the device spliced into a string by including information such as the owner id "adminUserId", device id "deviceId", timestamp "timestamp", etc. The string generates a QR Code for the shared user to scan the code to obtain the information and then pass it to the following method. After success, the sharedUser can further control the device.
```objc
[EHOMEDeviceModel sharedDeviceWithAdminUserId:adminUserId sharedUserId:sharedUserId deviceId:deviceid timestamp:timestamp startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```

**Note:** QR code is a bit more complex for users, and it may be kicked out. Share device by email will be provided on June 6, 2018. By then, you can share device only by email. It is very simple.

#### 4.6 Timing Countdown
Your operation on the device will take effect once timing countdown is finished.
@param deviceModel: the device do you want to set timing countdown
@param isOn: the status of the device is to be when countdown is finished
@param duration: the duration of timing countdown. The unit is second, such as 10seconds; if 10 minutes, the value is 600.

```objc
[EHOMEDeviceModel countdownDeviceWithDeviceModel:deviceModel toStatus:isOn duration:Duration startBlock:^{

} successBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```

**Note:** APIs for cancel timeing countdown will provided June 6.

#### 4.7 Add timer

Your operation on the device will take effect once the time of the timer arrives.
Set a timer to operate the device on some specifical time.

@param deviceModel: the device do you want to set a timer
@param day sequence: which day do you want to execute? 0 means unabled, 1 means enabled,and the order of days are:
Sunday Saturday Friday Thursday Wednesday Tuesday Monday
for example:
timer avaliable on Thursday and Monday,the param of "day sequence" is @"0001001"; if Sunday, "day sequence" is @"1000000"
@param finishTime: the time user set to, @"1857" means time is 18:57
@param isOn: the status of the device is to be when timer arrives

```objc
[EHOMEDeviceModel addTimerClockWithDeviceModel:deviceModel days:@"0010000" finishTime:@"1857" isOn:YES startBlock:^{

} successBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```

**Note:** APIs for cancel timer and timer list will provided June 6.

## Welcome to contact us:
* iOS Contributors: Yiran Ding, IIDreams, Linjun Chen
* Email : zhouwei20150901@icloud.com, whatie@qq.com

## LICENSE
WhatieSDK use MIT LICENSE, LICENSE file detail.

