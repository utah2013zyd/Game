
ORZ_PRE_BUILD_EVENT(FighterBase)



INCLUDE_DIRECTORIES("include")

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS -DPROJECT_NAME=\"${LIB_NAME}\")
INCLUDE_DIRECTORIES("include" ${Orz_SDK_DIR}/dependencies/include/Ogre
${Orz_SDK_DIR}/dependencies/include/Ogre/CEGUI ../FCBase/include ../FighterBase/include)



ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

TARGET_LINK_LIBRARIES(${LIB_NAME} 
Framework_Base
Toolkit_Base
View_OGRE3D
debug OgreMain_d 
debug CEGUIBase_d 
optimized OgreMain 
optimized CEGUIBase
FCBase
)

ORZ_POST_BUILD_EVENT()