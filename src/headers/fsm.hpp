#ifndef FSM
#define FSM

#include <stdlib.h>

#include "defines.h"

/**
 * @brief Структура отображающая текущий статус конечного автомата
 */
typedef enum {
  GAME_PLAY,
  GAME_PAUSE,
  GAME_END,
} GameState_t;

/**
 * @brief Структура со всеми матрицами и параметрами
 * @param matrix Матрица поля
 * @param next Матрица следующей фигуры
 * @param score Количество очков
 * @param max_score Максимальное количество очков
 * @param level Уровень игры
 * @param speed Скорость игры
 * @param pause Пауза
 */
typedef struct {
  int **matrix;
  int **next;
  int score;
  int max_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/**
 * @brief Коды нажатия клавиш для действия
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Up,
  Down,
  Right,
  Left,
  Action
} UserAction_t;

UserAction_t get_signal(int user_input);

/**
 * @brief Выбирает действие на основе сигнала
 *
 * Запускает функцию/метод в зависимости от входного сигнала и зажатия клавиши
 *
 * @param current_action Сигнал для взаимодействия с игрой
 * @param hold Сигнал о нажатии клавиши
 */
void userInput(UserAction_t current_action, bool hold);

/**
 * @brief Получения данных для отрисовки в интерфейсе
 *
 * @return Структуру, содержащую информацию о текущем состоянии игры
 */
GameInfo_t updateCurrentState();

/**
 * @brief Устанавливает состояние конечного автомата
 */
GameState_t SwitchState(UserAction_t current_action);

/**
 * @brief Запускает основной игровой цикл
 *
 * Создание и инициализация всех необходимых переменных и запуск игрового цикла
 *
 * @param argc Аргумент функции main, используется для уровня из меню
 * @param argv Аргумент функции main, используется для уровня из меню
 */
int game_loop(int argc, char *argv[]);

#endif  // FSM
