#include <raylib.h>
#include <stdio.h>
#include <sys/types.h>

#define HEAD 0b10
#define TAIL 0b01
#define WIRE 0b11
#define EMPTY 0b00
#define GRID_LENGTH 80
#define GRID_HEIGHT 80
#define CELL_SIZE 10
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define TITLE "Wireworld"
#define TARGET_FPS 12

typedef struct gameState {
    u_int8_t isEditing: 1;
} GameState;

typedef struct cell {
    u_int8_t state: 2;
} Cell;

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    Cell currentGrid[GRID_LENGTH * GRID_HEIGHT] = {0};
    Cell previousGrid[GRID_LENGTH * GRID_HEIGHT] = {0};

    GameState gameState = {1};

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);

    SetTargetFPS(TARGET_FPS);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (gameState.isEditing == 0) {
            for (u_int8_t y = 1; y < GRID_HEIGHT - 1; ++y) {
                for (u_int8_t x = 1; x < GRID_LENGTH - 1; ++x) {
                    if (previousGrid[y * GRID_LENGTH + x].state == HEAD) {
                        currentGrid[y * GRID_LENGTH + x].state = TAIL;
                    } else if (previousGrid[y * GRID_LENGTH + x].state == TAIL) {
                        currentGrid[y * GRID_LENGTH + x].state = WIRE;
                    } else if (previousGrid[y * GRID_LENGTH + x].state == WIRE) {
                        u_int8_t chargedNeighbours =
                                (previousGrid[y * GRID_LENGTH + x - 1].state == HEAD ? 1 : 0) + // left
                                (previousGrid[y * GRID_LENGTH + x + 1].state == HEAD ? 1 : 0) + // right
                                (previousGrid[(y - 1) * GRID_LENGTH + x].state == HEAD ? 1 : 0) + // top
                                (previousGrid[(y - 1) * GRID_LENGTH + x - 1].state == HEAD ? 1 : 0) + // top left
                                (previousGrid[(y - 1) * GRID_LENGTH + x + 1].state == HEAD ? 1 : 0) + // top right
                                (previousGrid[(y + 1) * GRID_LENGTH + x].state == HEAD ? 1 : 0) + // bot
                                (previousGrid[(y + 1) * GRID_LENGTH + x - 1].state == HEAD ? 1 : 0) + // bot left
                                (previousGrid[(y + 1) * GRID_LENGTH + x + 1].state == HEAD ? 1 : 0); // bot right
                        if(chargedNeighbours > 0 && chargedNeighbours <= 2) {
                            currentGrid[y * GRID_LENGTH + x].state = HEAD;
                        }
                    }
                }
            }
        } else {
            int cellId = ((GetMouseY() / CELL_SIZE) * GRID_LENGTH) + (GetMouseX() / CELL_SIZE);

            if (IsKeyDown(KEY_D)) {
                currentGrid[cellId].state = HEAD;
            } else if (IsKeyDown(KEY_F)) {
                currentGrid[cellId].state = TAIL;
            } else if (IsKeyDown(KEY_G)) {
                currentGrid[cellId].state = WIRE;
            } else if (IsKeyDown(KEY_H)) {
                currentGrid[cellId].state = EMPTY;
            }
        }

        if (IsKeyPressed(KEY_SPACE)) {
            gameState.isEditing = gameState.isEditing == 0 ? 1 : 0;
        }

        for (u_int8_t y = 0; y < GRID_HEIGHT; ++y) {
            for (u_int8_t x = 0; x < GRID_LENGTH; ++x) {
                previousGrid[y * GRID_LENGTH + x] = currentGrid[y * GRID_LENGTH + x];
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        ClearBackground(BLACK);

        for (u_int8_t y = 1; y < GRID_HEIGHT - 1; ++y) {
            for (u_int8_t x = 1; x < GRID_LENGTH - 1; ++x) {
                switch (currentGrid[y * GRID_LENGTH + x].state) {
                    case HEAD:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
                        break;
                    case TAIL:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
                        break;
                    case WIRE:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, YELLOW);
                        break;
                    default:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
                }
            }
        }

        if (gameState.isEditing) {
            DrawText("Editing... Press space to run",20, SCREEN_HEIGHT - 40, 20, RED);
            DrawText("[D] HEAD | [F] TAIL | [G] WIRE | [H] EMPTY",
                     SCREEN_WIDTH - 450,10, 20, RED);
        } else {
            DrawText("Running... Press space to edit", 20, SCREEN_HEIGHT - 40, 20, GREEN);
        }
        DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
