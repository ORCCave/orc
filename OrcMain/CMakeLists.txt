file(GLOB_RECURSE ORC_SOURCE_FILES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE ORC_HEADER_FILES CONFIGURE_DEPENDS *.h)

add_library(OrcMain STATIC ${ORC_SOURCE_FILES} ${ORC_HEADER_FILES})

target_include_directories(OrcMain PUBLIC "include")
target_link_libraries(OrcMain PRIVATE SDL3::SDL3)

if(WIN32)
	target_compile_definitions(OrcMain PUBLIC ORC_PLATFORM=ORC_PLATFORM_WIN32)
endif()