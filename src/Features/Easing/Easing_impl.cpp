// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.


#include "Easing.h"
#include <cmath>
#include <numbers>

float pi_float = static_cast<float>(std::numbers::pi);

void Easing::EaseNone(float _t)
{
	easedT_ = _t;
	return;
}

void Easing::EaseInSine(float _time)
{
	easedT_ = 1.0f - std::cosf((_time * pi_float) / 2.0f);
	return;
}

void Easing::EaseOutSine(float _t)
{
	easedT_ = std::sinf(_t * pi_float / 2.0f);
	return;
}

void Easing::EaseInOutSine(float _t)
{
	easedT_ = -(std::cosf(_t * pi_float) - 1.0f) / 2.0f;
	return;
}

void Easing::EaseInQuad(float _t)
{
	easedT_ = _t * _t;
	return;
}

void Easing::EaseOutQuad(float _t)
{
	easedT_ = 1.0f - (1.0f - _t) * (1.0f - _t);
	return;
}

void Easing::EaseInOutQuad(float _t)
{
	if (_t < 0.5f)
	{
		easedT_ = 2.0f * _t * _t;
	}
	else
	{
		easedT_ = 1.0f - std::powf(-2.0f * _t + 2.0f, 2.0f) / 2;
	}
	return;
}

void Easing::EaseInCubic(float _t)
{
	easedT_ = _t * _t * _t;
	return;
}

void Easing::EaseOutCubic(float _t)
{
	easedT_ = 1.0f - std::powf(1.0f - _t, 3.0f);
	return;
}

void Easing::EaseInOutCubic(float _t)
{
	if (easedT_ < 0.5f)
	{
		easedT_ = 4.0f * _t * _t * _t;
	}
	else
	{
		easedT_ = 1.0f - std::powf(-2.0f * _t + 2.0f, 3.0f) / 2.0f;
	}
	return;
}

void Easing::EaseInQuart(float _t)
{
	easedT_ = _t * _t * _t * _t;
	return;
}

void Easing::EaseOutQuart(float _t)
{
	easedT_ = 1.0f - std::powf(1.0f - _t, 4.0f);
	return;
}

void Easing::EaseInOutQuart(float _t)
{
	if (_t < 0.5f)
	{
		easedT_ = 8.0f * _t * _t * _t * _t;
	}
	else
	{
		easedT_ = 1.0f - std::powf(-2.0f * _t + 2.0f, 4.0f) / 2.0f;
	}
	return;
}

void Easing::EaseInBack(float _t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	easedT_ = c3 * _t * _t * _t - c1 * _t * _t;
	return;
}

void Easing::EaseOutBack(float _t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	easedT_ = 1.0f + c3 * std::powf(_t - 1.0f, 3.0f) + c1 * std::powf(_t - 1.0f, 2.0f);
	return;
}

void Easing::EaseInOutBack(float _t)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	if (_t < 0.5f)
	{
		easedT_ = (std::powf(2.0f * _t, 2.0f) * ((c2 + 1.0f) * 2.0f * _t - c2)) / 2.0f;
	}
	else
	{
		easedT_ = (std::powf(2.0f * _t - 2.0f, 2.0f) * ((c2 + 1.0f) * (_t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	}
	return;
}

void Easing::EaseInElastic(float _t)
{
	const float c4 = (2.0f * pi_float) / 3.0f;
	if (_t == 0)
	{
		easedT_ = 0.0f;
	}
	else if (_t == 1.0f)
	{
		easedT_ = 1.0f;
	}
	else
	{
		easedT_ = -std::powf(2.0f, 10.0f * _t - 10.0f) * std::sinf((_t * 10.0f - 10.75f) * c4);
	}

	return;
}

void Easing::EaseOutElastic(float _t)
{
	const float c4 = (2.0f * pi_float) / 3.0f;
	if (_t == 0)
	{
		easedT_ = 0.0f;
	}
	else if (_t == 1.0f)
	{
		easedT_ = 1.0f;
	}
	else
	{
		easedT_ = std::powf(2.0f, -10.0f * _t) * std::sinf((_t * 10.0f - 0.75f) * c4) + 1.0f;
	}

	return;
}

void Easing::EaseInOutElastic(float _t)
{
	const float c5 = (2.0f * pi_float) / 4.5f;

	if (_t == 0.0f)
	{
		easedT_ = 0.0f;
	}
	else if (_t == 1.0f)
	{
		easedT_ = 1.0f;
	}
	else if (_t < 0.5f)
	{
		easedT_ = -(std::powf(2.0f, 20.0f * _t - 10.0f) * std::sinf((20.0f * _t - 11.125f) * c5)) / 2.0f;
	}
	else
	{
		easedT_ = (std::powf(2.0f, -20.0f * _t + 10.0f) * std::sinf((20.0f * _t - 11.125f) * c5)) / 2.0f + 1.0f;
	}
	return;
}

void Easing::EaseInBounce(float _t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	float resultT = 0.0f;
	float time = 1.0f - _t;

	if (time < 1.0f / d1)
	{
		resultT = n1 * time * time;
	}
	else if (time < 2.0f / d1)
	{
		resultT = n1 * (time -= 1.5f / d1) * time + 0.75f;
	}
	else if (time < 2.5f / d1)
	{
		resultT = n1 * (time -= 2.25f / d1) * time + 0.9375f;
	}
	else
	{
		resultT = n1 * (time -= 2.625f / d1) * time + 0.984375f;
	}

	easedT_ = 1.0f - resultT;
	return;
}

void Easing::EaseOutBounce(float _t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (_t < 1.0f / d1)
	{
		easedT_ = n1 * _t * _t;
	}
	else if (_t < 2.0f / d1)
	{
		easedT_ = n1 * (_t -= 1.5f / d1) * _t + 0.75f;
	}
	else if (_t < 2.5f / d1)
	{
		easedT_ = n1 * (_t -= 2.25f / d1) * _t + 0.9375f;
	}
	else
	{
		easedT_ = n1 * (_t -= 2.625f / d1) * _t + 0.984375f;
	}

	return;
}

void Easing::EaseInOutBounce(float _t)
{
	
	if (_t < 0.5f)
	{
		EaseOutBounce(1.0f - 2.0f * _t);
		easedT_ = (1.0f - easedT_) / 2.0f;
	}
	else
	{
		EaseOutBounce(2.0f * _t - 1.0f);
		easedT_ = (1.0f + easedT_) / 2.0f;
	}
	return;
}
