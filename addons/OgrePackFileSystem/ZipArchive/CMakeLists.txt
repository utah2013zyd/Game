
ORZ_PRE_BUILD_EVENT(ZipArchive)


INCLUDE_DIRECTORIES("bzip2" "zlib")





ORZ_SET_PROJECT_FILES("bzip2"  ./bzip2/ .h)
ORZ_SET_PROJECT_FILES("bzip2"  ./bzip2/ .c)
ORZ_SET_PROJECT_FILES("zlib"  ./zlib/ .h)
ORZ_SET_PROJECT_FILES("zlib"  ./zlib/ .c)



ORZ_SET_PROJECT_FILES("Header Files"  ./ .h)
ORZ_SET_PROJECT_FILES("Source Files" ./ .cpp)

ORZ_ADD_PROJECT_FILE("Information" ./ Appnote.txt)
ORZ_ADD_PROJECT_FILE("Resource Files" ./ ZipArchive.rc )

SOURCE_GROUP("Resource Files" resource.h )
ADD_LIBRARY(${LIB_NAME} STATIC ${${LIB_NAME}_FILE})


ORZ_POST_BUILD_EVENT()