#ifndef FRONTEND_TETRIS
#define FRONTEND_TETRIS

#include "../fsm.hpp"
extern "C" {
#include <ncurses.h>
}

void use_option();

/*!
 * @brief Инизиализация стартовых значений параметров библиотеки ncurses.h и
 * вывод начального экрана
 *
 * @param windows_game_info Окно с игровой информацией
 */
void init_game_frontend(WINDOW **windows_game_info);

/*!
 * @brief Отрисовка форматированной строки в заданное окно
 *
 * @param window_game Окно для форматированной печати
 * @param i Координата по вертикали
 * @param j Координата по горизонтали
 * @param set_color Атрибуты для форматированного вывода
 * @param sign Выводимый символ
 */
void print_color_sign(WINDOW *window_game, int i, int j, chtype set_color,
                      char sign);

/*!
 * @brief Промежуточная функция форматированной печати с выбором типа фигуры
 *
 * @param window_game Окно для форматированной печати
 * @param game_field Значения для форматированной печати
 * @param i Координата по вертикали
 * @param j Координата по горизонтали
 * @param num Переменная для печати чёрного фона при отсутствии значения фигуры
 */
void print_next_figure(WINDOW *window_game, int game_field, int i, int j,
                       bool num);

/*!
 * @brief Форматированная отрисовка в стандартном окне
 *
 * @param i Координата по вертикали
 * @param j Координата по горизонтали
 * @param set_color Атрибуты для форматированного вывода
 * @param sign Выводимый символ
 */
void print_color_matrix(int i, int j, chtype set_color, char sign);

/*!
 * @brief Промежуточная функция форматированной печати с выбором типа фигуры
 *
 * @param game_field Значения для форматированной печати
 * @param i Координата по вертикали
 * @param j Координата по горизонтали
 */
void print_figure(int game_field, int i, int j, int figure);

/*!
 * @brief Форматированная отрисовка игрового поля
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void print_game_window(const GameInfo_t field);

/*!
 * @brief Форматированная отрисовка окна с игровой информацией
 *
 * @param windows_game_info Окно с игровой информацией
 * @param field Структура со всеми матрицами и параметрами
 */
bool print_game_info_window(WINDOW *windows_game_info, const GameInfo_t field);

/*!
 * @brief Отрисовка горизонтальной линии использую форматированный вывод
 *
 * @param windows_game_info Окно с игровой информацией
 * @param place Координата по вертикали
 */
void printf_line(WINDOW *windows_game_info, int place);

/*!
 * @brief Отрисовка границ в окне
 *
 * @param game_info Окно для отрисовки границ
 * @param top Верхняя граница
 * @param bottom Нижняя граница
 * @param left Левая граница
 * @param right Правая граница
 * @param status Переменная для переключения возможности отрисовки левой границы
 */
void draw_rectangle_window(WINDOW *game_info, int top, int bottom, int left,
                           int right, bool status);

/*!
 * @brief Отрисовка горизонтальных линий
 *
 * @param game_info Окно для отрисовки границ
 * @param y Координата линии по вертикали
 * @param left Левая граница
 * @param right Правая граница
 * @param right_corner Символ правого угла
 * @param line Символ линии
 */
void draw_horizontal_line_window(WINDOW *game_info, int y, int left, int right,
                                 chtype right_corner, chtype line);

/*!
 * @brief Отрисовка вертикальных линий
 *
 * @param game_info Окно для отрисовки границ
 * @param top Верхняя граница
 * @param bottom Нижняя граница
 * @param x Координата линии по горизонтали
 * @param line Символ линии
 */
void draw_vertical_lines_window(WINDOW *game_info, int top, int bottom, int x,
                                chtype line);

/*!
 * @brief Отрисовка окна с концом игры и освобождение ресурсов
 *
 * @param windows_game_info Окно с игровой информацией
 * @param score Итоговый счёт игрока
 */
void close_game_frontend(WINDOW *windows_game_info, int score);

#endif  // FRONTEND_TETRIS
