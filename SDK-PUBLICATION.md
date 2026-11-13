# SDK publication status

Current source layout and registry coverage. A GitHub download is distinct from publication on a central registry.

| Target | Status | Package / documentation |
| --- | --- | --- |
| CPP/vcpkg | published_custom_registry | interceptor-gamepad-controller |
| CPP/conan | public_recipe_builds_locally |  [Source](packaging/native/) |
| CPP/nuget | paused_pending_owner_email_confirmation | Interceptor-Gamepad-Controller.Native |
| NET/nuget | built_verified_publication_on_hold_by_owner | InterceptorGamepadController.SDK [Source](Interceptor-Gamepad-Controller-NET-nuget/) |
| Python/pypi | built_fixture_and_production_verified | interceptor-gamepad-controller [Source](Interceptor-Gamepad-Controller-Python-pypi/) |
| Python/conda-forge | built_tested_requires_official_review | interceptor-gamepad-controller [Source](packaging/conda/) |
| Rust/cargo | published_install_verified | [interceptor-gamepad-controller](https://crates.io/crates/interceptor-gamepad-controller) [Source](Interceptor-Gamepad-Controller-Rust-cargo/) |
| JavaScript/npm | awaiting_browser_publication_authentication | [interceptor-gamepad-controller](https://www.npmjs.com/package/interceptor-gamepad-controller) [Source](Interceptor-Gamepad-Controller-JavaScript-npm/) |
| Java/maven | published_install_verified | [interceptor-gamepad-controller](https://central.sonatype.com/artifact/io.github.titancsa/interceptor-gamepad-controller/1.0.0) [Source](Interceptor-Gamepad-Controller-Java-maven/) |
| Java/jitpack | published_install_verified | [interceptor-gamepad-controller](https://jitpack.io/#TitanCSA/Interceptor-Gamepad-Controller/java-sdk-v1.0.0) [Source](Interceptor-Gamepad-Controller-Java-maven/) |
| Go/modules | published_install_verified | [interceptor-gamepad-controller](https://pkg.go.dev/github.com/TitanCSA/Interceptor-Gamepad-Controller/Interceptor-Gamepad-Controller-Go-modules@v1.0.0) [Source](Interceptor-Gamepad-Controller-Go-modules/) |
| Go/pkg-go-dev | indexed_verified | [Documentation](https://pkg.go.dev/github.com/TitanCSA/Interceptor-Gamepad-Controller/Interceptor-Gamepad-Controller-Go-modules@v1.0.0) |
| PowerShell/gallery | published_install_verified | [InterceptorGamepadController](https://www.powershellgallery.com/packages/InterceptorGamepadController/1.0.0) [Source](Interceptor-Gamepad-Controller-PowerShell-gallery/) |
| Dart/pub-dev | published_install_verified | [interceptor_gamepad_controller](https://pub.dev/packages/interceptor_gamepad_controller) [Source](Interceptor-Gamepad-Controller-Dart-pub-dev/) |
| Ruby/rubygems | published_install_verified | [interceptor-gamepad-controller](https://rubygems.org/gems/interceptor-gamepad-controller) [Source](Interceptor-Gamepad-Controller-Ruby-rubygems/) |
| PHP/packagist | composer_source_and_tag_published | [titancsa/interceptor-gamepad-controller](https://packagist.org/packages/titancsa/interceptor-gamepad-controller) |

Native SDK source and CMake/Conan/vcpkg/NuGet recipes share [`packaging/native`](packaging/native/LAYOUT.md). Previous release archives and tags remain unchanged.

Application and driver prerequisites and original licensing remain those documented by each SDK. No driver installer is bundled with these source packages.
