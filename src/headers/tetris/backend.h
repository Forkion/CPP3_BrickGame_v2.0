#ifndef BACKEND_TETRIS
#define BACKEND_TETRIS

#include "../fsm.hpp"

#if defined(TETRIS_CLI)
#define START_SPEED 33000
#define SPEED_STEP 3000
#else
#define START_SPEED 1200
#define SPEED_STEP 110
#endif

/*!
 * @struct matrix_figure
 * @brief Структура, представляющая фигуру в игре
 *
 * @param matrix Матрица фигуры
 * @param number Номер фигуры
 * @param x Координата фигуры на поле по вертикали
 * @param y Координата фигуры на поле по горизонтали
 */
typedef struct {
  int **matrix;
  int number;
  int x;
  int y;
} matrix_figure;

/*!
 * @struct matrix_figure
 * @brief Структура со всеми матрицами и параметрами
 *
 * @param matrix Матрица поля
 * @param next Матрица следующей фигуры
 * @param score Количество очков
 * @param max_score Максимальное количество очков
 * @param level Уровень игры
 * @param speed Скорость игры
 * @param pause Пауза
 * @param game_end Конец игры
 */
typedef struct {
  int **matrix;
  int **next;
  int score;
  int max_score;
  int level;
  int speed;
  int pause;
  bool game_end;
  matrix_figure next_figure;
  matrix_figure figure;
} GameInfo;

/**
 * @brief Инизиализация игровых матриц и стартовых значений параметров
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void init_game_backend(GameInfo *field);

/**
 * @brief Инициализация игровых матриц и стартовых значений параметров
 *
 * @param field Структура со всеми матрицами и параметрами
 *
 * @return true при успешном создании или false при ошибке выделения памяти
 */
bool init_field(GameInfo *field);

/**
 * @brief Инизиализация матрицы фигуры для внутренней логики
 *
 * @param figure Структура с матрицей, координатами и типом фигуры
 * @param x Количество столбцов матрицы
 * @param y Количество строк матрицы
 *
 * @return true при успешном создании или false при ошибке выделения памяти
 */
bool init_matrix_figure(matrix_figure *figure, int x, int y);

/**
 * @brief Инизиализация матрицы фигуры для отрисовки
 *
 * @param next Указатель на матрицу следующей фигуры для фронтенда
 * @param x Количество столбцов матрицы
 * @param y Количество строк матрицы
 *
 * @return true при успешном создании или false при ошибке выделения памяти
 */
bool init_next_figure(int ***next, int x, int y);

/**
 * @brief Генерация типа фигуры с записью фигуры в матрицу
 *
 * @param figure Структура с матрицей, координатами и типом фигуры
 */
void create_figure(matrix_figure *figure);

/**
 * @brief Запись фигуры в массив
 *
 * @param figure Структура с матрицей, координатами и типом фигуры
 * @param rand_figure Сгенерированный тип фигуры
 */
void set_figure(matrix_figure *figure, int rand_figure);

/**
 * @brief Логика для смещения игрового поля (уничтожения заполненных линий)
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void offset_field(GameInfo *field);

/**
 * @brief Увеличение параметров уровня и скорости при росте количества игровых
 * очков
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void up_level(GameInfo *field);

/**
 * @brief Проверка на касание нижней стенки или другой фигуры при перемещении
 * вниз, при касании записывает фигуру в матрицу поля и генерирует новую
 *
 * @param field Структура со всеми матрицами и параметрами
 *
 * @return false, если фигура коснулась нижней стенки или другой фигуры
 */
bool check_down_touch(GameInfo *field);

/**
 * @brief Проверка на поражение, может ли фигура появиться на поле
 *
 * @param field Структура со всеми матрицами и параметрами
 *
 * @return true, если игра окончена
 */
bool check_defeat(GameInfo *field);

/**
 * @brief Проверка на касание левой стенки или другой фигуры при перемещении
 * влево, при касании не позволяет выйти за пределы левой стенки или войти в
 * другую фигуру
 *
 * @param field Структура со всеми матрицами и параметрами
 *
 * @return false при касании левой стенки или другой фигуры слева
 */
bool check_left_touch(GameInfo *field);

/**
 * @brief Проверка на касание правой стенки или другой фигуры при перемещении
 * вправо, при касании не позволяет выйти за пределы правой стенки или войти в
 * другую фигуру
 *
 * @param field Структура со всеми матрицами и параметрами
 *
 * @return false при касании правой стенки или другой фигуры справа
 */
bool check_right_touch(GameInfo *field);

/**
 * @brief Проверка статуса строк матрицы (была ли какая-либо из строк заполнена)
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void check_status_lines(GameInfo *field);

/**
 * @brief Увеличение количества очков, в зависимости от количества заполненных
 * строк матрицы поля
 *
 * @param field Структура со всеми матрицами и параметрами
 * @param offset Количество заполненных строк
 */
void add_score(GameInfo *field, int offset);

/**
 * @brief Переписывает параметры фигуры параметрами следующей фигуры
 *
 * @param figure Структура с матрицей, координатами и типом фигуры в которую
 * перезаписываются значения параметров
 * @param next_figure Структура с матрицей, координатами и типом фигуры, из
 * которой считываются значения параметров для перезаписи
 */
void rewrite_figure(matrix_figure *figure, matrix_figure next_figure);

/**
 * @brief Чистит только матрицу фигуры, не трогая остальные значения параметров
 * фигуры
 *
 * @param next_figure Структура с матрицей, координатами и типом фигуры, из
 * которой считываются значения параметров для перезаписи
 * @param next Указатель на матрицу следующей фигуры структуры GameInfo
 */
void rewrite_next(matrix_figure next_figure, int ***next);

/**
 * @brief Чистит только матрицу фигуры, не трогая остальные значения параметров
 * фигуры
 *
 * @param figure Структура с матрицей, координатами и типом фигуры
 */
void clean_matrix_figure(matrix_figure *figure);

/**
 * @brief Поворота фигуры на игровом поле
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void move_up(GameInfo *field);

/**
 * @brief Движение фигуры вниз по игровому полю
 *
 * @param figure Структура с матрицей, координатами и типом фигуры
 *
 * @return true при успешном движении, false при касании
 */
bool move_down(GameInfo *field);

/**
 * @brief Движение фигуры вправо по игровому полю
 *
 * @param figure Структура с матрицей, координатами и типом фигуры
 */
void move_right(GameInfo *field);

/**
 * @brief Движение фигуры влево по игровому полю
 *
 * @param figure Структура с матрицей, координатами и типом фигуры
 */
void move_left(GameInfo *field);

/**
 * @brief Записывает фигуру в игровое поле стуктуры GameInfo
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void write_figure_in_field(GameInfo *field);

/**
 * @brief Удаляет фигуру из игрового поле стуктуры GameInfo
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void delete_figure_out_field(GameInfo *field);

/**
 * @brief Логика поворота фигуры или возвращение в исходную позицию на игровом
 * поле
 *
 * @param field Структура со всеми матрицами и параметрами
 * @param rotate Временная структура фигуры, для реализации поворота
 */
void rotate_figure(GameInfo *field, matrix_figure rotate);

/**
 * @brief Логика возвращения фигуры в исходную позицию при невозможности
 * поворота
 *
 * @param field Структура со всеми матрицами и параметрами
 * @param rotate Временная структура фигуры, для реализации возвращения в
 * исходную позицию
 *
 */
void check_layering_figure_in_field(GameInfo *field, matrix_figure rotate);

/**
 * @brief Освобождение выделенных ресурсов и сохранение максимального количества
 * набранных очков
 *
 * @param field Структура со всеми матрицами и параметрами
 */
void close_game_backend(GameInfo *field);

/**
 * @brief Если есть файл с максимальным количеством набранных очков, считывает и
 * записывает из него значение
 *
 * @param field Структура со всеми матрицами и параметрами
 *
 * @return false при неудачной попытке открыть файл
 */
bool open_read(GameInfo *field);

/**
 * @brief Создаёт файл или открывает уже созданный, для записи максимального
 * количества очков
 *
 * @param field Структура со всеми матрицами и параметрами
 *
 * @return false при неудачной попытке создать или записать файл
 */
bool open_wright(GameInfo *field);

// /**
//  * @brief Логика для отображения подсказки (место падения фигуры)
//  *
//  * @param field Структура со всеми матрицами и параметрами
//  *
//  * @return Возвращает ложь, когда фигура достигла конца поля или другой
//  фигуры
//  */
// bool action_logic(GameInfo *field);

#endif  // BACKEND_TETRIS
