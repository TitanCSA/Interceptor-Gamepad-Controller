#ifndef INTERCEPTOR_GAMEPAD_CONTROLLER_BRIDGE_H
#define INTERCEPTOR_GAMEPAD_CONTROLLER_BRIDGE_H
#include <stdint.h>
#ifdef IC_BRIDGE_EXPORTS
#define INTERCEPTOR_GAMEPAD_CONTROLLER_API __declspec(dllexport)
#else
#define INTERCEPTOR_GAMEPAD_CONTROLLER_API __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
/* Windows x64, cdecl, natural alignment. Never free borrowed pointers.
   Receive views remain valid until the next receive or close. Copy data before
   another thread calls either. Every function catches C++ exceptions. */
enum {
    INTERCEPTOR_GAMEPAD_CONTROLLER_KEYBOARD = 1,
    INTERCEPTOR_GAMEPAD_CONTROLLER_MOUSE = 2,
    INTERCEPTOR_GAMEPAD_CONTROLLER_HID = 3,
    INTERCEPTOR_GAMEPAD_CONTROLLER_XINPUT = 4,
    INTERCEPTOR_GAMEPAD_CONTROLLER_XINPUTHID = 5,
    INTERCEPTOR_GAMEPAD_CONTROLLER_GIP = 6
};
enum {
    INTERCEPTOR_GAMEPAD_CONTROLLER_STATE = 0,
    INTERCEPTOR_GAMEPAD_CONTROLLER_AXIS = 1,
    INTERCEPTOR_GAMEPAD_CONTROLLER_TRIGGER = 2,
    INTERCEPTOR_GAMEPAD_CONTROLLER_HAT = 3,
    INTERCEPTOR_GAMEPAD_CONTROLLER_VALUE = 4,
    INTERCEPTOR_GAMEPAD_CONTROLLER_BUTTON = 5,
    INTERCEPTOR_GAMEPAD_CONTROLLER_BINARY = 6,
    INTERCEPTOR_GAMEPAD_CONTROLLER_REPORT = 7
};
enum {
    INTERCEPTOR_GAMEPAD_CONTROLLER_NORMALIZED = 1,
    INTERCEPTOR_GAMEPAD_CONTROLLER_RELATIVE = 2,
    INTERCEPTOR_GAMEPAD_CONTROLLER_HAS_NULL = 4,
    INTERCEPTOR_GAMEPAD_CONTROLLER_IS_NULL = 8,
    INTERCEPTOR_GAMEPAD_CONTROLLER_IS_TRIGGER = 16,
    INTERCEPTOR_GAMEPAD_CONTROLLER_READ_ONLY = 32
};
#pragma pack(push, 8)
typedef struct interceptor_gamepad_controller_error {
    uint32_t code, accepted;
    char message[768];
} interceptor_gamepad_controller_error;
typedef struct interceptor_gamepad_controller_field {
    const char *name;
    int64_t value, minimum, maximum, physical_minimum, physical_maximum;
    double normalized;
    uint32_t group, index, usage_page, usage, collection, report_id, data_index, element;
    uint32_t flags, units, units_exponent, bit_size;
    uint8_t *data;
    uint32_t length, reserved;
} interceptor_gamepad_controller_field;
typedef struct interceptor_gamepad_controller_event {
    uint32_t kind, blocked;
    uint64_t sequence, timestamp;
    const char *token;
    interceptor_gamepad_controller_field *fields;
    uint32_t field_count, decode_error, decode_complete, parser_status, device, reserved;
} interceptor_gamepad_controller_event;
typedef struct interceptor_gamepad_controller_batch {
    uint64_t id;
    uint32_t count, reserved;
    interceptor_gamepad_controller_event *events;
} interceptor_gamepad_controller_batch;
typedef struct interceptor_gamepad_controller_device {
    uint32_t kind, id;
    uint16_t vid, pid, usage_page, usage;
    const char *token;
    const char *name;
    const char *hardware_id;
} interceptor_gamepad_controller_device;
typedef struct interceptor_gamepad_controller_devices {
    uint32_t count, reserved;
    const interceptor_gamepad_controller_device *items;
} interceptor_gamepad_controller_devices;
typedef struct interceptor_gamepad_controller_blob {
    uint32_t applicable, format, length, reserved;
    const uint8_t *data;
} interceptor_gamepad_controller_blob;
typedef struct interceptor_gamepad_controller_init {
    uint32_t ready, reboot_required, associated, already_associated, skipped, reserved;
} interceptor_gamepad_controller_init;
typedef struct interceptor_gamepad_controller_info {
    uint32_t installed, active, trial_state, input_mask, controller_mask, device_count, queued,
        last_reset;
    uint64_t remaining_ms, timestamp_frequency, generation;
} interceptor_gamepad_controller_info;
#pragma pack(pop)
INTERCEPTOR_GAMEPAD_CONTROLLER_API const char* interceptor_gamepad_controller_product_name(void);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_input_mask(void);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_controller_mask(void);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_version(void);
/* sizeof: 1 field, 2 event, 3 batch, 4 device, 5 error, 6 info. */
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_sizeof(uint32_t type);
/* Open is lazy: no driver capture, installation, timer or worker thread. */
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_open(uint64_t *session, interceptor_gamepad_controller_error *error);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_close(uint64_t session, interceptor_gamepad_controller_error *error);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_initialize(uint64_t session, interceptor_gamepad_controller_init *result, interceptor_gamepad_controller_error *error);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_info_get(uint64_t session, interceptor_gamepad_controller_info *result, interceptor_gamepad_controller_error *error);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_devices_get(uint64_t session, const interceptor_gamepad_controller_devices **result, interceptor_gamepad_controller_error *error);
/* timeout in milliseconds, UINT32_MAX means infinite. Cancel returns empty. */
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_receive(uint64_t session, uint32_t timeout, const interceptor_gamepad_controller_batch **result,
                             interceptor_gamepad_controller_error *error);
/* Counts inferred from ticket. Retry forwards only the unaccepted remainder. */
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_send(uint64_t session, uint64_t ticket, interceptor_gamepad_controller_error *error);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_discard(uint64_t session, uint64_t ticket, interceptor_gamepad_controller_error *error);
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_cancel(uint64_t session, interceptor_gamepad_controller_error *error);
/* Native HID report: report=1; common descriptor: report=0. */
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_descriptor(uint64_t session, const char *token, uint32_t report,
                                interceptor_gamepad_controller_blob *result, interceptor_gamepad_controller_error *error);
/* Synthetic input: kind and capture token select an existing source. Named
   STATE fields plus REPORT bytes use the same representation as receive.
   Input and field counts belong only to this low-level FFI; wrappers infer them. */
INTERCEPTOR_GAMEPAD_CONTROLLER_API uint32_t interceptor_gamepad_controller_inject(uint64_t session, const interceptor_gamepad_controller_event *inputs, uint32_t count,
                            interceptor_gamepad_controller_error *error);
#ifdef __cplusplus
}
static_assert(sizeof(interceptor_gamepad_controller_field) == 120 && sizeof(interceptor_gamepad_controller_event) == 64 && sizeof(interceptor_gamepad_controller_batch) == 24,
              "C ABI layout");
static_assert(sizeof(interceptor_gamepad_controller_device) == 40 && sizeof(interceptor_gamepad_controller_error) == 776 && sizeof(interceptor_gamepad_controller_info) == 56,
              "C ABI layout");
#endif
#endif
