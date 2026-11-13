# Interceptor-Gamepad-Controller .NET SDK

.NET 8+ Windows x64. Package ID: `InterceptorGamepadController.SDK`; NuGet publication is on hold at the owner request. Local installation: `dotnet add package InterceptorGamepadController.SDK --version 1.0.0 --source PATH_TO_LOCAL_PACKAGES`.

```csharp
using TitanCSA.Products.InterceptorGamepadController;
using var client = new Interceptor();
var info = client.GetInfo();
```

Native DLL bundled as runtime asset. No driver installation or capture on construction. [API](docs/API.md). Original licensing/trial prerequisites remain unchanged.
