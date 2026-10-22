# Input viewer diagnostics

Use the [complete 1.0.9.0 release](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.9.0-demo),
which includes signed kernel **1.0.5.0** and user-mode applications rebuilt with the updated SDK.
The native bridge is embedded in `InterceptorExampleUI.exe`; no separate DLL is needed.
Use this release's installer to update the kernel, then finish all requested restart/reopen steps.
Use its rebuilt viewer and rebuild custom clients with the supplied `Interceptor.h` and `Interceptor.cpp`.
Wire ABI 4 is retained, but an older compiled client's legacy XUSB SOURCE/mirror startup path can still fail with the new backend.

## Observe once

Open the viewer and accept the administrator prompt. Observation starts
automatically; select a family to display it. After **Stop**, **Observe** resumes.
**XInput / XUSB** is the native Xbox 360-style transport; **XInputHID** is a separate HID-backed family.
The SDK creates and reuses its all-device capture automatically; the selected tab controls what the viewer displays.
Keyboard activity and repeated Observe clicks are not required.

Sources still enumerating can join capture when ready.
For the USB XUSB backend, discovery uses the central driver endpoint and does not depend on the former native-source or secondary-HID-mirror opening handshake.
HID and XInputHID collections can still need their own descriptor/reader preparation.
An unsupported format remains unsupported; the viewer does not invent a ready state or fake input.

If input remains invisible, check **Diagnose**, the family/transport, the installed kernel version and whether another application owns capture.
Then compare the same device in the affected consumer, such as the browser or `joy.cpl`.
A working SDK viewer alone does not prove that every Windows consumer receives correct input.
[Compatibility](COMPATIBILITY.md) · [validated scenarios](KNOWN-ISSUES.md).

## Automatic-stop report

When observation ends unexpectedly, **Copy diagnostic** retains:

- The failed stage: initialization, context creation, enumeration, wait, receive, forwarding or display decoding.
- The family, device and sequence when known; unavailable information is identified explicitly.
- The Windows error in decimal/hexadecimal, its explanation and native exception text.
- Received/forwarded totals and, on send failure, the accepted prefix and unsent suffix.
- Context/readiness information and the last driver reset reason.

A wait failure alone does not identify the responsible physical device, and a historical reset reason can predate the current capture.
Copy the complete report before starting another observation, which clears the previous failure.
Remove serial numbers and personal identifiers before publishing it.

## Browser, game-controller panel or Search problems

The legacy upper XUSB backend in kernels 1.0.2.0/1.0.3.0 could break native Windows consumer behavior even with no active SDK capture.
The 1.0.4.0 USB lower backend leaves native request validation, foreground policy and HID translation to Windows.
Upgrade the installed driver, not only this EXE; an upgrade can require two normal restarts with the same installer reopened after each request.

Controlled Windows 11 tests passed the real default browser Gamepad API, actual `joy.cpl` tester screens and a real Search mouse click opening Notepad.
Independent Windows.Gaming.Input readings also verified idle modifications and restoration after normal close and abrupt termination.
The user's physical-controller and Windows 10 Search cases remain to be checked on that machine.
Windows foreground/background input policy still applies. [Detailed scope](KNOWN-ISSUES.md).

## Display cadence

There is no fixed application FPS cap or permanent display timer.
The native worker forwards a received batch before decoding display data, publishes the latest state and requests a WPF rendering callback.
Notifications are coalesced so the UI displays current state without queuing old frames; the callback is removed when no update remains.

Visible cadence still depends on controller report rate, Windows, WPF, graphics and the screen.
The input counter counts reports, not displayed frames.
Compatibility tests do not measure physical end-to-end latency or promise zero delay.

Historical viewer-only releases remain downloadable, but their EXEs cannot update an older installed kernel.
