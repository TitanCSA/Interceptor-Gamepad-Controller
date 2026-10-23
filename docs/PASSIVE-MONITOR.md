# Passive IN / OUT monitoring

Available with the current Microsoft-signed kernel. For a permanent side-by-side live monitor, use [GamePad Remapper](https://github.com/TitanCSA/GamePad-Remapper), which includes a separate passive reader.

| Measurement | Exact boundary |
| --- | --- |
| IN | A report received on this filtered controller stack, before changes made at that layer. |
| OUT | A successful report completion from the filter towards Windows, after any modification. |

A translated HID view is a Windows-derived report, not another independent electrical USB measurement. OUT does not prove that a particular game or browser consumed the report. Several Windows readers can produce several OUT completions for one IN; their queues may have different ages. These counts are not controller FPS. Unrecognized formats, missing metadata, source errors and ring loss are reported explicitly rather than shown as neutral controls. No measured value exists until the corresponding report arrives.

Observation does not create an interception context, associate devices, suppress input, apply a script, or apply input changes. Subscribing starts the shared one-hour allowance; observation is revoked at expiry. It cannot reset or pause the allowance. It requires the installed compatible driver and administrator access. Read-only monitor IOCTLs cannot inject input. HID observation also uses normal asynchronous Windows readers after the driver's independent source is ready; they do not own a foreign application's capture.

The versioned monitor protocol is separate from the two-file SDK capture API. Its implementation is available in GamePad Remapper's `App/Capture/DriverMonitorProtocol.h` and `DriverMonitor.cpp`. It supports querying capabilities, subscribing a notification event, reading a bounded record batch, and requesting device metadata/preparsed HID data. Records include family-local identity, a driver-lifetime instance token, direction, sequence, QPC timestamp, raw bytes, validity/error flags and decoded native state where applicable. The per-capture text tokens used by `Get_Context()` belong to the SDK capture API and are different identities.

The ring is allocated only while an observer is subscribed. Without observers, the report path skips telemetry copying and allocation. A slow monitor can lose records from this bounded ring without blocking controller delivery; lost-record counts are explicit. Metadata changes and terminal removals invalidate outdated state. HID controls retain descriptor identities and report IDs; native XUSB/GIP states include partial Guide updates where available. Proprietary formats and hardware-specific features are not guessed.

[Validation scope and remaining limits](KNOWN-ISSUES.md). The tests are bounded virtual-device and isolated regression tests, not a claim of zero latency or support for every physical controller.