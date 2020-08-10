#pragma once
#include "common.h"
struct Texture {
	uint id;
	uint width, height;

	enum Slot {
		DIFFUSE = 0,
		NORMAL = 1,
	};

	void bind(Slot) const;

	bool load(string path);

	void from_raw(const void* data, uint w, uint h, int format);

	void free();
};