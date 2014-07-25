
ORZ_PRE_BUILD_EVENT(OrzMySQL)



INCLUDE_DIRECTORIES("include")

IF(UNIX)
	INSTALL(FILES 
		${LIBRARY_OUTPUT_PATH}/libOrzMySQL.so 
		DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/release)
ENDIF(UNIX)

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS)



ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)

IF( WIN32 )
	ORZ_SET_PROJECT_FILES(def ./ .def)
ENDIF( WIN32 )

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})


IF( UNIX )
        TARGET_LINK_LIBRARIES(${LIB_NAME}
        debug boost_system${BOOST_LIB_POSTFIX}
        debug boost_thread${BOOST_LIB_POSTFIX}
        debug boost_date_time${BOOST_LIB_POSTFIX}
        debug boost_regex${BOOST_LIB_POSTFIX}
        debug boost_serialization${BOOST_LIB_POSTFIX}
        debug Toolkit_Plus
	debug mysqlclient_r
        optimized boost_system${BOOST_LIB_POSTFIX}
        optimized boost_thread${BOOST_LIB_POSTFIX}
        optimized boost_date_time${BOOST_LIB_POSTFIX}
        optimized boost_regex${BOOST_LIB_POSTFIX}
        optimized boost_serialization${BOOST_LIB_POSTFIX}
        optimized Toolkit_Plus
	optimized mysqlclient_r
        )
ELSE()
        TARGET_LINK_LIBRARIES(${LIB_NAME}
        debug Toolkit_Plus
	debug libmysql
	debug mysqlclient
	debug mysys
	debug regex
	debug strings
	debug zlib
        optimized Toolkit_Plus
	optimized libmysql
	optimized mysqlclient
	optimized mysys
	optimized regex
	optimized strings
	optimized zlib
        )
ENDIF()

ORZ_POST_BUILD_EVENT()