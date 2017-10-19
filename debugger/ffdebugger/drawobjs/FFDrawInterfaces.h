#pragma once
// rgba color
typedef unsigned int FFColor;

#define BCRED(color) ((unsigned char)((color) >> 24))
#define BCGREEN(color) ((unsigned char)(((color) & 0x00FF0000) >> 16))
#define BCBLUE(color) ((unsigned char)(((color) & 0x0000FF00) >> 8))
#define BCALPHA(color) ((unsigned char)(color))
#define BUZZCOLOR(r,g,b,a) (FFColor)(((r) << 24) | ((g) << 16) | ((b) << 8) | a)

struct FFLocation {
	float x;
	float y;

	FFLocation(const float& x, const float& y) : x(x), y(y) {
	}
};
