# Interceptor-Gamepad-Controller PowerShell SDK

`Install-Module InterceptorGamepadController -Scope CurrentUser`

Requires PowerShell 7.4+ on Windows x64. Supported families: hid, xinput, xinputhid, gip. The native product SDK and license/trial behavior are unchanged. No driver is installed on import or construction.

```powershell
Import-Module InterceptorGamepadController
$client = Connect-InterceptorGamepadControllerSDK
try { Get-InterceptorGamepadControllerSDKInfo -Client $client }
finally { Disconnect-InterceptorGamepadControllerSDK -Client $client }
```

See [API guide](docs/API.md) and `Get-Command -Module InterceptorGamepadController`. The matching driver is installed separately.
