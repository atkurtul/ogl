
add_definitions(-DPROJECT_BASE_BUILD_DIR="${CMAKE_SOURCE_DIR}")

file(GLOB PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(ogl ${PROJECT_SOURCES})

target_include_directories(ogl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ogl glfw)
target_link_libraries(ogl glad)
target_link_libraries(ogl glm)
target_link_libraries(ogl assimp)
target_link_libraries (ogl BulletSoftBody)
target_link_libraries (ogl BulletDynamics)
target_link_libraries (ogl BulletCollision)
target_link_libraries (ogl BulletInverseDynamics)
target_link_libraries (ogl Bullet3Common)
target_link_libraries (ogl LinearMath)
add_subdirectory(demos)