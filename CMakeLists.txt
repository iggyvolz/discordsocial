cmake_minimum_required(VERSION 3.25)
project(discordsocial)

set(CMAKE_CXX_STANDARD 23)

set(PHP_ROOT CACHE PATH "Path to PHP installation directory")
set(DISCORD_SOCIAL_SDK_ROOT CACHE PATH "Path to Discord Social SDK installation directory")

include_directories(
        ${PHP_ROOT}/include/php
        ${PHP_ROOT}/include/php/main
        ${PHP_ROOT}/include/php/TSRM
        ${PHP_ROOT}/include/php/Zend
        ${PHP_ROOT}/include/php/ext
        ${PHP_ROOT}/include/php/ext/date/lib
        ${DISCORD_SOCIAL_SDK_ROOT}/include
)
if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        link_directories(${DISCORD_SOCIAL_SDK_ROOT}/bin/debug)
    else ()
        link_directories(${DISCORD_SOCIAL_SDK_ROOT}/bin/release)
    endif ()
else ()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        link_directories(${DISCORD_SOCIAL_SDK_ROOT}/lib/debug)
    else ()
        link_directories(${DISCORD_SOCIAL_SDK_ROOT}/lib/release)
    endif ()
endif ()
add_library(discordsocial SHARED main.cpp util.cpp
        discordpp.cpp
        activity.cpp
        client.cpp
        client_result.cpp
        authorization_code_verifier.cpp
        authorization_code_challenge.cpp
)
target_link_libraries(discordsocial PRIVATE discord_partner_sdk)
