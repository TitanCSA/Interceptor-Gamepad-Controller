require 'interceptor_gamepad_controller'
InterceptorGamepadController::Interceptor.open { |client| p client.info }
