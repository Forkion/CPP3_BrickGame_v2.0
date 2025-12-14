#include "../../headers/tetris/frontend.hpp"

// /*!
//  * @brief Выводит запрос на включение подсказки при падении фигуры
//  */
// void use_option(){
//   printf("\nPlease, enter \"Y\" if you would like to use a hint or enter
//   \"N\" if you not\n"); char check_hint = 0; while (1) {
//     check_hint = getchar(); // Ждёт Enter
//     // Очистка буфера (если ввели "abc" вместо одного символа)
//     while (getchar() != '\n');

//     if (check_hint == 'y' || check_hint == 'Y') {
//         hint = SUCCESS;
//         break;
//     }
//     else if (check_hint == 'n' || check_hint == 'N') {
//         hint = ERROR;
//         break;
//     }
//     else {
//       if(!hint)
//         printf("Invalid character, enter \"Y\" if you would like to use a
//         hint or enter \"N\" if you not\n");
//     }
//   }
// }

void init_game_frontend(WINDOW **windows_game_info) {
  initscr();
  keypad(stdscr, TRUE);
  *windows_game_info =
      newwin(HEIGHT_GAME + 2, WIDTH_GAME_INFO_FIELD, 1, WIDTH_GAME + 1);
  curs_set(0);  // Убирает отображение курсора
  cbreak();  // Режим непосредственного ввода (отключение буферизации)
  noecho();  // Отключение вывод на экран вводимых символов
  char any_key[] = "Press any key to start";

  start_color();
  init_pair(DEFOLT_PANEL, COLOR_WHITE,
            COLOR_BLACK);  // Стандартная палитра поля
  init_pair(FIGUR_l, COLOR_BLACK, COLOR_RED);  // Красная прямая полоса l
  init_pair(FIGUR_L_MIRRORED, COLOR_BLACK, COLOR_ORANGE);  // Оранжевая буква Г
  init_pair(FIGUR_L, COLOR_BLACK, COLOR_YELLOW);  // Желтая буква L
  init_pair(FIGUR_O, COLOR_BLACK, COLOR_PINK);  // Розовый квадрат
  init_pair(FIGUR_S, COLOR_BLACK, COLOR_GREEN);  // Зелёный зигзаг S
  init_pair(FIGUR_Z, COLOR_BLACK,
            COLOR_PURPLE);  // Фиолетовый обратный зигзаг Z
  init_pair(FIGUR_T, COLOR_BLACK, COLOR_DARK_BLUE);  // Синяя буква T
  init_pair(FULL_STR, COLOR_WHITE, COLOR_WHITE);  // Белая заполненная строка

  attron(A_BOLD | DEFOLT_PANEL);
  attron(A_UNDERLINE);
  mvprintw(1, 2 + 11, "            ");
  mvprintw(2, 2 + 11, "|  TETRIS  |");
  attroff(A_UNDERLINE);
  mvprintw(2 + 2, 2, "OPTIONS:");
  mvprintw(2 + 4, 2, "Move figure on left - \"left arrow\"");
  mvprintw(2 + 6, 2, "Move figure on right - \"right arrow\"");
  mvprintw(2 + 8, 2, "Flip figure - \"up arrow\"");
  mvprintw(2 + 10, 2, "Move figure on down - \"down arrow\"");
  mvprintw(2 + 12, 2, "Drop figure - \"SPACE\"");
  mvprintw(2 + 14, 2, "Pause - \"p\"");
  mvprintw(2 + 16, 2, "Exit - \"q\"");
  mvprintw(2 + 20, sizeof(any_key) / 2 - 3, "%s", any_key);
  draw_rectangle_window(stdscr, 0, 24, 0, 38, 1);
  attroff(A_BOLD | DEFOLT_PANEL);

  refresh();
  getch();
  werase(stdscr);
  timeout(10);
}

void print_color_sign(WINDOW *window_game, int i, int j, chtype set_color,
                      char sign) {
  wattron(window_game, set_color);
  mvwprintw(window_game, i, j, "%c", sign);
  wattroff(window_game, set_color);
}

void print_next_figure(WINDOW *window_game, int game_field, int i, int j,
                       bool num) {
  if (game_field == 1)
    print_color_sign(window_game, i, j, COLOR_PAIR(FIGUR_l), 32);
  else if (game_field == 2)
    print_color_sign(window_game, i, j, COLOR_PAIR(FIGUR_L_MIRRORED), 32);
  else if (game_field == 3)
    print_color_sign(window_game, i, j, COLOR_PAIR(FIGUR_L), 32);
  else if (game_field == 4)
    print_color_sign(window_game, i, j, COLOR_PAIR(FIGUR_O), 32);
  else if (game_field == 5)
    print_color_sign(window_game, i, j, COLOR_PAIR(FIGUR_S), 32);
  else if (game_field == 6)
    print_color_sign(window_game, i, j, COLOR_PAIR(FIGUR_Z), 32);
  else if (game_field == 7)
    print_color_sign(window_game, i, j, COLOR_PAIR(FIGUR_T), 32);
  else if (num)
    print_color_sign(window_game, i, j, COLOR_PAIR(DEFOLT_PANEL), 32);
}

void print_color_matrix(int i, int j, chtype set_color, char sign) {
  attron(set_color);
  mvprintw(i + 1, j + 1, "%c", sign = ' ');
  attroff(set_color);
}

void print_figure(int game_field, int i, int j, int figure) {
  if (game_field == 1)
    print_color_matrix(i, j, COLOR_PAIR(FIGUR_l), figure);
  else if (game_field == 2)
    print_color_matrix(i, j, COLOR_PAIR(FIGUR_L_MIRRORED), figure);
  else if (game_field == 3)
    print_color_matrix(i, j, COLOR_PAIR(FIGUR_L), figure);
  else if (game_field == 4)
    print_color_matrix(i, j, COLOR_PAIR(FIGUR_O), figure);
  else if (game_field == 5)
    print_color_matrix(i, j, COLOR_PAIR(FIGUR_S), figure);
  else if (game_field == 6)
    print_color_matrix(i, j, COLOR_PAIR(FIGUR_Z), figure);
  else if (game_field == 7)
    print_color_matrix(i, j, COLOR_PAIR(FIGUR_T), figure);
  else if (game_field == 8)
    print_color_matrix(i, j, COLOR_PAIR(FULL_STR), figure);
  else if (game_field == 9)
    print_color_matrix(i, j, COLOR_PAIR(FIGUR_T), figure);
  else if (!game_field)
    print_color_matrix(i, j, COLOR_PAIR(DEFOLT_PANEL), figure);
}

void print_game_window(const GameInfo_t field) {
  int score = 0;
  int scale_i = 0;
  int scale_j = 0;
  for (int i = 0; i < HEIGHT_GAME; i++) {
    scale_i = i / SIZE;
    for (int j = 0; j < WIDTH_GAME; j++) {
      scale_j = j / (SIZE * SIZE_FIGURE);
      print_figure(field.matrix[scale_i][scale_j], i + 1, j + 1,
                   field.matrix[scale_i][scale_j]);
      if (field.matrix[scale_i][scale_j]) score += 1;
    }
#ifdef TETRIS_CLI
    if (score == WIDTH_GAME) {
      for (int l = 0; l < WIDTH_GAME; ++l)
        print_color_matrix(i + 1, l + 1, COLOR_PAIR(FULL_STR), 32);
    }
#endif
    score = 0;
  }

  draw_rectangle_window(stdscr, 1, HEIGHT_GAME + 2, 1, WIDTH_GAME + 2, 1);
}

bool print_game_info_window(WINDOW *windows_game_info, const GameInfo_t field) {
  bool res = TRUE;
  char Score[100] = {0};
  char Max_Score[100] = {0};

  wbkgd(windows_game_info, COLOR_PAIR(DEFOLT_PANEL) | A_BOLD);

  sprintf(Score, "%d", field.score);
  mvwprintw(windows_game_info, SCORE_INFO_HEIGHT - 2, WIDTH_INFO_WINDOW, "%-1s",
            "Score:");
  for (int i = 0; Score[i] != 0; ++i)
    mvwprintw(windows_game_info, SCORE_INFO_HEIGHT, WIDTH_INFO_WINDOW + i, "%c",
              Score[i]);

  printf_line(windows_game_info, SCORE_INFO_HEIGHT + 1);

  sprintf(Max_Score, "%d", field.max_score);
  mvwprintw(windows_game_info, MAX_SCORE_INFO, WIDTH_INFO_WINDOW, "%-1s",
            "Max score:");
  for (int i = 0; Max_Score[i] != 0; ++i)
    mvwprintw(windows_game_info, MAX_SCORE_INFO + 2, WIDTH_INFO_WINDOW + i,
              "%c", Max_Score[i]);

  printf_line(windows_game_info, MAX_SCORE_INFO + 3);

  if (field.next) {
    mvwprintw(windows_game_info, NEXT_FIGURE, WIDTH_INFO_WINDOW, "%-1s",
              "Next figure:");
    int scale_i = 0;
    int scale_j = 0;
    for (int i = 0; i < HEIGHT_FIGURE / 2; ++i) {
      scale_i = i / SIZE;
      for (int j = 0; j < WIDTH_FIGURE; ++j) {
        scale_j = j / (SIZE * SIZE_FIGURE);
        print_next_figure(windows_game_info, field.next[scale_i][scale_j],
                          NEXT_FIGURE + SIZE + 1 + i, WIDTH_INFO_WINDOW + j,
                          1);  // ДЛЯ ПЕЧАТИ ВНУТРИ ОНКА С ИГРОВОЙ ИНФОРАЦИЕЙ
      }
    }
  }
  printf_line(windows_game_info, LEVEL);

  mvwprintw(windows_game_info, LEVEL + 2, WIDTH_INFO_WINDOW, "%-1s", "Level:");
  mvwprintw(windows_game_info, LEVEL + 4, WIDTH_INFO_WINDOW, "%d", field.level);

  draw_rectangle_window(windows_game_info, 0, HEIGHT_GAME + 1, 0,
                        WIDTH_GAME_INFO_FIELD - 1, 0);

  wrefresh(windows_game_info);

  return res;
}

void printf_line(WINDOW *windows_game_info, int place) {
  for (int i = 2; i < WIDTH_GAME_INFO_FIELD - 1; ++i)
    print_color_sign(windows_game_info, place, i, A_UNDERLINE, 32);
}

void draw_rectangle_window(WINDOW *game_info, int top, int bottom, int left,
                           int right, bool status) {
  draw_horizontal_line_window(game_info, top, left, right, ACS_URCORNER,
                              ACS_HLINE);
  if (status) {
    draw_vertical_lines_window(game_info, top, bottom, left, ACS_VLINE);
    mvwaddch(game_info, top, left, ACS_ULCORNER);
    mvwaddch(game_info, bottom, left, ACS_LLCORNER);
  }
  draw_vertical_lines_window(game_info, top, bottom, right, ACS_VLINE);
  draw_horizontal_line_window(game_info, bottom, left, right, ACS_LRCORNER,
                              ACS_HLINE);
}

void draw_horizontal_line_window(WINDOW *game_info, int y, int left, int right,
                                 chtype right_corner, chtype line) {
  //   mvwaddch(game_info, y, left, left_corner);
  for (int x = left + 1; x < right; x++) {
    mvwaddch(game_info, y, x, line);
  }
  mvwaddch(game_info, y, right, right_corner);
}

void draw_vertical_lines_window(WINDOW *game_info, int top, int bottom, int x,
                                chtype line) {
  for (int y = top + 1; y < bottom; y++) {
    mvwaddch(game_info, y, x, line);
  }
}

void close_game_frontend(WINDOW *windows_game_info, int score) {
  char over[] = "GAME OVER";
  werase(stdscr);
  mvprintw(HEIGHT_GAME / 2, WIDTH_GAME + WIDTH_INFO_WINDOW - sizeof(over) / 2,
           "%s", over);
  mvprintw(HEIGHT_GAME / 2 + 2,
           WIDTH_GAME + WIDTH_INFO_WINDOW - sizeof(over) / 2, "Final score: %d",
           score);
  refresh();
  sleep(2);
  delwin(windows_game_info);
  endwin();
  printf("\033[0m");  // сброс цветов
  fflush(stdout);     // сброс буфера
}
