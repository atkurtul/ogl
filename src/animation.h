#pragma once
#include "bone.h"

template<class T>
struct KeyTransform {
	T transform;
	float time;
};


struct KeyFrames {
	vector<KeyTransform<glm::vec3>> sca;
	vector<KeyTransform<glm::vec3>> pos;
	vector<KeyTransform<glm::quat>> rot;

	static glm::vec3 interpolate(glm::vec3 a, glm::vec3 b, float t) {
		return a * (1.f - t) + b * t;
	}
	static glm::quat interpolate(glm::quat a, glm::quat b, float t) {
		return glm::slerp(a, b, t);
	}

	template<class T>
	static bool find_transform(vector<KeyTransform<T>> const& xf, float t, T& out) {
		if (xf.empty())	
			return 0;

		if (xf.size() == 1) {
			out = xf.front().transform;
			return 1;
		}

		for (int i = 0; i < xf.size() - 1; ++i) {
			float t0 = xf[i].time;
			float t1 = xf[i + 1].time;
			if (t0 <= t && t1 >= t) {
				float blend = (t - t0) / (t1 - t0);
				out = interpolate(xf[i].transform, xf[i + 1].transform, blend);
				return 1;
			}
		}

		return 0;
	}

	void get_matrix(float time, glm::mat4& out) {
		glm::vec3 p;
		glm::vec3 s;
		glm::quat r;
		if (!find_transform(rot, time, r)) return;
		if (!find_transform(pos, time, p)) return;
		if (!find_transform(sca, time, s)) return;
			
		out = glm::mat4_cast(r);
		out[3] = glm::vec4(p, 1);
	}
};

struct Animation {
	unordered_map<string, KeyFrames> keyframes;
	float duration;
	float timer;

	void import(struct Scene*, const struct aiAnimation*);

	glm::mat4 get_frame(Bone* bone);

	void update(float dt);
};