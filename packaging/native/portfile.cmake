vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/download/sdk-v1.0.13/interceptor-gamepad-controller-sdk-1.0.13.tar.gz"
    FILENAME "interceptor-gamepad-controller-sdk-1.0.13.tar.gz"
    SHA512 60fe387df29e39be13e0af7189cc4a2f4ea7d16aff08df1960beed938c084ad4058e6695766721b3eb3321cc06dd74667f429a1f51fb6c211faca3d3c0991a43
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME InterceptorGamepadControllerSDK CONFIG_PATH lib/cmake/InterceptorGamepadControllerSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
