# WhatieSDK
# hello,eHome

Welcome to WhatieSDK 

Description
WhatieSDK is an SDK provided by ATI TECHNOLOGY (WUHAN) CO.,LTD for third party access to IOT platform quickly. Through this SDK, users can register and log in, find and add devices such as nearby smart sockets, and control device switches.

Installation
The SDK for iOS is developed using Objective-C. To use the WhatieSDK for iOS, you will need the following installed on your development machine:
Xcode 7 or later
iOS 8 or later

Install using CocoaPods by adding this line to your Podfile:
pod 'WhatieSDK'
Install the pods:
pod install
Importing:
#import <WhatieSDK/WhatieSDK.h>

Usage
About Users
The use of all methods is done by EHOMEUserModel class.
Local information of user
After login successfully or the information is modified, the user's latest information is archived locally in the form of a EHOMEUserModel model, after which user’s information can be obtained, updated and removed. 
The user's Model includes some basic attributes of users, such as ID, name, email, etc.
//determine whether the user is logged in
BOOL isLogin = [EHOMEUserModel isLogin];
//update user’s information
[EHOMEUserModel setCurrentUserWithUserModel:userModel];
//get user’s information 
EHOMEUserModel *currentUserModel = [EHOMEUserModel getCurrentUser];
//get user’s attributes
currentUserModel.name
currentUserModel.email
	…
// delete user’s information when log out
[EHOMEUserModel removeCurrentUser];
Registration and login of user
/** Registration and login, both new and old users, this method is invoked when the APP logs in.

@param email : user’s email,
@param passwordMD5 : MD5 encrypted password,
@param accessId and accessKey: Authorized accessId and accessKey.
**/
+(void)registerNewUserWithEmail:(NSString *)email password:(NSString *)passwordMD5 accessId:(NSString *)accessId accessKey:(NSString *)accessKey startBlock:(startBlock)startblock successBlock:(successBlock)successblock failBlock:(failBlock)failblock;
	/** change password */
+(void)setNewPasswordByOldPasswordWithEmail:(NSString *)email oldPassword:(NSString *)oldPassword newPassword:(NSString *)newPassword startBlock:(startBlock)startblock successBlock:(successBlock)successblock failBlock:(failBlock)failblock;
/** get information of current user */
+(void)getUserInfoWithStartBlock:(startBlock)startblock successBlock:(successBlock)successblock failBlock:(failBlock)failblock;
/** logout current user */
+(void)logoutWithStartBlock:(startBlock)startblock  successBlock:(successBlock)successblock failBlock:(failBlock)failblock;
About MQTT
MQTT uses singleton mode to provide MQTT login, subscription message, publish message, receive message, disconnect and other functions. The connection state and receiving new message are using block to get the callback data.
//MQTT initializes when APP starts (if the user is logged in) and initializes when the user is logging in. (refer to WhatieSDKDemo)
- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions{
...
        [[EHOMEMQTTClientManager shareInstance] loginMQTT];
...
}
//Receive new message
[[EHOMEMQTTClientManager shareInstance] setMqttBlock:^(NSData *data) { }];
// Publish messages to topic
[[EHOMEMQTTClientManager shareInstance] publishAndWaitData:data];
// disconnect MQTT
[[EHOMEMQTTClientManager shareInstance] close];
// Control switch of device
[[EHOMEMQTTClientManager shareInstance] switchDeviceStatusWithDeviceModel:deviceModel status:isOn startBlock:^{
} successBlock:^(id responseObject) {        
// returns the latest status of the device
 failBlock:^(NSError *error) {
}];

About TCP
TCP adopts partial singleton control. The singleton obtains the tcpClientDictionary of the connection TCP socket. The key corresponds to the devId of the device, and the value corresponds to the EHOMETcpClient object.
While creating a singleton, the UDP broadcast will be published. After receiving the device, a socket connection of the EHOMETcpClient is created, and the devId of the device is added to the tcpClientDictionary. When TCP is required to control the device, the corresponding tcpClient is obtained through the key, and further operations are performed.
//It is recommended to initialize TCP when APP starts.
[EHOMETCPManager shareInstance];
//While controlling the device, the corresponding tcpClient is obtained through the current device id, and then sends the control instruction. Before sending message, you should determine whether the current device's tcpClient is connected. If yes, use TCP to send control commands. Otherwise, use MQTT.
EHOMETcpClient *tcpClient;
= [[EHOMETCPManager shareInstance].tcpClientDictionary ]
if ([[[EHOMETCPManager shareInstance].tcpClientDictionary allKeys] containsObject:deviceModel.device.devId]) {     
        tcpClient = [[EHOMETCPManager shareInstance].tcpClientDictionary objectForKey:deviceModel.device.devId];
}       
if([tcpClient isConnected]){       
        NSDictionary *dic = @{ @"dps":@{@"1":@(isOn),@"2":@(isOn) }, @"devId":self.sendDevId};       
        NSString *tcpJson = [NSString stringWithFormat:@"%@###",[dic mj_JSONString]];       
        [tcpClient writeString:tcpJson withTag:[tcpClient getWriteTag]];   
}else{               
        //use MQTT
}
//Receive new message, Add a block callback to the ViewDidLoad in the TCP ViewController. Data is a JSON string type.
[[EHOMETCPManager shareInstance] setTcpBlock:^(GCDAsyncSocket *sock, NSString *data) {
        //your code...   
}];
// disconnect TCP
[tcpClient disconnect];
SmartConfig
The device uses the EHOMESmartConfig singleton to perform network distribution and calls the smartConfigWithWifiPasseord method to pass in the password of the wifi connected to the current mobile phone.
[[EHOMESmartConfig shareInstance] smartConfigWithWifiPassword:password startBlock:^{
} successBlock:^(id responseObject) {
 // distribution work successfully, return device devId, and success status YES
} failBlock:^(NSError *error) {
}];
General interaction
Get my device list
Get the device list and return an array of <EHOMEDeviceModel * >. The device properties are in EHOMEDeviceModel.h.
[EHOMEDeviceModel getMyDeviceListWithStartBlock:^{
} successBlock:^(id responseObject) {
        // Returns the device array, which is encapsulated by EHOMEDeviceModel.
} failBlock:^(NSError *error) {
}];
Edit the device name
[EHOMEDeviceModel updateDeviceNameWithDeviceModel:deviceModel name:@"newName" startBlock:^{
} suucessBlock:^(id responseObject) {
} failBlock:^(NSError *error) {
}];
Untie device
[EHOMEDeviceModel unBindDeviceWithDeviceModel:deviceModel startBlock:^{
} suucessBlock:^(id responseObject) {
} failBlock:^(NSError *error) {
}];
Share device
The sharing device is coordinated by the device owner and the shared person. This scheme is to achieve the purpose of sharing devices by scanning the QR Code. First of all, the owner of the device spliced into a string by including information such as the owner id "adminUserId", device id "deviceId", timestamp "timestamp", authorization id "accessId", authorization key "accessKey", etc. The string generates a QR Code for the shared user to scan the code to obtain the information and then pass it to the following method. After success, the sharedUser can further control the device.
+(void)sharedDeviceWithAdminUserId:(int)adminUserId sharedUserId:(int)sharedUserId deviceId:(int)deviceId timestamp:(long)timestamp accessId:(NSString *)accessId accessKey:(NSString *)accessKey startBlock:(startBlock)startblock suucessBlock:(successBlock)successblock failBlock:(failBlock)failblock;
