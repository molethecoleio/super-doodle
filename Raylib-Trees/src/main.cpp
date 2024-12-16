//from dust we are, to dust we return. kleinebekelc@gmail.com
#include <raylib.h> // Include Raylib library for graphics
#include <iostream> // Include iostream for potential debugging or future use
#include <math.h>

// Constants for screen dimensions
const int screenWidth = 1280, screenHeight = 800;

// Global Variables
Camera cameraPlayer1 = { 0 }; // Camera struct for player 1 (position, target, etc.)
RenderTexture screenPlayer1;  // A texture that allows us to render the 3D view off-screen
Rectangle ScreenRect;         // Rectangle for the screen texture to fix orientation

// Function Declarations
void InitGameplayScreen(void); // Initializes the game window and camera
void UpdateGameplay(void);     // Main game loop where updates and rendering happen

void InitGameplayScreen(void)
{
    // Initialize window
    InitWindow(screenWidth, screenHeight, "Raylib TREES Example; Init(C01E)");
    SetTargetFPS(60); // Set game to run at 60 FPS

    // Lock and hide mouse cursor
    DisableCursor();

    // Initialize camera
    cameraPlayer1.position = (Vector3){ 0.0f, 1.0f, -3.0f }; // Camera position
    cameraPlayer1.target = (Vector3){ 0.0f, 1.0f, 0.0f };    // Camera looking at
    cameraPlayer1.up = (Vector3){ 0.0f, 1.0f, 0.0f };       // Camera up direction
    cameraPlayer1.fovy = 45.0f;                             // Field of view
    cameraPlayer1.projection = CAMERA_PERSPECTIVE;          // Perspective camera mode

    // Create render texture 
    screenPlayer1 = LoadRenderTexture(screenWidth, screenHeight);
    ScreenRect = (Rectangle){ 0.0f, 0.0f, (float)screenWidth, -(float)screenHeight }; // Flipped vertically
}

//the raylib functiuon wasnt working so i just thew this in here
Vector3 Vector3NormalizeNew(Vector3 v)
{
    Vector3 result = v;

    float length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length != 0.0f)
    {
        float ilength = 1.0f/length;

        result.x *= ilength;
        result.y *= ilength;
        result.z *= ilength;
    }

    return result;
}


float mouseSensitivity = 0.1f; // Sensitivity for mouse movement
float yaw = 0.0f;              // Horizontal angle (left/right)
float pitch = 0.0f;            // Vertical angle (up/down)

// Function to update and render the game
void UpdateGameplay(void)
{
    int gridCount = 5;         // Number of cubes in the grid
    float gridSpacing = 4.0f;  // Distance between cubes

    Vector2 mouseDelta;        // To store mouse movement
    Vector3 forwardDirection;  // Direction the camera is looking at

    while (!WindowShouldClose())
    {
        // --- Mouse Input ---
        mouseDelta = GetMouseDelta(); // Get mouse movement

        // Update yaw (left/right) and pitch (up/down)
        yaw += mouseDelta.x * mouseSensitivity;    // Horizontal rotation
        pitch -= mouseDelta.y * mouseSensitivity;  // Vertical rotation

        // Clamp the pitch to avoid flipping the camera upside down
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        // Calculate forward direction from yaw and pitch
        forwardDirection = (Vector3){
            cosf(yaw * DEG2RAD) * cosf(pitch * DEG2RAD),
            sinf(pitch * DEG2RAD),
            sinf(yaw * DEG2RAD) * cosf(pitch * DEG2RAD)
        };

        // --- Movement Controls (WASD) ---
        float offsetThisFrame = 10.0f * GetFrameTime();

        // Strafe vector (perpendicular to forward on the XZ plane)
        Vector3 strafeDirection = (Vector3){
            forwardDirection.z,
            0.0f,
            -forwardDirection.x
        };

        // Normalize strafe direction
        strafeDirection = Vector3NormalizeNew(strafeDirection);

        if (IsKeyDown(KEY_W)) // Move forward
        {
            cameraPlayer1.position.x += forwardDirection.x * offsetThisFrame;
            cameraPlayer1.position.y += forwardDirection.y * offsetThisFrame;
            cameraPlayer1.position.z += forwardDirection.z * offsetThisFrame;
        }
        if (IsKeyDown(KEY_S)) // Move backward
        {
            cameraPlayer1.position.x -= forwardDirection.x * offsetThisFrame;
            cameraPlayer1.position.y -= forwardDirection.y * offsetThisFrame;
            cameraPlayer1.position.z -= forwardDirection.z * offsetThisFrame;
        }
        if (IsKeyDown(KEY_A)) // Strafe left
        {
            cameraPlayer1.position.x += strafeDirection.x * offsetThisFrame;
            cameraPlayer1.position.z += strafeDirection.z * offsetThisFrame;
        }
        if (IsKeyDown(KEY_D)) // Strafe right
        {
            cameraPlayer1.position.x -= strafeDirection.x * offsetThisFrame;
            cameraPlayer1.position.z -= strafeDirection.z * offsetThisFrame;
        }

        if (IsKeyDown(KEY_UP)) // UP
        {
            cameraPlayer1.position.y += offsetThisFrame;
        }
        if (IsKeyDown(KEY_DOWN)) // Down
        {
            cameraPlayer1.position.y -= offsetThisFrame;
        }

        // Update camera target for rendering (position + forward direction)
        cameraPlayer1.target.x = cameraPlayer1.position.x + forwardDirection.x;
        cameraPlayer1.target.y = cameraPlayer1.position.y + forwardDirection.y;
        cameraPlayer1.target.z = cameraPlayer1.position.z + forwardDirection.z;

        // --- Render ---
        BeginTextureMode(screenPlayer1);
            ClearBackground(SKYBLUE);
            BeginMode3D(cameraPlayer1);

                // Draw a simple plane and grid of cubes
                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 50.0f, 50.0f }, BEIGE);
                for (float x = -gridCount * gridSpacing; x <= gridCount * gridSpacing; x += gridSpacing)
                {
                    for (float z = -gridCount * gridSpacing; z <= gridCount * gridSpacing; z += gridSpacing)
                    {
                        DrawCube((Vector3){ x, 1.5f, z }, 1.0f, 1.0f, 1.0f, LIME);
                        DrawCube((Vector3){ x, 0.5f, z }, 0.25f, 1.0f, 0.25f, BROWN);
                    }
                }

            EndMode3D();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextureRec(screenPlayer1.texture, ScreenRect, (Vector2){ 0, 0 }, WHITE);
        EndDrawing();
    }

    // Cleanup
    UnloadRenderTexture(screenPlayer1);
    CloseWindow();
}


// Main function: program entry point
int main()
{
    InitGameplayScreen(); // Initialize the window, camera, and texture
    UpdateGameplay();     // Enter the main game loop
    return 0;             // Exit the program
}
