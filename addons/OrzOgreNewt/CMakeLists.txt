CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

IF( COMMAND CMAKE_POLICY )
	CMAKE_POLICY( SET CMP0003 NEW )
ENDIF( COMMAND CMAKE_POLICY )



LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../CMake)
INCLUDE(CPack)
INCLUDE(OrzCMakeModule)






ORZ_PRE_BUILD(OrzOgreNewt)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} 
${PROJECT_SOURCE_DIR}/OgreNewt/include 
${PROJECT_SOURCE_DIR}/View_OgreNewt/include
${Orz_SDK_DIR}/dependencies/include/Ogre
)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/orz DESTINATION ./include PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin DESTINATION ./ PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/bin DESTINATION ./ PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/media DESTINATION ./ PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/lib DESTINATION ./dependencies PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/include DESTINATION ./dependencies PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/OgreNewt/include/ DESTINATION ./include/OgreNewt PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/lib ${PROJECT_SOURCE_DIR}/dependencies/lib)


ADD_SUBDIRECTORY(OgreNewt)
ADD_SUBDIRECTORY(View_OgreNewt)
ADD_SUBDIRECTORY(OgreNewtUT)

ADD_DEPENDENCIES(View_OgreNewt OgreNewt)
ADD_DEPENDENCIES(OgreNewtUT View_OgreNewt OgreNewt)



ORZ_POST_BUILD()




