#include "../../headers/snake/backend.hpp"

/**
 * @namespace s21
 * @brief Пространство имен всего проекта змейки.
 */
namespace s21 {

GameFieldInfo::GameFieldInfo() : snake(this), apple(this) {
  srand(time(0));
  matrix = nullptr;
  matrix = new int *[HEIGHT * WIDTH + HEIGHT];
  for (int i = 0; i < HEIGHT; ++i) {
    matrix[i] = (int *)(matrix + HEIGHT) + (i * WIDTH);
    for (int j = 0; j < WIDTH; ++j) matrix[i][j] = 0;
  }

  next = nullptr;

  cell_index.resize(HEIGHT * WIDTH);
  for (short y = 0; y < HEIGHT; y++) {
    for (short x = 0; x < WIDTH; x++) {
      free_cells.emplace_back(y, x);
      cell_index[y * WIDTH + x] = free_cells.size() - 1;
    }
  }

  count_full_cell = 0;
  int a = 0;
  for (int i = HEIGHT / 2 + 3; i >= HEIGHT / 2; i--) {
    add_cell(std::pair<short, short>{i, WIDTH / 2});
    snake.set_front_snake_body(std::pair<short, short>{i, WIDTH / 2});
    if (i != HEIGHT / 2) matrix[i][WIDTH / 2] = 5;  // для цвета
    a++;
  }

  // for(int y = 0; y < HEIGHT; y++){
  // 	for(int x = 0; x < WIDTH; x++){
  // 		if(y == HEIGHT-1 && x == WIDTH-1){
  // 			continue;
  // 		}
  // 		add_cell(std::pair<short, short>{y, x});
  // 		snake.set_front_snake_body(std::pair<short, short>{y, x});
  // 		matrix[y][x] = 5;
  // 	}
  // }
  // matrix[HEIGHT-1][WIDTH-2] = 9;

  score = 0;
  max_score = 0;
  level = 1;
  speed = START_SPEED - SPEED_STEP * level;
  game_end = false;
  game_step = true;
  lust_action = Up;
  spawn_apple();
  read_file();
}

GameFieldInfo::~GameFieldInfo() {
  delete[] matrix;
  matrix = nullptr;
  write_in_file();
  score = 0;
  max_score = 0;
  level = 0;
  speed = 0;
  free_cells.clear();
  cell_index.clear();
  count_full_cell = 0;
  game_end = false;
  game_step = false;
}

GameInfo_t GameFieldInfo::updateCurrentState() {
  GameInfo_t field;
  field.matrix = matrix;
  field.next = next;
  field.score = score;
  field.max_score = max_score;
  field.level = level;
  field.speed = speed;
  return field;
}

void GameFieldInfo::set_score(short current_score) { score = current_score; }

void GameFieldInfo::set_level(short current_level) { level = current_level; }

void GameFieldInfo::set_speed(int current_speed) { speed = current_speed; }

void GameFieldInfo::set_game_step(bool status_step) { game_step = status_step; }

void GameFieldInfo::set_lust_action(UserAction_t current_action) {
  lust_action = current_action;
}

void GameFieldInfo::set_apple_pos(std::pair<short, short> pos) {
  auto a = apple.get_apple_pos();
  matrix[a.first][a.second] = 0;
  apple.set_apple_pos(pos);
  matrix[pos.first][pos.second] = 1;
}

short GameFieldInfo::get_score() { return score; }

short GameFieldInfo::get_level() { return level; }

int GameFieldInfo::get_speed() { return speed; }

bool GameFieldInfo::get_game_end() { return game_end; }

bool GameFieldInfo::get_game_step() { return game_step; }

UserAction_t GameFieldInfo::get_lust_action() { return lust_action; }

std::pair<short, short> GameFieldInfo::get_snake_head_pos() {
  return snake.get_head_pos();
}

int GameFieldInfo::get_size_snake_body() { return snake.get_size(); }

void GameFieldInfo::snake_move_up() {
  if (snake.get_head_pos().first <= 0 || count_full_cell >= 200) {
    status_game_info();
    game_end = true;
  } else {
    snake.move_up();
    check_defeat();
    check_status();
  }
}

void GameFieldInfo::snake_move_down() {
  if (snake.get_head_pos().first >= 19 || count_full_cell >= 200) {
    status_game_info();
    game_end = true;
  } else {
    snake.move_down();
    check_defeat();
    check_status();
  }
}

void GameFieldInfo::snake_move_left() {
  if (snake.get_head_pos().second <= 0 || count_full_cell >= 200) {
    status_game_info();
    game_end = true;
  } else {
    snake.move_left();
    check_defeat();
    check_status();
  }
}

void GameFieldInfo::snake_move_right() {
  if (snake.get_head_pos().second >= 9 || count_full_cell >= 200) {
    status_game_info();
    game_end = true;
  } else {
    snake.move_right();
    check_defeat();
    check_status();
  }
}

void GameFieldInfo::spawn_apple(int first, int second) {
  if (count_full_cell < 200) {
    if (!first) {
      auto pos = get_random_free_cell();
      apple.set_apple_pos(pos);
      auto [y, x] = pos;
      matrix[y][x] = 1;
    } else {
      std::pair<short, short> pos{first, second};
      apple.set_apple_pos(pos);
      auto [y, x] = pos;
      matrix[y][x] = 1;
    }
  }
}

void GameFieldInfo::check_defeat() {
  short index_cell = find_cell(snake.get_head_pos());
  if (index_cell < count_full_cell &&
      snake.get_back_snake_body() != snake.get_head_pos()) {
    game_end = true;
  }
}

void GameFieldInfo::check_status() {
  std::pair<short, short> head_pos = snake.get_head_pos();
  bool ate_apple = (apple.get_apple_pos() == head_pos);
  auto tail = snake.get_back_snake_body();
  if (ate_apple) {
    snake.set_head_on_apple(true);
  }
  if (snake.get_head_on_apple()) {
    score++;
    snake.set_head_on_apple(false);
  } else {
    free_cell(tail);
    snake.del_back_snake_body();
  }
  if (ate_apple) {
    matrix[head_pos.first][head_pos.second] = 9;
    status_game_info();
    spawn_apple();
  }
  add_cell(head_pos);
  snake.set_front_snake_body(head_pos);
}

void GameFieldInfo::status_game_info() {
  if (max_score < score) max_score = score;
  if (level < 10 && score && !(score % 5)) {
    level++;
    speed -= SPEED_STEP;
  }
}

void GameFieldInfo::add_cell(std::pair<short, short> cell) {
  short index_free_cell = find_cell(cell);

  if (index_free_cell >= count_full_cell) {
    // Обмен клетками между свободными и занятыми с обновлением индексов
    auto swapped_cell =
        free_cells[count_full_cell];  // Клетка, которая была на границе
    auto added_cell = free_cells[index_free_cell];  // Клетка, которую добавляем

    std::swap(free_cells[count_full_cell], free_cells[index_free_cell]);

    // обновление индексов для обеих клеток по их координатам
    cell_index[swapped_cell.first * WIDTH + swapped_cell.second] =
        index_free_cell;
    cell_index[added_cell.first * WIDTH + added_cell.second] = count_full_cell;
  }
  matrix[cell.first][cell.second] =
      9;  // цифровое значение цвет для головы во фронтенде
  count_full_cell++;
}

void GameFieldInfo::free_cell(std::pair<short, short> cell) {
  count_full_cell--;
  short index_full_cell = find_cell(cell);

  if (index_full_cell < count_full_cell) {
    // СОХРАНЯЕМ координаты ДО обмена
    auto freed_cell =
        free_cells[index_full_cell];  // Клетка, которую освобождаем
    auto swapped_cell =
        free_cells[count_full_cell];  // Клетка, которая была на границе

    std::swap(free_cells[index_full_cell], free_cells[count_full_cell]);

    // ОБНОВЛЯЕМ индексы для ОБЕИХ клеток по ИХ КООРДИНАТАМ
    cell_index[freed_cell.first * WIDTH + freed_cell.second] = count_full_cell;
    cell_index[swapped_cell.first * WIDTH + swapped_cell.second] =
        index_full_cell;
  }

  matrix[cell.first][cell.second] = 0;
}

short GameFieldInfo::find_cell(std::pair<short, short> find) {
  return cell_index[find.first * WIDTH + find.second];
}

std::pair<short, short> GameFieldInfo::get_random_free_cell() {
  if (static_cast<size_t>(count_full_cell) >= free_cells.size()) {
    return {-1, -1};
  }

  int random_index =
      (rand() % (free_cells.size() - count_full_cell)) + count_full_cell;
  auto result = free_cells[random_index];
  return result;
}

void GameFieldInfo::read_file() {
  std::ifstream input("max_score_snake.txt", std::ios::binary);
  if (input) {
    input.read(reinterpret_cast<char *>(&max_score), sizeof(max_score));
    input.close();
  } else {
    std::ofstream output("max_score_snake.txt", std::ios::binary);
    if (output) {
      output.write(reinterpret_cast<char *>(&max_score), sizeof(max_score));
      output.close();
    }
  }
}

void GameFieldInfo::write_in_file() {
  std::ofstream output("max_score_snake.txt", std::ios::binary);
  if (output) {
    output.write(reinterpret_cast<char *>(&max_score), sizeof(max_score));
    output.close();
  }
}

GameFieldInfo::GameSnake::GameSnake()
    : game_field(nullptr), head_on_apple(false) {}

GameFieldInfo::GameSnake::GameSnake(GameFieldInfo *game_field_ptr)
    : game_field(game_field_ptr), head_on_apple(false) {}

std::pair<short, short> GameFieldInfo::GameSnake::get_head_pos() {
  return snake_head_pos;
}

void GameFieldInfo::GameSnake::set_front_snake_body(
    std::pair<short, short> head) {
  snake_body.push_front(head);
  snake_head_pos = head;
}

void GameFieldInfo::GameSnake::set_head_on_apple(bool status_head) {
  head_on_apple = status_head;
}

bool GameFieldInfo::GameSnake::get_head_on_apple() { return head_on_apple; }

std::pair<short, short> GameFieldInfo::GameSnake::get_back_snake_body() {
  return snake_body.back();
}

int GameFieldInfo::GameSnake::get_size() { return snake_body.size(); }

void GameFieldInfo::GameSnake::del_back_snake_body() { snake_body.pop_back(); }

void GameFieldInfo::GameSnake::move_up() {
  if (!game_field->game_end)
    game_field->matrix[snake_head_pos.first][snake_head_pos.second] = 5;
  snake_head_pos.first--;
}

void GameFieldInfo::GameSnake::move_down() {
  if (!game_field->game_end)
    game_field->matrix[snake_head_pos.first][snake_head_pos.second] = 5;
  snake_head_pos.first++;
}

void GameFieldInfo::GameSnake::move_left() {
  if (!game_field->game_end)
    game_field->matrix[snake_head_pos.first][snake_head_pos.second] = 5;
  snake_head_pos.second--;
}

void GameFieldInfo::GameSnake::move_right() {
  if (!game_field->game_end)
    game_field->matrix[snake_head_pos.first][snake_head_pos.second] = 5;
  snake_head_pos.second++;
}

GameFieldInfo::GameApple::GameApple() : game_field(nullptr), apple_pos{0, 0} {}

GameFieldInfo::GameApple::GameApple(GameFieldInfo *game_field_ptr)
    : game_field(game_field_ptr), apple_pos{0, 0} {}

std::pair<short, short> GameFieldInfo::GameApple::get_apple_pos() {
  return apple_pos;
}

void GameFieldInfo::GameApple::set_apple_pos(std::pair<short, short> pos) {
  apple_pos = pos;
}

}  // namespace s21
