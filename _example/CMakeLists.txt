
add_executable(example_server server/main.c)

target_link_libraries(example_server PUBLIC Crpc)
