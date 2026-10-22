# Validation and remaining limits

Current release: **1.0.11.0**, including the Microsoft-signed primary driver and resident clock; SDK **1.0.4.2**. See [current demo validation](DEMO-VALIDATION.md). Historical tests below describe the versions named in their sections; current monitor use starts and obeys the same two-hour allowance as capture.

## Passive telemetry in 1.0.7.0

Kernel **1.0.5.0** adds independent, read-only IN / OUT observation used by GamePad Remapper 2.2.4. IN is a report received on a filtered controller stack before changes at that layer; OUT is a successful completion from that filter towards Windows. A successful SDK send or queue insertion is not itself an OUT measurement. Capture and its demo allowance are unchanged. [Measurement details](PASSIVE-MONITOR.md).

The exact production kernel body was exercised with a laboratory signature in an isolated Windows 11 VM under Driver Verifier. Controlled HID, XUSB and GIP sources produced decoded passive IN / OUT without starting the capture trial. Applying a script changed OUT; Stop kept observation active. Independent analysis found zero mismatches in 191 fresh modified and 192 post-Stop XUSB snapshots, 191/192 in the native GIP run, and 179/228 each for HID and GIP in a second run. Initial unknown, stale egress and transition samples were explicitly excluded; these counts are not polling-rate or latency measurements. No ring loss was observed in those bounded runs.

The returned Microsoft-signed 1.0.5.0 package was then installed in a separate Windows 11 VM pass: setup requested a normal restart (3010), reinstallation completed (0), and the loaded SYS matched the signed release hash with successful kernel/catalog signature verification. A passive XUSB/GIP probe recorded 601 snapshots, including 191 fresh modified and 191 fresh post-Stop snapshots for each family. The real Remapper UI showed physical HID/GIP IN, script-modified OUT, then matching IN/OUT after Stop while observation continued; three copied diagnostic reports contained no loss or monitor fault. The VM baseline had test mode enabled: these checks verify the returned package and runtime behavior, not a test-mode-off or Secure Boot certification.

The returned Microsoft package is separately checked for embedded/catalog signatures, catalog membership, identical INF and identical executable content. Live XInputHID, power/hotplug and foreign-capture transitions were not repeated in that VM run; isolated regression tests exercise their implemented boundaries. The user's physical controllers remain unverified. The earlier native-consumer tests below describe kernel 1.0.4.0, whose transparent XUSB backend is retained; they are not a new universal compatibility certification for 1.0.5.0.

## Installation failure in 1.0.6.0

The installer examined historical Xbox HID views while cleaning legacy associations. An unowned, disconnected view with an unbound ancestor could lack its setup-class property; reading it aborted setup with error 13 before any controller was associated. The same failure interrupted restoration, after which the old installer incorrectly deleted the newly staged driver package. A later install could then overlook the classless root endpoint and fail to recreate its stable ID.

Version **1.0.6.1** limits legacy cleanup to this product's own identity values or exact filter references, reuses its unbound root endpoint and retains the package/recovery files when restoration is incomplete. It also accepts the documented single-NUL form of an empty filter list and includes setup-stage/device/error details. The Microsoft-signed kernel remains **1.0.4.0**.

The original install/rollback sequence was reproduced on a Windows 11 VM with absent class-wide filter values and a historical HID view. The corrected installer repaired the orphaned state, completed association after a normal restart and passed a repeated full installation with exactly one control endpoint. Physical Windows 10 verification remains necessary; no manual registry cleanup is required for the repaired path.
## Native Windows consumer regression

Earlier kernels intercepted native XUSB requests above the Windows function driver.
That backend could return incorrect results to Windows input consumers even with no capture context open.
The regression was reproduced in a foreground browser; bypassing that backend restored native behavior.

Kernel 1.0.4.0 intercepts supported XUSB input below the Windows function driver at the USB transport.
Windows continues handling native request validation, foreground policy, HID translation and output requests.
The SDK no longer needs the legacy XUSB native-source/secondary-HID-mirror startup handshake.

| Release bundle | Kernel | Controlled browser result |
| --- | --- | --- |
| Earlier 1.0.4.0 demo | 1.0.2.0 | Regression reproduced |
| Earlier 1.0.5.0 preview | 1.0.3.0 | Regression reproduced |
| Earlier 1.0.6.1 demo | 1.0.4.0 | Both virtual XUSB and GIP controllers matched expected input |

Older downloads remain historical artifacts; replacing only a viewer cannot update their installed kernel.

## What was actually verified

The production candidate was tested in an isolated Windows 11 VM with controlled virtual wired-XUSB and GIP controllers.
The Microsoft-signed return was checked against that candidate's executable content.

| Check | Evidence and scope |
| --- | --- |
| Default browser Gamepad API, no capture | A real foreground Edge window used native `navigator.getGamepads()`. Both controllers matched four axes, two triggers, A/B and neutral phases; 140 stable readings per family matched. No browser gamepad emulation was used. |
| Modification during quiet input | A real SDK capture sent two different modified states while physical reports were quiet. Independent foreground Windows.Gaming.Input readings verified four axes, two triggers and A/B for each family. Send acceptance alone was insufficient. |
| Normal close and abrupt client termination | Independent Windows.Gaming.Input readings returned to the fixture's active and neutral physical states after each case. |
| Windows game-controller panel | Actual `joy.cpl` tester windows and their phase-correlated screens were reviewed for both virtual controllers. This was not inferred solely from XInput or a successful process exit. |
| Windows Search mouse input | With neutral fixtures, a real mouse click on a verified Search result opened a new Notepad process. The test used Windows 11 Search, not a UI Automation invoke action. |
| Build and regression checks | User-mode builds, installer continuation tests and production kernel regression tests passed; the release keeps the existing public API. |

## What remains unverified

- The user's physical controllers and the reported **Windows 10 Search** surface still require verification on that machine. A successful Windows 11 Search test does not certify Windows 10.
- Virtual-device results do not establish support for every firmware, proprietary report, Bluetooth mode, wireless receiver or vendor feature. See [compatibility](COMPATIBILITY.md).
- The tests are not physical input-latency, polling-rate or monitor-FPS measurements. There is no promise of zero delay or universal compatibility.
- Windows retains its foreground/background access policy. A background consumer's neutral reading must be assessed in that context.

## Upgrade correctly

Use this release's UI or console installer. A legacy-backend upgrade can require **two normal Windows restarts**, reopening the same installer after each request: first load the new image, then load the migrated controller filters.
The installer preserves pending work when an older backend remains loaded and rechecks older incomplete journals.
No forced device restart or automatic reboot is performed. [Installation](INSTALLATION.md).
Use this release's rebuilt examples and rebuild custom applications with both updated SDK files.
Wire ABI 4 is retained; an older compiled client's legacy XUSB SOURCE/mirror startup sequence can still require an SDK rebuild.

For a report, include bundle and installed-kernel versions, Windows build, VID/PID, connection type, affected application and whether capture was active.
Remove serial numbers and other personal identifiers from diagnostic exports.
