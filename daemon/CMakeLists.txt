# For the shared library:
set(PROJECT_LINK_LIBS pushbullet)
link_directories(build)

add_executable(pushbulletd
               daemon/main.c)
target_compile_options(pushbulletd PRIVATE -Wall -Wextra)
target_include_directories(pushbulletd
                           PRIVATE include
                           PRIVATE ${EVENT_INCLUDE_DIRS}
                           PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(pushbulletd
                      ${PROJECT_LINK_LIBS}
                      ${EVENT_LDFLAGS}
                      )
install(TARGETS pushbulletd RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
