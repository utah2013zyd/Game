ORZ_PRE_BUILD_EVENT(Model)

ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS -D_USRDLL)

INCLUDE_DIRECTORIES("include")




ORZ_SET_PROJECT("FILE" include include/ .h)
ORZ_SET_PROJECT("FILE" src src/ .cpp)
ORZ_SET_PROJECT("FILE" def ./ .def)



ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})



TARGET_LINK_LIBRARIES(
${LIB_NAME} 
Toolkit_Base
Framework_Base
)

ORZ_POST_BUILD_EVENT()