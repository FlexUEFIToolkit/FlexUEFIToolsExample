cmake_minimum_required(VERSION 3.16)
project(ignore_this_only_for_ide)
set(CMAKE_C_STANDARD 90)
execute_process(
    COMMAND uname -r
    OUTPUT_VARIABLE KERNEL_RELEASE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions(-D__KERNEL__ -DMODULE)



add_executable(${PROJECT_NAME} FlexUEFIToolkit.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC "/lib/modules/6.2.0/build"
    PUBLIC "/usr/src/linux-headers-6.2.0/include/"
    PUBLIC "/usr/src/linux-headers-6.2.0/arch/x86/include/"
    PUBLIC "/usr/src/linux-headers-6.2.0/arch/x86/include/generated"
    # PUBLIC "/usr/src/linux-hwe-6.2-headers-6.2.0-39/include/"4
)
