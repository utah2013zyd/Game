
FIND_PATH(Orz_DependenciesBin
  debug/orzconfig.xml
  PATHS 
  ${PROJECT_SOURCE_DIR}/all_dependencies/${Compiler}/bin
  ${PROJECT_SOURCE_DIR}/../all_dependencies/${Compiler}/bin
  ${PROJECT_BINARY_DIR}/all_dependencies/${Compiler}/bin
  ${PROJECT_BINARY_DIR}/../all_dependencies/${Compiler}/bin

  ${PROJECT_SOURCE_DIR}/bin
  ${PROJECT_SOURCE_DIR}/../binCompiler
  ${PROJECT_SOURCE_DIR}/../../bin
  ${PROJECT_SOURCE_DIR}/bin/bin  
  ${PROJECT_BINARY_DIR}/bin
  ${PROJECT_BINARY_DIR}/../bin
  ${PROJECT_BINARY_DIR}/../../bin
  ${PROJECT_BINARY_DIR}/bin/bin
  ${PROJECT_SOURCE_DIR}/../bin/bin
  ${PROJECT_SOURCE_DIR}/../../bin/bin
  ${PROJECT_SOURCE_DIR}/dependencies/bin/bin
  ${PROJECT_BINARY_DIR}/bin/bin
  ${PROJECT_BINARY_DIR}/../bin/bin
  ${PROJECT_BINARY_DIR}/../../bin/bin
  ${PROJECT_BINARY_DIR}/dependencies/bin/bin

  NO_DEFAULT_PATH
)


IF(NOT Orz_DependenciesBin)
	MESSAGE(SEND_ERROR "[You have to download and Expand the dependencies.zip file into ${PROJECT_SOURCE_DIR}]")
ENDIF(NOT Orz_DependenciesBin)


