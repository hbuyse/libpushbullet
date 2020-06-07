add_library(pushbullet SHARED
            lib/pb_config.c
            lib/pb_user.c
            lib/pb_device.c
            lib/pb_devices.c
            lib/pb_pushes.c
            lib/pb_requests.c
            lib/pb_session.c)
target_compile_options(pushbullet PRIVATE -Wall -Wextra)
target_include_directories(pushbullet
                           PRIVATE include
                           PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                           PRIVATE ${CONFIG_INCLUDE_DIRS})
target_link_libraries(pushbullet ${JANSSON_LDFLAGS} ${CURL_LDFLAGS} ${CONFIG_LDFLAGS} magic)
set_target_properties(pushbullet PROPERTIES PUBLIC_HEADER "include/pushbullet.h")
set_target_properties(pushbullet PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(pushbullet PROPERTIES SOVERSION 0)

# Set the location for library installation -- i.e., /usr/lib in this case not
# really necessary in this example. Use "sudo make install" to apply
install(TARGETS pushbullet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/netlogging)
