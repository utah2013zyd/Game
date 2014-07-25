
ORZ_PRE_BUILD_EVENT(View_AomiUI)
SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME} )
SET( ORZ_${LIB_NAME}_DIR ${PROJECT_SOURCE_DIR}/orz/${LIB_NAME} )

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS -D_USRDLL)
INCLUDE_DIRECTORIES("include" ${Orz_SDK_DIR}/dependencies/include/Ogre)




ORZ_SET_PROJECT_FILES(lib\\\\\\\\include include/ .h)
ORZ_SET_PROJECT_FILES(lib\\\\\\\\src src/ .cpp)
ORZ_SET_PROJECT_FILES("orz" ../include/orz/${LIB_NAME}/ .h)
   
ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

TARGET_LINK_LIBRARIES(${LIB_NAME} 
Toolkit_Base
Framework_Base
View_OIS
debug Awesomium_d
debug OgreMain_d
optimized Awesomium
optimized OgreMain
)

ORZ_POST_BUILD_EVENT()