cmake_minimum_required(VERSION 3.5)

project(Bee_Colony_Simulator_SSV_2022_STEP1)

#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(TEST_DIR "${PROJECT_SOURCE_DIR}/Tests")
set(CACHE_DIR "${PROJECT_SOURCE_DIR}/../extlib/Catch/include")


include_directories(${PROJECT_SOURCE_DIR})
include_directories(${CACHE_DIR})
include_directories(${TEST_DIR})

file(GLOB UTILITY_SOURCES
     "${PROJECT_SOURCE_DIR}/Utility/*.cpp"
     "${PROJECT_SOURCE_DIR}/Utility/*.hpp"
     "${PROJECT_SOURCE_DIR}/JSON/*.cpp"
     "${PROJECT_SOURCE_DIR}/JSON/*.hpp"
     "${PROJECT_SOURCE_DIR}/Random/*.cpp"
     "${PROJECT_SOURCE_DIR}/Random/*.hpp"
     )
   

file(GLOB PROJECT_SOURCES
     "${PROJECT_SOURCE_DIR}/*.cpp"
     "${PROJECT_SOURCE_DIR}/*.hpp"    
      "${PROJECT_SOURCE_DIR}/Interface/*.hpp"
      "${PROJECT_SOURCE_DIR}/Stats/*.hpp"
      "${PROJECT_SOURCE_DIR}/Stats/*.cpp"
      "${PROJECT_SOURCE_DIR}/Env/*.cpp"
      "${PROJECT_SOURCE_DIR}/Env/*.hpp"
      ${UTILITY_SOURCES}
      )
    
file(GLOB SOURCES
     ${PROJECT_SOURCES}
)   


list(REMOVE_ITEM PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/main.cpp")
   

file (GLOB CACHE_SOURCES
  "${CACHE_DIR}/catch.hpp"
   "${TEST_DIR}/UnitTests/CheckUtility.hpp"
   "${TEST_DIR}/UnitTests/CatchTests.cpp"
   #   ${SOURCES}
   ${UTILITY_SOURCES}
    )


file(GLOB SOURCES_GFX
         ${SOURCES}
)   
  
add_custom_target(application-run
    COMMAND ./application
    DEPENDS application
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    )
  
# Find any version 2.X of SFML, first trying 2.5 or above (for which CMake configuration changed)
find_package(SFML 2.5 QUIET COMPONENTS graphics window system)
    if(SFML_FOUND)
      message(STATUS "Found SFML ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR}.${SFML_VERSION_PATCH}")
      
      #main application:
     ## not yet available
	
      ##Units tests:
      ## Collider
      #  add_executable (colliderTest  ${TEST_DIR}/UnitTests/ColliderTest.cpp ${SOURCES} ${CACHE_SOURCES})
#	target_link_libraries(colliderTest sfml-graphics sfml-window sfml-system)
	#JSON parser
#       add_executable (jsonTest  ${TEST_DIR}/UnitTests/JSONTest.cpp ${SOURCES} ${CACHE_SOURCES})
#       target_link_libraries(jsonTest sfml-graphics sfml-window sfml-system)

       ##Vec2d
      add_executable (vec2dTest  ${TEST_DIR}/UnitTests/Vec2dTest.cpp ${SOURCES} ${CACHE_SOURCES})
        target_link_libraries(vec2dTest sfml-graphics sfml-window sfml-system)

       
    else()
        set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
        find_package(SFML 2 COMPONENTS graphics window system)
        if(SFML_FOUND)

            message(STATUS "SFML_INCLUDE_DIR : ${SFML_INCLUDE_DIR}")
            message(STATUS "SFML_LIBRARIES   : ${SFML_LIBRARIES}")
            message(STATUS "SFML_DEPENDENCIES: ${SFML_DEPENDENCIES}")

            include_directories(${SFML_INCLUDE_DIR})
	    
	    #main application:
	    # not yet available
	    
	    ##Units tests:
	    ## Collider
#	     add_executable (colliderTest ${TEST_DIR}/UnitTests/ColliderTest.cpp ${SOURCES} ${CACHE_SOURCES})
#             target_link_libraries(colliderTest ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
	     ## JSON parser
#	     add_executable (jsonTest ${TEST_DIR}/UnitTests/JSONTest.cpp ${SOURCES} ${CACHE_SOURCES})
#             target_link_libraries(jsonTest ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

	     ## Vec2d
	    add_executable (vec2dTest ${TEST_DIR}/UnitTests/Vec2dTest.cpp ${SOURCES} ${CACHE_SOURCES})
              target_link_libraries(vec2dTest ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
	    

        else()
            message(STATUS "SFML NOT FOUND ! ")
        endif() ## SFML 2
endif() ## SFML 2.5
