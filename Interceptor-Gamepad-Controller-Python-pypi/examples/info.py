from interceptor_gamepad_controller import Client
with Client() as client:
    print(client.info())
