// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once
#include "../Vector3.h"
#include "../Matrix4x4.h"

namespace Math
{
	float cotanf(float _x);
	float clamp(
		float _x,
		float _min,
		float _max
	);
	int clamp(
		int _x,
		int _min,
		int _max
	);
	float Lerp(float _x, float _y, float _t);
	Vector3 TransformNormal(const Vector3& _v, const Matrix4x4& _m);
	Vector3 Lerp(const Vector3& _vx, const Vector3& _vy, float _t);
	Vector3 Slerp(const Vector3& _vx, const Vector3& _vy, float _t);
}