
ORZ_PRE_BUILD_EVENT(OgreNewtUT)

ADD_DEFINITIONS( -D_CONSOLE)

INCLUDE_DIRECTORIES("include" ${PROJECT_SOURCE_DIR}/dependencies/include ${PROJECT_SOURCE_DIR}/dependencies/include/Newton ${PROJECT_SOURCE_DIR}/dependencies/include/OgreNewt)




ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)
ADD_EXECUTABLE(${EXE_NAME} ${${EXE_NAME}_FILE})


TARGET_LINK_LIBRARIES(${EXE_NAME} 
View_OGRE3D
Framework_Base
Toolkit_Base
View_OIS
newton
debug OgreMain_d
optimized OgreMain
OgreNewt
View_OgreNewt
)

ORZ_POST_BUILD_EVENT()