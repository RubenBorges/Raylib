#include "Vec2F.h"

Vec2F::Vec2F() {x = 0.0f; y = 0.0f;};
	Vec2F::Vec2F(float _x, float _y) : x{ _x }, y{ _y } {};
	Vec2F::Vec2F(const Vector2 vec) : x{ vec.x }, y{ vec.y } {};
	Vec2F::Vec2F(const Vector2& vec) : x{ vec.x }, y{ vec.y } {};

	// Overload + operator for vector addition
	Vec2F Vec2F::operator+(const Vector2& other) const {
		return Vec2F(x + other.x, y + other.y);
	}
	Vec2F Vec2F::operator+(const Vec2F& other) const {
		return Vec2F(x + other.x, y + other.y);
	}

	void Vec2F::operator=(const Vector2& other)  {
		x = other.x;
		y = other.y;
		return;
	}
	void Vec2F::operator=( const Vec2F& other ) {
		x = other.x;
		y = other.y;
		return;
	}
	bool Vec2F::operator==(const Vector2& other) const {
		return (x == other.x && y == other.y);
	}
	bool Vec2F::operator==(const Vec2F& other) const {
		return (x == other.x && y == other.y);
	}

	bool Vec2F::operator!=(const Vector2& other) const {
		return !(*this == other);
	}
	bool Vec2F::operator!=(const Vec2F& other) const {
		return !(*this == other);
	}

	Vec2F Vec2F::operator-(const Vec2F& other) const {
		return Vec2F(x - other.x, y - other.y);
	}
	Vec2F Vec2F::operator-(const Vector2& other) const {
		return Vec2F(x - other.x, y - other.y);
	}
	Vec2F Vec2F::operator*(const Vector2& other) const {
		return Vec2F(x * other.x, y * other.y);
	}
	Vec2F Vec2F::operator*(const Vec2F& other) const {
		return Vec2F(x * other.x, y * other.y);
	}
	Vec2F Vec2F::operator/(const Vector2& other) const {
		if (other.x == 0.0f || other.y == 0.0f) {
			throw std::runtime_error("Division by zero in Vec2F Vec2F::operator/");
		}
		return Vec2F(x / other.x, y / other.y);
	}
	Vec2F Vec2F::operator/(const Vec2F& other) const {
		if (other.x == 0.0f || other.y == 0.0f) {
			throw std::runtime_error("Division by zero in Vec2F Vec2F::operator/");
		}
		return Vec2F(x / other.x, y + other.y);
	}
	// Overload += Vec2F::operator for in-place addition
	Vec2F& Vec2F::operator+=(const Vec2F& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2F& Vec2F::operator+=(const Vec2F&& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2F& Vec2F::operator+=(const Vector2& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2F& Vec2F::operator+=(const Vector2&& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2F& Vec2F::operator-=(const Vec2F& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vec2F& Vec2F::operator-=(const Vec2F&& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vec2F& Vec2F::operator-=(const Vector2& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vec2F& Vec2F::operator-=(const Vector2&& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vec2F& Vec2F::operator*=(const Vec2F& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2F& Vec2F::operator*=(const Vec2F&& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2F& Vec2F::operator*=(const Vector2& other)noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2F& Vec2F::operator*=(const Vector2&& other)noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2F Vec2F::operator/=(const Vector2& other) {
		if (other.x == 0.0f || other.y == 0.0f) { throw std::runtime_error("Division by zero in Vec2F Vec2F::operator/"); }
		x /= other.x;
		y /= other.y;
		return *this;
	}
	Vec2F Vec2F::operator/=(const Vec2F& other) {
		if (other.x == 0.0f || other.y == 0.0f) { throw std::runtime_error("Division by zero in Vec2F Vec2F::operator/"); }
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vec2F Vec2F::operator/=(const Vector2&& other) {
		if (other.x == 0.0f || other.y == 0.0f) { throw std::runtime_error("Division by zero in Vec2F Vec2F::operator/"); }
		x /= other.x;
		y /= other.y;
		return *this;
	}
	Vec2F Vec2F::operator/=(const Vec2F&& other) {
		if (other.x == 0.0f || other.y == 0.0f) { throw std::runtime_error("Division by zero in Vec2F operator/"); }
		x /= other.x;
		y /= other.y;
		return *this;
	}
