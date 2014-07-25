
ORZ_PRE_BUILD_EVENT(Toolkit_Plus)


INCLUDE_DIRECTORIES("include")

IF(UNIX)
	INSTALL(FILES 
		${LIBRARY_OUTPUT_PATH}/libToolkit_Plus.so 
		DESTINATION /lib)
ENDIF(UNIX)

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS )


MACRO(ORZ_ADD_SUBLIB name)
	ORZ_SET_PROJECT_FILES("orz\\\\\\\\${name}" ../orz/${LIB_NAME}/${name}/ .h)
	ORZ_SET_PROJECT_FILES("lib\\\\\\\\include\\\\\\\\${name}" include/${name}/ .h)
	ORZ_SET_PROJECT_FILES("lib\\\\\\\\src\\\\\\\\${name}" src/${name}/ .cpp)
ENDMACRO(ORZ_ADD_SUBLIB)


ORZ_ADD_SUBLIB(Agent)
ORZ_ADD_SUBLIB(DynLib)
ORZ_ADD_SUBLIB(Iconv)
ORZ_ADD_SUBLIB(Log)
ORZ_ADD_SUBLIB(Plugin)
ORZ_ADD_SUBLIB(Database)

ORZ_SET_PROJECT_FILES("orz" ../orz/${LIB_NAME}/ .h)
   

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

IF( UNIX )
        TARGET_LINK_LIBRARIES(${LIB_NAME}
        debug boost_system${BOOST_LIB_POSTFIX}
        debug boost_thread${BOOST_LIB_POSTFIX}
        debug boost_date_time${BOOST_LIB_POSTFIX}
        debug boost_regex${BOOST_LIB_POSTFIX}
        debug boost_serialization${BOOST_LIB_POSTFIX}
	debug log4cxx
        optimized boost_system${BOOST_LIB_POSTFIX}
        optimized boost_thread${BOOST_LIB_POSTFIX}
        optimized boost_date_time${BOOST_LIB_POSTFIX}
        optimized boost_regex${BOOST_LIB_POSTFIX}
        optimized boost_serialization${BOOST_LIB_POSTFIX}
	optimized log4cxx
        )
ELSE() 
        TARGET_LINK_LIBRARIES(${LIB_NAME}
        debug log4cxx
	debug libiconv
        optimized log4cxx
	optimized libiconv
        )
ENDIF()


ORZ_POST_BUILD_EVENT()
