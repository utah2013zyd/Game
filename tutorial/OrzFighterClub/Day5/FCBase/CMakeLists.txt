
ORZ_PRE_BUILD_EVENT(FCBase)



INCLUDE_DIRECTORIES("include")

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS )

ORZ_SET_PROJECT_FILES(include include/ .h)
ORZ_SET_PROJECT_FILES(src src/ .cpp)

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

TARGET_LINK_LIBRARIES(${LIB_NAME} 
Framework_Base
Toolkit_Base
)

ORZ_POST_BUILD_EVENT()