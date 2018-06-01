
![](https://img.shields.io/badge/platform-iOS-red.svg) ![](https://img.shields.io/badge/language-Objective--C-orange.svg)   ![](https://img.shields.io/badge/license-MIT%20License-brightgreen.svg)  

WhatieSDK is an SDK provided by ATI TECHNOLOGY (WUHAN) CO.,LTD for third party access to IOT platform quickly. Through this SDK, users can register and log in, find and add devices such as nearby smart sockets, and control device switches.


[中文文档](https://www.jianshu.com/p/9afa0004a772) 


## Requirements
* iOS 8 or later
* Xcode 8 or later

## Installation
### Manual Install
first add the "WhatieSDK.framework" into your project
then
`#import <WhatieSDK/WhatieSDK.h>`

### Installation with CocoaPods:
first install using CocoaPods by adding this line to your Podfile:
`pod 'WhatieSDK',:git => 'https://github.com/IIDreams/WhatieSDK_iOS.git'`
then
`pod install or pod install --no-repo-update`
Importing:
`#import <WhatieSDK/WhatieSDK.h>`

If the result of "pod search WhatieSDK" is not latest,run "pod setup" in terminal to update local spec.

## Usage
### 1. SDK Init
The SDK should be init before any function will be used.This init work was recommanded to add into app didFinishLaunching... with accessId and accessKey.such as:
```objc
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    //your code...

    //Init WhatieSDK   

    [[EHOMESDK shareInstance] startWithAccessId:AccessId andAccessKey:AccessKey];

    //your code...

}
```
### 2. About User
The use of all methods is done by EHOMEUserModel class.
And the EHOMEUserModel includes attributes:
```objc
/**  The unique ID of user, called userId.  It's very important.And it is used everywhere.  */ 
@property (nonatomic, assign) int id;

/**  The email of user,which can login in app with password together  such as:zhouwei2015@whut.edu.cn  */

@property (nonatomic, copy) NSString *email;

...
```
After login successfully or the information is modified, the user's latest information is archived locally in the form of a EHOMEUserModel model, after which user’s information can be obtained, updated and removed. 
The user's Model includes some basic attributes of users, such as ID, name, email, etc.

#### 2.1 isLogin
Login information of user.The BOOL value will be return.YES means user has been login,else NO.
```objc
//determine whether the user is logged in
BOOL isLogin = [EHOMEUserModel isLogin];
```
#### 2.2 setCurrentUser
Update user information with userModel. 
```objc
//update user’s information
[EHOMEUserModel setCurrentUserWithUserModel:userModel];
```
#### 2.3 getCurrentUser
Get the latest userModel from local.
```objc
//get user’s information 
EHOMEUserModel *currentUserModel = [EHOMEUserModel getCurrentUser];
```

#### 2.4 Login
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
#### 2.5 updateLoginPassword
Update login password with newPassword and oldPassword.
```objc
[EHOMEUserModel updateLoginPasswordOldPasswordMD5:oldPasswordMD5 newPasswordMD5:newPasswordMD5 startBlock:^{
    NSLog(@"Start Updating Login Password");
} successBlock:^(id responseObject) {
    NSLog(@"Update Login Password Success = %@", responseObject);
} failBlock:^(NSError *error) {
    NSLog(@"Update Login Password Failed = %@", error);
}];
```
#### 2.6 Logout
```objc
[EHOMEUserModel logoutWithStartBlock:^{
    NSLog(@"logout...");
} successBlock:^(id responseObject) {
    NSLog(@"logout success = %@", responseObject);
} failBlock:^(NSError *error) {
    NSLog(@"logout failed = %@", error);
}];
```
### 3. About MQTT
MQTT uses singleton mode to provide MQTT login, subscription message, publish message, receive message, disconnect and other functions. The connection state and receiving new message are using block to get the callback data.

#### 3.1 Recived MQTT Data
The recived data from MQTT as block recived in ViewDidLoad.
```objc
[[EHOMEMQTTClientManager shareInstance] setMqttBlock:^(NSData *data) {
    //data
}];
```
#### 3.2 Publish Other Data
You can publish other data to your smart device.
```objc
[[EHOMEMQTTClientManager shareInstance] publishAndWaitData:data];
```
#### 3.3 Close MQTT
You can close connection of MQTT.
```objc
[[EHOMEMQTTClientManager shareInstance] close];
```
### 4.About TCP
TCP adopts partial singleton control. The singleton obtains the tcpClientDictionary of the connection TCP socket. The key corresponds to the devId of the device, and the value corresponds to the EHOMETcpClient object.
While creating a singleton, the UDP broadcast will be published. After receiving the device, a socket connection of the EHOMETcpClient is created, and the devId of the device is added to the tcpClientDictionary. When TCP is required to control the device, the corresponding tcpClient is obtained through the key, and further operations are performed.

#### 4.1 Recive TCP Data
Receive new message, Add a block callback to the ViewDidLoad in the TCP ViewController. Data is a JSON string type.
```objc
[[EHOMETCPManager shareInstance] setTcpBlock:^(GCDAsyncSocket *sock, NSString *data) {
    //your code...   
}];
```
#### 4.2 isTcpConnected
Return BOOL value wether the device is connected with app by TCP.
```objc
//TCP

BOOL isTcpConnected = [[EHOMETCPManager shareInstance] isCurrentDeviceTCPConnectedWithDeviceModel:deviceModel];
```
#### 4.3 Close TCP
First get the tcpClient from tcpClientDictionary with devId,then close the connection of TCP.
```objc
EHOMETcpClient *tcpClient;   

if ([[self.tcpClientDictionary allKeys] containsObject:deviceModel.device.devId]) {       

tcpClient = [self.tcpClientDictionary objectForKey:deviceModel.device.devId];

}

[tcpClient disconnect];
```

### 5. SmartConfig and Device Init
#### 5.1 SmartConfig
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
#### 5.2 Device Init
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
### 6. About Device
#### 6.1 Get My Devices List
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
#### 6.2 Device Control
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
#### 6.3 Edit the device name

```objc
[EHOMEDeviceModel updateDeviceNameWithDeviceModel:deviceModel name:@"newName" startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```
#### 6.4 Unbind device

```objc
[EHOMEDeviceModel unBindDeviceWithDeviceModel:deviceModel startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```
#### 6.5 Share device
The sharing device is coordinated by the device owner and the shared person. This scheme is to achieve the purpose of sharing devices by scanning the QR Code. First of all, the owner of the device spliced into a string by including information such as the owner id "adminUserId", device id "deviceId", timestamp "timestamp", etc. The string generates a QR Code for the shared user to scan the code to obtain the information and then pass it to the following method. After success, the sharedUser can further control the device.
```objc
[EHOMEDeviceModel sharedDeviceWithAdminUserId:adminUserId sharedUserId:sharedUserId deviceId:deviceid timestamp:timestamp startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```

## Contact Us:
* iOS Contributors: Yiran Ding, Wei Zhou, Linjun Chen
* Email : zhouwei20150901@icloud.com

## LICENSE
WhatieSDK use MIT LICENSE, LICENSE file detail.

