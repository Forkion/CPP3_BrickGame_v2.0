#include <gtest/gtest.h>

#include <iostream>

#include "../headers/snake/backend.hpp"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
using namespace s21;

TEST(test_constructor_destructor, test1) { GameFieldInfo field; };

TEST(test_move_left, test2) {
  GameFieldInfo field;
  std::pair<short, short> head{HEIGHT / 2, WIDTH / 2 - 1};
  field.snake_move_left();
  EXPECT_TRUE(field.get_snake_head_pos() == head);
};

TEST(test_move_right, test3) {
  GameFieldInfo field;
  std::pair<short, short> head{HEIGHT / 2, WIDTH / 2 + 1};
  field.snake_move_right();
  EXPECT_TRUE(field.get_snake_head_pos() == head);
};

TEST(test_move_up, test4) {
  GameFieldInfo field;
  std::pair<short, short> head{HEIGHT / 2 - 1, WIDTH / 2};
  field.snake_move_up();
  EXPECT_TRUE(field.get_snake_head_pos() == head);
};

TEST(test_move_down, test5) {
  GameFieldInfo field;
  std::pair<short, short> head{HEIGHT / 2 + 1, WIDTH / 2 + 1};
  field.snake_move_right();
  field.snake_move_down();
  EXPECT_TRUE(field.get_snake_head_pos() == head);
};

TEST(test_setters_getters, test6) {
  GameFieldInfo field;
  field.set_game_step(false);
  EXPECT_TRUE(field.get_game_step() == false);

  field.set_level(2);
  EXPECT_TRUE(field.get_level() == 2);

  field.set_lust_action(Left);
  EXPECT_TRUE(field.get_lust_action() == Left);

  field.set_score(5);
  EXPECT_TRUE(field.get_score() == 5);

  field.set_speed(10);
  EXPECT_TRUE(field.get_speed() == 10);
};

TEST(test_move_on_apple, test7) {
  GameFieldInfo field;
  field.set_apple_pos(std::pair<short, short>{HEIGHT / 2 - 2, WIDTH / 2});
  field.snake_move_up();
  field.snake_move_up();
  EXPECT_TRUE(field.get_size_snake_body() == 5);
};

TEST(test_dead_from_wall, test8) {
  GameFieldInfo field;
  while (!field.get_game_end()) field.snake_move_right();
  std::pair<short, short> head = {HEIGHT / 2, WIDTH - 1};
  EXPECT_TRUE(field.get_snake_head_pos() == head);
};

TEST(test_dead_from_tail, test9) {
  GameFieldInfo field;
  field.set_apple_pos(std::pair<short, short>{HEIGHT / 2, WIDTH / 2 + 1});
  field.snake_move_right();
  field.snake_move_down();
  field.snake_move_left();
  std::pair<short, short> head = {HEIGHT / 2 + 1, WIDTH / 2};
  EXPECT_TRUE(field.get_game_end() == true);
  EXPECT_TRUE(field.get_snake_head_pos() == head);
};

TEST(test_gameinfo_getter, test9) {
  GameFieldInfo field1;
  GameInfo_t field = field1.updateCurrentState();
  EXPECT_TRUE(field.score == field1.get_score());
  EXPECT_TRUE(field.level == field1.get_level());
  EXPECT_TRUE(field.speed == field1.get_speed());
};
