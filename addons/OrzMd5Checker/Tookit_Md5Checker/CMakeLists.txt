
ORZ_PRE_BUILD_EVENT(Tookit_Md5Checker)


INCLUDE_DIRECTORIES("include" md5)






ORZ_SET_PROJECT_FILES("lib\\\\\\\\include"  include/ .h)
ORZ_SET_PROJECT_FILES("lib\\\\\\\\src" src/ .cpp)
ORZ_SET_PROJECT_FILES("md5" md5/ .c)
ORZ_SET_PROJECT_FILES("md5" md5/ .h)
ORZ_SET_PROJECT_FILES("orz" ${PROJECT_SOURCE_DIR}/orz/${LIB_NAME}/ .h)
   
ADD_LIBRARY(${LIB_NAME} STATIC ${${LIB_NAME}_FILE})


TARGET_LINK_LIBRARIES(${LIB_NAME} 
Framework_Base
)


ORZ_POST_BUILD_EVENT()