# Gamepad and controller compatibility

This edition captures **controller input only**: HID game controllers,
XInput/XUSB, XInputHID and GIP. There is no keyboard/mouse injection API or
keyboard/mouse mapper. The driver leaves excluded stacks in passthrough.

| Family | Supported approach | Limits |
| --- | --- | --- |
| HID gamepad / joystick | Descriptor-driven decoding and re-encoding; dynamic axes, triggers, hats, buttons and other declared values | Proprietary meanings and malformed descriptors cannot be inferred |
| XInput / XUSB | Supported wired Xbox 360-style USB transport profiles | Not an `XInputGetState` hook or every adapter/proprietary wireless protocol |
| XInputHID | Supported Windows controller HID collection and source association | Collection/connection must expose the expected profile |
| GIP | Validated controller metadata and complete input frames, including supported extended frames | No firmware/authentication/audio command injection; unsupported formats are explicit |

Compatible profiles can include DS4 / DualShock 4 (PS4), DualSense (PS5), wired
Xbox 360, Xbox One and Xbox Series X|S. A model name alone is not evidence that
every firmware, Bluetooth mode, wireless adapter or vendor feature is supported.
The source descriptor/protocol and connection determine compatibility.

For HID and XInputHID, `input.state` describes the current report. Repeated X axes
remain separate fields with report/collection identity; no control is collapsed
solely because it shares a usage name. Check decoding flags before relying on
normalized values. Standard XUSB and GIP native states have their own layouts and
trigger/button ranges; they are not interchangeable binary structures.

An attached filter is not proof of physical capture. A device can be preparing
while Windows completes enumeration. Capture creation accepts future sources;
it does not require moving a stick at the instant you start. Diagnostics distinguish
absent, preparing, unsupported, unavailable and ready sources.

Original Interceptor must be removed from an overlapping controller stack before
association. The separate keyboard/mouse edition can remain installed.