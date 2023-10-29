#include <raylib.h>

int main()
{
	const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "RayPong"); //window heigh, width and name
    InitAudioDevice();
    SetTargetFPS(60); //Set game to run at 60 fps    
    SetWindowIcon(LoadImage(SPRITE_PATH"pong_icon.png"));

    //texture load
    Texture2D playerLeft = LoadTexture(SPRITE_PATH"playerLeft.png");
    Texture2D playerRight = LoadTexture(SPRITE_PATH"playerRight.png");
    Texture2D ball = LoadTexture(SPRITE_PATH"ball.png");
    Texture2D border = LoadTexture(SPRITE_PATH"border.png");
    Texture background = LoadTexture(SPRITE_PATH"background.png");
	
    //sound load
    Sound playerCollisionSound = LoadSound(SOUND_PATH"player.wav");
    Sound borderCollisionSound = LoadSound(SOUND_PATH"border.wav");
    Sound winningSound = LoadSound(SOUND_PATH"win.wav");
    Sound countSound = LoadSound(SOUND_PATH"count.wav");
    Sound countEndSound = LoadSound(SOUND_PATH"countend.wav");
    Music gameMusic = LoadMusicStream(SOUND_PATH"game-music.mp3");
    PlayMusicStream(gameMusic);

    //position and variable decleration
    Rectangle screen = {0,0,800,450};
    Vector2 playerLeftPosition = {25.0f, (float)screenHeight/2 - 50};
    Vector2 playerRightPosition = {760.0f, (float)screenHeight/2 - 50};
    Vector2 ballPosition = {(float)screenWidth/2-10, (float)screenHeight/2-10};
    Vector2 ballMove = {5, 0};
    int secondCount = 239;
    int playerRightPoint = 0, playerLeftPoint = 0;

    //game loop
    while (!WindowShouldClose())
    {
        //music loop
        UpdateMusicStream(gameMusic);

    	//left player movement
        if(playerLeftPosition.y+100 < 450)
    	   if(IsKeyDown(KEY_S))
    		  playerLeftPosition.y += 5.0f;

        if(playerLeftPosition.y > 0)
    	   if(IsKeyDown(KEY_W))
    		  playerLeftPosition.y -= 5.0f;
    	
        //right player movement
    	if(playerRightPosition.y+100 < 450)
            if(IsKeyDown(KEY_DOWN))
    		  playerRightPosition.y += 5.0f;
    	
        if(playerRightPosition.y > 0)
            if(IsKeyDown(KEY_UP))
    		  playerRightPosition.y -= 5.0f;
    	
    	//ball movement
        secondCount--;
        if(secondCount<=0)
        {
            ballPosition.x += ballMove.x;
            ballPosition.y += ballMove.y;
        }

        //border collision
        if(ballPosition.y <= 0 || ballPosition.y+20 >= 450)
        { 
            ballMove.y = -ballMove.y;
            PlaySoundMulti(borderCollisionSound);
        }

        //player right collision
        if(ballPosition.x == 740.0f)
            if(ballPosition.y+20 > playerRightPosition.y && ballPosition.y < playerRightPosition.y+100)
            {
                PlaySoundMulti(playerCollisionSound);
                ballMove.x = -ballMove.x;
                if(ballMove.y < 6.0f)
                {
                    if(IsKeyDown(KEY_DOWN))
                        ballMove.y += 2.0f;
                    if(IsKeyDown(KEY_UP))
                        ballMove.y -= 2.0f;                
                }
            }

        //player left collision
        if (ballPosition.x == 40.0f)
            if (ballPosition.y+20 > playerLeftPosition.y && ballPosition.y < playerLeftPosition.y+100)
            {
                PlaySoundMulti(playerCollisionSound);
                ballMove.x = -ballMove.x;
                if(ballMove.y < 6.0f)
                {
                    if(IsKeyDown(KEY_S))
                        ballMove.y += 2.0f;
                    if(IsKeyDown(KEY_W))
                        ballMove.y -= 2.0f;
                }
            }

        //defeat
        if(ballPosition.x > 820 || ballPosition.x < -20)
        {
            PlaySoundMulti(winningSound);
            if(ballPosition.x > 820)
                playerRightPoint++;
            if(ballPosition.x < -20)
                playerLeftPoint++;
            ballPosition.x = screenWidth/2-10;
            ballPosition.y = screenHeight/2-10;
            ballMove.x = -ballMove.x;
            ballMove.y = 0.0f;
            secondCount = 239;
            playerLeftPosition.x = 25.0f;
            playerLeftPosition.y = screenHeight/2 - 50;
            playerRightPosition.x = 760.0f;
            playerRightPosition.y = screenHeight/2 - 50;
        }

        //count sound
        if (secondCount >= 0)
        {
            if (secondCount == 238)
                PlaySoundMulti(countSound);
            if (secondCount == 179)
                PlaySoundMulti(countSound);
            if (secondCount == 119)
                PlaySoundMulti(countSound);
            if (secondCount == 59)
                PlaySoundMulti(countEndSound);
        }

        //-Draw-Start----------------------------------------------------------------------
        BeginDrawing();

        DrawTexturePro(background, screen, screen, (Vector2){0.0f,0.0f}, 0, WHITE);
        DrawTexture(border, screenWidth/2-5, 0, WHITE);
        DrawTexture(playerLeft, playerLeftPosition.x, playerLeftPosition.y, WHITE);
        DrawTexture(playerRight, playerRightPosition.x, playerRightPosition.y, WHITE);
        DrawTexture(ball, ballPosition.x, ballPosition.y, WHITE);
        DrawText(TextFormat("%d", playerRightPoint), 330, 20, 50, LIGHTGRAY);
        DrawText(TextFormat("%d", playerLeftPoint), 445, 20, 50, LIGHTGRAY);
        
        if(secondCount >= 60 && secondCount < 120)
            DrawText(TextFormat("%d", (secondCount/3*5)/100), 385, 180, 100, LIGHTGRAY);

        else if (secondCount > 0)
            DrawText(TextFormat("%d", (secondCount/3*5)/100), 375, 180, 100, LIGHTGRAY);

        EndDrawing();
        //-Draw-End------------------------------------------------------------------------
    }
    StopMusicStream(gameMusic);
    CloseAudioDevice();
    CloseWindow();
}
