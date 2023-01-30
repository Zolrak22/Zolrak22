#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround (AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main ()
{
    // Window Dimensions
    float windowDimensions[2];
    windowDimensions[0] = 512; //Window width
    windowDimensions[1] = 380; // Window Height

    // Initialize the window
    InitWindow(windowDimensions[0],windowDimensions[1],"Dapper Dasher!");
    
    // Acceleration due to gravity (pixels/s)/s
    const float gravity{42'000.0}, jumpVel{gravity/-2.5};
    int velocity{0}, scarfySpeed{10};
    bool isInAir{};

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    
    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{}; //AnimData for nebulae

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.x = windowDimensions[0] + i * 400; //Vector2 pos x
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8; //Vector2 pos y
        nebulae[i].frame = 0; // int frame
        nebulae[i].updateTime = 1.0/12.0;
        nebulae[i].runningTime = 0.0; //float runningtime
    }

    float finishLine { nebulae[sizeOfNebulae - 1].pos.x};

    // Nebula X velocty (pixels/second)
    int nebVel{-300};

    // Scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // Scarfy AnimData
    AnimData scarfyData{
        {0.0, 0.0, scarfy.width/6, scarfy.height}, // Rectangle rec
        {windowDimensions[0]/2 - scarfyData.rec.width/2, windowDimensions[1] - scarfyData.rec.height}, // Vector2 pos
        0, // int frame
        1.0/12.0 , // float updateTime
        0.0 // float running time
    };

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    // checks if scarfy touched an enemy
    bool collision{};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};

        // Start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        float xNumber = 20;
        bgX -= xNumber * dT;
        mgX -= xNumber*2 * dT;
        fgX -= xNumber*4 * dT;

        //scroll background
        if (bgX <= -background.width*2)
        {
            bgX = 0.0;
        }
        //scroll midground
        if (mgX <= -midground.width*2)
        {
            mgX = 0.0;
        }
        //scroll foreground
        if (fgX <= -foreground.width*2)
        {
            fgX = 0.0;
        }


        // draw the background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2, WHITE);
        Vector2 bg2Pos{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2, WHITE);

        // draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2, WHITE);
        Vector2 mg2Pos{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2, WHITE);

        // draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2, WHITE);
        Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2, WHITE);


        // Ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            // Character is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // character is in the air
            velocity =+ gravity * dT;
            isInAir = true;
        }
        // Check for Jumping
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        // Update the position of each nebula
        for (int i = 0; i < sizeOfNebulae; i++)
        {
        nebulae[i].pos.x += nebVel * dT;
        }

        // update finish line
        finishLine += nebVel * dT;

        // Update Scarfy position
        scarfyData.pos.y += velocity * dT;

        //Updates Scarfy animation frame
        if (IsKeyDown(KEY_D))
        {
         scarfyData.pos.x += scarfySpeed;
        }

        if (IsKeyDown(KEY_A))
        {
         scarfyData.pos.x -= scarfySpeed;
        }

        if (!isInAir)
        {
            //updates scarfy's animation frames
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            //updates nebulae animation frames
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }
        
        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            // lose the game
            DrawText("Game Over!", windowDimensions[0]/4, windowDimensions[1]/2, 40, YELLOW );
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            // win the game
            DrawText("You win!", windowDimensions[0]/4, windowDimensions[1]/2, 40, BLUE );
        }
        else
        {
             //Draws each nebula
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            // Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
                
        // Stop drawing
        EndDrawing();

    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}