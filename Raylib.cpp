// Raylib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Written by: Ruben Michael Borges
// First Created : 1 September 2025
// 
// Description: A simple program to demonstrate basic collision detection, sound effects, and camera movement using a small and experimental game developed in C++ with the Raylib library.
//

#include "raylib.h"
import std;

int main()
{
    //Variables, Constants and Devices
    const int screenWidth{ 800 },
              screenHeight{ 600 };
	//std::random device to set random positions for power-up
    std::random_device randXPos, 
                       randYPos;
    std::mt19937 genX(randXPos()),  
                 genY(randYPos()); // Mersenne Twister generator
    std::uniform_int_distribution<> disX(0, (screenWidth - 25)),
                                    disY(0, (screenHeight - 25)); // Range [0, ScreenWidth-25]
    Vector2 targetOrigin = { screenWidth / 2, screenHeight / 2 },
            recTwoOrigin = { (screenWidth) / 5, (screenHeight / 2) - 50 },
            recThreeOrigin = { (screenWidth / 5) * 3, (screenHeight / 2) - 50 },
            powerBumpOrigin = {disX(genX),disY(genY)};
    int recWidth{ 100 }, recHeight{ 100 }, factor{ 0 }, bumpFactor{ 3 }, powerBumpBoost{ 0 }, collisionCounter{ 0 }, powerUpWidth{ 25 }, powerUpHeight{ 25 };
    Camera2D camera = { Vector2 {0,0},targetOrigin, 0.0f, 1.0f };
    Rectangle rec = { targetOrigin.x - 50, targetOrigin.y - 50 ,recWidth / 2,recHeight / 2 },
        recTwo = { recTwoOrigin.x, recTwoOrigin.y, recWidth / 2, recHeight / 2 },
        recThree = { recThreeOrigin.x, recThreeOrigin.y, recWidth / 2, recHeight / 2 },
        powerBump = { powerBumpOrigin.x, powerBumpOrigin.y, powerUpWidth, powerUpHeight};
	bool powerUpActive{ false };
    bool objectVisible[4]{ true, true, true, true };
    InitWindow(screenWidth, screenHeight, "Raylib basic window");
    SetTargetFPS(60);




    //Setup Basic Audio
    InitAudioDevice();
    std::filesystem::path assetBase = std::filesystem::current_path() / "AudioFX" / "Collision";

    Sound soundClips[3] = {LoadSound("resources/AudioFX/Collision/beer-bottles-82232.mp3"),
                           LoadSound("resources/AudioFX/Collision/plastic-hit-3-34297.mp3"),
                           LoadSound("resources/AudioFX/Reset/a-sudden-appearance-143034.mp3")};

    if (IsSoundValid(soundClips[0]) && IsSoundValid(soundClips[1]) && IsSoundValid(soundClips[2])) std::cout << "Sound clips loaded successfully\n";
    else std::cout << "Sound clips failed to load\n";
    // Initial sound to indicate start of program
    PlaySound(soundClips[2]); 
    while (!WindowShouldClose()) {
        //Check Collision
        bool isCollision = (CheckCollisionRecs(rec, recTwo) || CheckCollisionRecs(rec, recThree));
        if (isCollision) {
            collisionCounter++;
        }
        bool CollisionTwo{ CheckCollisionRecs(rec,recTwo) },
            CollisionThree{ CheckCollisionRecs(rec,recThree) },
            CollisionPowerUp{ CheckCollisionRecs(rec,powerBump) };
        int velocityX{ 0 }, velocityY{ 0 };

        //Poll and Handle Input Events
        PollInputEvents();
        if (IsKeyDown(KEY_RIGHT))  velocityX += 2.0f;
        if (IsKeyDown(KEY_LEFT)) velocityX -= 2.0f;
        if (isCollision)
        {
            velocityX = -velocityX;
            velocityX > 0 ? factor = 1 : factor = -1;
            if (velocityX != 0) rec.x = rec.x + ((rec.width / 3) * factor);
            if (CollisionTwo && velocityX != 0) recTwo.x = recTwo.x + (((recTwo.width / bumpFactor) + powerBumpBoost) * -factor);
            if (CollisionThree && velocityX != 0) recThree.x = recThree.x + (((recThree.width / bumpFactor) + powerBumpBoost) * -factor);
        }
        rec.x += velocityX;

        if (IsKeyDown(KEY_UP)) velocityY -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) velocityY += 2.0f;
        if (isCollision) {
            velocityY = -velocityY;
            velocityY > 0 ? factor = 1 : factor = -1;
            if (velocityY != 0)  rec.y = rec.y + ((rec.height / bumpFactor)* factor);
            if (CollisionTwo && velocityY != 0) recTwo.y = recTwo.y + (((recTwo.height / bumpFactor) + powerBumpBoost) * - factor);
            if (CollisionThree && velocityY != 0) recThree.y = recThree.y + (((recThree.height / bumpFactor) + powerBumpBoost) * -factor);
        }
        rec.y += velocityY;

        //Collision Sound Effects
        if (CollisionTwo) PlaySound(soundClips[0]);
        if (CollisionThree) PlaySound(soundClips[1]);
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
        if (recTwo.x > screenWidth || recTwo.x <  (0 + (recTwo.width / 2)) ||
            recTwo.y >(screenHeight - (recTwo.height / 2)) || recTwo.y < (0 + (recTwo.height / 2)) ||
            recThree.x > screenWidth || recThree.x < (0 + (recThree.width / 2)) ||
            recThree.y >(screenHeight - (recThree.height / 2)) || recThree.y < (0 + (recThree.height / 2))) {
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
        DrawText(std::to_string(collisionCounter).c_str(), 20, 20, 20, BLACK);
        if (objectVisible[0]) DrawRectangle(rec.x, rec.y, rec.width, rec.height, RED);
        if (objectVisible[1]) DrawRectangle(recTwo.x, recTwo.y, recTwo.width, recTwo.height, BLUE);
        if (objectVisible[2]) DrawRectangle(recThree.x, recThree.y, recThree.width, recThree.height, GREEN);
        if (objectVisible[3]) DrawRectangle(powerBump.x, powerBump.y, powerBump.width, powerBump.height, YELLOW);
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

