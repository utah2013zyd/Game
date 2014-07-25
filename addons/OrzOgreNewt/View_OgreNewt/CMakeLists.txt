
ORZ_PRE_BUILD_EVENT(View_OgreNewt)
SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME} )
SET( ORZ_${LIB_NAME}_DIR ${PROJECT_SOURCE_DIR}/orz/${LIB_NAME} )

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS -D_USRDLL)

INCLUDE_DIRECTORIES("include" 
${PROJECT_SOURCE_DIR}/dependencies/include ${PROJECT_SOURCE_DIR}/dependencies/include/Newton ${PROJECT_SOURCE_DIR}/dependencies/include/OgreNewt)






ORZ_SET_PROJECT_FILES("lib\\\\\\\\include"  include/ .h)
ORZ_SET_PROJECT_FILES("lib\\\\\\\\src" src/ .cpp)
ORZ_SET_PROJECT_FILES("orz" ${PROJECT_SOURCE_DIR}/orz/${LIB_NAME}/ .h)
   
ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})
TARGET_LINK_LIBRARIES(${LIB_NAME} 
View_OGRE3D
Framework_Base
newton
debug OgreMain_d
optimized OgreMain
OgreNewt
)


ORZ_POST_BUILD_EVENT()