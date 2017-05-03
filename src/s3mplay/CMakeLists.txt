#s3mplay
    
set(s3mplay_SRC
    main.cpp
)

include_directories(${SDL2_INCLUDE_DIR} ../libs3m)

#list all source files here
add_executable(s3mplay main.cpp)

target_link_libraries(s3mplay libs3m ${SDL2_LIBRARY})
