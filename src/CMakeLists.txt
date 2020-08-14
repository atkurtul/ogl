
add_definitions(-DPROJECT_BASE_BUILD_DIR="${CMAKE_SOURCE_DIR}")

file(GLOB PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(ogl ${PROJECT_SOURCES})

target_link_libraries(ogl 
    glfw 
    glad 
    glm 
    assimp 
    BulletSoftBody 
    BulletDynamics 
    BulletCollision 
    BulletInverseDynamics 
    Bullet3Common
    LinearMath
)

add_subdirectory(demos)