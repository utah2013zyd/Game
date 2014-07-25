
ORZ_PRE_BUILD_EVENT(OrzAsioDemo)

ADD_DEFINITIONS( -D_CONSOLE)


INCLUDE_DIRECTORIES("include")

INSTALL(FILES 
	${PROJECT_SOURCE_DIR}/Config/cacert.pem 
	${PROJECT_SOURCE_DIR}/Config/dh512.pem 
	${PROJECT_SOURCE_DIR}/Config/mycert.pem 
	${PROJECT_SOURCE_DIR}/Config/mykey.pem 
	${PROJECT_SOURCE_DIR}/Config/log4cxx.properties  
	DESTINATION 
	./bin/debug )
INSTALL(FILES 
	${PROJECT_SOURCE_DIR}/Config/cacert.pem 
	${PROJECT_SOURCE_DIR}/Config/dh512.pem 
	${PROJECT_SOURCE_DIR}/Config/mycert.pem 
	${PROJECT_SOURCE_DIR}/Config/mykey.pem 
	${PROJECT_SOURCE_DIR}/Config/log4cxx.properties  
	DESTINATION 
	./bin/release )


ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)
ADD_EXECUTABLE(${EXE_NAME} ${${EXE_NAME}_FILE})


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
	debug OrzAsio
        optimized boost_system${BOOST_LIB_POSTFIX}
        optimized boost_thread${BOOST_LIB_POSTFIX}
        optimized boost_date_time${BOOST_LIB_POSTFIX}
        optimized boost_regex${BOOST_LIB_POSTFIX}
        optimized boost_serialization${BOOST_LIB_POSTFIX}
        optimized dl
        optimized ssl
        optimized crypto
        optimized Toolkit_Plus
	optimized OrzAsio
        )
ELSE()
        TARGET_LINK_LIBRARIES(${LIB_NAME}
        debug libeay32
        debug ssleay32
        debug Toolkit_Plus
	debug OrzAsio
        optimized libeay32
        optimized ssleay32
        optimized Toolkit_Plus
	optimized OrzAsio
        )
ENDIF()

ORZ_POST_BUILD_EVENT()
