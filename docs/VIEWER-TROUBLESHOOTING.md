# Input viewer diagnostics

Use the [1.0.1.0 demo release](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.1.0-demo),
which includes the Microsoft-signed driver fix and updated viewer. Its native
bridge is embedded in `InterceptorExampleUI.exe`; no separate DLL needs copying.

Click **Observe** once in the required tab. **XInput / XUSB** covers the native
Xbox 360 transport; **XInputHID** is a different input family. Keyboard activity
is not required by either one.

If observation ends automatically, the window retains the diagnostic and shows
**Copy diagnostic**. The report includes:

- The failed operation: initialization, context creation, enumeration, input
  wait, input read, unchanged forwarding or display decoding.
- The actual family, device ID and sequence when the operation identifies
  them; otherwise they are explicitly unknown.
- The Windows error in decimal and hexadecimal, its Windows explanation and
  the native exception message.
- For forwarding errors, the received batch, accepted prefix and unsent suffix.
  Read buffer capacity is reported separately from received inputs.
- Received/forwarded totals, context generation, ready mask and the last
  reset reason reported by the driver.

A wait failure does not identify which physical device caused a reset. The
last reset reason can also belong to an earlier context. Copy the whole report
when reporting an issue; a new observation clears the previous failure.

## XUSB preparation issue

A kernel race was identified in the original 1.0.0.0 driver: a native XUSB
report could arrive before its translated HID view became capturable. The
all-device context treated the still-unattached source as a device change and
revoked capture. Keyboard activity could change timing but is not a solution.

The signed 1.0.1.0 driver keeps that source in normal passthrough until it
can join the context. Attached-device failures and queue overflow still release
capture. Packet formats and the two-hour demo are unchanged.

Download the new **InterceptorSetup.exe** and choose **Install / reinstall**,
or run the new **InterceptorSetupConsole.exe /install**. Use Windows **Restart**
when requested, then reopen the installer or rerun `/install` to finish device
associations. Console exit code `3010` means a restart is required.

The earlier [1.0.1.0 viewer-only release](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.1.0-viewer)
added diagnostics without replacing the installed driver. Its EXE alone cannot
fix the race in an older 1.0.0.0 driver; use the full demo release's installer.

While a source is preparing, Observe remains active and physical inputs pass
through until it is ready. There is no fixed 30-second readiness deadline in
the all-device context. An unsupported secondary HID profile is not made
compatible by this change; support depends on the Windows stack and descriptor.

## Validation scope

The regression harness compiles the actual kernel reader and dequeue functions
with deterministic WDF/mirror boundaries. It covers pending sources, first
eligible attachment, ring wrap, partial dequeue, event reset/rearming, overflow,
fixed snapshots and attached-device failures. Restoring the old reader must
fail the pending-source test. Strict Release and matched AddressSanitizer runs
pass 180,139 checks over 10,000 cycles.

Viewer builds use warnings as errors. Offline tests verify native/managed ABI
sizes, automatic-stop presentation, partial forwarding, read-capacity wording,
complete reports and concurrent bridge disposal. More than 400 native assertions
pass in each Release and AddressSanitizer run, including publication of a final
input after a quiet wait, without another input to wake the viewer.

A dedicated Windows 11 VM exercised the candidate and the published native
bridge with one virtual Xbox 360 source: 12 Observe/Stop cycles, 192 exact
button/axis/trigger states checked against an independent XInput consumer,
and 230 inputs received and forwarded unchanged. No keyboard or mouse input
was used. The exact published EXE passed its embedded-bridge/UI self-tests in
the same VM. Installation, associations and lab changes were restored, followed
by a normal Windows shutdown.

These bounded tests do not establish support for every physical controller.
Kernel source and signing material are not included here.
