
![](https://img.shields.io/badge/platform-iOS-red.svg) ![](https://img.shields.io/badge/language-Objective--C-orange.svg)   ![](https://img.shields.io/badge/license-MIT%20License-brightgreen.svg)  

## WahtieSDK Version 1.1.1 updated at 2018-06-04

```
What's new:
Easy to build your smart home with us.
```

WhatieSDK is an SDK provided by ATI TECHNOLOGY (WUHAN) CO.,LTD for third party access to IOT platform quickly. Through this SDK, users can register and log in, find and add devices such as nearby smart sockets, and control device switches.


[中文文档](https://www.jianshu.com/p/9afa0004a772) 


## Requirements
* iOS 8 or later
* Xcode 9 or later

## Installation
### Manual Install
First add the "WhatieSDK.framework" into your project
then
`#import <WhatieSDK/WhatieSDK.h>`

## Usage
### 1. SDK Init
The SDK should be init before any function will be used.This init work was recommanded to add into app didFinishLaunching... with accessId and accessKey.Such as:
```objc
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    //your code...

    //Init WhatieSDK   

    [[EHOMESDK shareInstance] startWithAccessId:AccessId andAccessKey:AccessKey];

    //your code...

}
```
### 2. About User

After login successfully or the information is modified, the user's latest information is archived locally in the form of a EHOMEUserModel model, after which user’s information can be obtained, updated and removed. 
The user's Model includes some basic attributes of users, such as ID, name, email, etc.

#### 2.1 Login
Login with user's email and password.
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
Update login password with encrypted newPassword and oldPassword with MD5.
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
The device uses the EHOMESmartConfig singleton to perform network distribution and calls the smartConfigWithWifiPasseord method to pass in the password of the wifi connected to the current mobile phone.
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
After SmartConfig success,it should be init with devId and device name.
```objc
[EHOMEDeviceModel getStartedWithDevId:devId deviceName:deviceName startBlock:^{       

    //init...

} successBlock:^(id responseObject) {

    //init success

} failBlock:^(NSError *error) {

    //init failed

}];
```
### 4. About Device
#### 4.1 Get My Devices List
Get the device list and return an array of <EHOMEDeviceModel * >. The device properties are in EHOMEDeviceModel.h.
```objc
[EHOMEDeviceModel getMyDeviceListWithStartBlock:^{
    //Start getting my devices...
} successBlock:^(id responseObject) {
    //Return the device array, which is encapsulated by EHOMEDeviceModel.
} failBlock:^(NSError *error) {
    //Get my devices failed
}];
```
#### 4.2 Device Control
Just tell us witch device and status do you want to control.
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
#### 4.5 Share device
The sharing device is coordinated by the device owner and the shared person. This scheme is to achieve the purpose of sharing devices by scanning the QR Code. First of all, the owner of the device spliced into a string by including information such as the owner id "adminUserId", device id "deviceId", timestamp "timestamp", etc. The string generates a QR Code for the shared user to scan the code to obtain the information and then pass it to the following method. After success, the sharedUser can further control the device.
```objc
[EHOMEDeviceModel sharedDeviceWithAdminUserId:adminUserId sharedUserId:sharedUserId deviceId:deviceid timestamp:timestamp startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```

#### 4.6 Countdown
Control your device when countdown is finished.
@param deviceModel : the device do you want set timer
@param isOn :  turn device isOn when countdown is done
@param duration :  the duration of countdown, such as 10s.If 10 minutes ,it is 600.

```objc
[EHOMEDeviceModel countdownDeviceWithDeviceModel:deviceModel toStatus:isOn duration:10 startBlock:^{

} successBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```

#### 4.7 AddTimerClock

Control your device when time is ready.
Set a time to turn on or turn off device on someday.

@param deviceModel : the device do you want set timer
@param days :  which day do you want to execute?0 means unable, 1 means enable,and the order of days are:
Sunday Saturday Friday Thursday Wednesday Tuesday Monday
for example:
timer avaliable on Thursday and Monday,the param of "days" is @"0001001",if Sunday, "days" is @"1000000"
@param finishTime :  time user set,@"1857" means time is 18:57
@param isOn :  turn device isOn when time is ready

```objc
[EHOMEDeviceModel addTimerClockWithDeviceModel:deviceModel days:@"0010000" finishTime:@"1857" isOn:YES startBlock:^{

} successBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```

## Contact Us:
* iOS Contributors: Yiran Ding, Wei Zhou, Linjun Chen
* Email : zhouwei20150901@icloud.com

## LICENSE
WhatieSDK use MIT LICENSE, LICENSE file detail.

