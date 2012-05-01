FIND_PACKAGE(Boost COMPONENTS unit_test_framework REQUIRED)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}  # Needed for moc files in out of source builds.
  "../logWitch"
)

#### UnitTests

SET(UNITTESTS_SOURCES
	TestExample.cpp	
)

ADD_EXECUTABLE( UnitTests ${UNITTESTS_SOURCES} )

TARGET_LINK_LIBRARIES(UnitTests 
  	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

ADD_TEST(NAME UnitTests COMMAND ${CMAKE_CURRENT_BINARY_DIR}/UnitTests)

##### testStringCacheTreeItem
SET(testStringCacheTreeItem_SOURCES
	test_StringCacheTreeItem.cpp	
)

ADD_EXECUTABLE( testStringCacheTreeItem ${testStringCacheTreeItem_SOURCES} )

TARGET_LINK_LIBRARIES(testStringCacheTreeItem 
  	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
 	${LFA_MODELS_LIB}
  	${QT_LIBRARIES}
)

ADD_TEST(NAME testStringCacheTreeItem COMMAND ${CMAKE_CURRENT_BINARY_DIR}/testStringCacheTreeItem)