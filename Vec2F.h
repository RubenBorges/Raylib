#pragma once
#include "raylib.h"
import std;
struct Vec2F {
	float x, y;

	Vec2F();
	Vec2F(float _x, float _y);
	Vec2F(const Vector2 vec);
	Vec2F(const Vector2& vec);

	// Overload + operator for vector addition
	bool operator==(const Vector2& other) const;
	bool operator==(const Vec2F& other) const;
	void operator=(const Vector2& other);
	void operator=(const Vec2F& other);
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


class SpriteAnimator {
	int currentIndex{ 0 };
	std::vector<int>Ids;
	std::vector<Texture2D> animationFrame;
	std::vector<Vec2F> position, velocity;
	std::vector<int>Directions;
	public:
	void newAnimations( std::vector<Texture2D>& texturearray ) {
		animationFrame.clear();
		animationFrame = std::move( texturearray );		
		return;
	}
	void appendAnimations( std::vector<Texture2D>& texturearray ) {
		animationFrame.append_range( texturearray );
		return;
	}
	//void animate() {////////////
	//	if ( state == ANIMATION_STATE::MOVING ) animateSprite();
	//}
	//void animateSprite() {//////////
	//	cycleSwitch == BOOLEAN_SWITCH::BASE ? cycleSwitch = BOOLEAN_SWITCH::CYCLE :cycleSwitch = BOOLEAN_SWITCH::BASE;
	//	currentIndex = (static_cast<int>(direction) * 2) + (static_cast<int>(cycleSwitch));
	//}
	// 
	//Returns the animation frames as a std::vector<Texture2D>
	std::vector<Texture2D> TextureArray() {
		return animationFrame;
	}
	private:

	void storeAnimations( Texture2D texture ) {
		animationFrame.emplace_back( texture );
	}
	void storeAnimations( std::vector<Texture2D> texturearray ) {
		animationFrame = std::move( texturearray );
	}

};