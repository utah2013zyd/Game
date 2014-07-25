
ORZ_PRE_BUILD_EVENT(Toolkit_Base)


INCLUDE_DIRECTORIES("include")


ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS)







MACRO(ORZ_ADD_SUBLIB name)
	ORZ_SET_PROJECT("FILE" "orz\\\\\\\\${name}" ../orz/${LIB_NAME}/${name}/ .h)
	ORZ_SET_PROJECT("FILE" "lib\\\\\\\\include\\\\\\\\${name}" include/${name}/ .h)
	ORZ_SET_PROJECT("FILE" "lib\\\\\\\\src\\\\\\\\${name}" src/${name}/ .cpp)
ENDMACRO(ORZ_ADD_SUBLIB)




ORZ_ADD_SUBLIB(IDManager)
ORZ_ADD_SUBLIB(DynLibManager)
ORZ_ADD_SUBLIB(EventSystem)
ORZ_ADD_SUBLIB(FSMLogic)
ORZ_ADD_SUBLIB(LogManager)
ORZ_ADD_SUBLIB(TimerManager)

ORZ_SET_PROJECT_FILES("orz" ../orz/${LIB_NAME}/ .h)
   

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

ORZ_POST_BUILD_EVENT()