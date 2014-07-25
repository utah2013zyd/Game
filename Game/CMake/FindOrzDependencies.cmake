FIND_PATH( Orz_Dependencies_DIR 
  include/utf8/utf8.h
  PATHS 
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/..
  ${PROJECT_SOURCE_DIR}/../..
  ${PROJECT_SOURCE_DIR}/dependencies  
  ${PROJECT_BINARY_DIR}
  ${PROJECT_BINARY_DIR}/..
  ${PROJECT_BINARY_DIR}/../..
  ${PROJECT_BINARY_DIR}/dependencies
  ${PROJECT_SOURCE_DIR}/../dependencies
  ${PROJECT_SOURCE_DIR}/../../dependencies
  ${PROJECT_SOURCE_DIR}/dependencies/dependencies  
  ${PROJECT_BINARY_DIR}/dependencies
  ${PROJECT_BINARY_DIR}/../dependencies
  ${PROJECT_BINARY_DIR}/../../dependencies
  ${PROJECT_BINARY_DIR}/dependencies/dependencies
  NO_DEFAULT_PATH
)



IF(NOT Orz_Dependencies_DIR )
	MESSAGE(SEND_ERROR "[You have to download and Expand the dependencies.zip file into ${PROJECT_SOURCE_DIR}]")
ENDIF(NOT Orz_Dependencies_DIR)

