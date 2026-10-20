# Interceptor Gamepad & Controller — Windows Kernel Filter Driver

**Microsoft-signed Windows kernel-mode input filter driver for HID, XInput / XUSB, XInputHID and GIP gamepad / controller input, with a two-file C++17 SDK.**

> **[1.0.6.0 demo](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.6.0-demo) · signed kernel 1.0.4.0.**
> Native XUSB interception now runs below the Windows function driver, preserving its input-consumer behavior.
> Real browser, Windows game-controller panel and Search tests passed with controlled Windows 11 virtual devices. [Validation and remaining limits](docs/KNOWN-ISSUES.md).

Receive input, identify its source, read or change it, then forward it through one API.
Windows 10 1709+ / Windows 11 **x64**; installation and capture require administrator rights.
Compatible USB profiles include **DualShock 4 (DS4 / PS4), DualSense (PS5), wired Xbox 360, Xbox One and Xbox Series X|S**.
Connection, descriptor and protocol determine support; a model name does not certify every firmware or wireless mode.

**[Download](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/latest)** · [Installation](docs/INSTALLATION.md) · [C++ API](docs/API.md) · [Compatibility](docs/COMPATIBILITY.md) · [Examples](ExamplesConsole/README.md)

| Setup and diagnostics | Input viewer |
| --- | --- |
| ![Product-specific installer](docs/images/installer.png) | ![Input viewer with simulated input](docs/images/viewer.png) |

Images illustrate the interface; simulated viewer input is not a physical-device test.

## Quick start

1. Run this release's **InterceptorSetup.exe**, accept elevation, then click **Install / reinstall**.
2. If requested, use Windows **Restart** and reopen the same installer. Upgrading the legacy XUSB backend can require **two restarts**: one to load the new driver, then one to load migrated controller filters. Continue until no restart is requested.
3. Run **InterceptorExampleUI.exe** as administrator and click **Observe** once. **Stop** releases capture; close other capture applications first.
4. **Diagnose** provides per-device installation/API checks and a copyable report. Physical features not exercised automatically remain **unverified**.

The console installer uses the same installation engine:

```powershell
.\InterceptorSetupConsole.exe /install
.\InterceptorSetupConsole.exe /uninstall
```

Exit code `3010` requests a restart; rerun `/install` afterward. Replacing only the viewer EXE does not upgrade the driver.
The Microsoft signature covers the driver package, not the installer or example EXEs. [Recovery and troubleshooting](docs/INSTALLATION.md).

## What changed

Kernel **1.0.4.0** replaces the legacy XUSB upper backend with a USB lower filter.
Windows keeps ownership of native XInput / Windows.Gaming.Input requests, foreground policy and translated HID views.
The SDK discovers XUSB through the central endpoint, without requiring the former native-source or secondary-HID-mirror startup handshake.
The installers migrate only this product's legacy associations and preserve pending work until the expected backend is loaded.
All supplied user-mode applications are rebuilt with the updated SDK.
Rebuild custom applications with this release's **Interceptor.h + Interceptor.cpp**.
Wire ABI 4 is retained, but older compiled clients can still attempt the obsolete XUSB source/mirror startup path.

Validation used real Windows consumers, including the default browser Gamepad API, actual `joy.cpl` tester windows and a real Search mouse click.
Independent Windows.Gaming.Input readings also confirmed modified XUSB/GIP inputs sent during quiet periods and restoration after normal close and abrupt client termination.
These are bounded Windows 11 virtual-device tests. The reported physical-controller and Windows 10 Search cases still require verification on that hardware.
[Details](docs/KNOWN-ISSUES.md).

## Downloads and source

The [complete 1.0.6.0 release](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.6.0-demo) includes every download below, including rebuilt examples and both installers; no earlier release is needed.
Bundle version **1.0.6.0** contains kernel **1.0.4.0** and the updated two-file SDK.

| File | Contents |
| --- | --- |
| `InterceptorSetup.exe` | Standalone graphical installer and diagnostics |
| `InterceptorSetupConsole.exe` | Standalone `/install` and `/uninstall` installer |
| `InterceptorExampleUI.exe` | Standalone input viewer with embedded bridge |
| `Interceptor-Gamepad-Controller-1.0.6.0-demo.zip` | Complete installers, viewer and console examples |
| `Interceptor-Gamepad-Controller-1.0.6.0-demo-SDK.zip` | SDK, example and installer source |
| `Interceptor-Gamepad-Controller-1.0.6.0-Examples.zip` | Viewer and console examples |
| `SHA256SUMS.txt` | Download checksums |

```text
Interceptor/       Interceptor.h + Interceptor.cpp; no SDK DLL required
ExamplesConsole/   Controller pass-through and descriptor-decoded HID axis inversion
ExamplesUI/        Graphical input viewer
InstallerConsole/  Console installer
InstallerUI/       UI, shared installation engine and precompiled driver/recovery
docs/              API, installation, compatibility and images
licenses/          Driver binary terms and attribution
```

With Visual Studio 2022 C++ tools, a Windows SDK and CMake installed, run
`powershell -ExecutionPolicy Bypass -File .\Build.ps1` to build user-mode applications.
Builds embed the supplied signed driver and precompiled recovery binaries.
**Kernel source and private signing material are not included.**
[Build instructions](docs/BUILD.md). User-mode source is [MIT](LICENSE); the demo driver has [separate terms](licenses/BINARY-LICENSE.txt).

## Two-hour demo

**Two hours per Windows boot**, starting with the first successful capture, shared across this edition's applications.
Sleep time counts. Closing applications, uninstalling or reinstalling does not reset the allowance; diagnostics that create a capture can start it.
At expiry, captures are revoked, new capture/injection is refused, and physical input continues normally.
Use Windows **Restart** for a new allowance; Fast Startup shutdown does not reset the kernel boot.
For the **full version without the demo time limit**, DM **`titancsa`** on Discord.

## Separate editions and attribution

This edition handles **gamepad/controller input only**. [Interceptor-Keyboard-Mouse](https://github.com/TitanCSA/Interceptor-Keyboard-Mouse) has its own service, associations, recovery state and demo allowance; the two editions can coexist.
Use each edition's own SDK. Remove an overlapping original [Interceptor](https://github.com/TitanCSA/Interceptor) installation first; the split installers refuse conflicting stacking and do not uninstall another product automatically.

This independent project evolves the input-interception workflow popularized by
[oblitum's Interception](https://github.com/oblitum/Interception), with its own implementation and API.
It is not affiliated with oblitum or ABI-compatible with the original. See [attribution](licenses/NOTICE).
For a bug report, include the release, installed driver version, Windows build, VID/PID, connection type and affected consumer; remove personal identifiers from diagnostic exports.

## Minimal example: receive, read, forward

Install this edition first, restart if requested, and run your application as
administrator. Include `Interceptor.h`, compile `Interceptor.cpp` with your
application, and enable C++17. This loop forwards every received input:

```cpp
#include "Interceptor.h"
#include <iostream>
#include <system_error>

int main() try {
    using namespace Interceptor;
    const auto init = Init_AllDevices();
    if (!init.ready) {
        std::cerr << "Initialization error: " << init.error
                  << ", restart required: " << init.reboot_required << '\n';
        return 1;
    }
    for (;;) {
        auto inputs = Get_Input_With_Timeout(1000);
        for (auto& input : inputs) {
            if (auto* pad = input.Get<Controller_Hid_Input>()) {
                // Inspect pad->state.axes, triggers, hats and buttons.
                // Each declared control keeps its own descriptor identity.
                (void)pad;
            } else if (auto* xusb = input.Get<Controller_Xinput_Input>()) {
                // Example: xusb->state.left_x is the left stick X value.
                (void)xusb;
            }
            // XInputHID and GIP entries stay in this same batch too.
        }
        const auto sent = Send_Input(inputs);
        if (!sent) {
            std::cerr << "Send error: " << sent.error << '\n';
            return 1;
        }
    }
} catch (const std::system_error& error) {
    std::cerr << error.what() << '\n';
    return 1;
}
```

1. **Receive:** `Get_Input_With_Timeout(1000)` waits up to 1,000 milliseconds for
   input after initial context setup. Timeout returns an empty batch. The library
   creates and reuses the capture automatically; no context argument is needed.
2. **Read:** each `input` is one event. `input.type` gives its family, `input.device`
   its source ID within that family, and `inputs.size()` the event count.
   `input.Get<T>()` returns a pointer to that family's data or `nullptr`.
   Change typed fields before forwarding to modify an input; set
   `input.blocked = true` to block that event.
3. **Forward:** `Send_Input(inputs)` returns the batch to its source devices.
   Destination, type and count are automatic. Receiving consumes input, so
   forward unchanged entries too. `sent.accepted` gives the accepted count;
   `sent.error` is `0` on success. This basic example exits on a send error.

A batch borrows reusable storage: finish it before receiving another one.
Closing the application releases capture. See [API details](docs/API.md) for
field layouts, cancellation, errors and partial-send retries.
