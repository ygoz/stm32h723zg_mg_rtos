cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME rtos_mg)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${CMAKE_SOURCE_DIR}/Core/Src/mongoose.c

    ${CMAKE_SOURCE_DIR}/Core/Src/http/web_ui/file_system/packed_fs.c

    ${CMAKE_SOURCE_DIR}/Core/Src/http/settings/network.c

    ${CMAKE_SOURCE_DIR}/Core/Src/http/routers/get_router.c
    ${CMAKE_SOURCE_DIR}/Core/Src/http/routers/post_router.c
    ${CMAKE_SOURCE_DIR}/Core/Src/http/routers/main_router.c
    
    ${CMAKE_SOURCE_DIR}/Core/Src/http/FOTA/firmware_update.c
    ${CMAKE_SOURCE_DIR}/Core/Src/flash/internal_flash.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_SOURCE_DIR}/Core/Inc

    ${CMAKE_SOURCE_DIR}/Core/Inc/http/web_ui/file_system
    ${CMAKE_SOURCE_DIR}/Core/Inc/http/settings
    ${CMAKE_SOURCE_DIR}/Core/Inc/http/routers
    ${CMAKE_SOURCE_DIR}/Core/Inc/http/FOTA

    ${CMAKE_SOURCE_DIR}/Core/Inc/flash

    ${CMAKE_SOURCE_DIR}/Core/Inc/HaGeneral
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

# Generate a .bin file after building
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating binary file from ELF"
)