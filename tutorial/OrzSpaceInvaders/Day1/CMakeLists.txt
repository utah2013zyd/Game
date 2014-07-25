
ORZ_PRE_BUILD_EVENT(Day1)

ADD_DEFINITIONS( -D_CONSOLE)


INCLUDE_DIRECTORIES("include")




ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)
ADD_EXECUTABLE(${EXE_NAME} ${${EXE_NAME}_FILE})

TARGET_LINK_LIBRARIES(${EXE_NAME} 
Framework_Base
Toolkit_Base
View_OIS
View_Fmod
View_OGRE3D
debug OgreMain_d 
optimized OgreMain 
)

ORZ_POST_BUILD_EVENT()