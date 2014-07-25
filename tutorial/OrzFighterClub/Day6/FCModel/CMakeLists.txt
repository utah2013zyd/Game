
ORZ_PRE_BUILD_EVENT(FCModel)



INCLUDE_DIRECTORIES("include")

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS -DTIXML_USE_STL)
INCLUDE_DIRECTORIES("include" ${Orz_SDK_DIR}/dependencies/include/Ogre
${Orz_SDK_DIR}/dependencies/include/Ogre/CEGUI ../FCBase/include ../FighterBase/include)



ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)
ORZ_SET_PROJECT_FILES(def ./ .def)

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

TARGET_LINK_LIBRARIES(${LIB_NAME} 
Framework_Base
Toolkit_Base
View_OGRE3D
debug OgreMain_d 
debug tinyxmld_STL 
debug CEGUIBase_d 
optimized OgreMain 
optimized CEGUIBase
optimized tinyxml_STL
FCBase
FighterBase
)

ORZ_POST_BUILD_EVENT()