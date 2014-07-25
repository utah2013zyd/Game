ORZ_PRE_BUILD_EVENT(Framework_Base)


INCLUDE_DIRECTORIES("include")


ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS -DTIXML_USE_STL)



MACRO(ORZ_ADD_SUBLIB name)
	ORZ_SET_PROJECT_FILES("orz\\\\\\\\${name}" ../orz/${LIB_NAME}/${name}/ .h)
	ORZ_SET_PROJECT_FILES("lib\\\\\\\\include\\\\\\\\${name}" include/${name}/ .h)
	ORZ_SET_PROJECT_FILES("lib\\\\\\\\src\\\\\\\\${name}" src/${name}/ .cpp)
ENDMACRO(ORZ_ADD_SUBLIB)


ORZ_ADD_SUBLIB(Logic)
ORZ_ADD_SUBLIB(System)
ORZ_ADD_SUBLIB(LogicConfiger)

ORZ_SET_PROJECT_FILES(lib\\\\\\\\include include/ .h)
ORZ_SET_PROJECT_FILES(lib\\\\\\\\src src/ .cpp)
ORZ_SET_PROJECT_FILES("orz" ../orz/${LIB_NAME}/ .h)
   

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

TARGET_LINK_LIBRARIES(${LIB_NAME} 
debug tinyxmld_STL 
optimized tinyxml_STL
Toolkit_Base
)


ORZ_POST_BUILD_EVENT()