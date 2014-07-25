
PROJECT(OrzAsioExample)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

IF( COMMAND CMAKE_POLICY )
   CMAKE_POLICY( SET CMP0003 NEW )
ENDIF( COMMAND CMAKE_POLICY )


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)

INCLUDE(CPack)
INCLUDE(OrzCmakeModule)
FIND_PACKAGE(OrzDependencies)
FIND_PACKAGE(OrzBin)
FIND_PACKAGE(OrzAsio)



IF( UNIX )
	LINK_DIRECTORIES(${Orz_Dependencies_DIR}/lib ${Orz_Asio_DIR}/lib /lib /usr/lib /usr/local/lib)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} ${Orz_Dependencies_DIR}/include ${Orz_Asio_DIR}/include /usr/include /usr/local/include)
ELSE()
	LINK_DIRECTORIES(${Orz_Dependencies_DIR}/lib ${Orz_Asio_DIR}/lib)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} ${Orz_Dependencies_DIR}/include ${Orz_Asio_DIR}/include)
ENDIF()


INSTALL(DIRECTORY ${Orz_Asio_DIR}/bin DESTINATION ./ PATTERN "CVS" EXCLUDE
  PATTERN ".svn" EXCLUDE)

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


IF( UNIX )
	SET(BOOST_LIB_POSTFIX CACHE STRING "Set boost lib postfix. for example: boost_system${BOOST_LIB_POSTFIX} == boost_system-gcc41-mt-1_39 .")
ENDIF(UNIX)


ADD_SUBDIRECTORY(EchoServer)
ADD_SUBDIRECTORY(EchoClient)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
    "${PROJECT_SOURCE_DIR}/../build/OrzAsioExample" CACHE PATH "OrzAsioExample install prefix" FORCE
    )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)