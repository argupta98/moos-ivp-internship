#--------------------------------------------------------
# The CMakeLists.txt for:                       pGenPath
# Author(s):                              Arjun Gupta
#--------------------------------------------------------

SET(SRC
  GenPath.cpp
  GenPath_Info.cpp
  Point.cpp
  main.cpp
)

ADD_EXECUTABLE(pHazardPath ${SRC})

TARGET_LINK_LIBRARIES(pHazardPath
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   geometry)

