cmake_minimum_required(VERSION 3.0)
project(MyDaemon)

set(SOURCES ../vscdaemon.cpp ../vscd_workingdirectory.cpp ../vscd_pid.cpp ../vscd_socket.cpp ../vscd_daemon.cpp ../vscd_service.cpp)
set(HEADERS ../vscd_workingdirectory.h ../vscd_pid.h ../vscd_socket.h ../vscd_daemon.h ../vscd_service.h)

find_package(OpenSSL REQUIRED)

add_executable(vscdaemon ${SOURCES} ${HEADERS})

target_link_libraries(vscdaemon OpenSSL::Crypto)

# Add any necessary compile options or additional dependencies
# target_compile_options(vscdaemon PRIVATE ...)
# target_link_libraries(vscdaemon PRIVATE ...)

install(TARGETS vscdaemon DESTINATION /usr/local/sbin)

