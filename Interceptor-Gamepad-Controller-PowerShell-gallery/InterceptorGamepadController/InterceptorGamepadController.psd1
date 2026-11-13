@{
RootModule = 'InterceptorGamepadController.psm1'
ModuleVersion = '1.0.0'
GUID = '4e461bdd-ce2a-5185-88e4-0d62c4eef2d5'
Author = 'TitanCSA'
Description = 'Windows x64 Interceptor-Gamepad-Controller input SDK for PowerShell 7.4+. Driver installed separately.'
PowerShellVersion = '7.4'
CompatiblePSEditions = @('Core')
FunctionsToExport = @('Connect-InterceptorGamepadControllerSDK','Initialize-InterceptorGamepadControllerSDK','Get-InterceptorGamepadControllerSDKInfo','Get-InterceptorGamepadControllerSDKDevice','Receive-InterceptorGamepadControllerSDKInput','Send-InterceptorGamepadControllerSDKBatch','Remove-InterceptorGamepadControllerSDKBatch','Stop-InterceptorGamepadControllerSDKWait','Get-InterceptorGamepadControllerSDKDescriptor','Send-InterceptorGamepadControllerSDKInput','Disconnect-InterceptorGamepadControllerSDK')
CmdletsToExport = @()
VariablesToExport = @()
AliasesToExport = @()
PrivateData = @{ PSData = @{ Tags = @('Windows','SDK','Keyboard','Mouse','Gamepad'); LicenseUri = 'https://github.com/TitanCSA/Interceptor-Gamepad-Controller/blob/main/LICENSE'; ProjectUri = 'https://github.com/TitanCSA/Interceptor-Gamepad-Controller'; ReleaseNotes = 'First PowerShell SDK; docs and native bridge included.' } }
}
