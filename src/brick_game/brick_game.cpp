#include "../headers/brick_game.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
  bool status = true;
  int res = 0;

  while (status) {
    QApplication app(argc, argv);
    s21::QGameLauncher gameMenu;

    gameMenu.show();
    gameMenu.init();
    res = app.exec();
    status = gameMenu.get_game_loop();
  }

  return res;
}
