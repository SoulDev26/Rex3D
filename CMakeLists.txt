cmake_minimum_required(VERSION 3.2)

project(Rex LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall -O1")

include_directories(PRIVATE ${CMAKE_SOURCE_DIR})
set(TRON_UTILS_SRC 
    src/utils/ref.c
    src/utils/str.c
)
set(TRON_MATH_SRC
    src/math/vec.c
    src/math/matrix.c
)
set(TRON_SRC 
    src/tron.c
    src/model.c
    src/camera.c
    
    ${TRON_UTILS_SRC}
    ${TRON_MATH_SRC}
)

add_executable(rex ${TRON_SRC})
set_property(TARGET rex PROPERTY C_STANDARD 23)
target_link_libraries(rex m SDL2)
