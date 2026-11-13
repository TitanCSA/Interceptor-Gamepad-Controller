# Interceptor-Gamepad-Controller Python SDK

`pip install interceptor-gamepad-controller`

Windows x64, Python 3.10+. This package uses the product's own native SDK and driver identity. Supported families: hid, xinput, xinputhid, gip. No Python dependencies and no automatic driver installation.

```python
from interceptor_gamepad_controller import Client
with Client() as client:
    print(client.info())
    # Input operations require the matching driver installed separately.
```

See [API guide](docs/API.md). `Client` aliases the input-oriented `Interceptor` class. Native product licensing and trial restrictions are unchanged. Installing/importing this SDK does not initialize capture.
