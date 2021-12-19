

#include "glad/glad.h"

#include "algorithm"
#include "texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Texture::bind(Slot slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id);
}

bool Texture::load(string path) {
  std::replace(path.begin(), path.end(), '\\', '/');
  path = PROJECT_BASE_BUILD_DIR + ("/" + path);

  int w, h, d;
  unsigned char* data = stbi_load(path.data(), &w, &h, &d, 4);
  if (!data) {
    printf("Failed to load %s\n", path.data());
    return 0;
  }
  from_raw(data, w, h, GL_RGBA);
  ::free(data);
  return 1;
}

void Texture::from_raw(const void* data, uint w, uint h, int format) {
  width = w;
  height = h;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
  // data);
  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::free() {
  glDeleteTextures(1, &id);
  delete this;
}
