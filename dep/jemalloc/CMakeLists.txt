# This file is part of the BlizzLikeCore Project.
# See CREDITS.md and LICENSE.md files for Copyright information.

file(GLOB sources src/*.c)

set(jemalloc_STAT_SRC
  ${sources}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_definitions(-D_GNU_SOURCE -D_REENTRANT)

add_library(jemalloc STATIC ${sources})
