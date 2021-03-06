CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( GeotiffReader VERSION 1.0 )

SET( CMAKE_CXX_STANDARD 11 )

FIND_PACKAGE( Boost 1.71 COMPONENTS
	system
	filesystem
	program_options
	REQUIRED )
FIND_PACKAGE( CURL REQUIRED )
FIND_PACKAGE( GDAL REQUIRED )
FIND_PACKAGE( GTest REQUIRED )

SET( TARGET_LIBS ${TARGET_LIBS} geotiff_reader )
SET( TARGET_LIBS ${TARGET_LIBS} base )
SET( TARGET_LIBS ${TARGET_LIBS} CURL::libcurl )
SET( TARGET_LIBS ${TARGET_LIBS} ${Boost_LIBRARIES} )
SET( TARGET_LIBS ${TARGET_LIBS} ${GDAL_LIBRARIES} )
SET( TARGET_LIBS ${TARGET_LIBS} pthread )

SET( GTEST_LIBS ${GTEST_LIBS} GTest::GTest )
SET( GTEST_LIBS ${GTEST_LIBS} GTest::Main )

INCLUDE_DIRECTORIES(
	${PROJECT_SOURCE_DIR}/inc
	${PROJECT_SOURCE_DIR}/src
	)

FILE(GLOB BASE_LIB_SOURCES
	"${PROJECT_SOURCE_DIR}/src/base/*.cpp"
	)

FILE(GLOB READER_LIB_SOURCES
	"${PROJECT_SOURCE_DIR}/src/utils/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/remote/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/geotiff_types/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/geotiff_reader/*.cpp"
	)

ADD_LIBRARY( base SHARED ${BASE_LIB_SOURCES} )
ADD_LIBRARY( geotiff_reader SHARED ${READER_LIB_SOURCES} )
ADD_EXECUTABLE( GeotiffReader "${PROJECT_SOURCE_DIR}/example/application.cpp" )
ADD_EXECUTABLE( ReaderTest "${PROJECT_SOURCE_DIR}/test/test.cpp" )

TARGET_LINK_LIBRARIES( GeotiffReader ${TARGET_LIBS} )
TARGET_LINK_LIBRARIES( ReaderTest ${TARGET_LIBS} ${GTEST_LIBS} )
