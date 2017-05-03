
#s3mplay
    
set(s3mplay_SRC
    main.cpp
)

#add definitions, compiler switches, etc.
add_definitions(-Wall -O2)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ../libs3m)

#list all source files here
add_executable(s3mplay ${s3mplay_SRC})

#need to link to some other libraries ? just add them here
target_link_libraries(s3mplay ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} libs3m)

install(TARGETS s3mplay RUNTIME DESTINATION bin)
