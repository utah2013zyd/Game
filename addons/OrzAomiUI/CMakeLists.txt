CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

IF( COMMAND CMAKE_POLICY )
	CMAKE_POLICY( SET CMP0003 NEW )
ENDIF( COMMAND CMAKE_POLICY )



LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)
INCLUDE(CPack)
INCLUDE(OrzCmakeModule)





ORZ_PRE_BUILD(OrzAomiUI)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/dependencies/include )


INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include/orz DESTINATION ./include PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/media DESTINATION . PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/include/Awesomium DESTINATION ./include/orz/View_AomiUI PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/Debug DESTINATION ./bin PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/Release DESTINATION ./bin PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)




INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/plugins DESTINATION ./bin/Release PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/dependencies/plugins DESTINATION ./bin/Debug PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)

INSTALL(FILES 
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/icudt38.dll
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/avcodec-52.dll
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/avformat-52.dll
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/avutil-50.dll
 DESTINATION ./bin/Release)


INSTALL(FILES 
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/icudt38.dll
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/avcodec-52.dll
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/avformat-52.dll
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/bin/avutil-50.dll
 DESTINATION ./bin/Debug)

INSTALL(FILES 
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/lib/Debug/Awesomium_d.lib 
 DESTINATION ./lib/Debug)

INSTALL(FILES 
${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/lib/Release/Awesomium.lib 
 DESTINATION ./lib/Release)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/dependencies/${Compiler}/lib)

ADD_SUBDIRECTORY(View_AomiUI)
ADD_SUBDIRECTORY(UT_AomiUI)
ADD_SUBDIRECTORY(Demo_AomiUI)


ADD_DEPENDENCIES(UT_AomiUI View_AomiUI)
ADD_DEPENDENCIES(Demo_AomiUI View_AomiUI)


ORZ_POST_BUILD()



