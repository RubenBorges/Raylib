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

void parseDirectory( std::filesystem::path directoryPath, std::vector<std::string>& filenames ) {
	std::filesystem::directory_iterator dir( directoryPath );
	for ( std::filesystem::directory_entry entry : dir ) {
		filenames.push_back( entry.path().string() );
	//	std::println( "Found file: {0}", entry.path().string() );
	}
}

enum class SPRITE : int {
	BLINKY = 0, PINKY = 1, INKY = 2, CLYDE = 3, FUNKY = 4, SUE = 5, DEADPAC = 8, PACMAN = 18
};
enum class SPRITEINDEX : int {
	BLINKY = 0, PINKY = 1, INKY = 2, CLYDE = 3, FUNKY = 4, SUE = 5, PACMAN =6,DEADPAC = 7
};

enum class ANIM : int {
	RIGHT_A = 0, RIGHT_B = 1,
	DOWNA = 2, DOWN_B = 3,
	LEFT_A = 4, LEFT_B = 5,
	UP_A = 6, UP_B = 7,
	BACK_A = 8, BACK_B = 9
};
enum class PACANIM : int {
	RIGHT_A = 0, RIGHT_B = 1, RIGHT_C = 2,
	DOWN_A = 3, DOWN_B = 4, DOWN_C = 5,
	LEFT_A = 6, LEFT_B = 7, LEFT_C = 8,
	UP_A = 9, UP_B = 10, UP_C = 11,
	BACK_A = 12, BACK_B = 13, BACK_C = 14
};

enum class DIR : int {
	NULL_DIR = -1, RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3, BACK = 4
}; 
// DIR : int; DIR * 2 && DIR * 2 + 1 to get animation frames per direction
enum class ANIMATION_STATE : int {
	IDLE = 0, MOVING = 1
};
//false = 0, true = 1; IF cycleAnimation == true (1) THEN  !cycleAnimation = false (0) THEN ANOTHER !cycleAnimation == true (1).. (1>0>1>0>1>0....); 
enum class BOOLEAN_SWITCH : int {
	EAT = -1,
	BASE = 0, 
	CYCLE = 1
};

class GameSprite {
	std::string name{ "" };
	int id{ 0 };
	int FrameCount{ 0 };
	int currentIndex{ 0 };
	std::vector<Texture2D> animationFrameVec{};
	Texture2D activeTexture{};
	std::unique_ptr<Texture2D[]> animationFrames_T{};
	Vec2F size{ 0.0f, 0.0f }, velocity{ 0.0f, 0.0f }, position{ 0.0f, 0.0f }, lastPosition{ 0.0f, 0.0f };
	bool isAnimating{ false }, cycleAnimation{ false }, isEating{ false };
	BOOLEAN_SWITCH animationCycleSwitch{ BOOLEAN_SWITCH::BASE };
	ANIMATION_STATE currentAnimationState{ ANIMATION_STATE::IDLE };
	DIR currentDirection{ DIR::RIGHT }, lastDirection{ DIR::NULL_DIR };
	std::chrono::system_clock::time_point lastFrameTime;

	public:
	
	GameSprite() : id(genID()) { animationFrameVec.reserve(12); }
	GameSprite(const GameSprite&) = delete;
	GameSprite& operator=(const GameSprite&) = delete;
	GameSprite(GameSprite&&) noexcept = default;
	GameSprite& operator=(GameSprite&&) noexcept = default;

	GameSprite( const std::string spriteName, const int _frameCount, Image* frames) : GameSprite()
	{
		name = spriteName;
		animationFrameVec.clear();
		animationFrameVec.reserve(_frameCount);
		for ( int i = 0; i < _frameCount; ++i ) animationFrameVec.emplace_back( LoadTextureFromImage( frames[i] ) );
		storeAnimations( animationFrameVec );
	}

	GameSprite( const std::string spriteName, Image frame ) : GameSprite()
	{
		name = spriteName;
		storeAnimations( LoadTextureFromImage( frame ) );
		
	}
	GameSprite( const std::string spriteName, const int _frameCount, Texture* frames ) : GameSprite()
	{
		name = spriteName;
		animationFrameVec.clear();
		animationFrameVec.reserve(_frameCount);
		for ( int i = 0; i < _frameCount; ++i ) animationFrameVec.emplace_back( frames[i]);
		storeAnimations( animationFrameVec );
	}
	GameSprite( const std::string spriteName, std::vector<Texture2D> frameVec ) : GameSprite()
	{
		name = spriteName;
		storeAnimations( std::move(frameVec) );
	}
	~GameSprite() = default;
	
	/*
	* THIS IS WHERE UTILITY FUNCTIONS BEGIN
	*/
	

	//{ NULL_DIR = -1, RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3, BACK = 4 };
	void setDirection( const DIR dir ) {//////////////////
		currentDirection = dir;
		return;
	}
	DIR Direction() const {
		return currentDirection;
	} 
	//{ NULL_DIR = -1, RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3, BACK = 4 };
	void newAnimations( Image* imagearray, int _frameCount, Rectangle source ) {
		std::vector<Texture2D> textureArray;
		Image subimage;
		for ( int i = 0; i < _frameCount; ++i ) {
			textureArray.emplace_back( LoadTextureFromImage(
				subimage = ImageFromImage( imagearray[i], source ) ) );
		}
		storeAnimations( textureArray );
		return;
	}
	void newAnimations( std::vector<Texture2D>& texturearray ) {
		FrameCount = texturearray.size();
		storeAnimations( texturearray );
		return;
	}
	void newAnimations( std::vector<Image>& ImageVec) {
		animationFrameVec.clear();
		for ( Image I : ImageVec ) {
			animationFrameVec.emplace_back( LoadTextureFromImage( I ) );
		}
		FrameCount = animationFrameVec.size();
		storeAnimations( animationFrameVec );
		return;
	}
	void newAnimations( Texture2D texture) {
		animationFrameVec.clear();
		storeAnimations( texture );
		return;
	}
	void newAnimations( Texture2D* texturearray, const int _frameCount ) {
		animationFrameVec.clear();
		for ( size_t i = 0; i < _frameCount; ++i ) {
			animationFrameVec.emplace_back( texturearray[i] );
		}
		storeAnimations( animationFrameVec );
		return;
	}

	void animate() {////////////
		if ( currentAnimationState == ANIMATION_STATE::MOVING ) {
			if ( name == "pacman" ) animatePacmanSprite();
			else {animateSprite();}
		}
	}
	void animatePacmanSprite() {
		auto now = std::chrono::system_clock::now();
		(eating() == true)? currentIndex = (static_cast<int>(currentDirection) * 3) + static_cast<int>( BOOLEAN_SWITCH::EAT) :
		currentIndex = (static_cast<int>(currentDirection) * 3) + (cycleSwitch());
		if ( currentDirection != lastDirection ) {
			setDirection( currentDirection );
			setActiveTexture( animationFrameVec[currentIndex] );
		}
		return;
	}
	void animateSprite() {//////////
		animationCycleSwitch == BOOLEAN_SWITCH::BASE ? animationCycleSwitch = BOOLEAN_SWITCH::CYCLE : animationCycleSwitch = BOOLEAN_SWITCH::BASE;
		currentIndex = (static_cast<int>(currentDirection) * 2) + (static_cast<int>(animationCycleSwitch));
		setActiveTexture( animationFrameVec[currentIndex]);
	}

	void setAnimationState( const ANIMATION_STATE state ) {//////
		currentAnimationState = state;
	}
	int currentIndexFrame() const {
		return currentIndex;
	}
	int animationState() const {
		return static_cast<int>(currentAnimationState);
	}
	Texture2D& ActiveTexture() {
		return activeTexture;
	}//////////


	// Provide access to animation frame data and size;
	//{OUT} dataPtr : pointer to hold address of animation frames array
	//{OUT} dataSize : integer reference to hold number of frames in animation
	void Data( void* dataPtr, int& dataSize ) {
		dataPtr = static_cast<Texture2D*>(dataPtr);
		dataPtr = &animationFrames_T;
		dataSize = FrameCount; 
		return;
	}

	//Returns the animation frames as a std::vector<Texture2D>
	std::vector<Texture2D> TextureArray() {	return animationFrameVec;}
	void setVelocity(float x, float y) {velocity.x = x; velocity.y = y;}//////////////////////
	void setVelocity(Vector2 other) {velocity.x = other.x; velocity.y = other.y;}/////////////////
	void resetVelocity() {velocity.x = 0.0f; velocity.y = 0.0f;}//////////////////
	Vec2F Velocity() {return velocity;}
	void ApplyVelocity() {////////////
		position += velocity;
		return;
	}
	void setPosition( Vector2 _position ) {	position = _position;}//////
	bool Travel() {	return (lastPosition != position);}/////

	void updatePosition() {/////////////
		lastPosition = position;
		position.x += velocity.x;
		position.y += velocity.y;
	};
	Vector2 Position() {return Vector2{ position.x, position.y };}///////
	int ID() {	return id;}
	int setID( int _id ) {
		if ( id == _id ) {
			return -1;
		}
		id = _id;
		return 0;
	}
	Rectangle boundingBox() {return Rectangle{ position.x, position.y, size.x, size.y };}/////////
	Vec2F Location() {return (position + size);}
	Rectangle FrameSize( int frame = -1) {
		if(frame < 0) frame = currentIndex;
		if (frame < 0 || frame >= static_cast<int>(animationFrameVec.size())) return Rectangle{ 0,0,0,0 };
		return Rectangle{ 0.0f, 0.0f, (float)animationFrameVec[frame].width, (float)animationFrameVec[frame].height };
	}

	private:
	void setSize( Texture2D texture ) {	size = Vector2{ static_cast<float>(texture.width), static_cast<float>(texture.height) };}
	void storeAnimations( Texture2D texture ) {
		animationFrameVec.clear();
		animationFrameVec.emplace_back(texture);
		FrameCount = 1;
		setActiveTexture( texture );
	}
	void storeAnimations( std::vector<Texture2D> texturearray ) {
		animationFrameVec = std::move( texturearray );
		FrameCount = static_cast<int>(animationFrameVec.size());
		ActivateTextureFromVec();
	}

	void ActivateTextureFromVec(int frameIndex = 0) {
		if (animationFrameVec.size() >= frameIndex) setActiveTexture( animationFrameVec[frameIndex] );
	}
	void setActiveTexture( const Texture2D _texture ) {activeTexture = _texture; setSize( _texture );}
	int genID() {
		std::random_device rando;
		std::mt19937 genRand( rando() );// Mersenne Twister generator
		std::uniform_int_distribution<> dis( 0, (99999999) ); // Range [0, ScreenWidth-25]
		return dis( genRand );
	}
	int cycleSwitch(  ) {
		switch ( animationCycleSwitch ) {
		case BOOLEAN_SWITCH::BASE:
			if (shouldTick()) animationCycleSwitch = BOOLEAN_SWITCH::CYCLE;
			break;
		case BOOLEAN_SWITCH::CYCLE:
			if ( shouldTick() ) animationCycleSwitch = BOOLEAN_SWITCH::BASE;
			break;
		case BOOLEAN_SWITCH::EAT:
			animationCycleSwitch = BOOLEAN_SWITCH::BASE;
			break;
		}
		return static_cast<int>(animationCycleSwitch);
	}
	bool eating(){
		return isEating; }
	
	bool shouldTick() {
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		if ((now - lastFrameTime) >= std::chrono::milliseconds(100 ) ) {
			lastFrameTime = now;
			return true;
		}
	return false;
	}
};


int main()
{
	std::filesystem::directory_entry levelRoot = std::filesystem::directory_entry( "C:\\Projects\\Raylib\\assets\\pacman\\CEDX\\course_packs\\Skin B\\Original Rip\\" );
	std::filesystem::directory_entry levelMaskRoot = std::filesystem::directory_entry( "C:\\Projects\\Raylib\\assets\\pacman\\CEDX\\course_packs\\Skin B\\Skin B\\" );
	Image image = LoadImage( "C:\\Projects\\Raylib\\assets\\pacman\\CEDX\\Character Skins - Smooth Neon.png" );

	//will hold textures of current ghost animation frames
	Texture blinky, pinky, inky, clyde, funky, sue;
	std::vector<std::filesystem::directory_entry> pathList = { levelMaskRoot , levelRoot};
	std::vector<std::vector<std::string>> filenames;
	filenames.resize( pathList.size() );
	for ( int i = 0; i < pathList.size(); ++i ) {
		parseDirectory( pathList[i], filenames[i] );
	}


	int const spriteWidth{ 50 }, const spriteHeight{ 50 }, const spritesprites{ 6 }, const spriteAnimeFrames{ 10 };
	std::vector<std::vector<Image>> spriteImgSheet( spritesprites );
	std::vector<std::vector<Texture2D>> spriteTextSheet( spritesprites );
	std::vector<Texture2D> levelTextSheet, levelMaskTextSheet;
	for ( int i = 0; i < spritesprites; ++i ) {
		spriteImgSheet[i].resize( spriteAnimeFrames );
		spriteTextSheet[i].resize( spriteAnimeFrames );
	}
	int const glyphWidth{ 75 }, const glyphHeight{ 125 }, const glyphCount{ 12 }, const glyphSets{ 2 /*columns*/ * 4 /*rows*/ };
	int const levelOuterRectWidth{ 520 }, const levelInnerRectWidth{ 240 }, const levelOuterRectHeight{ 720 }, const levelCount{ 34 };
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

	for ( int i = 0; i < filenames[0].size(); ++i ) {
		levelTextSheet.push_back( LoadTextureFromImage(  LoadImage( filenames[0][i].c_str() )) );
	}
	for ( int i = 0; i < filenames[1].size(); ++i ) {
		levelMaskTextSheet.push_back( LoadTextureFromImage( LoadImage( filenames[1][i].c_str() ) ) );
	}


	for ( int i = 0; i < spritesprites; i++ ) {
		for ( int j = 0; j < spriteAnimeFrames; j++ ) {
			spriteTextSheet[i][j] = LoadTextureFromImage(
				spriteImgSheet[i][j] = ImageFromImage( image, Rectangle{ static_cast<float>( i * spriteWidth ), static_cast<float>( j * spriteHeight ), static_cast<float>( spriteWidth ), static_cast<float>( spriteHeight ) } )
			);
		}
	}
	
	spriteTextSheet.push_back( std::vector<Texture2D>() ); // Add empty vector for PACMAN
	for ( int i = 0; i < (spriteAnimeFrames+2); ++i ) {
		spriteTextSheet[static_cast<int>( SPRITEINDEX::PACMAN )].push_back(LoadTextureFromImage(ImageFromImage(image, Rectangle{static_cast<float>( static_cast<int>( SPRITE::PACMAN ) * spriteWidth), (i*static_cast<float>( spriteWidth )), static_cast<float>(spriteWidth), static_cast<float>(spriteHeight)})));
	}
//	GameSprite pacman("Pacman", spriteAnimeFrames, spriteTextSheet[static_cast<int>(SPRITE::PACMAN)].data());
	std::vector<GameSprite> sprites;
	sprites.emplace_back("Blinky", spriteAnimeFrames, spriteTextSheet[static_cast<int>(SPRITEINDEX::BLINKY)].data());
	sprites.emplace_back("Pinky", spriteAnimeFrames, spriteTextSheet[static_cast<int>(SPRITEINDEX::PINKY)].data());
	sprites.emplace_back("Inky", spriteAnimeFrames, spriteTextSheet[static_cast<int>(SPRITEINDEX::INKY)].data());
	sprites.emplace_back("Clyde", spriteAnimeFrames, spriteTextSheet[static_cast<int>(SPRITEINDEX::CLYDE)].data());
	sprites.emplace_back("Funky", spriteAnimeFrames, spriteTextSheet[static_cast<int>(SPRITEINDEX::FUNKY)].data());
	sprites.emplace_back("Sue", spriteAnimeFrames, spriteTextSheet[static_cast<int>(SPRITEINDEX::SUE)].data());
	sprites.emplace_back( "pacman", (spriteAnimeFrames+2), spriteTextSheet[static_cast<int>(SPRITEINDEX::PACMAN)].data() );

	float frameTime = 0.1f; // Time per frame in seconds
	//Rectangle sourceRect = { spriteWidth * currentFrame, 0, spriteWidth, spriteHeight};
	Rectangle sourceRect = { 0, 0, spriteWidth, spriteHeight };
	Rectangle destRect = { 200, 200, spriteWidth * 2, spriteHeight * 2 };

	//Replace initializer_list construction (requires copying GameSprite) with emplace_back

	Texture2D testTex = LoadTextureFromImage( image );
	//Setup Basic Audio
	InitAudioDevice();

	std::filesystem::path assetBase = std::filesystem::current_path() / "AudioFX" / "Collision";

	int m = image.height;
	Sound soundClips[3] = { LoadSound( "resources/AudioFX/Collision/beer-bottles-82232.mp3" ),
						   LoadSound( "resources/AudioFX/Collision/plastic-hit-3-34297.mp3" ),
						   LoadSound( "resources/AudioFX/Reset/a-sudden-appearance-143034.mp3" ) };

	if ( IsSoundValid( soundClips[0] ) && IsSoundValid( soundClips[1] ) && IsSoundValid( soundClips[2] ) ) std::cout << "Sound clips loaded successfully\n";
	else std::cout << "Sound clips failed to load\n";
	// Initial sound to indicate start of program
	PlaySound( soundClips[2] );
	while ( !WindowShouldClose() ) {
		float velocityX{ 0.0f }, velocityY{ 0.0f };
		sprites[static_cast<int>(SPRITEINDEX::PACMAN)].resetVelocity();

		//Poll and Handle Input Events
		PollInputEvents();
		if ( IsKeyDown( KEY_RIGHT ) ) {
			velocityX += 2.0f;
			sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setDirection( DIR::RIGHT );
			sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setVelocity( Vector2{ 2.0f , 0.0f } );

		}
		if ( IsKeyDown( KEY_LEFT ) ) {
			velocityX -= 2.0f;
			sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setDirection( DIR::LEFT );
			sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setVelocity( Vector2{ -2.0f , 0.0f } );
		}

		if ( IsKeyDown( KEY_UP ) ) {
			velocityY -= 2.0f;
			sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setDirection( DIR::UP );
			sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setVelocity( Vector2{ 0.0f , -2.0f } );
		}
		if ( IsKeyDown( KEY_DOWN ) ) {
			velocityY += 2.0f;
			sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setDirection( DIR::DOWN );
			sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setVelocity( Vector2{ 0.0f , 2.0f } );
		}
		sprites[static_cast<int>(SPRITEINDEX::PACMAN)].ApplyVelocity();
		std::println( "BLINKY HAS A NEW POSITION: {0}, {1}", sprites[static_cast<int>(SPRITEINDEX::PACMAN)].Position().x, sprites[static_cast<int>(SPRITEINDEX::PACMAN)].Position().y );
		Rectangle nextBox{ (rec.x + velocityX),(rec.y + velocityY),rec.width,rec.height }, spriteBox = sprites[static_cast<int>(SPRITEINDEX::PACMAN)].boundingBox();
		bool CollisionTwo{ CheckCollisionRecs( nextBox, recTwo ) }, CollisionThree{ CheckCollisionRecs( nextBox, recThree ) },
			CollisionPowerUp{ CheckCollisionRecs( nextBox,powerBump ) }, isCollision{ CollisionTwo && CollisionThree };
		
		if ( CollisionTwo ) {
			//float deltaX, deltaY, velocityX2;

			PlaySound( soundClips[0] );
			Rectangle collisionBox = GetCollisionRec( nextBox, recTwo );
			if ( collisionBox.width != 0.0f ) {
				velocityX > 0 ? recTwo.x += velocityX : recTwo.x += velocityX;
			}
			if ( collisionBox.height != 0.0f ) {
				velocityY > 0 ? recTwo.y += velocityY : recTwo.y += velocityY;
			}

		}
		if ( CollisionThree ) {
			PlaySound( soundClips[1] );
			Rectangle collisionBox = GetCollisionRec( nextBox, recThree );
			if ( collisionBox.width != 0.0f ) {
				velocityX > 0 ? recThree.x += velocityX : recThree.x += velocityX;
			}
			if ( collisionBox.height != 0.0f ) {
				velocityY > 0 ? recThree.y += velocityY : recThree.y += velocityY;

			}
		};

		rec = nextBox;
		Rectangle spriteBounds = GetImageAlphaBorder( LoadImageFromTexture( sprites[static_cast<int>(SPRITEINDEX::PACMAN)].ActiveTexture()), 0.1f);

		bool SpriteHitHero{ CheckCollisionRecs( spriteBox,rec ) },
			SpriteHitTwo{ CheckCollisionRecs( spriteBox,recTwo ) },
			SpriteHitThree{ CheckCollisionRecs( spriteBox,recThree ) },
			SpriteGetPower{ CheckCollisionRecs( spriteBox,powerBump ) },
			SpriteCollision = { SpriteHitHero || SpriteHitTwo || SpriteHitThree };
		if ( SpriteHitTwo ) {
			Rectangle collisionBox = GetCollisionRec( spriteBounds, recTwo );
			if ( collisionBox.width != 0.0f ) {
				velocityX > 0 ? recTwo.x += velocityX : recTwo.x += velocityX;
			}
			if ( collisionBox.height != 0.0f ) {
				velocityY > 0 ? recTwo.y += velocityY : recTwo.y += velocityY;
			}
		}
		if ( SpriteHitThree ) {
			Rectangle collisionBox = GetCollisionRec( spriteBounds, recThree );
			if ( collisionBox.width != 0.0f ) {
				velocityX > 0 ? recThree.x += velocityX : recThree.x += velocityX;
			}
			if ( collisionBox.height != 0.0f ) {
				velocityY > 0 ? recThree.y += velocityY : recThree.y += velocityY;
			}
		}
		if ( SpriteHitHero ) {
			Rectangle collisionBox = GetCollisionRec( spriteBounds, rec );
			if ( collisionBox.width != 0.0f ) {
				velocityX > 0 ? rec.x += velocityX : rec.x += velocityX;
			}
			if ( collisionBox.height != 0.0f ) {
				velocityY > 0 ? rec.y += velocityY : rec.y += velocityY;
			}
		}
		sprites[static_cast<int>(SPRITEINDEX::PACMAN)].ApplyVelocity();

		sprites[static_cast<int>(SPRITEINDEX::PACMAN)].Travel() ? sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setAnimationState( ANIMATION_STATE::MOVING ) : sprites[static_cast<int>(SPRITEINDEX::PACMAN)].setAnimationState( ANIMATION_STATE::IDLE );
		sprites[static_cast<int>(SPRITEINDEX::PACMAN)].animate();
		sprites[static_cast<int>(SPRITEINDEX::PACMAN)].updatePosition();
		//Collision Sound Effects
		Rectangle src = { 0, 0, 50, 50 };          // part of the texture (pixels)
		Rectangle dst = { sprites[static_cast<int>(SPRITEINDEX::PACMAN)].Position().x, sprites[static_cast<int>(SPRITEINDEX::PACMAN)].Position().y, 50, 50 };    // where/how big on screen
		Vector2 origin = { 0, 0 };

		
		if ( CollisionPowerUp ) {
			objectVisible[3] = false; powerUpActive = true; PlaySound( soundClips[0] ); powerBumpBoost = 25.0f;
		}
	//Reset Counter and Positions
		if ( IsKeyDown( KEY_R ) ) {
			collisionCounter = 0;
			PlaySound( soundClips[2] );
			rec.x = targetOrigin.x - 50;
			rec.y = targetOrigin.y - 50;
			recTwo.x = recTwoOrigin.x;
			recTwo.y = recTwoOrigin.y;
			recThree.x = recThreeOrigin.x;
			recThree.y = recThreeOrigin.y;
			objectVisible[3] = true;
			powerUpActive = false;
			powerBumpBoost = 0.0f;
			powerBump.x = disX( genX );// Generate random numbers to
			powerBump.y = disY( genY );// randomly place power-up
		}

		//Escape Key to Exit
		if ( IsKeyDown( KEY_ESCAPE ) ) break;

		if ( recTwo.x + recTwo.width > screenWidth || recTwo.x <  0.0f
			 || (recTwo.y + recTwo.height) >screenHeight || recTwo.y < 0.0f
			 || (recThree.x + recThree.width) > screenWidth || recThree.x < 0.0f
			 || ((recThree.y + recThree.height) > screenHeight) || (recThree.y < 0.0f) ) {
			rec.x = targetOrigin.x - 50;
			rec.y = targetOrigin.y - 50;
			recTwo.x = recTwoOrigin.x;
			recTwo.y = recTwoOrigin.y;
			recThree.x = recThreeOrigin.x;
			recThree.y = recThreeOrigin.y;
			PlaySound( soundClips[2] );
			collisionCounter = 0;
			objectVisible[3] = true;
			powerUpActive = false;
			powerBumpBoost = 0.0f;
			powerBump.x = disX( genX );// Generate random numbers to
			powerBump.y = disY( genY );// randomly place power-up
		}

		//Drawing Loop
		BeginDrawing();
		ClearBackground( RAYWHITE );
		DrawTexturePro( sprites[static_cast<int>(SPRITEINDEX::PACMAN)].ActiveTexture(), src, dst, origin, 0.0f, YELLOW );
		if ( objectVisible[0] ) DrawRectangle( rec.x, rec.y, rec.width, rec.height, RED );
		if ( objectVisible[1] ) DrawRectangle( recTwo.x, recTwo.y, recTwo.width, recTwo.height, BLUE );
		if ( objectVisible[2] ) DrawRectangle( recThree.x, recThree.y, recThree.width, recThree.height, GREEN );
		if ( objectVisible[3] ) DrawRectangle( powerBump.x, powerBump.y, powerBump.width, powerBump.height, YELLOW );
		//DrawTextureRec( testTex, rec, sprites[2].Position(),WHITE);
		EndDrawing();
	}

	//Unload and Close
	for ( int i = 0; i < 3; i++ ) {
		UnloadSound( soundClips[i] );
		std::println( "soundClips[{0}] UNLOADED SUCCESSFULLY", i );
	}
	CloseWindow();
	return 0;
}