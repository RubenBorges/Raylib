// Raylib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Written by: Ruben Michael Borges
// First Created : 1 September 2025
// 
// Description: A simple program to demonstrate basic collision detection, sound effects, and camera movement using a small and experimental game developed in C++ with the Raylib library.
//

#include "raylib.h"
#include "Vec2F.h"
import std;

enum class GHOST : int { BLINKY = 0, PINKY = 1, INKY = 2, CLYDE = 3, FUNKY = 4, SUE = 5 };
enum class ANIM : int {
	RIGHT_A = 0, RIGHT_B = 1,
	DOWNA = 2, DOWN_B = 3,
	LEFT_A = 4, LEFT_B = 5,
	UP_A = 6, UP_B = 7,
	BACK_A = 8, BACK_B = 9
};

enum class DIR : int { NULL_DIR = -1, RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3, BACK = 4 }; // DIR : int; DIR * 2 && DIR * 2 + 1 to get animation frames per direction
enum class ANIMATION_STATE : int { IDLE = 0, MOVING = 1 };
enum class FRAME_FMT :bool { IMAGE = false, TEXTURE = true };
enum class SPRITE_TYPE : int { PLAYER = 0, ENEMY = 1, NPC = 2, OBJECT = 3 };
//false = 0, true = 1; IF cycleAnimation == true (1) THEN  !cycleAnimation = false (0) THEN ANOTHER !cycleAnimation == true (1).. (1>0>1>0>1>0....); 
enum class BOOLEAN_SWITCH : int {
	BASE = false // =0
	, CYCLE = true//  =1 
};

class GameSprite {
	std::string name;
	int id = genID();	//std::random device to set random positions for power-up
	int FrameCount;
	Image* animationFrames;
	Texture2D activeTexture, *animationFrames_T;
	Vec2F size, velocity{ 0.0f,0.0f }, position{ 0.0f,0.0f }, lastPosition{ 0.0f,0.0f };
	bool isAnimating{ false }, cycleAnimation{ false }, frameType;// false = Image, true = Texture2D
	BOOLEAN_SWITCH animationCycleSwitch{ BOOLEAN_SWITCH::BASE };
	ANIMATION_STATE currentAnimationState{ ANIMATION_STATE::IDLE };
	DIR currentDirection{ DIR::DOWN };

public:
	GameSprite() = default;
	GameSprite(const std::string spriteName, const int _frameCount, Image* frames, bool frameType = (static_cast<bool>(FRAME_FMT::IMAGE))) {
		name = spriteName;
		storeAnimations(frames, _frameCount);
	}
	GameSprite(const std::string spriteName, const int _frameCount, Texture* frames, bool frameType = static_cast<bool>(FRAME_FMT::TEXTURE)) {
		name = spriteName;
		storeAnimations(frames, _frameCount);
	}
	~GameSprite() {
		delete[] animationFrames;
		delete[] animationFrames_T;
	}

	//{ NULL_DIR = -1, RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3, BACK = 4 };
	void setDirection(const DIR dir, bool animate = true) {
		currentDirection = dir;
		return;
	}
	DIR Direction() const { return currentDirection; } //{ NULL_DIR = -1, RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3, BACK = 4 };
	void newAnimations(Image* imagearray, const int _frameCount) {
		delete[] animationFrames;
		delete[] animationFrames_T;
		frameType = static_cast<bool>(FRAME_FMT::IMAGE);
		storeAnimations(imagearray, _frameCount);
		return;
	}
	void newAnimations(Texture2D* texturearray, const int _frameCount) {
		delete[] animationFrames;
		delete[] animationFrames_T;
		frameType = static_cast<bool>(FRAME_FMT::TEXTURE);
		storeAnimations(texturearray, _frameCount);
		return;
	}
	void animate() { if (currentAnimationState == ANIMATION_STATE::MOVING) animateSprite();}
	void animateSprite() {
		animationCycleSwitch == BOOLEAN_SWITCH::BASE ? animationCycleSwitch = BOOLEAN_SWITCH::CYCLE : animationCycleSwitch = BOOLEAN_SWITCH::BASE;
		const int frameIndex = (static_cast<int>(currentDirection) * 2) + (static_cast<int>(animationCycleSwitch));
		setActiveTexture( animationFrames_T[frameIndex] );
	}

	void setAnimationState(const ANIMATION_STATE state) { currentAnimationState = state; }
	int animationState() const { return static_cast<int>(currentAnimationState); }
	GameSprite* SpritePtr() { return this; }

	// Provide access to animation frame data and size; 
	//{OUT} dataPtr : pointer to hold address of animation frames array
	//{OUT} dataSize : integer reference to hold number of frames in animation
	void Data(void* dataPtr, int& dataSize) {
		if (frameType == static_cast<bool>(FRAME_FMT::IMAGE)) {
			dataPtr = static_cast<Image*>(dataPtr);
			dataPtr = animationFrames;
		}
		else {
			dataPtr = static_cast<Texture2D*>(dataPtr);
			dataPtr = animationFrames_T;
		}
		dataSize = FrameCount;
		return;
	}

	//Function to ascertain the current format of THIS object's FrameData
	//if Image you can call the Object.ImageArray() function  to get vector of Image frames
	//if Texture2D you can call the Object.TextureArray() function to get vector of Texture2D frames
	int frameFormat() const { return static_cast<int>(frameType); }
	std::vector<Image> ImageArray() {
		std::vector<Image> animationArray;
		for (size_t i = 0; i<FrameCount; ++i) {
			animationArray.push_back(animationFrames[i]);
		}
		return std::move(animationArray);
	}
	std::vector<Texture2D> TextureArray() {
		std::vector<Texture2D> animationArray;
		for (size_t i = 0; i < FrameCount; ++i) {
			animationArray.push_back(animationFrames_T[i]);
		}
		return std::move(animationArray);
	}

	void setVelocity(Vector2 _velocity) { velocity = _velocity; }
	void resetVelocity() { velocity.x = 0.0f; velocity.y = 0.0f; }
	Vec2F Velocity() { return velocity; }
	void ApplyVelocity() {
		position += velocity;
		return;
	}
	void setPosition(Vector2 _position) { position = _position; }
	bool Travel() { return (lastPosition != position); }

	void updatePosition() {
		lastPosition = position;
		position.x += velocity.x;
		position.y += velocity.y;
	};
	Vector2 Position() { return Vector2{ position.x, position.y }; }
	int ID() { return id; }
	int setID(int _id) {
		if (id == _id) { return -1; }
		id = _id;
		return 0;
	}
	Rectangle boundingBox() { return Rectangle{ position.x, position.y, size.x, size.y }; }
	Vec2F Location() { return (position + size); }
	Texture2D& ActiveTexture() { return activeTexture; }
	Rectangle indexedFrameRect(const int frameIndex) {
		if (frameType == static_cast<bool>(FRAME_FMT::IMAGE)) {
			return Rectangle{ 0.0f,0.0f,static_cast<float>(animationFrames[frameIndex].width),static_cast<float>(animationFrames[frameIndex].height) };
		}
		else {
			return Rectangle{ 0.0f,0.0f,static_cast<float>(animationFrames_T[frameIndex].width),static_cast<float>(animationFrames_T[frameIndex].height) };
		}
	}
	Rectangle FrameRect() { return Rectangle{ position.x,position.y, size.x, size.y }; }

 
private:
	void setSize(Image image) { size = Vector2{ static_cast<float>(image.width), static_cast<float>(image.height) }; }
	void setSize(Texture2D texture) { size = Vector2{ static_cast<float>(texture.width), static_cast<float>(texture.height) }; }
	void storeAnimations(Image* imagearray, const int _frameCount) {
		animationFrames = new Image[_frameCount];
		FrameCount = _frameCount;
		for (size_t i = 0; i < _frameCount; ++i) {
			animationFrames[i] = imagearray[i];
		}
		setActiveTexture(animationFrames[static_cast<int>(currentDirection) * 2]);
	}
	void storeAnimations(Texture2D* texturearray, const int _frameCount) {
		animationFrames_T = new Texture2D[_frameCount];
		FrameCount = _frameCount;
		for (size_t i = 0; i < _frameCount; ++i) {
			animationFrames_T[i] = texturearray[i];
		}
		setActiveTexture(animationFrames_T[static_cast<int>(currentDirection) * 2]);
	}

	void setActiveTexture(const Image image) { activeTexture = LoadTextureFromImage(image); setSize(image); }
	void setActiveTexture(const Texture2D _texture) { activeTexture = _texture; setSize(_texture); }
	int genID() {
		std::random_device rando;
		std::mt19937 genRand(rando());// Mersenne Twister generator
		std::uniform_int_distribution<> dis(0, (99999999)); // Range [0, ScreenWidth-25]
		return dis(genRand);
	}
};


int main()
{
	//will hold textures of current ghost animation frames
	Texture blinky, pinky, inky, clyde, funky, sue;
	Image image = LoadImage( "C:\\Projects\\Raylib\\assets\\pacman\\CEDX\\Character Skins - Smooth.png" );
	const int spriteWidth = 50;
	const int spriteHeight = 50;
	const int ghostSprites = 6;
	const int ghostAnimeFrames = 10;

	std::vector<std::vector<Image>> ghostImgSheet;
	std::vector<std::vector<Texture2D>> ghostTextSheet;
	ghostImgSheet.resize( ghostSprites );
	ghostTextSheet.resize( ghostSprites );
	for ( int i = 0; i < ghostSprites; ++i ) {
		ghostImgSheet[i].resize( ghostAnimeFrames );
		ghostTextSheet[i].resize( ghostAnimeFrames );
	}

	Texture2D testTex = ghostTextSheet[0][0];
//	std::println("Texture loaded with ID: {0}, Width: {1}, Height: {2}", texture.id, texture.width, texture.height);
	//Variables, Constants and Devices
	const int screenWidth{ 800 },
		screenHeight{ 600 };
	//std::random device to set random positions for power-up
	std::random_device randXPos, randYPos;
	std::mt19937 genX( randXPos() ), genY( randYPos() ); // Mersenne Twister generator
	std::uniform_int_distribution<> disX( 0, (screenWidth - 25) ), disY( 0, (screenHeight - 25) ); // Range [0, ScreenWidth-25]

	Vector2 targetOrigin = { screenWidth / 2, screenHeight / 2 },
		recTwoOrigin = { (screenWidth) / 5, (screenHeight / 2) - 50 },
		recThreeOrigin = { (screenWidth / 5) * 3, (screenHeight / 2) - 50 },
		powerBumpOrigin = { disX( genX ),disY( genY ) };
	int recWidth{ 100 }, recHeight{ 100 }, factor{ 0 }, bumpFactor{ 3 }, powerBumpBoost{ 0 }, collisionCounter{ 0 }, spriteHitCount{ 0 }, powerUpWidth{ 25 }, powerUpHeight{ 25 };
	Camera2D camera = { Vector2 {0,0},targetOrigin, 0.0f, 1.0f };
	Rectangle rec = { targetOrigin.x - 50, targetOrigin.y - 50 ,recWidth / 2,recHeight / 2 },
		recTwo = { recTwoOrigin.x, recTwoOrigin.y, recWidth / 2, recHeight / 2 },
		recThree = { recThreeOrigin.x, recThreeOrigin.y, recWidth / 2, recHeight / 2 },
		powerBump = { powerBumpOrigin.x, powerBumpOrigin.y, powerUpWidth, powerUpHeight };
	bool powerUpActive{ false };
	bool objectVisible[4]{ true, true, true, true };
	InitWindow( screenWidth, screenHeight, "Raylib basic window" );
	SetTargetFPS( 60 );

	for ( int i = 0; i < ghostSprites; i++ ) {
		for ( int j = 0; j < ghostAnimeFrames; j++ ) {
			ghostTextSheet[i][j] = LoadTextureFromImage(
				ghostImgSheet[i][j] = ImageFromImage( image, Rectangle{ static_cast<float>( i * spriteWidth ), static_cast<float>( j * spriteHeight ), static_cast<float>( spriteWidth ), static_cast<float>( spriteHeight ) } )
			);
		}

	}


	float frameTime = 0.1f; // Time per frame in seconds
	//Rectangle sourceRect = { spriteWidth * currentFrame, 0, spriteWidth, spriteHeight};
	Rectangle sourceRect = { 0, 0, spriteWidth, spriteHeight };
	Rectangle destRect = { 200, 200, spriteWidth * 2, spriteHeight * 2 };
	GameSprite Blinky( "Blinky", ghostAnimeFrames, ghostTextSheet[static_cast<int>( GHOST::BLINKY )].data() );
	std::println( "BLINKY HAS {0}",ghostAnimeFrames);
	std::println( "BLINKY HAS {0}",ghostAnimeFrames);
	std::println( "BLINKY HAS {0}",ghostAnimeFrames);
	std::println( "BLINKY HAS {0}",ghostAnimeFrames);
	// columns of 10 rows: 6 ghosts + 1 (5 empty, 4 eyes) + 1 pacman + 1 (4 empty,( 1 small dotsx2, med dot,big dot) x2 )currentFrame
	//std::vector<GameSprite> ghosts = { GameSprite( "Blinky", ghostAnimeFrames, ghostAnimeSheet[static_cast<int>(GHOST::BLINKY)].data() ),
	//	GameSprite( "Pinky", ghostAnimeFrames, ghostAnimeSheet[static_cast<int>(GHOST::PINKY)].data() ),
	//	GameSprite( "Inky", ghostAnimeFrames, ghostAnimeSheet[static_cast<int>(GHOST::INKY)].data() ),
	//	GameSprite( "Clyde", ghostAnimeFrames, ghostAnimeSheet[static_cast<int>(GHOST::CLYDE)].data() ),
	//	GameSprite( "Funky", ghostAnimeFrames, ghostAnimeSheet[static_cast<int>(GHOST::FUNKY)].data() ),
	//	GameSprite( "Sue", ghostAnimeFrames, ghostAnimeSheet[static_cast<int>(GHOST::SUE)].data() ) };

	Texture2D pacman = LoadTexture("./assets/pacman/CEDX/Character Skins - Smooth.png"); // Texture loading
	Rectangle frameRect = { 0.0f, 0.0f, (float)pacman.width / 6, (float)pacman.height };
	//Setup Basic Audio
	InitAudioDevice();

	std::filesystem::path assetBase = std::filesystem::current_path() / "AudioFX" / "Collision";

	int m = image.height;
	Sound soundClips[3] = { LoadSound("resources/AudioFX/Collision/beer-bottles-82232.mp3"),
						   LoadSound("resources/AudioFX/Collision/plastic-hit-3-34297.mp3"),
						   LoadSound("resources/AudioFX/Reset/a-sudden-appearance-143034.mp3") };

	if (IsSoundValid(soundClips[0]) && IsSoundValid(soundClips[1]) && IsSoundValid(soundClips[2])) std::cout << "Sound clips loaded successfully\n";
	else std::cout << "Sound clips failed to load\n";
	// Initial sound to indicate start of program
	PlaySound(soundClips[2]);
	while (!WindowShouldClose()) {
		float velocityX{ 0.0f }, velocityY{ 0.0f };
		Blinky.resetVelocity();

		//Poll and Handle Input Events
		PollInputEvents();
		if (IsKeyDown(KEY_RIGHT)) {
			velocityX += 2.0f;
			Blinky.setDirection(DIR::RIGHT);
			Blinky.setVelocity(Vector2{ 2.0f , 0.0f });

		}
		if (IsKeyDown(KEY_LEFT)) {
			velocityX -= 2.0f;
			Blinky.setDirection(DIR::LEFT);
			Blinky.setVelocity(Vector2{ -2.0f , 0.0f });
		}

		if (IsKeyDown(KEY_UP)) {
			velocityY -= 2.0f;
			Blinky.setDirection(DIR::UP);
			Blinky.setVelocity(Vector2{ 0.0f , -2.0f });
		}
		if (IsKeyDown(KEY_DOWN)) {
			velocityY += 2.0f;
			Blinky.setDirection(DIR::DOWN);
			Blinky.setVelocity(Vector2{ 0.0f , 2.0f });
		}
		Blinky.ApplyVelocity();
		std::println( "BLINKY HAS A NEW POSITION: {0}, {1}", Blinky.Position().x, Blinky.Position().y );
		Rectangle nextBox{ (rec.x + velocityX),(rec.y + velocityY),rec.width,rec.height }, spriteBox = Blinky.boundingBox();

		bool CollisionTwo{ CheckCollisionRecs(nextBox, recTwo) }, CollisionThree{ CheckCollisionRecs(nextBox, recThree) },
			CollisionPowerUp{ CheckCollisionRecs(nextBox,powerBump) }, isCollision{ CollisionTwo && CollisionThree };
		GameSprite* currentSprite = Blinky.SpritePtr();
		if (CollisionTwo) {
			//float deltaX, deltaY, velocityX2;

			PlaySound(soundClips[0]);
			Rectangle collisionBox = GetCollisionRec(nextBox, recTwo);
			if (collisionBox.width != 0.0f) {
				velocityX > 0 ? recTwo.x += velocityX : recTwo.x += velocityX;
			}
			if (collisionBox.height != 0.0f) {
				velocityY > 0 ? recTwo.y += velocityY : recTwo.y += velocityY;
			}

		}
		if (CollisionThree) {
			PlaySound(soundClips[1]);
			Rectangle collisionBox = GetCollisionRec(nextBox, recThree);
			if (collisionBox.width != 0.0f) {
				velocityX > 0 ? recThree.x += velocityX : recThree.x += velocityX;
			}
			if (collisionBox.height != 0.0f) {
				velocityY > 0 ? recThree.y += velocityY : recThree.y += velocityY;

			}
		};

		rec = nextBox;
		Rectangle spriteBounds = GetImageAlphaBorder(LoadImageFromTexture(Blinky.ActiveTexture()), 0.1f);
		
		bool SpriteHitHero{ CheckCollisionRecs(spriteBox,rec) },
			SpriteHitTwo{ CheckCollisionRecs(spriteBox,recTwo) },
			SpriteHitThree{ CheckCollisionRecs(spriteBox,recThree) },
			SpriteGetPower{ CheckCollisionRecs(spriteBox,powerBump) },
			SpriteCollision = { SpriteHitHero || SpriteHitTwo || SpriteHitThree };
		if (SpriteHitTwo) {
			Rectangle collisionBox = GetCollisionRec(spriteBounds, recTwo);
			if (collisionBox.width != 0.0f) {
				velocityX > 0 ? recTwo.x += velocityX : recTwo.x += velocityX;
			}
			if (collisionBox.height != 0.0f) {
				velocityY > 0 ? recTwo.y += velocityY : recTwo.y += velocityY;
			}
		}
		if (SpriteHitThree) {
			Rectangle collisionBox = GetCollisionRec(spriteBounds, recThree);
			if (collisionBox.width != 0.0f) {
				velocityX > 0 ? recThree.x += velocityX : recThree.x += velocityX;
			}
			if (collisionBox.height != 0.0f) {
				velocityY > 0 ? recThree.y += velocityY : recThree.y += velocityY;
			}
		}
		if (SpriteHitHero) {
			Rectangle collisionBox = GetCollisionRec(spriteBounds, rec);
			if (collisionBox.width != 0.0f) {
				velocityX > 0 ? rec.x += velocityX : rec.x += velocityX;
			}
			if (collisionBox.height != 0.0f) {
				velocityY > 0 ? rec.y += velocityY : rec.y += velocityY;
			}
		}
		Blinky.ApplyVelocity();

		Blinky.Travel() ? Blinky.setAnimationState(ANIMATION_STATE::MOVING) : Blinky.setAnimationState(ANIMATION_STATE::IDLE);
		Blinky.animate();
		Blinky.updatePosition();
		//Collision Sound Effects
		Rectangle src = { 0, 0, 50, 50 };          // part of the texture (pixels)
		Rectangle dst = { Blinky.Position().x, Blinky.Position().y, 50, 50 };    // where/how big on screen
		Vector2 origin = { 0, 0 };

		DrawTexturePro( Blinky.ActiveTexture(), src, dst, origin, 0.0f, WHITE);
		if (CollisionPowerUp) { objectVisible[3] = false; powerUpActive = true; PlaySound(soundClips[0]); powerBumpBoost = 25.0f; }
		//Reset Counter and Positions
		if (IsKeyDown(KEY_R)) {
			collisionCounter = 0;
			PlaySound(soundClips[2]);
			rec.x = targetOrigin.x - 50;
			rec.y = targetOrigin.y - 50;
			recTwo.x = recTwoOrigin.x;
			recTwo.y = recTwoOrigin.y;
			recThree.x = recThreeOrigin.x;
			recThree.y = recThreeOrigin.y;
			objectVisible[3] = true;
			powerUpActive = false;
			powerBumpBoost = 0.0f;
			powerBump.x = disX(genX);// Generate random numbers to
			powerBump.y = disY(genY);// randomly place power-up
		}

		//Escape Key to Exit
		if (IsKeyDown(KEY_ESCAPE)) break;

		if (recTwo.x + recTwo.width  > screenWidth || recTwo.x <  0.0f
			|| (recTwo.y + recTwo.height ) >screenHeight || recTwo.y < 0.0f
			|| (recThree.x + recThree.width) > screenWidth || recThree.x < 0.0f
			||( (recThree.y +recThree.height) > screenHeight ) || (recThree.y < 0.0f)){
			rec.x = targetOrigin.x - 50;
			rec.y = targetOrigin.y - 50;
			recTwo.x = recTwoOrigin.x;
			recTwo.y = recTwoOrigin.y;
			recThree.x = recThreeOrigin.x;
			recThree.y = recThreeOrigin.y;
			PlaySound(soundClips[2]);
			collisionCounter = 0;
			objectVisible[3] = true;
			powerUpActive = false;
			powerBumpBoost = 0.0f;
			powerBump.x = disX(genX);// Generate random numbers to
			powerBump.y = disY(genY);// randomly place power-up
		}

		//Drawing Loop
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (objectVisible[0]) DrawRectangle(rec.x, rec.y, rec.width, rec.height, RED);
		if (objectVisible[1]) DrawRectangle(recTwo.x, recTwo.y, recTwo.width, recTwo.height, BLUE);
		if (objectVisible[2]) DrawRectangle(recThree.x, recThree.y, recThree.width, recThree.height, GREEN);
		if (objectVisible[3]) DrawRectangle(powerBump.x, powerBump.y, powerBump.width, powerBump.height, YELLOW);
		DrawTextureRec(testTex, rec, Blinky.Position(), GREEN);
		EndDrawing();
	}

	//Unload and Close
	for (int i = 0; i < 3; i++) {
		UnloadSound(soundClips[i]);
		std::println("soundClips[{0}] UNLOADED SUCCESSFULLY", i);
	}
	CloseWindow();
	return 0;
}