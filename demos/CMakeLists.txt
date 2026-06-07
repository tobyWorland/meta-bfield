add_executable(demo_t32
    demo_t32.c
)
target_link_libraries(demo_t32 gen_t32_encoding)

add_executable(demo_prelude
    demo_prelude.c
)
