cmake_minimum_required(VERSION 3.31.3)

Project(Advanced-types)
add_compile_options(-O3 -Wall)
option(ENABLE_DARRAY_BASED_QUEUE OFF)
option(ENABLE_DARRAY_BASED_STACK OFF)

#file(GLOB SOURCES "source/*.c")
set(SOURCES
	source/utility.c
	source/dlinked_list.c
	source/dynamic_array.c
	source/red_black_tree.c
	source/hmap.c
	source/stack.c
	source/queue.c
	source/json.c
	source/yaml.c
	source/parsers.c
	source/set.c
)

if(ENABLE_DARRAY_BASED_QUEUE)
	set_source_files_properties(source/queue.c PROPERTIES COMPILE_DEFINITIONS "DARRAY_QUEUE_H")
endif()

if(ENABLE_DARRAY_BASED_STACK)
	set_source_files_properties(source/stack.c PROPERTIES COMPILE_DEFINITIONS "DARRAY_STACK_H")
endif()

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC headers)

