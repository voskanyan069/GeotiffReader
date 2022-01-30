CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( GeotiffReader VERSION 1.0 )

SET( CMAKE_CXX_STANDARD 11 )

FIND_PACKAGE( Boost 1.71 COMPONENTS
	system
	filesystem
	iostreams
	program_options
	REQUIRED )
FIND_PACKAGE( GDAL REQUIRED )
FIND_PACKAGE( GTest REQUIRED )

SET( TARGER_LIBS ${TARGER_LIBS} geotiff_reader )
SET( TARGER_LIBS ${TARGER_LIBS} ${Boost_LIBRARIES} )
SET( TARGER_LIBS ${TARGER_LIBS} ${GDAL_LIBRARIES} )
SET( TARGER_LIBS ${TARGER_LIBS} pthread )

SET( GTEST_LIBS ${GTEST_LIBS} GTest::GTest )
SET( GTEST_LIBS ${GTEST_LIBS} GTest::Main )

INCLUDE_DIRECTORIES(
	${PROJECT_SOURCE_DIR}/inc
	${PROJECT_SOURCE_DIR}/src
	)

FILE(GLOB ALL_SOURCES
	"${PROJECT_SOURCE_DIR}/inc/remote/*.hpp"
	"${PROJECT_SOURCE_DIR}/inc/geotiff_reader/*.hpp"
	"${PROJECT_SOURCE_DIR}/inc/geotiff_structures/*.hpp"
	"${PROJECT_SOURCE_DIR}/src/remote/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/geotiff_reader/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/geotiff_structures/*.cpp"
	)

ADD_LIBRARY( geotiff_reader SHARED ${ALL_SOURCES} )
ADD_EXECUTABLE( GeotiffReader "${PROJECT_SOURCE_DIR}/example/application.cpp" )
ADD_EXECUTABLE( ReaderTest "${PROJECT_SOURCE_DIR}/test/test.cpp")

TARGET_LINK_LIBRARIES( GeotiffReader ${TARGER_LIBS} )
TARGET_LINK_LIBRARIES( ReaderTest ${TARGER_LIBS} ${GTEST_LIBS} )
