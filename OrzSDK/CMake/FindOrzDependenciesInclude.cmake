FIND_PATH( Orz_DependenciesInclude
  utf8/utf8.h
  PATHS 
  ${PROJECT_SOURCE_DIR}/all_dependencies/include
  ${PROJECT_BINARY_DIR}/all_dependencies/include
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/../include
  ${PROJECT_SOURCE_DIR}/../../include
  ${PROJECT_SOURCE_DIR}/dependencies/include  
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_BINARY_DIR}/../include
  ${PROJECT_BINARY_DIR}/../../include
  ${PROJECT_BINARY_DIR}/dependencies/include
  ${PROJECT_SOURCE_DIR}/../dependencies/include
  ${PROJECT_SOURCE_DIR}/../../dependencies/include
  ${PROJECT_SOURCE_DIR}/dependencies/dependencies/include  
  ${PROJECT_BINARY_DIR}/dependencies/include
  ${PROJECT_BINARY_DIR}/../dependencies/include
  ${PROJECT_BINARY_DIR}/../../dependencies/include
  ${PROJECT_BINARY_DIR}/dependencies/dependencies/include
  NO_DEFAULT_PATH
)



IF(NOT Orz_DependenciesInclude )
	MESSAGE(SEND_ERROR "[You have to download and Expand the dependencies.zip file into ${PROJECT_SOURCE_DIR}]")
ENDIF(NOT Orz_DependenciesInclude)




