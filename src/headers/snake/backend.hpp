#ifndef BACKEND_SNAKE
#define BACKEND_SNAKE

#include <fstream>
#include <list>
#include <random>
#include <vector>

#include "../../headers/fsm.hpp"

#if defined(SNAKE_CLI)
#define START_SPEED 33000
#define SPEED_STEP 3000
#else
#define START_SPEED 1200
#define SPEED_STEP 110
#endif

/**
 * @namespace s21
 * @brief Пространство имен всего проекта змейки.
 */
namespace s21 {

/**
 * @class GameFieldInfo
 * @brief Основной класс, управляющий игровой логикой змейки
 *
 * Содержит игровое поле, состояние змейки, яблока и всю игровую механику.
 * Реализует логику движения, столкновений, роста змейки и подсчета очков.
 * Унаследован от структуры, предназначенной для хранения текущего состояния
 * игры
 */
class GameFieldInfo : public GameInfo_t {
 public:
  /**
   * @brief Конструктор
   *
   * Выделение памяти для матрицы поля.
   * Присваивание начальных значений для змейки
   * Инизиализация остальных переменных класса
   *
   */
  GameFieldInfo();

  /**
   * @brief Деструктор
   *
   * Освобождение памяти для матрицы поля.
   * Присваивание начальных значений для змейки
   * Инизиализация остальных переменных класса
   *
   */
  ~GameFieldInfo();

  /**
   * @brief Запрет на копирование
   */
  GameFieldInfo(const GameFieldInfo &) = delete;

  /**
   * @brief Запрет на копирование присваиванием
   */
  GameFieldInfo &operator=(const GameFieldInfo &) = delete;

  /**
   * @brief Запрет на присваивание
   */
  GameFieldInfo(GameFieldInfo &&) = delete;

  /**
   * @brief Запрет на присваивание перемещением
   */
  GameFieldInfo &operator=(GameFieldInfo &&) = delete;

  /**
   * @brief Возвращает текущее состояние игры для отображения во фронтенде
   * @return GameInfo_t структура, содержащая информацию о текущем состоянии
   * игры
   */
  GameInfo_t updateCurrentState();

  /**
   * @brief Сеттер для установки количества очков
   */
  void set_score(short current_score);

  /**
   * @brief Сеттер для установки уровня
   */
  void set_level(short current_level);

  /**
   * @brief Сеттер для установки скорости
   */
  void set_speed(int current_speed);

  /**
   * @brief Сеттер для установки игрового шага (true - можно сделать шаг
   * клавишей)
   */
  void set_game_step(bool status_step);

  /**
   * @brief Сеттер для установки последнего совершённого движения (Up, Doun,
   * Left, Right)
   */
  void set_lust_action(UserAction_t current_action);

  /**
   * @brief Утановка позиция яблочка вручную для теста
   *
   * @param pos Новая позиция яблочка
   */
  void set_apple_pos(std::pair<short, short> pos);

  /**
   * @brief Геттер для количества очков
   * @return Текущее количество очков
   */
  short get_score();

  /**
   * @brief Геттер для уровня
   * @return Текущий уровнь
   */
  short get_level();

  /**
   * @brief Геттер для уровня
   * @return Текущую скорость
   */
  int get_speed();

  /**
   * @brief Геттер состояния игры
   * @return Текущее состояние игры (true - конец игры)
   */
  bool get_game_end();

  /**
   * @brief Геттер игрового шага
   * @return Текущее состояние игрового шага (true - можно сделать шаг клавишей)
   */
  bool get_game_step();
  UserAction_t get_lust_action();

  /**
   * @brief Узнать позицию головы для теста
   * @return Текущую позицию головы змейки
   */
  std::pair<short, short> get_snake_head_pos();

  /**
   * @brief возвращает размер змейки
   * @return Размер змейки
   */
  int get_size_snake_body();
  /**
   * @brief Движение объекта змейки вверх с проверкой границ и поражения
   */
  void snake_move_up();

  /**
   * @brief Движение объекта змейки вниз с проверкой границ и поражения
   */
  void snake_move_down();

  /**
   * @brief Движение объекта змейки влево с проверкой границ и поражения
   */
  void snake_move_left();

  /**
   * @brief Движение объекта змейки вправо с проверкой границ и поражения
   */
  void snake_move_right();

  /**
   * @brief Создание нового яблока на случайной свободной клетке
   *
   * @param first Координата высоты
   * @param second Координата ширины
   */
  void spawn_apple(int first = 0, int second = 0);

 private:
  /**
   * @brief Проверка столкновения головы змейки с ее телом
   */
  void check_defeat();

  /**
   * @brief Обновление состояния игры после движения змейки
   *
   * Обрабатывает съедание яблока, рост змейки, движение хвоста
   */
  void check_status();

  /**
   * @brief Обновление состояния информационных полей игры
   *
   * Увеличивает уровень, кол-во очков, кол-во макс. очков и скорость
   *
   */
  void status_game_info();

  /**
   * @brief Добавление клетки в занятые с обновлением индексов
   * @param cell Координаты добавляемой клетки
   *
   * Использует систему быстрого доступа к свободным клеткам через cell_index
   */
  void add_cell(std::pair<short, short> cell);

  /**
   * @brief Освобождение клетки с обновлением индексов
   * @param cell Координаты освобождаемой клетки
   */
  void free_cell(std::pair<short, short> cell);

  /**
   * @brief Быстрый поиск индекса клетки в массиве free_cells
   * @param find Координаты искомой клетки
   * @return Индекс клетки в free_cells
   */
  short find_cell(std::pair<short, short> find);

  /**
   * @brief Выбор случайной свободной клетки для яблока
   * @return Координаты свободной клетки
   */
  std::pair<short, short> get_random_free_cell();

  /**
   * @brief Чтение рекорда из файла
   */
  void read_file();

  /**
   * @brief Сохранение рекорда в файл
   */
  void write_in_file();

 private:
  class GameSnake {
    GameFieldInfo *game_field;  // Указатель на родительское игровое поле
    std::list<std::pair<short, short>> snake_body;  // Список клеток тела змейки
    std::pair<short, short> snake_head_pos;  // Текущие координаты головы
    bool head_on_apple;  // Флаг съедения яблока головой
   public:
    /**
     * @brief Конструктор по умолчанию для змейки
     *
     * Инициализирует змейку в начальное состояние:
     * - Позиция (0, 0) как временное значение
     */
    GameSnake();

    /**
     * @brief Конструктор змейки с привязкой к игровому полю
     *
     * @param game_field_ptr Указатель на родительское игровое поле
     */
    GameSnake(GameFieldInfo *game_field_ptr);

    /**
     * @brief Добавляет новую клетку в начало тела змейки
     *
     * Используется при движении змейки для добавления новой головы
     * при росте змейки после съедания яблока.
     *
     * @param head Координаты добавляемой клетки
     */
    void set_front_snake_body(std::pair<short, short> head);

    /**
     * @brief Устанавливает флаг нахождения головы на яблоке
     *
     * Флаг используется для определения момента съедания яблока
     * и последующего роста змейки.
     *
     * @param status_head Состояние флага (true - голова на яблоке)
     */
    void set_head_on_apple(bool status_head);

    /**
     * @brief Возвращает текущие координаты головы змейки
     * @return Координаты головы змейки (y, x)
     */
    std::pair<short, short> get_head_pos();

    /**
     * @brief Проверяет, находится ли голова змейки на яблоке
     * @return true если голова на яблоке, иначе false
     */
    bool get_head_on_apple();

    /**
     * @brief Возвращает координаты хвоста змейки
     *
     * Используется при движении для определения клетки,
     * которую нужно освободить если змейка не растет.
     *
     * @return Координаты последней клетки тела змейки
     */
    std::pair<short, short> get_back_snake_body();

    /**
     * @brief Возвращает размер змейки
     *
     * @return размер змейки
     */
    int get_size();

    /**
     * @brief Удаляет последнюю клетку хвоста змейки
     *
     * Вызывается при обычном движении змейки (без роста)
     * для поддержания постоянной длины.
     */
    void del_back_snake_body();

    /**
     * @brief Изменение координат головы змейки для движения вверх с изменением
     * матрицы поля
     *
     * Перед изменением позиции обновляет текущую клетку в матрице
     * для корректного отображения цвета тела змейки в интерфейсе.
     *
     */
    void move_up();

    /**
     * @brief Изменение координат головы змейки для движения вниз с изменением
     * матрицы поля
     *
     * Перед изменением позиции обновляет текущую клетку в матрице
     * для корректного отображения цвета тела змейки в интерфейсе.
     *
     */
    void move_down();

    /**
     * @brief Изменение координат головы змейки для движения влево с изменением
     * матрицы поля
     *
     * Перед изменением позиции обновляет текущую клетку в матрице
     * для корректного отображения цвета тела змейки в интерфейсе.
     *
     */
    void move_left();

    /**
     * @brief Изменение координат головы змейки для движения вправо с изменением
     * матрицы поля
     *
     * Перед изменением позиции обновляет текущую клетку в матрице
     * для корректного отображения цвета тела змейки в интерфейсе.
     *
     */
    void move_right();
  };

  class GameApple {
    GameFieldInfo *game_field;  // Указатель на родительское игровое поле
    std::pair<short, short> apple_pos;  // Координаты яблочка на поле
   public:
    /**
     * @brief Конструктор по умолчанию для яблока
     *
     * Инициализирует яблоко в начальное состояние:
     * - Позиция (0, 0) как временное значение
     */
    GameApple();

    /**
     * @brief Конструктор яблока с привязкой к игровому полю
     *
     * @param game_field_ptr Указатель на родительское игровое поле
     */
    GameApple(GameFieldInfo *game_field_ptr);

    /**
     * @brief Возвращает текущие координаты яблока
     * @return Координаты яблока (y, x)
     */
    std::pair<short, short> get_apple_pos();

    /**
     * @brief Устанавливает новые координаты для яблока
     *
     * Используется при создании нового яблока
     * на случайной свободной клетке поля.
     *
     * @param pos Новые координаты яблока (y, x)
     */
    void set_apple_pos(std::pair<short, short> pos);
  };

 private:
  std::vector<std::pair<short, short>>
      free_cells;  // занятые и свободные клетки поля
  std::vector<short> cell_index;  // индексы клеток для быстрого поиска
  short count_full_cell;  // Счётчик заполненных клеток в free_cells
  GameSnake snake;  // Управление змейкой на поле
  GameApple apple;  // Управление яблоком на поле
  bool game_end;    // Флаг для установки конца игры
  bool game_step;  // Флаг для единственного поворота
  UserAction_t lust_action;  // Последнее направление движения
};

}  // namespace s21
#endif  // BACKEND_SNAKE
