FIND_PATH(Orz_Bin_DIR
  debug/orzconfig.xml
  PATHS 
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/..
  ${PROJECT_SOURCE_DIR}/../..
  ${PROJECT_SOURCE_DIR}/bin  
  ${PROJECT_BINARY_DIR}
  ${PROJECT_BINARY_DIR}/..
  ${PROJECT_BINARY_DIR}/../..
  ${PROJECT_BINARY_DIR}/bin
  ${PROJECT_SOURCE_DIR}/../bin
  ${PROJECT_SOURCE_DIR}/../../bin
  ${PROJECT_SOURCE_DIR}/dependencies/bin
  ${PROJECT_BINARY_DIR}/bin
  ${PROJECT_BINARY_DIR}/../bin
  ${PROJECT_BINARY_DIR}/../../bin
  ${PROJECT_BINARY_DIR}/dependencies/bin
  NO_DEFAULT_PATH
)




IF(NOT Orz_Bin_DIR)
	MESSAGE(SEND_ERROR "[You have to download and Expand the dependencies.zip file into ${PROJECT_SOURCE_DIR}]")
ENDIF(NOT Orz_Bin_DIR)




