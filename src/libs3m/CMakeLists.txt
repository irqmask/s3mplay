# libs3m

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
)


# system dependent include directories
# ---------------------------------------------------------
if (WIN32)

else (WIN32)

endif (WIN32)


# list of source files
# ---------------------------------------------------------

set (libs3m_SRCS
    s3m.h
    s3m_intern.h

    channel.c
    pattern.c
    s3m.c
    s3m_file.c
    s3m_info.c
    s3m_sound.c
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${libs3m_SRCS})
source_group(include FILES ${libs3m_SRCS})

# library
# ---------------------------------------------------------
add_library (libs3m ${libs3m_SRCS})
