#--------------------------------------------------------
# The CMakeLists.txt for:                       uFldHazardMgr
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  HazardMgr.cpp
  HazardMgr_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(uFldHazardMgrY ${SRC})

TARGET_LINK_LIBRARIES(uFldHazardMgrY
   ${MOOS_LIBRARIES}
   ufld_hazards
   geometry
   apputil
   mbutil
   ufield
   ${SYSTEM_LIBS}
)

