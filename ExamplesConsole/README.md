# Gamepad / controller C++ examples

Install this product driver first, then run one example as administrator. Each example checks for its own driver service at startup. The library creates its capture automatically. Closing the application releases its context and restores passthrough. The demo lasts two hours from the first successful capture per Windows boot, shared by all applications using this edition.

| Executable | Purpose |
| --- | --- |
| `01_Controller_Minimal` | Receive and forward all supported controller families |
| `07_Manette_HID_Inverse_X_Y` | Invert descriptor-decoded HID X/Y axes |

Compile with `powershell -File .\Build.ps1 -Target ExamplesConsole` from the project root. Every example uses `Interceptor/Interceptor.h` and `Interceptor/Interceptor.cpp`.
