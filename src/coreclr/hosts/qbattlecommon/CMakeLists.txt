project(qbattlecommon)

add_compile_options(-fPIC)

_add_library(qbattlecommon
    STATIC
    qbattlecommon.cpp
)

if(CLR_CMAKE_PLATFORM_LINUX)
  target_link_libraries(qbattlecommon dl)
endif(CLR_CMAKE_PLATFORM_LINUX)
