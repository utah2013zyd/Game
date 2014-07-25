


ORZ_PRE_BUILD_EVENT(View_OGRE3D)

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS -D_USRDLL)
INCLUDE_DIRECTORIES("include" ${Orz_DependenciesInclude}/Ogre
${Orz_DependenciesInclude}/Ogre/CEGUI)




ORZ_SET_PROJECT_FILES(lib\\\\\\\\include include/ .h)
ORZ_SET_PROJECT_FILES(lib\\\\\\\\src src/ .cpp)
ORZ_SET_PROJECT_FILES("orz" ../orz/${LIB_NAME}/ .h)
   
ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

TARGET_LINK_LIBRARIES(${LIB_NAME} 
debug OgreMain_d 
debug CEGUIBase_d 
debug OgreGUIRenderer_d
optimized OgreMain 
optimized CEGUIBase
optimized OgreGUIRenderer
Framework_Base 
Toolkit_Base
)

ORZ_POST_BUILD_EVENT()

