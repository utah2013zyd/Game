CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

IF( COMMAND CMAKE_POLICY )
	CMAKE_POLICY( SET CMP0003 NEW )
ENDIF( COMMAND CMAKE_POLICY )



LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../CMake)
INCLUDE(CPack)
INCLUDE(OrzCmakeModule)
INCLUDE(OrzAomiUI)




ORZ_PRE_BUILD(OrzScript)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include )
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION . PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)


ADD_SUBDIRECTORY(Toolkit_JSBinding)
ADD_SUBDIRECTORY(UnitTest)
ADD_SUBDIRECTORY(Demo_JSBinding)
ADD_SUBDIRECTORY(Toolkit_Script)


ADD_DEPENDENCIES(Toolkit_JSBinding Toolkit_Script)
ADD_DEPENDENCIES(UnitTest Toolkit_JSBinding)
ADD_DEPENDENCIES(Demo_JSBinding Toolkit_JSBinding)


ORZ_POST_BUILD()



