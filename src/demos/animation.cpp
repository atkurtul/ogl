#include "../scene.h"

int main() {
  Scene scene;
  string base = PROJECT_BASE_BUILD_DIR;
  scene.import(base + "/scene_Scene.fbx").render();
}
