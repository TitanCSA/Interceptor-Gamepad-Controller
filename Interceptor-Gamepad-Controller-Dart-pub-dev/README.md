# Interceptor-Gamepad-Controller Dart / Flutter Windows SDK

`dart pub add interceptor_gamepad_controller` or `flutter pub add interceptor_gamepad_controller`

Windows x64 and Dart 3.4+. Direct FFI to the product SDK; no Python dependency or driver installer. Supported families: hid, xinput, xinputhid, gip.

```dart
import 'package:interceptor_gamepad_controller/interceptor_gamepad_controller.dart';
Future<void> main() async {
  final client = await Interceptor.open();
  try { print(client.info()); } finally { client.close(); }
}
```

Calls are synchronous: use a worker isolate for a Flutter UI. Send `client.cancellationToken` to another isolate to cancel an active receive; the token contains only an immutable library path and session number. See [API documentation](doc/API.md).

For compiled Dart and deployed Flutter Windows apps, bundle `lib/native/InterceptorGamepadControllerBridge.dll` next to the executable, or provide its absolute `libraryPath`. This plain Dart package does not automatically alter a Flutter build. Native licensing, trial limits and driver requirements are unchanged.
