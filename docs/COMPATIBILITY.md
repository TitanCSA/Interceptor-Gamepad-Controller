# Gamepad and controller compatibility

This edition captures **controller input only**: HID game controllers, XInput/XUSB, XInputHID and GIP.
It has no keyboard/mouse injection API or keyboard/mouse mapper; excluded stacks remain in passthrough.

| Family | Supported approach | Limits |
| --- | --- | --- |
| HID gamepad / joystick | Descriptor-driven decoding and re-encoding; dynamic axes, triggers, hats, buttons and other declared values | Proprietary meanings and malformed descriptors cannot be inferred. |
| XInput / XUSB | Supported wired Xbox 360-style USB profiles; kernel 1.0.4.0 filters below `xusb22` | Windows retains native input-consumer policy and HID translation. This does not cover every adapter, wireless receiver or proprietary protocol. |
| XInputHID | Supported Windows controller HID collection and source association | The collection and connection must expose the expected profile. This is a separate family from native XUSB. |
| GIP | Validated metadata and supported complete input frames, including extended input frames | No firmware, authentication or audio command injection; unsupported formats remain explicit. |

Compatible USB profiles can include **DS4 / DualShock 4 (PS4), DualSense (PS5), wired Xbox 360, Xbox One and Xbox Series X|S**.
A model name alone does not establish support for every firmware, Bluetooth connection, wireless adapter or vendor feature.
The actual descriptor, transport and Windows stack determine compatibility.

## Input state

For HID and XInputHID, `input.state` describes the current report.
Repeated X axes remain separate fields with their report/collection identity; controls are not collapsed merely because they share a usage name.
Check decoding flags before relying on normalized values.
Standard XUSB and GIP states have their own layouts and trigger/button ranges; they are not interchangeable binary structures.

`Send_Input` selects the input family and source automatically.
The current XUSB USB backend feeds modified reports through the native Windows translator, so its Windows-facing views follow the same transport state.
Windows still decides whether a particular consumer is entitled to foreground/background input.

## Readiness and validation

An attached filter is not proof of physical capture. A source can be preparing while Windows enumerates and configures it.
The all-device capture accepts future ready sources; moving a stick at the exact moment of **Observe** is not a required startup gesture.
Diagnostics distinguish absent, preparing, unsupported, unavailable and ready sources.
XUSB discovery in the updated SDK uses the central driver endpoint and does not require opening the former secondary HID mirror.
Rebuild custom clients with the current two-file SDK: preserving wire ABI 4 does not make their older XUSB SOURCE/mirror startup sequence suitable for this backend.

Windows 11 tests with controlled virtual XUSB/GIP devices verified real browser input, actual `joy.cpl` tester screens, independent modified-input readings and physical-state restoration after capture close or client termination.
Those tests do not certify every physical device or the user's Windows 10 machine. [Validation details](KNOWN-ISSUES.md).

Remove original Interceptor from an overlapping controller stack before association.
The separate keyboard/mouse edition can remain installed.
