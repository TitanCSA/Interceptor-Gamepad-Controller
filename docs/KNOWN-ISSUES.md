# Preview known issues and validation

The **1.0.5.0 demo prerelease** packages Microsoft-signed kernel **1.0.3.0**.
The viewer and C++ SDK remain **1.0.4.0**. The two-hour demo allowance is unchanged.
The default latest release remains **1.0.4.0**, with kernel **1.0.2.0**.
Neither version is claimed to fix the browser issue described below.

## Browser / Gamepad API: still reproduced

Controlled Windows 11 tests used virtual Xbox 360 / XUSB and Xbox One / GIP
device stacks, a normal foreground Microsoft Edge window, and the real
`navigator.getGamepads()` API. No browser gamepad emulation was used.

| Installed filter | Browser result |
| --- | --- |
| No Interceptor Gamepad Controller | Both controllers reported the expected changing buttons, sticks and triggers |
| Published kernel 1.0.2.0 | No gamepads exposed |
| No filter again, after restoring the baseline | Both controllers reported the expected changes again |
| Preview kernel 1.0.3.0 | No gamepads exposed |

The same fixture and browser test were used throughout. Native XInput readings
confirmed the fixture's expected states while the browser failed.
A separate foreground Windows.Gaming.Input test with kernel 1.0.3.0 enumerated
one controller but returned neutral readings with a zero timestamp in all
97 samples taken during active button / axis phases. The API calls succeeded;
that does not make their unchanged readings correct. The precise kernel cause
has not been established.

These results reproduce a consumer compatibility regression after installation,
including with no application capture context open. A successful SDK viewer,
signature check or native XInput check alone is insufficient to verify its fix.

## Windows game controller panel: limited result

The actual `joy.cpl` interface showed changing axes and buttons in the tested
virtual-device baseline and preview cases. This did **not** reproduce the
reported failure on a physical GIP controller, so that physical-device issue
remains unverified. Virtual-device results do not certify every controller,
firmware, connection mode or Windows input consumer.

## What the preview does not establish

The Microsoft signature validates the driver package's signature and integrity;
it is not evidence that the browser regression is resolved. These tests are
compatibility checks, not latency measurements, hardware coverage or proof of
zero delay. Capture and forwarding were not qualified for every family in these
consumer tests.

When reporting a problem, include the package and installed driver versions,
Windows build, controller VID/PID, connection type, affected application and
whether capture was active. Remove personal identifiers from diagnostic exports.
