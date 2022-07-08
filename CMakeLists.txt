cmake_minimum_required(VERSION 3.23.2)

project(sysconfig_linux)

include_directories(${PROJECT_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON-C REQUIRED IMPORTED_TARGET json-c)

add_executable(sysconfig main.c mqtt_aws.c jsonparse_libcurl.c openssl_decode.c decrypt.c )

target_link_libraries(sysconfig PUBLIC -lmosquitto -lcurl -lcrypto PkgConfig::JSON-C )
