CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

IF( COMMAND CMAKE_POLICY )
	CMAKE_POLICY( SET CMP0003 NEW )
ENDIF( COMMAND CMAKE_POLICY )


LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)
INCLUDE(CPack)
INCLUDE(OrzCmakeModule)



INCLUDE_DIRECTORIES("FCBase/include")

ORZ_PRE_BUILD(ShootingGame)



ADD_SUBDIRECTORY(FCBase)
ADD_SUBDIRECTORY(FCModel)
ADD_SUBDIRECTORY(FCController)



ADD_DEPENDENCIES(FCModel FCBase)
ADD_DEPENDENCIES(FCController FCModel FCBase)

ORZ_POST_BUILD()

