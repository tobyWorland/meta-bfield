
file(READ prelude.c PRELUDE_CONTENT_FOR_C)

configure_file(preludes.cpp.in preludes.cpp @ONLY)

add_library(preludes STATIC
	preludes.cpp
)
target_include_directories(preludes PUBLIC ".")
