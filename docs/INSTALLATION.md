# Installation, diagnostics and recovery

The [complete 1.0.6.1 demo](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.6.1-demo)
contains Microsoft-signed kernel **1.0.4.0**, rebuilt examples and the updated SDK.
The two-hour allowance is unchanged. Test signing and development certificates are not required.
The Microsoft signature covers the driver package, not the installer or example EXEs.

## Install or upgrade

1. Close capture applications. Run this release's **InterceptorSetup.exe** and accept elevation.
2. Choose **Install / reinstall**. Replacing a viewer EXE alone does not update the installed kernel.
3. If requested, save your work and use Windows **Restart**, then reopen the same installer.
4. Repeat a requested restart/reopen until the operation completes without another restart request. Then use **Diagnose** and the viewer to verify the connected devices.

The console installer performs the same operations:

```powershell
.\InterceptorSetupConsole.exe /install
.\InterceptorSetupConsole.exe /uninstall
```

Exit code `3010` means **restart required**; rerun `/install` after restarting.
Windows never restarts automatically.

Use the examples supplied in this release. Rebuild custom applications with its **Interceptor.h + Interceptor.cpp**.
Wire ABI 4 remains unchanged, but older compiled clients can still use the obsolete XUSB SOURCE/mirror startup path and need rebuilding for the USB backend.

### Why an upgrade may request two restarts

Legacy XUSB installations used an upper filter and a secondary HID mirror.
Kernel 1.0.4.0 uses a USB lower filter and native Windows translation instead.
The first restart can be needed to replace the loaded image; after reopening, the installer migrates this product's old associations and may request another restart to load the new stacks.
It preserves other products' filter names and order, and does not force a live device restart.

The installer checks the loaded backend before accepting completion.
An older loaded backend keeps the operation pending rather than reporting success after skipping XUSB migration.
The same installer resumes that work; no scheduled installation or RunOnce continuation is added.

## Recover an earlier failed installation

If 1.0.6.0 ended with error 13, followed by a stopped driver or a root-control creation error, run **Install / reinstall** using **1.0.6.1**. It skips unrelated historical Xbox HID views during legacy cleanup, accepts valid empty Windows filter lists and reuses this product's unbound control endpoint. A manual registry cleanup is unnecessary for that case. Restart only when requested.

If restoration fails, the installer keeps the staged package and recovery tools available, and logs the failing stage, device and numeric Windows error. Include the exported log when reporting a different failure.

## Verify the connected devices

Installation and capture require administrator rights. Compiling an example does not install the driver.
Each example checks for this edition's service and explains how to install it when absent.

The installer lists this edition's controller families and its **control endpoint**, the application's connection to the driver rather than a physical gamepad.
A HID collection can use a service in its parent stack, so a blank service field is not necessarily a failure.
An associated filter alone does not prove that interception or every physical control works.

**Diagnose** checks recognized devices individually, reports installation/API state and provides copy/export actions.
Unexercised physical interception/modification remains **unverified**; a quiet controller is not automatically declared unsupported.
Compare the affected consumer too: the viewer, browser and `joy.cpl` use different paths.
[Validation limits](KNOWN-ISSUES.md) and [viewer troubleshooting](VIEWER-TROUBLESHOOTING.md).

## Editions and recovery

The keyboard/mouse and gamepad/controller editions have independent services and state; their installers leave the other edition intact.
Remove an overlapping original Interceptor installation first. Ambiguous stacking is refused.

Close capture applications before uninstalling. `/uninstall` restores this product's associations and removes its packages.
Normal client closure releases capture; the kernel also restores passthrough when a capture client terminates abruptly.
Product-specific precompiled recovery binaries are embedded in both installers.
Their startup marker and recovery state use this product's own identity; consult the installer status message and restart normally when requested.

Diagnostic exports can contain PnP instance IDs and controller protocol metadata.
Remove serial numbers before sharing them publicly.
