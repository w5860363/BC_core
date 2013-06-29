# This file is part of the BlizzLikeCore Project.
# See CREDITS.md and LICENSE.md files for Copyright information.

if( SERVERS )
  set(sources_Debugging
    ${CMAKE_SOURCE_DIR}/src/shared/Debugging/WheatyExceptionReport.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/Debugging/WheatyExceptionReport.h
  )
  add_subdirectory(genrev)
  add_subdirectory(framework)
  add_subdirectory(shared)
  add_subdirectory(authserver)
  add_subdirectory(worldserver)
  add_subdirectory(collision)
  add_subdirectory(game)
  add_subdirectory(scripts)
else()
  if( TOOLS )
    add_subdirectory(framework)
    add_subdirectory(collision)
  endif()
endif()

if(TOOLS)
  add_subdirectory(tools)
endif(TOOLS)
