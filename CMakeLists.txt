# CMakeList.txt : CMake project for NES Emulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Chess_engine")

include(FetchContent)


# ------------------------------
# SDL2 Download
# ------------------------------
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.8   # pick a stable release
)

FetchContent_MakeAvailable(SDL2)

# ------------------------------
# SDL2_Image Download
# ------------------------------
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.6.3
)

set(SDL2IMAGE_INSTALL OFF)

FetchContent_MakeAvailable(SDL2_image)





# ------------------------------
# Source and Header files
# ------------------------------
set( SOURCE_PATH "${PROJECT_SOURCE_DIR}/src" )
set( INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include" )

file( GLOB_RECURSE INCLUDES "${INCLUDE_PATH}/*.h" )
file( GLOB_RECURSE CPPS "${SOURCE_PATH}/*.cpp" )

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${CPPS} ${INCLUDES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_PATH})
# Include SDL2_image headers
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_image_SOURCE_DIR}/include)


# ------------------------------
# Link SDL2 & SDL2_image
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2        # Core SDL2
    SDL2::SDL2main    # Windows entry point
    SDL2_image::SDL2_image  # SDL2_image
)


# ------------------------------
# Copy resources and DLLs to output directory
# ------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_image>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)