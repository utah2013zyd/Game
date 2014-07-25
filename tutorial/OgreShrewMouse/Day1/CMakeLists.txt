ORZ_PRE_BUILD_EVENT(Day1)


INCLUDE_DIRECTORIES("include")




ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)


ADD_EXECUTABLE(${EXE_NAME} WIN32 ${${EXE_NAME}_FILE})



TARGET_LINK_LIBRARIES(
${EXE_NAME} 
debug OgreMain_d
optimized OgreMain
debug OIS_d
optimized OIS
)

ORZ_POST_BUILD_EVENT()