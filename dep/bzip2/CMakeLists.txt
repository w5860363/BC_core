# This file is part of the BlizzLikeCore Project.
# See CREDITS.md and LICENSE.md files for Copyright information.

file(GLOB sources *.c)

set(bzip2_STAT_SRCS
  ${sources}
)

include_directories(
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(bzip2 STATIC ${bzip2_STAT_SRCS})
