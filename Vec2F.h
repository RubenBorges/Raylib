#pragma once
#include "raylib.h"
import std;
struct Vec2F {
	float x{ 0.0f };
	float y{ 0.0f };
	Vec2F();
	Vec2F(float _x, float _y);
	Vec2F(const Vector2 vec);
	Vec2F(const Vector2& vec);

	// Overload + operator for vector addition
	bool operator==(const Vector2& other) const;
	bool operator==(const Vec2F& other) const;
	void operator=(const Vector2& other) ;
	void operator=(const Vec2F& other) ;
	bool operator!=(const Vector2& other) const;
	bool operator!=(const Vec2F& other) const;
	Vec2F operator+(const Vector2& other) const;
	Vec2F operator+(const Vec2F& other) const; 
	Vec2F operator-(const Vec2F& other) const; 
	Vec2F operator-(const Vector2& other) const; 
	Vec2F operator*(const Vector2& other) const;
	Vec2F operator*(const Vec2F& other) const;
	Vec2F operator/(const Vector2& other) const;
	Vec2F operator/(const Vec2F& other) const;
	// Overload += operator for in-place addition
	Vec2F& operator+=(const Vec2F& other) noexcept;
	Vec2F& operator+=(const Vec2F&& other) noexcept;
	Vec2F& operator+=(const Vector2& other) noexcept;
	Vec2F& operator+=(const Vector2&& other) noexcept;
	Vec2F& operator-=(const Vec2F& other) noexcept;
	Vec2F& operator-=(const Vec2F&& other) noexcept;
	Vec2F& operator-=(const Vector2& other) noexcept;
	Vec2F& operator-=(const Vector2&& other) noexcept;
	Vec2F& operator*=(const Vec2F& other) noexcept;
	Vec2F& operator*=(const Vec2F&& other) noexcept;
	Vec2F& operator*=(const Vector2& other)noexcept;
	Vec2F& operator*=(const Vector2&& other)noexcept;
	Vec2F operator/=(const Vector2& other) ;
	Vec2F operator/=(const Vec2F& other);
	Vec2F operator/=(const Vector2&& other);
	Vec2F operator/=(const Vec2F&& other);
};


