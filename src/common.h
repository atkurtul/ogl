#pragma once
#include "vector"
#include "unordered_map"
#include "string"
#include "iostream"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

using std::string;
using std::vector;
using std::unordered_map;
typedef unsigned uint;
typedef unsigned long long uint64;

inline void printv(glm::vec3 v) {
	printf("%5f, %5f, %5f\n", v.x, v.y, v.z);
}

inline void printv(glm::vec4 v) {
	printf("%5f, %5f, %5f, %5f\n", v.x, v.y, v.z, v.w);
}

inline void printv(glm::mat4 v) {
	printv(v[0]);
	printv(v[1]);
	printv(v[2]);
	printv(v[3]);
	printf("----\n");
}
