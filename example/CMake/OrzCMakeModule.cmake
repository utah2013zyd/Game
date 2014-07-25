# settings for Microsoft Visual C++
IF(CMAKE_CXX_COMPILER MATCHES cl)
     IF(CMAKE_GENERATOR STREQUAL "Visual Studio 7" OR CMAKE_GENERATOR STREQUAL "Visual Studio 7 .NET 2003")
		SET(Compiler "vc71")
     ENDIF(CMAKE_GENERATOR STREQUAL "Visual Studio 7" OR CMAKE_GENERATOR STREQUAL "Visual Studio 7 .NET 2003")
     IF(CMAKE_GENERATOR STREQUAL "Visual Studio 8 2005")
        	SET(Compiler "vc80")
     ENDIF(CMAKE_GENERATOR STREQUAL "Visual Studio 8 2005")
     IF(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")
        	SET(Compiler "vc90")
     ENDIF(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")
    IF(NOT Compiler)
        SET(Compiler "unknown")
    ENDIF(NOT Compiler)
ENDIF(CMAKE_CXX_COMPILER MATCHES cl)

MACRO(ORZ_ADD_PROJECT type group dir)

	FOREACH(var ${ARGN})
		LIST(APPEND ${LIB_NAME}_${type} ${dir}${var})
		SOURCE_GROUP(${group} FILES ${dir}${var})
	ENDFOREACH( var )

ENDMACRO(ORZ_ADD_PROJECT)



MACRO(ORZ_SET_PROJECT type group dir match)

	FILE(GLOB var ${dir}*${match})
	
	FOREACH(f ${var})
		GET_FILENAME_COMPONENT(f ${f} NAME_WE)
		ORZ_ADD_PROJECT(${type} ${group} ${dir} "${f}${match}")
	ENDFOREACH()

ENDMACRO(ORZ_SET_PROJECT)



MACRO(ORZ_ADD_PROJECT_FILE group dir)
	FOREACH(var ${ARGN})
		LIST(APPEND ${LIB_NAME}_FILE ${dir}${var})
		SOURCE_GROUP(${group} FILES ${dir}${var})
	ENDFOREACH( var )
ENDMACRO(ORZ_ADD_PROJECT_FILE)



MACRO(ORZ_SET_PROJECT_FILES group dir match)

	FILE(GLOB orz_files  ${dir}*${match})
	
	FOREACH(f ${orz_files})
		GET_FILENAME_COMPONENT(f ${f} NAME_WE)
		ORZ_ADD_PROJECT_FILE(${group} ${dir} "${f}${match}")

	ENDFOREACH()

ENDMACRO(ORZ_SET_PROJECT_FILES)






MACRO( ORZ_PRE_BUILD_EVENT target )
	SET( LIB_NAME ${target} )
	SET( EXE_NAME ${target} )
	STRING(TOUPPER ${LIB_NAME} LIB_NAME_TOUPPER)
ENDMACRO( ORZ_PRE_BUILD_EVENT )


MACRO( ORZ_POST_BUILD_EVENT )
	INSTALL(TARGETS ${LIB_NAME} 
		RUNTIME DESTINATION bin/debug
		CONFIGURATIONS Debug
		LIBRARY DESTINATION lib/debug
		CONFIGURATIONS Debug
		ARCHIVE DESTINATION lib/debug
		CONFIGURATIONS Debug
	)

	INSTALL(TARGETS ${LIB_NAME} 
		RUNTIME DESTINATION bin/release
		CONFIGURATIONS Release
		LIBRARY DESTINATION lib/release 
		CONFIGURATIONS Release
		ARCHIVE DESTINATION lib/release
		CONFIGURATIONS Release
	)


ENDMACRO(  ORZ_POST_BUILD_EVENT)





MACRO(ORZ_FIND name dir file)
	SET(${name}_DIR "" CACHE PATH "the directory of the ${name}")


	IF(${name}_DIR)
 		FIND_PATH( 
		_temp_ 	
		${file}
		PATHS 
		${${name}_DIR}
		NO_DEFAULT_PATH
		)
	
		IF(NOT _temp_)
			MESSAGE("\"${${name}_DIR}\" is not the ${name}'s directory")
			SET(${name}_DIR  ${name}_DIR-NOTFOUND CACHE PATH "the directory of the ${name}" FORCE)
			UNSET(${name} CACHE)
		ENDIF(NOT _temp_)
	
		UNSET(_temp_ CACHE)
	ENDIF(${name}_DIR)

	IF(NOT ${name}_DIR)
		UNSET(${name} CACHE)
		FIND_PATH(
		_temp_	
		${file}
		PATHS 
		${PROJECT_SOURCE_DIR}/build/${Compiler}/${dir}
		${PROJECT_SOURCE_DIR}/../build/${Compiler}/${dir}
		${PROJECT_SOURCE_DIR}/../../build/${Compiler}/${dir}
		${PROJECT_SOURCE_DIR}/../OrzSDK/build/${Compiler}/${dir}
		${PROJECT_SOURCE_DIR}/../../OrzSDK/build/${Compiler}/${dir}
		${PROJECT_SOURCE_DIR}/../../../OrzSDK/build/${Compiler}/${dir}
		NO_DEFAULT_PATH
		)
		IF(_temp_)
			SET(${name}_DIR  ${_temp_} CACHE PATH "the directory of the ${name}" FORCE)
		ELSE(_temp_)
			SET(${name}_DIR  ${name}_DIR-NOTFOUND CACHE PATH "the directory of the ${name}" FORCE)
		ENDIF(_temp_)
			
		UNSET(_temp_ CACHE)

		
		MESSAGE(${name}_DIR)	
	ENDIF(NOT ${name}_DIR)

	IF(NOT ${name}_DIR)
		MESSAGE("[You have to select the directory of the ${name}!] ")
	ENDIF(NOT ${name}_DIR)	
ENDMACRO(ORZ_FIND)



MACRO( ORZ_PRE_BUILD project )


	PROJECT(${project})
	


	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


	
	FIND_PACKAGE(OrzSDK)


	LINK_DIRECTORIES(
		${Orz_SDK_DIR}/lib 
		${Orz_SDK_DIR}/dependencies/lib
		)


	INCLUDE_DIRECTORIES(${Orz_SDK_DIR}/dependencies/include ${Orz_SDK_DIR}/include)


	INSTALL(DIRECTORY ${Orz_SDK_DIR}/bin DESTINATION ./ PATTERN "CVS" EXCLUDE  PATTERN ".svn" EXCLUDE PATTERN "*.exe" EXCLUDE)
	INSTALL(DIRECTORY ${Orz_SDK_DIR}/media DESTINATION ./ PATTERN "CVS" EXCLUDE PATTERN ".svn" EXCLUDE)
ENDMACRO(ORZ_PRE_BUILD)



MACRO( ORZ_POST_BUILD)
	IF(CMAKE_INSTALL_PREFIX STREQUAL "../build/${Compiler}/${PROJECT_NAME}")
		IF(Orz_SDK_DIR)
				SET(CMAKE_INSTALL_PREFIX "${Orz_SDK_DIR}/../${PROJECT_NAME}/" CACHE PATH "${PROJECT_NAME} install prefix" FORCE )
		ENDIF(Orz_SDK_DIR)
	ENDIF(CMAKE_INSTALL_PREFIX STREQUAL "../build/${Compiler}/${PROJECT_NAME}")

	IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		IF(NOT Orz_SDK_DIR)
			SET(CMAKE_INSTALL_PREFIX "../build/${Compiler}/${PROJECT_NAME}/" CACHE PATH "${PROJECT_NAME} install prefix" FORCE )
		ELSE(NOT Orz_SDK_DIR)
			SET(CMAKE_INSTALL_PREFIX "${Orz_SDK_DIR}/../${PROJECT_NAME}/" CACHE PATH "${PROJECT_NAME} install prefix" FORCE )
		ENDIF(NOT Orz_SDK_DIR)
	ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

ENDMACRO(  ORZ_POST_BUILD)