#pragma once
#include <utility>
#include <cmath>

namespace Loki {
	//Its funny
	constexpr float Pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f;
	constexpr float twoPi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f * 2.0f;
	constexpr float halfpi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f / 2.0f;

	constexpr float RadToDeg(float radians) { return radians * (180.0f / Pi); }
	constexpr float DegToRad(float degrees) { return degrees * (Pi / 180.0f); }

	constexpr int Wrap(int value, int max) { return (value % max) + ((value < 0) ? max : 0); }

	inline float Wrap(float value, float max) { return std::fmod(value, max) + ((value < 0) ? max : 0); }

	template <typename T>
	inline T Max(T a, T b) { return (a > b) ? a : b; }

	template <typename T>
	inline T Min(T a, T b) { return (a < b) ? a : b; }

	template <typename T>
	inline T Clamp(T value, T min, T max) { 
		if (min > max) std::swap(min, max);

		return (value < min) ? min : (value > max) ? max : value;
	}

	template<typename T>
	constexpr T Lerp(const T& a, const T& b, float t) {
		return (a * (1.0f - t)) + (b * t);
	}
}