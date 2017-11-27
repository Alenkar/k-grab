project(k-grabber)
cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)



FIND_PACKAGE ( X11 REQUIRED )
IF ( X11_FOUND )
      INCLUDE_DIRECTORIES ( ${X11_INCLUDE_DIR} )
      LINK_LIBRARIES ( ${X11_LIBRARIES} )
      MESSAGE ( STATUS " X11_INCLUDE_DIR: " ${X11_INCLUDE_DIR} )
      MESSAGE ( STATUS " X11_LIBRARIES: " ${X11_LIBRARIES} )
ENDIF ( X11_FOUND )





aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

message("OpenCV_LIBS : ${OpenCV_LIBS}")
message("CMAKE_C_FLAGS : ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS : ${CMAKE_CXX_FLAGS}")
message("CMAKE_EXE_LINKER_FLAGS : ${CMAKE_EXE_LINKER_FLAGS}")

option(CUDA_USE_STATIC_CUDA_RUNTIME "Fuck it" OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -fopenmp -std=c++11 -fPIC -pthread -lX11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -fopenmp -std=c++11 -fPIC -pthread -lX11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgomp")
