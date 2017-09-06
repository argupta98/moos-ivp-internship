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

ADD_EXECUTABLE(pGenPath ${SRC})

TARGET_LINK_LIBRARIES(pGenPath
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   geometry)

