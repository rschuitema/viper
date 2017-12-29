#pragma once

#include <stdint.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <libusb.h>
#pragma GCC diagnostic pop
#include "gmock/gmock.h"

typedef int32_t STATUS_T;

/*
    Since lib usb device is a opaque data type provide some definition.
*/
struct libusb_device {
    int id;
};

struct libusb_device_handle {
    int id;
};
struct libusb_context {
    int id;
};

class LibUsbMock
{
public:
    MOCK_METHOD1(init, STATUS_T(libusb_context**));
    MOCK_METHOD1(exit, void(libusb_context*));
    MOCK_METHOD0(get_version, const struct libusb_version*());

    MOCK_METHOD2(set_debug, void(libusb_context*, int));

    MOCK_METHOD1(error_name, const char * (int));
    MOCK_METHOD1(has_capability, int(uint32_t));

    MOCK_METHOD9(hotplug_register_callback, int(libusb_context*,
                                                libusb_hotplug_event,
                                                libusb_hotplug_flag,
                                                int, int, int,
                                                libusb_hotplug_callback_fn,
                                                void*,
                                                libusb_hotplug_callback_handle*));

    MOCK_METHOD2(get_device_list, ssize_t(libusb_context* , libusb_device***));
    MOCK_METHOD2(get_device_descriptor, int(libusb_device*, struct libusb_device_descriptor*));
    MOCK_METHOD1(get_device_speed, int(libusb_device*));
    MOCK_METHOD1(get_bus_number, uint8_t(libusb_device*));
    MOCK_METHOD1(get_port_number, uint8_t(libusb_device*));
    MOCK_METHOD3(get_port_numbers, int(libusb_device*, uint8_t*, int));

    MOCK_METHOD2(open, int(libusb_device *dev, libusb_device_handle **handle));
    MOCK_METHOD3(open_device_with_vid_pid, libusb_device_handle*(libusb_context*, uint16_t, uint16_t));
    MOCK_METHOD1(close, void(libusb_device_handle *handle));
    MOCK_METHOD2(kernel_driver_active, int(libusb_device_handle*, int));
    MOCK_METHOD2(detach_kernel_driver, int(libusb_device_handle*, int));
    MOCK_METHOD4(get_string_descriptor_ascii, int(libusb_device_handle *dev, uint8_t desc_index, unsigned char *data, int length));

    MOCK_METHOD3(handle_events_timeout_completed, int(libusb_context*, struct timeval*, int*));
    MOCK_METHOD2(free_device_list, void(libusb_device**,int));

    MOCK_METHOD8(control_transfer, int( libusb_device_handle *  dev_handle,
                                        uint8_t     bmRequestType,
                                        uint8_t     bRequest,
                                        uint16_t    wValue,
                                        uint16_t    wIndex,
                                        unsigned char *     data,
                                        uint16_t    wLength,
                                        unsigned int    timeout));

    MOCK_METHOD6(bulk_transfer, int (struct libusb_device_handle *   dev_handle,
                                     unsigned char   endpoint,
                                     unsigned char * data,
                                     int             length,
                                     int *           transferred,
                                     unsigned int    timeout));

    MOCK_METHOD2(get_active_config_descriptor, int(libusb_device *dev,
                                                   struct libusb_config_descriptor **config));
    MOCK_METHOD1(free_config_descriptor, void(struct libusb_config_descriptor *config));

    MOCK_METHOD2(claim_interface, int(libusb_device_handle *dev, int interface_number));
    MOCK_METHOD2(release_interface, int(libusb_device_handle *dev, int interface_number));

    MOCK_METHOD2(clear_halt, int(libusb_device_handle *dev, unsigned char endpoint));
    MOCK_METHOD1(reset_device, int(libusb_device_handle *dev));

    MOCK_METHOD2(get_configuration, int(libusb_device_handle *dev, int *config));
};


extern LibUsbMock* pLibUsbMock;

/* Boost requires a simple class to construct, since we use Gmock to wrap a C interface
we need to use pLibUsbMock. This simple struct just wraps to this specific pointer */
struct BoostLibUsbMock {
    BoostLibUsbMock()
    {
         pLibUsbMock = new LibUsbMock();
    }
    ~BoostLibUsbMock()
    {
        delete pLibUsbMock;
    }
};
