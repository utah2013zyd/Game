ORZ_FIND(Orz_AomiUI OrzAomiUI bin/debug/View_AomiUI.dll)
INCLUDE_DIRECTORIES(${Orz_AomiUI_DIR}/include ${Orz_AomiUI_DIR}/dependencies/include
 ${Orz_AomiUI_DIR}/dependencies/include/awesomium
)
LINK_DIRECTORIES(${Orz_AomiUI_DIR}/lib ${Orz_AomiUI_DIR}/dependencies/lib)
INSTALL(FILES 
${Orz_AomiUI_DIR}/bin/Debug/View_AomiUI.dll 
${Orz_AomiUI_DIR}/bin/Debug/Awesomium_d.dll
${Orz_AomiUI_DIR}/bin/Debug/icudt38.dll
${Orz_AomiUI_DIR}/bin/Debug/avcodec-52.dll
${Orz_AomiUI_DIR}/bin/Debug/avformat-52.dll
${Orz_AomiUI_DIR}/bin/Debug/avutil-50.dll
DESTINATION ./bin/Debug )





INSTALL(FILES 
${Orz_AomiUI_DIR}/bin/Release/View_AomiUI.dll 
${Orz_AomiUI_DIR}/bin/Release/Awesomium.dll
${Orz_AomiUI_DIR}/bin/Release/icudt38.dll
${Orz_AomiUI_DIR}/bin/Release/avcodec-52.dll
${Orz_AomiUI_DIR}/bin/Release/avformat-52.dll
${Orz_AomiUI_DIR}/bin/Release/avutil-50.dll
 DESTINATION ./bin/Release)

INSTALL(DIRECTORY ${Orz_AomiUI_DIR}/bin/Debug/plugins DESTINATION ./bin/debug PATTERN "CVS" EXCLUDE PATTERN ".svn" )
INSTALL(DIRECTORY ${Orz_AomiUI_DIR}/bin/Release/plugins DESTINATION ./bin/release PATTERN "CVS" EXCLUDE PATTERN ".svn" )
