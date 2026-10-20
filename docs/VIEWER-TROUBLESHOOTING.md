# Input viewer diagnostics

Use the [1.0.2.0 demo release](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.2.0-demo),
which includes the Microsoft-signed driver and updated viewer. Its native
bridge is embedded in `InterceptorExampleUI.exe`; no separate DLL needs copying.
The Microsoft signature covers the driver package, not the application EXEs.

## Open source temporarily reports Windows error 21

Use the [1.0.4.0 viewer and SDK update](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.4.0-viewer).
The driver remains signed version 1.0.2.0. Replace the viewer EXE; applications
using the SDK should rebuild with the updated `Interceptor.cpp` and header.

The previous SDK closed an already-open collection file when its SOURCE query
reported `ERROR_NOT_READY` (21). For HID and XInputHID this could discard the
pending reader; for a mandatory XUSB HID view it aborted the entire all-family
capture. Consequently, an error from an XUSB view could even stop the HID tab.

The all-device context now retains these open HID, XInputHID and XUSB mirror
files until capture ends. The kernel still decides when each source is actually
ready and attaches it only then; an incomplete XUSB pair remains in passthrough.
No fake ready state, repeated Observe clicks, source polling, sleep or capture
restart is added. GIP has no user-mode source file and continues through the
same all-family context while other sources prepare.

Only a temporarily unready, successfully opened collection uses this behavior.
Access denied, an unsupported mandatory mirror, trial expiry and other failures
still retain their exact diagnostic and release all acquired files. A native
XUSB parent's SOURCE response completes its bootstrap handshake, so its failure
is still reported rather than hidden as collection preparation. An unsuccessful
CreateFile does not provide a file that can be retained. This change does not
make unsupported profiles capturable or certify every physical controller.

The actual SDK passes 1,210,681 checks in Release and again with AddressSanitizer,
including 10,000 pending-source lifecycles, late HID/XUSB/XInputHID inputs,
concurrent GIP forwarding, exact cleanup, real errors and the two-hour limit.
The previous SDK fails the same fixture with the reported mandatory HID mirror
SOURCE / Win32 21 error. These injected I/O tests do not access host devices;
the specific physical controller shown in the report has not been tested here.

## Display responsiveness in viewer 1.0.3.0

Download the [1.0.3.0 standalone viewer](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.3.0-viewer)
for the latest display changes. The signed 1.0.2.0 driver remains current.
The older complete 1.0.2.0 ZIP contains the preceding viewer.

The native worker publishes changed state immediately after forwarding the
received batch. The two former 33 ms display waits are removed. The UI requests
one WPF rendering callback and reads the newest snapshot just before rendering;
notifications arriving before that frame are coalesced. A final change during
snapshot acknowledgement can request the following frame. The callback is
unsubscribed after delivery and on close, so an idle viewer has no permanent
rendering callback, display timer or busy loop.

There is no fixed application FPS cap. Actual visible updates depend on WPF,
Windows, the graphics system and the display. This does not promise zero
end-to-end latency or make the hardware produce more reports. Forwarding remains
before display decoding; the inputs counter measures reports, not presented FPS.

Offline tests cover immediate final-state publication across 10,000 sessions,
preserved forwarding and diagnostic behavior, bounded notifications, updates
arriving during a frame read, disposal and dispatcher shutdown. Release and
AddressSanitizer native fixtures each pass 120,291 checks; managed scheduler
fixtures pass 60,019 checks. These are software tests, not physical latency or
monitor-refresh measurements.

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

The fix introduced in signed driver 1.0.1.0 and retained in 1.0.2.0 keeps that
source in normal passthrough until it can join the context. Attached-device
failures and queue overflow still release capture. Packet formats and the
two-hour demo are unchanged.

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

## Idle XUSB startup follow-up

The 1.0.2.0 SDK and viewer report native XUSB source-opening failures with
their exact operation, input family and Windows error. The SDK no longer
silently discards a registered native source and leaves an empty observation
running when that source fails to start. Unsupported transparent generic
HID/XInputHID collections remain omitted; failures of a mandatory XUSB mirror
are reported. Interfaces that actually disappear keep their normal behavior.

The signed 1.0.2.0 kernel also initializes the reader from
Windows' cached unfiltered physical state, then arms ordinary input reception
before declaring the source ready. It does not insert a fake input into the
capture queue or poll the controller. **Use the full 1.0.2.0 release's installer
to update the kernel; the viewer EXE alone does not replace an installed driver.**
The earlier [1.0.2.0 viewer-only preview](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.2.0-viewer)
was published while kernel signing was pending and is now superseded by the full
signed package. Earlier release downloads remain unchanged.

The reported physical-controller failure has not been reproduced with the
virtual Xbox 360 used in the Windows 11 lab: the signed 1.0.1.0 baseline already
passes six idle Observe starts, three untouched seconds and one delayed report
per cycle. Consequently, passing that virtual-device test alone does not
establish that the physical-controller issue is resolved. Deterministic tests
exercise actual startup/source-opening code, including the formerly silent
failure path, cancellation, malformed responses and repeated restarts.

The tested 1.0.2.0 candidate underlying the signed package and the viewer also
pass six such Windows 11 VM cycles: metadata is visible before stimulation,
each startup has three untouched seconds, then one report is received and
forwarded unchanged. No report submission or independent XInput read helps
discovery during startup.
The actual kernel startup bodies pass 760,764 deterministic checks and 10,000
restart cycles in both Release and AddressSanitizer builds. The SDK source-open
tests pass 123 checks and reject the former silent-failure implementation.

## Earlier release validation

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

A dedicated Windows 11 VM exercised the earlier 1.0.1.0 candidate and its
published native bridge with one virtual Xbox 360 source: 12 Observe/Stop cycles,
192 exact button/axis/trigger states checked against an independent XInput consumer,
and 230 inputs received and forwarded unchanged. No keyboard or mouse input
was used. The exact published EXE passed its embedded-bridge/UI self-tests in
the same VM. Installation, associations and lab changes were restored, followed
by a normal Windows shutdown.

These bounded tests do not establish support for every physical controller.
Kernel source and signing material are not included here.
