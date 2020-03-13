add_library(libpushbullet SHARED
            lib/pb_config.c
            lib/pb_device.c
            lib/pb_devices.c
            lib/pb_pushes.c
            lib/pb_requests.c
            lib/pb_user.c)
target_compile_options(libpushbullet PRIVATE -Wall -Wextra)
target_include_directories(libpushbullet
                           PRIVATE include
                           PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                           PRIVATE ${EVENT_INCLUDE_DIRS}
                           PRIVATE ${CONFIG_INCLUDE_DIRS})
target_link_libraries(libpushbullet ${EVENT_LDFLAGS} ${CONFIG_LDFLAGS})
set_target_properties(libpushbullet PROPERTIES PUBLIC_HEADER "include/pushbullet.h")
set_target_properties(libpushbullet PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(libpushbullet PROPERTIES SOVERSION 0)

# Set the location for library installation -- i.e., /usr/lib in this case not
# really necessary in this example. Use "sudo make install" to apply
install(TARGETS libpushbullet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/netlogging)
