cmake_minimum_required(VERSION 3.24)
project(InterceptorGamepadControllerSDK VERSION 1.0.13.0 LANGUAGES C CXX)
if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "This SDK requires Windows x64. No driver is installed by this build.")
endif()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
add_library(TitanInterceptorGamepadController STATIC
  "sdk/Interceptor/Interceptor.cpp"
)
add_library(InterceptorGamepadControllerSDK::sdk ALIAS TitanInterceptorGamepadController)
set_target_properties(TitanInterceptorGamepadController PROPERTIES EXPORT_NAME sdk)
target_compile_features(TitanInterceptorGamepadController PUBLIC cxx_std_17)
target_compile_definitions(TitanInterceptorGamepadController PUBLIC )
target_compile_definitions(TitanInterceptorGamepadController PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0A00)
if(MSVC)
  target_compile_options(TitanInterceptorGamepadController PRIVATE /utf-8 /EHsc /W4)
endif()
target_include_directories(TitanInterceptorGamepadController PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk>"
  "$<INSTALL_INTERFACE:include/>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk/Interceptor>"
  "$<INSTALL_INTERFACE:include/Interceptor>"
)
target_link_libraries(TitanInterceptorGamepadController PUBLIC advapi32 bcrypt cfgmgr32 comctl32 hid newdev ole32 setupapi user32)
install(TARGETS TitanInterceptorGamepadController EXPORT InterceptorGamepadControllerSDKTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY sdk/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
install(FILES LICENSE DESTINATION share/interceptor-gamepad-controller)
install(EXPORT InterceptorGamepadControllerSDKTargets NAMESPACE InterceptorGamepadControllerSDK:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/InterceptorGamepadControllerSDK)
configure_file("InterceptorGamepadControllerSDKConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/InterceptorGamepadControllerSDKConfig.cmake" @ONLY)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/InterceptorGamepadControllerSDKConfigVersion.cmake" VERSION 1.0.13.0 COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/InterceptorGamepadControllerSDKConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/InterceptorGamepadControllerSDKConfigVersion.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/InterceptorGamepadControllerSDK)
