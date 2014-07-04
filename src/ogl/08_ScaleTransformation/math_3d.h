#pragma once

struct Vector3f
{
	float x, y, z;

	Vector3f() {}
	Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Matrix4f
{
	float m[4][4];
};

