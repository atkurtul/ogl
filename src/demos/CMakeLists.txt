
add_executable(animation animation.cpp)
add_executable(physics physics.cpp)


target_link_libraries(animation ogl)
target_link_libraries(physics ogl)