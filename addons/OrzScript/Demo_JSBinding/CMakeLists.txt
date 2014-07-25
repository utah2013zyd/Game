ORZ_PRE_BUILD_EVENT(Demo_JSBinding)

ADD_DEFINITIONS( -D_CONSOLE)

INCLUDE_DIRECTORIES("include" ${PROJECT_SOURCE_DIR})




ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)


ADD_EXECUTABLE(${EXE_NAME} ${${EXE_NAME}_FILE})



TARGET_LINK_LIBRARIES(
${EXE_NAME} 
Framework_Base
View_OGRE3D
Toolkit_Base
View_OIS
View_AomiUI
debug OgreMain_d
optimized OgreMain
debug Awesomium_d
optimized Awesomium
Toolkit_JSBinding
Toolkit_Script
)

ORZ_POST_BUILD_EVENT()