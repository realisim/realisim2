set(BOX2D_DIR ${CMAKE_SOURCE_DIR}/thirdParties/box2d/v2.4.1)
set(BOX2D_INCLUDE_DIR ${BOX2D_DIR}/include)

include_directories(${BOX2D_INCLUDE_DIR})

set(BOX2D_LIB 
    debug ${BOX2D_DIR}/lib/debug/box2d.lib
    optimized ${BOX2D_DIR}/lib/release/box2d.lib)