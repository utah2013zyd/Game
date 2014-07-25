
ORZ_PRE_BUILD_EVENT(Plugin_PackFileSystem)

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS)

INCLUDE_DIRECTORIES("include" ${Orz_SDK_DIR}/dependencies/include/Ogre)





ORZ_SET_PROJECT_FILES("def"  ./misc/ .def)
ORZ_SET_PROJECT_FILES("include"  ./include/ .h)
ORZ_SET_PROJECT_FILES("src" ./src/ .cpp)


ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

TARGET_LINK_LIBRARIES(
${LIB_NAME} 
Framework_Base
debug OgreMain_d
optimized OgreMain
ZipArchive
)


ORZ_POST_BUILD_EVENT()