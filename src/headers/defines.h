#ifndef DEFINES_BRICK_GAME
#define DEFINES_BRICK_GAME

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SUCCESS 1
#define ERROR 0

// ДЛЯ ИЗМЕНЕНИЯ РАЗМЕРА МЕНЯТЬ ТОЛЬКО SIZE, МИНИМАЛЬЫНЙ РАЗМЕР 1, НА 3 КОНСОЛЬ
// ДОЛЖНА ЗАНИМАТЬ ВЕСЬ ЭКРАН МОНИТОРА
#define SIZE 1  // общий размер поля и высота фигуры
#define SIZE_FIGURE 2  // Ширина пикселя фигуры
#define WIDTH_PIXEL_FIGURE \
  SIZE *SIZE_FIGURE  // Ширина пикселя фигуры при масштабировании
#define WIDTH_GAME_INFO 15  // Ширина информационного поля
#define WIDTH_GAME_INFO_FIELD \
  WIDTH_GAME_INFO + WIDTH_PIXEL_FIGURE *SIZE - WIDTH_PIXEL_FIGURE + 1
#define WIDTH 10   // Ширина матрицы игрового поля
#define HEIGHT 20  // Высота матрицы игрового поля
#define HEIGHT_GAME 20 * SIZE  // Масштабируемая высота игрового поля
#define WIDTH_GAME \
  10 * SIZE *SIZE_FIGURE  // Масштабируемая ширина игрового поля
#define WIDTH_FIGURE 4 * SIZE *SIZE_FIGURE  // Ширина фиругы
#define HEIGHT_FIGURE 4 * SIZE              // Высота фигуры
#define SIZE_NEXT 4  // размер матрицы next 4х4

#define COLOR_PINK 13
#define COLOR_DARK_BLUE 19
#define COLOR_PURPLE 53
#define COLOR_ORANGE 130

#define DEFOLT_PANEL 0  // Стандартная палитра поля
#define FIGUR_l 1  // Красная палитра для прямой фигуры l
#define FIGUR_L_MIRRORED 2  // Оранжевая палитра для фигуры Г
#define FIGUR_L 3           // Желтая палитра для фигуры L
#define FIGUR_O 4  // Розовая палитра для фигуры O
#define FIGUR_S 5  // Зелёная палитра для фигуры S
#define FIGUR_Z 6  // Фиолетовая палитра для фигуры Z
#define FIGUR_T 7  // Синяя палитра для фигуры T
#define FULL_STR 8  // Белая палитра для заполненной строки

#define WIDTH_INFO_WINDOW 3
#define SCORE_INFO_HEIGHT 3
#define MAX_SCORE_INFO 6
#define NEXT_FIGURE 11
#define LEVEL                                                  \
  (NEXT_FIGURE + (HEIGHT_GAME / 10) + (WIDTH_GAME_INFO / 10) + \
   (HEIGHT_GAME / 10) - 1)

#define KEY_SPACE 32
#define KEY_p 112
#define KEY_P 80
#define KEY_q 113
#define KEY_Q 81

extern bool hint;  // отображение подсказки

#endif  // DEFINES_BRICK_GAME
