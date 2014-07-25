
FIND_PATH(Orz_DependenciesLibrary
  fmodex_vc.lib
  PATHS   
  ${PROJECT_SOURCE_DIR}/all_dependencies/${Compiler}/lib
  ${PROJECT_SOURCE_DIR}/../all_dependencies/${Compiler}/lib
  ${PROJECT_BINARY_DIR}/all_dependencies/${Compiler}/lib
  ${PROJECT_BINARY_DIR}/../all_dependencies/${Compiler}/lib

  ${PROJECT_SOURCE_DIR}/dependencies/lib
  ${PROJECT_SOURCE_DIR}/../dependencies/lib
  ${PROJECT_SOURCE_DIR}/../../dependencies/lib 
  ${PROJECT_SOURCE_DIR}/dependencies/lib  
  ${PROJECT_BINARY_DIR}/dependencies/lib
  ${PROJECT_BINARY_DIR}/../dependencies/lib
  ${PROJECT_BINARY_DIR}/../../dependencies/lib
  ${PROJECT_BINARY_DIR}/dependencies/dependencies/lib
  ${PROJECT_SOURCE_DIR}/../dependencies/dependencies/lib
  ${PROJECT_SOURCE_DIR}/../../dependencies/dependencies/lib
  ${PROJECT_SOURCE_DIR}/dependencies/dependencies/dependencies/lib 
  ${PROJECT_BINARY_DIR}/dependencies/dependencies/lib
  ${PROJECT_BINARY_DIR}/../dependencies/dependencies/lib
  ${PROJECT_BINARY_DIR}/../../dependencies/dependencies/lib
  ${PROJECT_BINARY_DIR}/dependencies/dependencies/dependencies/lib
  NO_DEFAULT_PATH
)




IF(NOT Orz_DependenciesLibrary)
	MESSAGE(SEND_ERROR "[You have to download and Expand the dependencies.zip file into ${PROJECT_SOURCE_DIR}]")
ENDIF(NOT Orz_DependenciesLibrary)

