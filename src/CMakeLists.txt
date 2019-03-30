include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

# Compile sources
add_executable(mblusb mblusb.c)
target_include_directories(mblusb PUBLIC ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(mblusb ${LIBUSB_LIBRARIES})
target_compile_options(mblusb PUBLIC ${LIBUSB_CFLAGS_OTHER})
