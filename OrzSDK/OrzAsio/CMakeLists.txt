ORZ_PRE_BUILD_EVENT(OrzAsio)


INCLUDE_DIRECTORIES("include")

IF(UNIX)
	INSTALL(FILES 
		${LIBRARY_OUTPUT_PATH}/libOrzAsio.so 
		DESTINATION /lib)
ENDIF(UNIX)

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS )


MACRO(ORZ_ADD_SUBLIB name)
	ORZ_SET_PROJECT_FILES("orz\\\\\\\\${name}" ../orz/${LIB_NAME}/${name}/ .h)
	ORZ_SET_PROJECT_FILES("lib\\\\\\\\include\\\\\\\\${name}" include/${name}/ .h)
	ORZ_SET_PROJECT_FILES("lib\\\\\\\\src\\\\\\\\${name}" src/${name}/ .cpp)
ENDMACRO(ORZ_ADD_SUBLIB)


ORZ_ADD_SUBLIB(NetEngine)
ORZ_SET_PROJECT_FILES(lib\\\\\\\\include include/ .h)
ORZ_SET_PROJECT_FILES(lib\\\\\\\\src src/ .cpp)
ORZ_SET_PROJECT_FILES("orz" ../orz/${LIB_NAME}/ .h)
   

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

IF( UNIX )
	TARGET_LINK_LIBRARIES(${LIB_NAME}
	debug boost_system${BOOST_LIB_POSTFIX}
        debug boost_thread${BOOST_LIB_POSTFIX}
        debug boost_date_time${BOOST_LIB_POSTFIX}
        debug boost_regex${BOOST_LIB_POSTFIX}
        debug boost_serialization${BOOST_LIB_POSTFIX}
	debug dl 
	debug ssl
	debug crypto
	debug Toolkit_Plus
	optimized boost_system${BOOST_LIB_POSTFIX}
        optimized boost_thread${BOOST_LIB_POSTFIX}
        optimized boost_date_time${BOOST_LIB_POSTFIX}
        optimized boost_regex${BOOST_LIB_POSTFIX}
        optimized boost_serialization${BOOST_LIB_POSTFIX}
	optimized dl
	optimized ssl
	optimized crypto
	optimized Toolkit_Plus
	)
ELSE()
	TARGET_LINK_LIBRARIES(${LIB_NAME} 
	debug libeay32 
	debug ssleay32
	debug Toolkit_Plus
	optimized libeay32 
	optimized ssleay32
	optimized Toolkit_Plus
	)	
ENDIF()

ADD_SUBDIRECTORY(OrzAsioDemo)

ADD_DEPENDENCIES(OrzAsioDemo Toolkit_Plus OrzAsio)

ORZ_POST_BUILD_EVENT()
