#s3mplay
    
set(s3mplay_SRC
    main.cpp
)

include_directories(${SDL2_INCLUDE_DIR} ../libs3m)

add_executable(s3mplay ${s3mplay_SRC})

target_link_libraries(s3mplay libs3m ${SDL2_LIBRARY})
