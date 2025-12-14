#include <check.h>
#include <stdio.h>

#include "../headers/tetris/backend.h"

START_TEST(tetris_move_left) {
  GameInfo field = {0};
  GameInfo field_standart = {0};
  init_game_backend(&field);
  init_game_backend(&field_standart);
  for (int i = 0; i < 2; ++i)
    for (int j = 3; j < 8; j++) field.matrix[i][j] = 0;
  clean_matrix_figure(&field.figure);
  clean_matrix_figure(&field_standart.figure);
  set_figure(&field.figure, FIGUR_l);
  set_figure(&field_standart.figure, FIGUR_l);
  for (int i = 0; i < WIDTH; ++i) {
    move_left(&field);
  }
  field_standart.figure.x = 0;

  ck_assert_int_eq(field.figure.x, field_standart.figure.x);
  close_game_backend(&field);
  close_game_backend(&field_standart);
}
END_TEST

START_TEST(tetris_move_right) {
  GameInfo field = {0};
  GameInfo field_standart = {0};
  init_game_backend(&field);
  init_game_backend(&field_standart);
  for (int i = 0; i < 2; ++i)
    for (int j = 3; j < 8; j++) field.matrix[i][j] = 0;
  clean_matrix_figure(&field.figure);
  clean_matrix_figure(&field_standart.figure);
  set_figure(&field.figure, FIGUR_l);
  set_figure(&field_standart.figure, FIGUR_l);
  for (int i = 0; i < 2; ++i) {
    move_right(&field);
  }
  field_standart.figure.x = 6;

  ck_assert_int_eq(field.figure.x, field_standart.figure.x);
  close_game_backend(&field);
  close_game_backend(&field_standart);
}
END_TEST

START_TEST(tetris_move_down) {
  GameInfo field = {0};
  GameInfo field_standart = {0};
  init_game_backend(&field);
  init_game_backend(&field_standart);
  for (int i = 0; i < 2; ++i)
    for (int j = 3; j < 8; j++) field.matrix[i][j] = 0;
  clean_matrix_figure(&field.figure);
  clean_matrix_figure(&field_standart.figure);
  set_figure(&field.figure, FIGUR_l);
  set_figure(&field_standart.figure, FIGUR_l);
  while (move_down(&field));
  for (int i = 0; i < 2; ++i)
    for (int j = 3; j < 8; j++) field.matrix[i][j] = 0;
  clean_matrix_figure(&field.figure);
  set_figure(&field.figure, FIGUR_l);
  while (move_down(&field));

  for (int i = 0; i < 2; ++i)
    for (int j = 4; j < 8; j++) field_standart.matrix[HEIGHT - 1 - i][j] = 1;
  for (int i = 17; i < HEIGHT; ++i)
    for (int j = 0; j < WIDTH; j++)
      ck_assert_int_eq(field.matrix[i][j], field_standart.matrix[i][j]);
  close_game_backend(&field);
  close_game_backend(&field_standart);
}
END_TEST

START_TEST(tetris_move_up_L_figure) {
  GameInfo field = {0};
  GameInfo field_standart = {0};
  init_game_backend(&field);
  init_game_backend(&field_standart);
  for (int i = 0; i < 2; ++i)
    for (int j = 3; j < 8; j++) field.matrix[i][j] = 0;
  clean_matrix_figure(&field.figure);
  clean_matrix_figure(&field_standart.figure);
  set_figure(&field.figure, FIGUR_l);
  set_figure(&field_standart.figure, FIGUR_l);
  move_up(&field);
  field.figure.x = -2;
  field_standart.figure.x = -2;
  move_up(&field);
  move_up(&field);
  move_up(&field);
  field_standart.figure.x = 0;

  ck_assert_int_eq(field.figure.x, field_standart.figure.x);
  close_game_backend(&field);
  close_game_backend(&field_standart);
}
END_TEST

START_TEST(tetris_move_up_O_figure) {
  GameInfo field = {0};
  GameInfo field_standart = {0};
  init_game_backend(&field);
  init_game_backend(&field_standart);
  for (int i = 0; i < 2; ++i)
    for (int j = 3; j < 8; j++) field.matrix[i][j] = 0;
  clean_matrix_figure(&field.figure);
  clean_matrix_figure(&field_standart.figure);
  set_figure(&field.figure, FIGUR_O);
  set_figure(&field_standart.figure, FIGUR_O);
  move_up(&field);
  move_up(&field);
  move_up(&field);
  ck_assert_int_eq(field.figure.x, field_standart.figure.x);
  close_game_backend(&field);
  close_game_backend(&field_standart);
}
END_TEST

START_TEST(tetris_spawn) {
  GameInfo field = {0};
  GameInfo field_standart = {0};
  init_game_backend(&field);
  init_game_backend(&field_standart);
  for (int i = 0; i < 2; ++i)
    for (int j = 3; j < 8; j++) field.matrix[i][j] = 0;
  clean_matrix_figure(&field.figure);
  clean_matrix_figure(&field_standart.figure);

  for (int i = 0; i < 7; ++i) {
    set_figure(&field.figure, FIGUR_l + i);
    field_standart.figure.number = 1 + i;
    ck_assert_int_eq(field.figure.number, field_standart.figure.number);
    clean_matrix_figure(&field.figure);
    clean_matrix_figure(&field_standart.figure);
  }

  close_game_backend(&field);
  close_game_backend(&field_standart);
}
END_TEST

START_TEST(tetris_move_offset) {
  GameInfo field = {0};
  GameInfo field_standart = {0};
  init_game_backend(&field);
  init_game_backend(&field_standart);
  for (int i = 0; i < 2; ++i)
    for (int j = 3; j < 8; j++) field.matrix[i][j] = 0;
  clean_matrix_figure(&field.figure);
  clean_matrix_figure(&field_standart.figure);
  set_figure(&field.figure, FIGUR_l);
  set_figure(&field_standart.figure, FIGUR_l);

  for (int j = 0; j < 4; ++j)
    for (int i = 0; i < WIDTH; ++i) {
      field_standart.matrix[HEIGHT - 1 - j][i] = 1;
      field.matrix[HEIGHT - 1 - j][i] = 1;
    }
  while (move_down(&field));
  field_standart.score = 1500;
  field_standart.level = 3;
  field_standart.speed = 24000;
  ck_assert_int_eq(field.score, field_standart.score);
  ck_assert_int_eq(field.speed, field_standart.speed);
  ck_assert_int_eq(field.level, field_standart.level);

  close_game_backend(&field);
  close_game_backend(&field_standart);
}
END_TEST

Suite *tests_tetris() {
  Suite *suite_create_matrix = suite_create("tetris");
  TCase *group = tcase_create("tetris");

  tcase_add_test(group, tetris_move_left);
  tcase_add_test(group, tetris_move_right);
  tcase_add_test(group, tetris_move_down);
  tcase_add_test(group, tetris_move_up_L_figure);
  tcase_add_test(group, tetris_move_up_O_figure);
  tcase_add_test(group, tetris_spawn);
  tcase_add_test(group, tetris_move_offset);

  suite_add_tcase(suite_create_matrix, group);

  return suite_create_matrix;
}

int main() {
  SRunner *suite_runner = srunner_create(tests_tetris());

  srunner_run_all(suite_runner, CK_NORMAL);
  srunner_free(suite_runner);

  return 0;
}
