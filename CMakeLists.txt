cmake_minimum_required(VERSION 3.20)
project(engine_fluxo CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Biblioteca com os módulos que têm .cpp. Os demais (spatial_partition.hpp,
# vertex_delta_codec.hpp, fold, symmetry, procedural...) são só headers.
add_library(fluxo STATIC
    src/core/hardware_profiler.cpp
    src/stream/slice_streamer.cpp
    src/stream/window_controller.cpp
    src/render/bounding_reference.cpp
    src/render/projective_pipeline.cpp
)
target_include_directories(fluxo PUBLIC src)

add_executable(engine_fluxo src/main.cpp)
target_link_libraries(engine_fluxo PRIVATE fluxo)

# --- Testes ---------------------------------------------------------------
# Rodar sempre a partir da raiz do projeto (os testes leem assets/...):
#   cmake -S . -B build && cmake --build build && ctest --test-dir build
enable_testing()

function(add_fluxo_test name)
    add_executable(${name} tests/${name}.cpp)
    target_link_libraries(${name} PRIVATE fluxo)
    add_test(NAME ${name} COMMAND ${name} ${ARGN} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endfunction()

add_fluxo_test(test_slice_streamer)
add_fluxo_test(test_morton assets/world_bunny_sparse_32.stream)
add_fluxo_test(test_arvore_via_v)
add_fluxo_test(test_section22_engine)
add_fluxo_test(test_section25_engine)
add_fluxo_test(test_section29_reflexao_engine)
# Este teste lê teclas da entrada padrão: manda 'wwwddq' (esperado x=2.0, z=3.0).
add_executable(test_keyboard_input tests/test_keyboard_input.cpp)
target_link_libraries(test_keyboard_input PRIVATE fluxo)
add_test(NAME test_keyboard_input
         COMMAND sh -c "printf wwwddq | $<TARGET_FILE:test_keyboard_input>"
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_fluxo_test(test_real_input_loop)
add_fluxo_test(test_render_loop)
add_fluxo_test(test_main_photo_integration)   # modo foto: 57 Draw + 3 Photo = 60 (README v24)
add_fluxo_test(test_vanishing_point_engine)   # ponto de fuga medido, slope=-1 (README v21)
add_fluxo_test(test_inplace_parity)           # BuildInPlace x Build: mesma contagem (README v25)
add_fluxo_test(test_wallclock_sparse)         # tempo de parede da construcao esparsa (README v25)

# Paridade de nós entre WFLB, WFLS e WFLV (bunny 64): 0 divergências esperadas.
add_fluxo_test(test_formatos
    assets/world_bunny_64.stream
    assets/world_bunny_sparse_64.stream
    assets/world_bunny_vertexdelta_64.stream)
