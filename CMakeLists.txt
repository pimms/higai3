cmake_minimum_required(VERSION 2.8)


file(GLOB ann_SRC
	"src/*.h"
	"src/*.cpp"
)

# This may not work for VS. Fuck the police.
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (WIN32)
	LINK_DIRECTORIES(C:/lib/SDL2/lib/x86)
	INCLUDE_DIRECTORIES(C:/lib/SDL2/include)
endif(WIN32)

add_executable(ann ${ann_SRC})
target_link_libraries(ann SDL2_image)
target_link_libraries(ann SDL2)
