#include "../headers/fsm.hpp"
// bool hint = false;

int main(int argc, char *argv[]) {
  int res = 0;
  res = game_loop(argc, argv);
  return res;
}

// ============================================================================
// ДЕСКТОПНАЯ ВЕРСИЯ ТЕТРИСА(qt)
// ============================================================================
#ifdef TETRIS_QT
extern "C" {
#include "../headers/tetris/backend.h"
}
#include "../headers/snake/frontend.hpp"

static GameInfo field;
s21::QBrickGame *gameWindowPointer;

int game_loop(int argc, char *argv[]) {
  init_game_backend(&field);

  if (argc > 1 && !strcmp(argv[1], "--level")) {
    int level = atoi(argv[2]);
    if (level >= 1 && level <= 10) {
      field.level = level;
      field.speed = START_SPEED - SPEED_STEP * level;
    }
  } else {
    field.speed = START_SPEED - SPEED_STEP * field.level;
  }

  QApplication app(argc, argv);
  s21::QBrickGame *gameWindow = new (s21::QBrickGame);
  gameWindowPointer = gameWindow;
  gameWindow->setFocusPolicy(Qt::StrongFocus);
  gameWindow->init();
  gameWindow->show();

  int result = app.exec();

  delete (gameWindow);
  close_game_backend(&field);

  return result;
}

GameState_t SwitchState(UserAction_t current_action) {
  static GameState_t Game_state = GAME_PLAY;

  if (field.game_end) {
    Game_state = GAME_END;
    return Game_state;
  }

  switch (current_action) {
    case Terminate:
      Game_state = GAME_END;
      break;
    case Start:
    case Up:
    case Down:
    case Left:
    case Right:
    case Action:
      if (Game_state != GAME_PAUSE) Game_state = GAME_PLAY;
      break;
    case Pause:
      if (Game_state == GAME_PAUSE)
        Game_state = GAME_PLAY;
      else if (Game_state == GAME_PLAY)
        Game_state = GAME_PAUSE;
      break;
    default:
      break;
  }

  return Game_state;
}

void userInput(UserAction_t current_action, bool hold) {
  (void)hold;

  switch (current_action) {
    case Up:
      move_up(&field);
      break;
    case Down:
      move_down(&field);
      gameWindowPointer->updateTimer(field.speed);
      break;
    case Left:
      move_left(&field);
      break;
    case Right:
      move_right(&field);
      break;
    case Action:
      while (move_down(&field));
      break;
    default:
      break;
  }
}

GameInfo_t updateCurrentState() {
  GameInfo_t current_field;
  current_field.matrix = field.matrix;
  current_field.next = field.next;
  current_field.score = field.score;
  current_field.max_score = field.max_score;
  current_field.level = field.level;
  current_field.speed = field.speed;
  return current_field;
}

#endif

// ============================================================================
// ДЕСКТОПНАЯ ВЕРСИЯ ЗМЕЙКИ(qt)
// ============================================================================
#ifdef SNAKE_QT

#include "../headers/snake/backend.hpp"
#include "../headers/snake/frontend.hpp"

static s21::GameFieldInfo field;
s21::QBrickGame *gameWindowPointer = nullptr;

int game_loop(int argc, char *argv[]) {
  if (argc > 1 && !strcmp(argv[1], "--level")) {
    int level = atoi(argv[2]);
    if (level >= 1 && level <= 10) {
      field.set_level(level);
      field.set_speed(START_SPEED - SPEED_STEP * level);
    }
  }

  QApplication app(argc, argv);
  s21::QBrickGame *gameWindow = new (s21::QBrickGame);
  gameWindowPointer = gameWindow;
  gameWindow->setFocusPolicy(Qt::StrongFocus);
  gameWindow->init();
  gameWindow->show();
  int res = app.exec();
  delete (gameWindow);
  return res;
}

GameState_t SwitchState(UserAction_t current_action) {
  static GameState_t Game_state = GAME_PLAY;

  if (field.get_game_end()) {
    Game_state = GAME_END;
    return Game_state;
  }

  switch (current_action) {
    case Terminate:
      Game_state = GAME_END;
      break;
    case Start:
    case Up:
    case Down:
    case Left:
    case Right:
    case Action:
      if (Game_state != GAME_PAUSE) Game_state = GAME_PLAY;
      break;
    case Pause:
      if (Game_state == GAME_PAUSE)
        Game_state = GAME_PLAY;
      else if (Game_state == GAME_PLAY)
        Game_state = GAME_PAUSE;
      break;
    default:
      break;
  }
  return Game_state;
}

void userInput(UserAction_t current_action, bool hold) {
  // hold = false → движение по таймеру (автоматическое)
  // hold = true → движение по пользовательскому вводу

  if (!hold) {
    // Автоматическое движение по таймеру
    switch (field.get_lust_action()) {
      case Up:
        field.snake_move_up();
        break;
      case Down:
        field.snake_move_down();
        break;
      case Left:
        field.snake_move_left();
        break;
      case Right:
        field.snake_move_right();
        break;
      default:
        break;
    }
    field.set_game_step(true);
    gameWindowPointer->updateTimer(field.get_speed());
  } else {
    // Пользовательский ввод
    switch (current_action) {
      case Up:
        if (field.get_lust_action() != Down && field.get_lust_action() != Up &&
            field.get_game_step()) {
          field.set_lust_action(Up);
          field.set_game_step(false);
        }
        break;
      case Down:
        if (field.get_lust_action() != Up && field.get_lust_action() != Down &&
            field.get_game_step()) {
          field.set_lust_action(Down);
          field.set_game_step(false);
        }
        break;
      case Left:
        if (field.get_lust_action() != Right &&
            field.get_lust_action() != Left && field.get_game_step()) {
          field.set_lust_action(Left);
          field.set_game_step(false);
        }
        break;
      case Right:
        if (field.get_lust_action() != Left &&
            field.get_lust_action() != Right && field.get_game_step()) {
          field.set_lust_action(Right);
          field.set_game_step(false);
        }
        break;
      case Action:
        // Принудительное движение по пользовательскому запросу
        switch (field.get_lust_action()) {
          case Up:
            field.snake_move_up();
            break;
          case Down:
            field.snake_move_down();
            break;
          case Left:
            field.snake_move_left();
            break;
          case Right:
            field.snake_move_right();
            break;
          default:
            break;
        }
        field.set_game_step(true);
        gameWindowPointer->updateTimer(field.get_speed());
        break;
      default:
        break;
    }
  }
}

GameInfo_t updateCurrentState() { return field.updateCurrentState(); }

#endif

// ============================================================================
// КОНСОЛЬНАЯ ВЕРСИЯ ТЕТРИСА(ncurses)
// ============================================================================
#ifdef TETRIS_CLI

extern "C" {
#include "../headers/tetris/backend.h"
}
#include "../headers/tetris/frontend.hpp"

static GameInfo field;

int game_loop(int argc, char *argv[]) {
  WINDOW *windows_game_info = {0};
  clock_t startTime = 0;
  clock_t currentTime = 0;
  UserAction_t current_action = Start;
  GameState_t current_state = GAME_PLAY;
  int input;

  init_game_backend(&field);
  init_game_frontend(&windows_game_info);

  if (argc > 1 && !strcmp(argv[1], "--level")) {
    int level = atoi(argv[2]);
    if (level >= 1 && level <= 10) {
      field.level = level;
      field.speed = START_SPEED - SPEED_STEP * level;
    }
  }

  while (current_state != GAME_END) {
    startTime = clock();
    input = getch();

    current_action = get_signal(input);
    current_state = SwitchState(current_action);

    if (current_state == GAME_PAUSE || current_action == Down) {
      currentTime = startTime;
      if (current_state == GAME_PAUSE) continue;
    } else if (startTime - currentTime >= field.speed * SIZE) {
      if (current_action == Up || current_action == Left ||
          current_action == Right) {
        userInput(current_action, true);
      }
      current_action = Down;
      currentTime = startTime;
    }

    if (current_state != GAME_END) {
      userInput(current_action, true);
      print_game_window(updateCurrentState());
      print_game_info_window(windows_game_info, updateCurrentState());
    }
  }

  close_game_frontend(windows_game_info, field.score);
  close_game_backend(&field);
  return 0;
}

GameState_t SwitchState(UserAction_t current_action) {
  static GameState_t Game_state = GAME_PLAY;

  if (field.game_end) {
    Game_state = GAME_END;
    return Game_state;
  }

  switch (current_action) {
    case Terminate:
      Game_state = GAME_END;
      break;
    case Start:
    case Up:
    case Down:
    case Left:
    case Right:
    case Action:
      if (Game_state != GAME_PAUSE) Game_state = GAME_PLAY;
      break;
    case Pause:
      if (Game_state == GAME_PAUSE)
        Game_state = GAME_PLAY;
      else if (Game_state == GAME_PLAY)
        Game_state = GAME_PAUSE;
      break;
    default:
      break;
  }

  return Game_state;
}

UserAction_t get_signal(int user_input) {
  UserAction_t current_action = Start;
  switch (user_input) {
    case KEY_UP:
      current_action = Up;
      break;
    case KEY_DOWN:
      current_action = Down;
      break;
    case KEY_LEFT:
      current_action = Left;
      break;
    case KEY_RIGHT:
      current_action = Right;
      break;
    case 'q':
    case 'Q':
    case 27:
      current_action = Terminate;
      break;
    case '\n':
    case '\r':
      current_action = Start;
      break;
    case ' ':
      current_action = Action;
      break;
    case 'p':
    case 'P':
      current_action = Pause;
      break;
    default:
      break;
  }
  return current_action;
}

GameInfo_t updateCurrentState() {
  GameInfo_t current_field;
  current_field.matrix = field.matrix;
  current_field.next = field.next;
  current_field.score = field.score;
  current_field.max_score = field.max_score;
  current_field.level = field.level;
  current_field.speed = field.speed;
  return current_field;
}

void userInput(UserAction_t current_action, bool hold) {
  (void)hold;

  switch (current_action) {
    case Up:
      move_up(&field);
      break;
    case Down:
      move_down(&field);
      break;
    case Left:
      move_left(&field);
      break;
    case Right:
      move_right(&field);
      break;
    case Action:
      while (move_down(&field));
      break;
    default:
      break;
  }
}

#endif

// ============================================================================
// КОНСОЛЬНАЯ ВЕРСИЯ ЗМЕЙКИ(ncurses)
// ============================================================================
#ifdef SNAKE_CLI

#include "../headers/snake/backend.hpp"
#include "../headers/tetris/frontend.hpp"

static s21::GameFieldInfo field;

int game_loop(int argc, char *argv[]) {
  WINDOW *windows_game_info = {0};
  clock_t startTime = 0;
  clock_t currentTime = 0;
  static UserAction_t current_action = Start;
  GameState_t current_state = GAME_PLAY;
  int input;

  init_game_frontend(&windows_game_info);

  if (argc > 1 && !strcmp(argv[1], "--level")) {
    int level = atoi(argv[2]);
    if (level >= 1 && level <= 10) {
      field.set_level(level);
      field.set_speed(START_SPEED - level * SPEED_STEP);
    }
  } else {
    field.set_speed(START_SPEED - SPEED_STEP * field.get_level());
  }

  while (current_state != GAME_END) {
    startTime = clock();
    input = getch();
    current_action = get_signal(input);
    current_state = SwitchState(current_action);

    if (current_state == GAME_PAUSE) {
      currentTime = startTime;
      continue;
    } else if (startTime - currentTime >= updateCurrentState().speed * SIZE) {
      userInput(Start, false);
      currentTime = startTime;
      continue;
    }
    userInput(current_action, true);
    if (current_action == Action) currentTime = startTime;
    if (current_state != GAME_END) {
      print_game_window(updateCurrentState());
      print_game_info_window(windows_game_info, updateCurrentState());
    }
  }

  close_game_frontend(windows_game_info, field.get_score());
  return 0;
}

GameState_t SwitchState(UserAction_t current_action) {
  static GameState_t Game_state = GAME_PLAY;

  if (field.get_game_end()) {
    Game_state = GAME_END;
    return Game_state;
  }

  switch (current_action) {
    case Terminate:
      Game_state = GAME_END;
      break;
    case Start:
    case Up:
    case Down:
    case Left:
    case Right:
    case Action:
      if (Game_state != GAME_PAUSE) Game_state = GAME_PLAY;
      break;
    case Pause:
      if (Game_state == GAME_PAUSE)
        Game_state = GAME_PLAY;
      else if (Game_state == GAME_PLAY)
        Game_state = GAME_PAUSE;
      break;
    default:
      break;
  }

  return Game_state;
}

UserAction_t get_signal(int user_input) {
  UserAction_t current_action = Start;
  switch (user_input) {
    case KEY_UP:
      current_action = Up;
      break;
    case KEY_DOWN:
      current_action = Down;
      break;
    case KEY_LEFT:
      current_action = Left;
      break;
    case KEY_RIGHT:
      current_action = Right;
      break;
    case 'q':
    case 'Q':
    case 27:
      current_action = Terminate;
      break;
    case '\n':
    case '\r':
      current_action = Start;
      break;
    case ' ':
      current_action = Action;
      break;
    case 'p':
    case 'P':
      current_action = Pause;
      break;
    default:
      break;
  }
  return current_action;
}

GameInfo_t updateCurrentState() { return field.updateCurrentState(); }

void userInput(UserAction_t current_action, bool hold) {
  // hold = false → движение по таймеру (автоматическое)
  // hold = true → движение по пользовательскому вводу

  if (!hold) {
    // Автоматическое движение по таймеру
    switch (field.get_lust_action()) {
      case Up:
        field.snake_move_up();
        break;
      case Down:
        field.snake_move_down();
        break;
      case Left:
        field.snake_move_left();
        break;
      case Right:
        field.snake_move_right();
        break;
      default:
        break;
    }
    field.set_game_step(true);
  } else {
    // Пользовательский ввод
    switch (current_action) {
      case Up:
        if (field.get_lust_action() != Down && field.get_lust_action() != Up &&
            field.get_game_step()) {
          field.set_lust_action(Up);
          field.set_game_step(false);
        }
        break;
      case Down:
        if (field.get_lust_action() != Up && field.get_lust_action() != Down &&
            field.get_game_step()) {
          field.set_lust_action(Down);
          field.set_game_step(false);
        }
        break;
      case Left:
        if (field.get_lust_action() != Right &&
            field.get_lust_action() != Left && field.get_game_step()) {
          field.set_lust_action(Left);
          field.set_game_step(false);
        }
        break;
      case Right:
        if (field.get_lust_action() != Left &&
            field.get_lust_action() != Right && field.get_game_step()) {
          field.set_lust_action(Right);
          field.set_game_step(false);
        }
        break;
      case Action:
        // Принудительное движение по пользовательскому запросу
        switch (field.get_lust_action()) {
          case Up:
            field.snake_move_up();
            break;
          case Down:
            field.snake_move_down();
            break;
          case Left:
            field.snake_move_left();
            break;
          case Right:
            field.snake_move_right();
            break;
          default:
            break;
        }
        field.set_game_step(true);
        break;
      default:
        break;
    }
  }
}

#endif
