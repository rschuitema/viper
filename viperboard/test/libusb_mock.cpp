#include "libusb_mock.hpp"

LibUsbMock* pLibUsbMock;

extern "C" {

   STATUS_T libusb_init(libusb_context **ctx)
   {
       return pLibUsbMock->init(ctx);
   }

   void libusb_exit(libusb_context *ctx)
   {
       pLibUsbMock->exit(ctx);
   }

   const char * libusb_error_name(int errcode)
   {
        return pLibUsbMock->error_name(errcode);
   }

   const struct libusb_version* libusb_get_version()
   {
        return pLibUsbMock->get_version();
   }

   int libusb_has_capability(uint32_t capability)
   {
        return pLibUsbMock->has_capability(capability);
   }

   int libusb_hotplug_register_callback(libusb_context *ctx,
                                        libusb_hotplug_event events,
                                        libusb_hotplug_flag flags,
                                        int vendor_id, int product_id,
                                        int dev_class,
                                        libusb_hotplug_callback_fn cb_fn,
                                        void *user_data,
                                        libusb_hotplug_callback_handle *handle)
   {
       return pLibUsbMock->hotplug_register_callback(ctx, events, flags,
                                                     vendor_id, product_id, dev_class,
                                                      cb_fn, user_data, handle);
   }
   ssize_t libusb_get_device_list(libusb_context *ctx, libusb_device ***list)
   {
       return pLibUsbMock->get_device_list(ctx, list);
   }

   int libusb_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc)
   {
        return pLibUsbMock->get_device_descriptor(dev, desc);
   }

   int libusb_get_device_speed(libusb_device *dev)
   {
        return pLibUsbMock->get_device_speed(dev);
   }

   uint8_t libusb_get_bus_number(libusb_device *dev)
   {
        return pLibUsbMock->get_bus_number(dev);
   }
   uint8_t libusb_get_port_number(libusb_device *dev)
   {
        return pLibUsbMock->get_port_number(dev);
   }
   int libusb_get_port_numbers(libusb_device *dev, uint8_t* port_numbers, int port_numbers_len)
   {
        return pLibUsbMock->get_port_numbers(dev, port_numbers, port_numbers_len);
   }

   int libusb_open(libusb_device *dev, libusb_device_handle **handle)
   {
        return pLibUsbMock->open(dev, handle);
   }

   libusb_device_handle* libusb_open_device_with_vid_pid(libusb_context *ctx, uint16_t vendor_id, uint16_t product_id)
   {
        return pLibUsbMock->open_device_with_vid_pid(ctx, vendor_id, product_id);
   }

   void libusb_close(libusb_device_handle *handle)
   {
        pLibUsbMock->close(handle);
   }

   int libusb_kernel_driver_active(libusb_device_handle* handle, int interface_number)
   {
       return pLibUsbMock->kernel_driver_active(handle, interface_number);
   }
   
   int libusb_detach_kernel_driver(libusb_device_handle* handle, int interface_number)
   {
       return pLibUsbMock->detach_kernel_driver(handle, interface_number);
   }

   int libusb_get_string_descriptor_ascii(libusb_device_handle *dev, uint8_t desc_index, unsigned char *data, int length)
   {
        return pLibUsbMock->get_string_descriptor_ascii(dev, desc_index, data, length);
   }
   void libusb_set_debug(libusb_context* ctx, int level)
   {
        return pLibUsbMock->set_debug(ctx, level);
   }

   int libusb_handle_events_timeout_completed(libusb_context *ctx, struct timeval *tv, int *completed)
   {
        return pLibUsbMock->handle_events_timeout_completed(ctx, tv, completed);
   }

   void libusb_free_device_list(libusb_device **list, int unref_devices) 
   {
        return pLibUsbMock->free_device_list(list, unref_devices);
   }


   int libusb_control_transfer ( libusb_device_handle *  dev_handle,
                                 uint8_t     bmRequestType,
                                 uint8_t     bRequest,
                                 uint16_t    wValue,
                                 uint16_t    wIndex,
                                 unsigned char *     data,
                                 uint16_t    wLength,
                                 unsigned int    timeout)
   {
        return pLibUsbMock->control_transfer(dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
   }

    int libusb_bulk_transfer(struct libusb_device_handle *   dev_handle,
                             unsigned char   endpoint,
                             unsigned char * data,
                             int             length,
                             int *           transferred,
                             unsigned int    timeout)
    {
        return pLibUsbMock->bulk_transfer(dev_handle, endpoint, data, length, transferred, timeout);
    }

    int libusb_get_active_config_descriptor(libusb_device *dev,
         struct libusb_config_descriptor **config)
    {
        return pLibUsbMock->get_active_config_descriptor(dev, config);
    }

    void libusb_free_config_descriptor(struct libusb_config_descriptor *config)
    {
        return pLibUsbMock->free_config_descriptor(config);
    }

    int libusb_claim_interface(libusb_device_handle *dev, int interface_number)
    {
        return pLibUsbMock->claim_interface(dev, interface_number);
    }
    int libusb_release_interface(libusb_device_handle *dev, int interface_number)
    {
        return pLibUsbMock->release_interface(dev, interface_number);
    }

    int libusb_clear_halt(libusb_device_handle *dev, unsigned char endpoint)
    {
        return pLibUsbMock->clear_halt(dev, endpoint);
    }
    int libusb_reset_device(libusb_device_handle *dev)
    {
        return pLibUsbMock->reset_device(dev);
    }
    int libusb_get_configuration(libusb_device_handle *dev, int *config)
    {
        return pLibUsbMock->get_configuration(dev, config);
    }
}
