
ORZ_PRE_BUILD_EVENT(Demo_AomiUI)

ADD_DEFINITIONS( -D_CONSOLE)


INCLUDE_DIRECTORIES("include")




ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)
ADD_EXECUTABLE(${EXE_NAME} ${${EXE_NAME}_FILE})


TARGET_LINK_LIBRARIES(${EXE_NAME} 
Toolkit_Base
Framework_Base
View_OIS
View_OGRE3D
View_AomiUI
debug Awesomium_d
debug OgreMain_d
optimized Awesomium
optimized OgreMain
)

ORZ_POST_BUILD_EVENT()