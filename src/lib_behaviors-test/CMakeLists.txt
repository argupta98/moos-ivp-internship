#--------------------------------------------------------
# The CMakeLists.txt for:              lib_behaviors-test
# Author(s):			       Arjun Gupta                            
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  # Windows Libraries
  SET(SYSTEM_LIBS
      )
else (${WIN32})
  # Linux and Apple Libraries
  SET(SYSTEM_LIBS
      m )
endif (${WIN32})


MACRO(ADD_BHV BHV_NAME)
  ADD_LIBRARY(${BHV_NAME} SHARED ${BHV_NAME}.cpp)
  TARGET_LINK_LIBRARIES(${BHV_NAME} 
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )
ENDMACRO(ADD_BHV)


#--------------------------------------------------------
#                                      BHV_SimpleWaypoint
#--------------------------------------------------------
#ADD_LIBRARY(BHV_SimpleWaypoint SHARED 
#   BHV_SimpleWaypoint.cpp AOF_SimpleWaypoint.cpp)
#TARGET_LINK_LIBRARIES(BHV_SimpleWaypoint
#   helmivp
#   behaviors 
#   ivpbuild 
#   logic 
#   ivpcore 
#   bhvutil 
 #  mbutil 
  # geometry 
   #${SYSTEM_LIBS} )

#-------------------------------------------------------
#						BHV_Pulse
#-------------------------------------------------------
#ADD_LIBRARY(BHV_Pulse SHARED 
 #  BHV_Pulse.cpp)
#TARGET_LINK_LIBRARIES(BHV_Pulse
 #  helmivp
  # behaviors 
   #ivpbuild 
  # logic 
  # ivpcore 
   #bhvutil 
   #mbutil 
   #geometry 
   #${SYSTEM_LIBS} )
#-------------------------------------------------------
#						BHV_ZigLeg
#-------------------------------------------------------
#ADD_LIBRARY(BHV_ZigLeg SHARED 
 #  BHV_ZigLeg.cpp)
#TARGET_LINK_LIBRARIES(BHV_ZigLeg
 #  helmivp
  # behaviors 
   #ivpbuild 
   #logic 
   #ivpcore 
   #bhvutil 
   #mbutil 
   #geometry 
   #${SYSTEM_LIBS} )
   
#-------------------------------------------------------
#						BHV_FrontEstimate
#-------------------------------------------------------
#ADD_LIBRARY(BHV_FrontEstimate SHARED 
 #  BHV_FrontEstimate.cpp)
#TARGET_LINK_LIBRARIES(BHV_FrontEstimate
 #  helmivp
  # behaviors 
 #  ivpbuild 
 #  logic 
 #  ivpcore 
 #  bhvutil 
 #  mbutil
 #  ufield
 #  geometry 
 #  ${SYSTEM_LIBS} )
   
#-------------------------------------------------------
#						BHV_FrontOscillate
#-------------------------------------------------------
#ADD_LIBRARY(BHV_FrontOscillate SHARED 
 #  BHV_FrontOscillate.cpp)
#TARGET_LINK_LIBRARIES(BHV_FrontOscillate
 #  helmivp
 #  behaviors 
 #  ivpbuild 
 #  logic 
 #  ivpcore 
 #  bhvutil 
 #  mbutil 
 #  geometry 
 #  ${SYSTEM_LIBS} )
   
#-------------------------------------------------------
#						BHV_CollabEstimate
#-------------------------------------------------------
ADD_LIBRARY(BHV_CollabEstimate SHARED 
   BHV_CollabEstimate.cpp)
TARGET_LINK_LIBRARIES(BHV_CollabEstimate
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil
   ufield
   geometry 
   ${SYSTEM_LIBS} )
