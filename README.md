# Interceptor Gamepad & Controller — Windows Kernel Filter Driver

**Microsoft-signed Windows kernel-mode input filter driver for HID, XInput / XUSB, XInputHID and GIP gamepad / controller,
with a two-file C++17 SDK.**

Receive physical input, identify its source, read or change it, then forward it
to Windows through one API. Windows 10 1709+ / Windows 11 **x64**; administrator
rights are required for installation and capture. Compatible USB profiles include **DualShock 4 (DS4 / PS4), DualSense (PS5), wired Xbox 360, Xbox One and Xbox Series X|S**. Connection, descriptor and protocol determine support; a model name alone is not a promise for every firmware or wireless mode.
See [compatibility](docs/COMPATIBILITY.md) for supported profiles and limits.

**[Download](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/latest)** · [Installation](docs/INSTALLATION.md)
· [C++ API](docs/API.md) · [Examples](ExamplesConsole/README.md)

**[1.0.1.0 demo update](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.1.0-demo):**
the Microsoft-signed driver fixes capture stopping when XUSB input arrives
during preparation. The viewer adds detailed automatic-stop errors and
**Copy diagnostic**. Upgrade with this release's UI or console installer;
replacing the viewer EXE alone does not update an older driver.
[Details and troubleshooting](docs/VIEWER-TROUBLESHOOTING.md).

| Setup and diagnostics | Input viewer |
| --- | --- |
| ![Product-specific installer](docs/images/installer.png) | ![Input viewer with simulated input](docs/images/viewer.png) |

Images illustrate the interface; the viewer uses simulated input and is not a
physical-device compatibility test. See release notes for validation details.

## Quick start

1. Download this edition's **InterceptorSetup.exe**, run it and accept elevation.
2. Click **Install / reinstall**. If requested, use Windows **Restart**, then
   reopen the same installer to finish device associations.
3. Run **InterceptorExampleUI.exe** as administrator and click **Observe**.
   **Stop** releases capture. Close other capture applications first.
4. Use **Diagnose** for per-device installation/API checks and copyable reports.
   Input features that cannot be exercised automatically remain **unverified**.

The console installer uses the same installation engine:

```powershell
.\InterceptorSetupConsole.exe /install
.\InterceptorSetupConsole.exe /uninstall
```

Exit code `3010` means a restart is required; after installing, rerun `/install`
after that restart. The Microsoft signature covers the driver package, not the
installer or example EXEs. [Installation and recovery](docs/INSTALLATION.md).

## Separate editions

This repository handles **HID, XInput / XUSB, XInputHID and GIP gamepad / controller input only**. The companion
[Interceptor-Keyboard-Mouse](https://github.com/TitanCSA/Interceptor-Keyboard-Mouse) handles the other input families.
The two editions have separate driver services, device associations, recovery
state and demo allowances, and can run side by side. Each application must use
the SDK for its installed edition. Do not link both alternative SDKs together.

The original [Interceptor](https://github.com/TitanCSA/Interceptor) combines both
groups. Remove an overlapping original Interceptor installation before using a
split edition; the installers refuse conflicting filter stacking and do not
uninstall another product automatically.

## Two-hour demo

**Two hours per Windows boot**, starting with the first successful capture,
shared by all applications using this edition. Sleep time counts. Closing an
application, uninstalling or reinstalling does not reset the current allowance.
Diagnostics that create a capture can start it.

At expiry, captures are revoked, new capture/injection is refused, and physical
input continues normally. Use Windows **Restart** for another allowance; Fast
Startup shutdown does not reset the kernel boot. For the **full version without
the demo time limit**, DM **`titancsa`** on Discord.

## Downloads and source

All binaries and checksums are in [Releases](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/latest).

| File | Contents |
| --- | --- |
| `InterceptorSetup.exe` | Standalone graphical installer and diagnostics |
| `InterceptorSetupConsole.exe` | Standalone `/install` and `/uninstall` installer |
| `InterceptorExampleUI.exe` | Standalone input viewer |
| `Interceptor-Gamepad-Controller-1.0.1.0-demo.zip` | Complete bundle: installers, console examples and viewer |
| `Interceptor-Gamepad-Controller-1.0.1.0-demo-SDK.zip` | Two-file SDK and example source |

```text
Interceptor/       Interceptor.h + Interceptor.cpp; no SDK DLL required
ExamplesConsole/   Controller pass-through and descriptor-decoded HID axis inversion
ExamplesUI/        Graphical viewer for this edition's input families
InstallerConsole/  Console installer
InstallerUI/       UI, shared installation engine and precompiled driver/recovery
docs/              API, installation, compatibility and images
licenses/          Driver binary terms and attribution
```

With Visual Studio 2022 C++ tools, a Windows SDK and CMake installed, run
`powershell -ExecutionPolicy Bypass -File .\Build.ps1` to build the user-mode
applications. Builds embed the supplied signed driver and precompiled recovery
binaries. **Kernel source and private signing material are not included.**
[Build instructions](docs/BUILD.md). User-mode source is [MIT](LICENSE); the
demo driver has [separate terms](licenses/BINARY-LICENSE.txt).

This independent project evolves the input-interception workflow popularized by
[oblitum's Interception](https://github.com/oblitum/Interception), with its own
implementation and API. It is not affiliated with oblitum or ABI-compatible with
the original. See [attribution](licenses/NOTICE). For a bug report, include the
release, Windows build, VID/PID, connection type and a diagnostic report with
personal identifiers removed.

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
