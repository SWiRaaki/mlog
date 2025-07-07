if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed. Please create a separate build directory.")
endif()

cmake_minimum_required( VERSION 3.10 )

project( mlog C )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release )
endif()
set( CMAKE_C_FLAGS "" )
set( CMAKE_C_FLAGS_DEBUG "-DDEBUG" )
set( CMAKE_C_FLAGS_RELEASE "-DRELEASE" )
# set( CMAKE_VERBOSE_MAKEFILE ON )

set( SRCFILES
	"src/mlog.c"
)
add_library(
	mlog_static STATIC
	${SRCFILES}
)
add_executable(
	mlog_test
	"main.c"
)
if( WIN32 )
	# message( STATUS "Build for Windows starts.." )
	add_definitions( -DPLATFORM_WINDOWS )
	set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} " /clr:nostdlib /GS- /GR- /EHsc /Wall /WX /permissive /wd4820 /wd4668 /wd5039 /wd4514 /wd5045" )
	set( OS_LIBS "" )
elseif( UNIX )
	# message( STATUS "Build for Linux starts.." )
	add_definitions( -DPLATFORM_LINUX )
	set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} " -nodefaultlibs -fno-exceptions -pedantic -Wall -Wextra -Wconversion -Wreturn-type -Werror=all -Werror=extra -Werror=conversion -Werror=return-type -Wno-unused-function" )
	set( CMAKE_C_FLAGS_DEBUG "-g2" )
	set( CMAKE_C_FLAGS_RELEASE "-O3" )
	set( OS_LIBS c m )
else()
	set( OS_LIBS "" )
endif()
target_compile_definitions(
	mlog_static
	PRIVATE MMEM_BUILD_STATIC
)
target_compile_definitions(
	mlog_test
	PRIVATE MMEM_BUILD_UTEST
)
target_link_libraries(
	mlog_test
	mlog_static
	${OS_LIBS}
)
target_include_directories(
	mlog_static
	PRIVATE inc/
	PRIVATE tpl/
)
target_include_directories(
	mlog_test
	PRIVATE inc/
	PRIVATE tpl/
)
set_property(
	TARGET mlog_static
	PROPERTY C_STANDARD 99
)
set_property(
	TARGET mlog_static
	PROPERTY OUTPUT_NAME mlog
)
set_property(
	TARGET mlog_test
	PROPERTY C_STANDARD 99
)
set_property(
	TARGET mlog_test
	PROPERTY OUTPUT_NAME mlog
)
include_directories(
	inc/
	tpl/
)
