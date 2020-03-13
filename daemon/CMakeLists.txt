add_executable(pushbulletd
               daemon/main.c)
target_compile_options(pushbulletd PRIVATE -Wall -Wextra)
target_include_directories(pushbulletd
                           PRIVATE include
                           PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(pushbulletd
                      pushbullet
                      dl
                      ${EVENT_LDFLAGS}
                      ${CONFIG_LDFLAGS})
install(TARGETS pushbulletd RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
