SET(Orz_BUILD_DIR  "" CACHE PATH "the directory of the Orz build")
    
IF(NOT Orz_BUILD_DIR )
	MESSAGE(STATUS "[You can select the directory of the Orz build!] ")
ELSE(NOT Orz_BUILD_DIR)
  FIND_PATH( 
	_temp_ 	
	OrzSDK/bin/debug/Toolkit_Base.dll
	OrzSDK/bin/debug/Toolkit_Base.so
	PATHS 
	${Orz_BUILD_DIR}
	NO_DEFAULT_PATH
	)
	IF(NOT _temp_)
		MESSAGE(STATUS "\"${Orz_BUILD_DIR}\" is not the Orz build's directory")
	ENDIF(NOT _temp_)
	UNSET(_temp_ CACHE)
ENDIF(NOT Orz_BUILD_DIR)

