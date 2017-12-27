if (NOT LIBUSB_FOUND)

    find_package(PkgConfig)
    if (PKG_CONFIG_FOUND)
        pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
    else (PKG_CONFIG_FOUND)
        message (FATAL_ERROR "Could not find PkgConfig")
    endif (PKG_CONFIG_FOUND)

    find_package_handle_standard_args(LIBUSB DEFAULT_MSG LIBUSB_LIBRARIES LIBUSB_INCLUDE_DIRS)

endif (NOT LIBUSB_FOUND)
