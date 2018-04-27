
![](https://img.shields.io/badge/platform-iOS-red.svg) ![](https://img.shields.io/badge/language-Objective--C-orange.svg)   ![](https://img.shields.io/badge/license-MIT%20License-brightgreen.svg)  

WhatieSDK is an SDK provided by ATI TECHNOLOGY (WUHAN) CO.,LTD for third party access to IOT platform quickly. Through this SDK, users can register and log in, find and add devices such as nearby smart sockets, and control device switches.


[中文文档](https://www.jianshu.com/p/9afa0004a772) 


## Requirements
* iOS 8 or later
* Xcode 8 or later

## Installation
### CocoaPods:
first install using CocoaPods by adding this line to your Podfile:
`pod 'WhatieSDK',:git => 'https://github.com/IIDreams/WhatieSDK.git'`
then
`pod install或pod install --no-repo-update`
Importing:
`#import <WhatieSDK/WhatieSDK.h>`

If the result of "pod search PPNetworkHelper" is not latest,run "pod setup" in terminal to update local spec.
## Usage
### 1. About Users
The use of all methods is done by EHOMEUserModel class.
#### 1.1 Local information of user
After login successfully or the information is modified, the user's latest information is archived locally in the form of a EHOMEUserModel model, after which user’s information can be obtained, updated and removed. 
The user's Model includes some basic attributes of users, such as ID, name, email, etc.
##### 1.1.1 isLogin
```objc
//determine whether the user is logged in
BOOL isLogin = [EHOMEUserModel isLogin];
```
##### 1.1.2 setCurrentUser
```objc
//update user’s information
[EHOMEUserModel setCurrentUserWithUserModel:userModel];
```
##### 1.1.3 getCurrentUser
```objc
//get user’s information 
EHOMEUserModel *currentUserModel = [EHOMEUserModel getCurrentUser];
```
##### 1.1.4 Attributes
```objc
//get user’s attributes
currentUserModel.name
currentUserModel.email
…
```
##### 1.1.1 removeCurrentUser
```objc
// delete user’s information
[EHOMEUserModel removeCurrentUser];
…
```
#### 1.2 Login & Logout
##### 1.2.1 Login
```objc
[EHOMEUserModel loginWithEmail:email password:password accessId:AccessId accessKey:AccessKey startBlock:^{
    //Start login...
} successBlock:^(id responseObject) {
    //Login success
} failBlock:^(NSError *error) {
    //Login failed
}];
```
##### 1.2.2 updateLoginPassword
```objc
[EHOMEUserModel updateLoginPasswordWithEmail:email OldPasswordMD5:oldPassword newPasswordMD5:newPassword startBlock:^{
    //Start setting new password
} successBlock:^(id responseObject) {
    //update new password success
} failBlock:^(NSError *error) {
    //update new password failed
}];
```
##### 1.2.3 Logout
```objc
[EHOMEUserModel logoutWithAccessId:accessId accessKey:accessKey startBlock:^{
    //Start logout...
} successBlock:^(id responseObject) {
    //logout success
} failBlock:^(NSError *error) {
    //logout failed
}];
```
### 2. About MQTT
MQTT uses singleton mode to provide MQTT login, subscription message, publish message, receive message, disconnect and other functions. The connection state and receiving new message are using block to get the callback data.
#### 2.1 Init
MQTT initializes when APP starts (if the user is logged in) and initializes when the user is logging in. (refer to WhatieSDKDemo)
```objc
- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions{
    ...
    [[EHOMEMQTTClientManager shareInstance] loginMQTT];
    ...
}
```
#### 2.2 Turn on & Turn off
Just tell us which device you want to control with status!
```objc
[[EHOMEMQTTClientManager shareInstance] switchDeviceStatusWithDeviceModel:devicemodel status:isOn startBlock:^{
    //Start turning on/off...
} successBlock:^(id responseObject) {
    //Turn on/off success
} failBlock:^(NSError *error) {
    //Turn on/off failed
}];
```
#### 2.3 Recived MQTT Data
The recived data from MQTT as block recived in ViewDidLoad.
```objc
[[EHOMEMQTTClientManager shareInstance] setMqttBlock:^(NSData *data) {
    //data
}];
```
#### 2.4 Publish Other Data
You can publish other data to your smart device.
```objc
[[EHOMEMQTTClientManager shareInstance] publishAndWaitData:data];
```
#### 2.5 Close MQTT
You can close connection of MQTT.
```objc
[[EHOMEMQTTClientManager shareInstance] close];
```
### 3.About TCP
TCP adopts partial singleton control. The singleton obtains the tcpClientDictionary of the connection TCP socket. The key corresponds to the devId of the device, and the value corresponds to the EHOMETcpClient object.
While creating a singleton, the UDP broadcast will be published. After receiving the device, a socket connection of the EHOMETcpClient is created, and the devId of the device is added to the tcpClientDictionary. When TCP is required to control the device, the corresponding tcpClient is obtained through the key, and further operations are performed.

#### 3.1 Init
It is recommended to initialize TCP when APP starts.
```objc
[EHOMETCPManager shareInstance];
```
#### 3.2 Turn on & Turn off
It is similar to MQTT, just tell us the deviceModel and status you want to control.
```objc
[[EHOMETCPManager shareInstance] switchDeviceStatusWithDeviceModel:deviceModel status:isOn startBlock:^{
    //Start Turning on/off...
} successBlock:^(id responseObject) {
    //Turn on/off success
} failBlock:^(NSError *error) {
    //Turn on/off failed
}];
```
#### 3.3 isTcpConnected
Return BOOL value wether the device is connected with app by TCP.
```objc
[[EHOMETCPManager shareInstance] isCurrentDeviceTCPConnectedWithDeviceModel:deviceModel];
```
#### 3.4 Recive TCP Data
Receive new message, Add a block callback to the ViewDidLoad in the TCP ViewController. Data is a JSON string type.
```objc
[[EHOMETCPManager shareInstance] setTcpBlock:^(GCDAsyncSocket *sock, NSString *data) {
    //your code...   
}];
```
### 4.SmartConfig
The device uses the EHOMESmartConfig singleton to perform network distribution and calls the smartConfigWithWifiPasseord method to pass in the password of the wifi connected to the current mobile phone.
Just tell us the password of your wifi.
```objc
[[EHOMESmartConfig shareInstance] smartConfigWithWifiPassword:password startBlock:^{
    //Start smartconfig...
} successBlock:^(id responseObject) {
    //Smartconfig successfully, return device devId, and success status YES
} failBlock:^(NSError *error) {
    //SmartConfig failed
}];
```
### 5.General Interaction
#### 5.1 Get My Devices List
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
#### 5.2 Edit the device name

```objc
[EHOMEDeviceModel updateDeviceNameWithDeviceModel:deviceModel name:@"newName" startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```
#### 5.3 Untie device

```objc
[EHOMEDeviceModel unBindDeviceWithDeviceModel:deviceModel startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```
#### 5.4 Share device
The sharing device is coordinated by the device owner and the shared person. This scheme is to achieve the purpose of sharing devices by scanning the QR Code. First of all, the owner of the device spliced into a string by including information such as the owner id "adminUserId", device id "deviceId", timestamp "timestamp", authorization id "accessId", authorization key "accessKey", etc. The string generates a QR Code for the shared user to scan the code to obtain the information and then pass it to the following method. After success, the sharedUser can further control the device.
```objc
[EHOMEDeviceModel sharedDeviceWithAdminUserId:adminUserId sharedUserId:sharedUserId deviceId:deviceid timestamp:timestamp accessId:accessId accessKey:accessKey startBlock:^{

} suucessBlock:^(id responseObject) {

} failBlock:^(NSError *error) {

}];
```

## Contact Us:
* iOS Contributors: Yiran Ding, Wei Zhou, Linjun Chen
* Email : zhouwei20150901@icloud.com

## LICENSE
WahtieSDK use MIT LICENSE, LICENSE file detail.

