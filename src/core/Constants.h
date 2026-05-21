#pragma once

constexpr int GRID_COLS  = 30;
constexpr int GRID_ROWS  = 30;
constexpr int CELL_SIZE  = 20;

constexpr int GAME_W     = GRID_COLS * CELL_SIZE;
constexpr int PANEL_W    = 220;
constexpr int WINDOW_W   = GAME_W + PANEL_W;
constexpr int WINDOW_H   = GRID_ROWS * CELL_SIZE;

constexpr float BASE_TICK  = 0.130f;
constexpr float MIN_TICK   = 0.055f;
constexpr float SPEED_INC  = 0.003f;
