
ORZ_PRE_BUILD_EVENT(OgreNewt)
SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME} )
SET( ORZ_${LIB_NAME}_DIR ${PROJECT_SOURCE_DIR}/orz/${LIB_NAME} )

ADD_DEFINITIONS( -DWEBCAM_PLUGIN_EXPORTS -D_USRDLL -DTIXML_USE_STL -D_OGRENEWT_EXPORTS -D_OGRENEWT_DYNAMIC)

INCLUDE_DIRECTORIES("include" 
${PROJECT_SOURCE_DIR}/dependencies/include 
${PROJECT_SOURCE_DIR}/dependencies/include/Newton ${PROJECT_SOURCE_DIR}/dependencies/include/OgreNewt
${Orz_SDK_DIR}/dependencies/include/Ogre
)




ORZ_SET_PROJECT_FILES("include" include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)
   
ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})
TARGET_LINK_LIBRARIES(${LIB_NAME} 
newton
debug OgreMain_d
optimized OgreMain
debug JointLibrary_d
optimized JointLibrary
debug dMath_d
optimized dMath_d
)

ORZ_POST_BUILD_EVENT()