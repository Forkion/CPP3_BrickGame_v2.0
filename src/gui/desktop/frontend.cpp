#include "../../headers/snake/frontend.hpp"

/**
 * @namespace s21
 * @brief Пространство имен всего проекта.
 */
namespace s21 {

QBrickGame::QBrickGame() : QWidget(nullptr) {
  setStyleSheet(
      "QBrickGame {"
      "    background-color: #2b2b2b;"
      "    border: 3px solid #5CAF59;"
      "    border-radius: 10px;"
      "    padding: 5px;"
      "}");

  // Главный менеджер окон
  layout = new QHBoxLayout(this);

  // Подокона для игры и для панели информации
  gameWindow = new QWidget(this);
  infoWindow = new QWidget(this);

  // Контейнерная для игрового поля
  gameContainer = new QWidget(this);
  gameContainerLayout = new QVBoxLayout(gameContainer);

  // Подокона панели информации
  scoreWindow = new QWidget(infoWindow);
  maxScoreWindow = new QWidget(infoWindow);
  levelWindow = new QWidget(infoWindow);
  nextFigureWindow = new QWidget(infoWindow);

  // Подпись (статический текст)
  scoreLabel = new QLabel(scoreWindow);
  maxScoreLabel = new QLabel(maxScoreWindow);
  levelLabel = new QLabel(levelWindow);
  nextFigureLabel = new QLabel(nextFigureWindow);

  // Горизонтальное поцизионирование внутри информационных окон для
  // центрирование цекста
  scoreWindow->setLayout(new QHBoxLayout());
  scoreWindow->layout()->addWidget(scoreLabel);
  maxScoreWindow->setLayout(new QHBoxLayout());
  maxScoreWindow->layout()->addWidget(maxScoreLabel);
  levelWindow->setLayout(new QHBoxLayout());
  levelWindow->layout()->addWidget(levelLabel);
  nextFigureWindow->setLayout(new QHBoxLayout());
  nextFigureWindow->layout()->addWidget(nextFigureLabel);

  // Вертикальное расположение виджетов infoWindow
  infoLayour = new QVBoxLayout(infoWindow);

  // Количество пикселей на дюйм (DPI) установленных на машине
  screen = QApplication::primaryScreen();
  dpi = screen->logicalDotsPerInch();
}

/**
 * @brief Деструктор
 *
 * Вызывает метод остановки таймеров
 *
 */
QBrickGame::~QBrickGame() { stopTimers(); }

void QBrickGame::init() {
  gameContainerLayout->setContentsMargins(0, 0, 0, 0);
  gameContainerLayout->setSpacing(0);
  gameContainerLayout->addWidget(gameWindow);

  // стиль для окна с игровым полем
  gameWindow->setStyleSheet(
      "QWidget {"
      "    border: 2px solid #5CAF59;"
      "    border-radius: 5px;"
      "}");

  // Создаем виджет-обертку для gameWindow + линий
  gameContainer->setLayout(gameContainerLayout);

  // Теперь добавляем в главный горизонтальный layout

  layout->addWidget(gameContainer, 2);  // контейнер с игрой и линиями
  layout->addWidget(infoWindow, 1);

  // стиль для информационных окон
  infoWindow->setStyleSheet(
      "QWidget {"
      "    background-color: #2b2b2b;"
      "    border: 2px solid #5CAF59;"
      "    border-radius: 5px;"
      "    padding: 5px;"
      "    background-color: transparent;"
      "}");
  infoLayour->setContentsMargins(0, 0, 0, 0);

  infoLayour->addWidget(scoreWindow);

  infoLayour->addWidget(maxScoreWindow);

  infoLayour->addWidget(levelWindow);

  infoLayour->addWidget(nextFigureWindow);

  //   Стиль для лейблов
  QString labelStyle =
      "QLabel {"
      "    color: black;"
      "    font-weight: bold;"
      "}";

  scoreLabel->setAlignment(Qt::AlignCenter);
  scoreLabel->setStyleSheet(labelStyle);
  maxScoreLabel->setAlignment(Qt::AlignCenter);
  maxScoreLabel->setStyleSheet(labelStyle);
  levelLabel->setAlignment(Qt::AlignCenter);
  levelLabel->setStyleSheet(labelStyle);
  nextFigureLabel->setContentsMargins(0, 0, 0, 0);
  nextFigureLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
  nextFigureWindow->setStyleSheet(labelStyle);

  resize(20, 40);

  setupTimer();
}

void QBrickGame::show() { QWidget::show(); }

void QBrickGame::updateTimer(int time) { gameTimer->start(time); }

void QBrickGame::refreshDisplay() {
  GameInfo_t currentState = updateCurrentState();
  scoreLabel->setText(QString("Score:\n%1").arg(currentState.score));
  maxScoreLabel->setText(QString("Max score:\n%1").arg(currentState.max_score));
  levelLabel->setText(QString("Level:\n%1").arg(currentState.level));

  if (currentState.next) nextFigureLabel->setText("Next figure:");

  update();
}

void QBrickGame::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  updateFontSize();

  update();  // Перерисовываем чтобы фигура тоже обновилась
}

void QBrickGame::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  const GameInfo_t currentState = updateCurrentState();

  if (!currentState.matrix) {
    return;
  }

  QSize gameSize = gameWindow->size();

  // Вычисляем размер клетки чтобы сохранить пропорции
  int cellSize = qMin(gameSize.width() / 10, gameSize.height() / 20);
  int cellWidth = cellSize;
  int cellHeight = cellSize;

  // Центрируем поле если нужно
  int offsetX = ((gameSize.width() - cellWidth * 10) / 2) + 14;
  int offsetY = ((gameSize.height() - cellHeight * 20) / 2) + 12;

  painter.translate(gameWindow->geometry().topLeft());
  painter.translate(offsetX,
                    offsetY);  // дополнительное смещение для центрирования

  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 10; x++) {
      QRect cellRect(x * cellWidth, y * cellHeight, cellWidth - 3,
                     cellHeight - 3);
      switch (currentState.matrix[y][x]) {
        case 0:
          painter.fillRect(cellRect, QGRAY);
          break;

        case 1:
          painter.fillRect(cellRect, QRED);
          break;

        case 2:
          painter.fillRect(cellRect, QORANGE);
          break;

        case 3:
          painter.fillRect(cellRect, QYELLOW);
          break;

        case 4:
          painter.fillRect(cellRect, QPINK);
          break;

        case 5:
          painter.fillRect(cellRect, QGREEN);
          break;

        case 6:
          painter.fillRect(cellRect, QPURPLE);
          break;

        case 7:
          painter.fillRect(cellRect, QDARK_BLUE);
          break;

        case 9:
          painter.fillRect(cellRect, QBLACK);
          break;
      }
      if (updateCurrentState().next) {
        bool line = true;
        for (int i = 0; i < 10 && line; i++) {
          if (!currentState.matrix[y][i]) line = false;
        }
        if (line) {
          painter.fillRect(cellRect, QWHITE);
        }
      }
    }
  }

  if (!currentState.next) {
    return;
  }

  painter.resetTransform();

  // Вычисление области рисования следующей фигуры
  QSize infoSize = nextFigureWindow->size();
  int nextCellSize = qMin(infoSize.width() / 5, infoSize.height() / 5);
  QPoint labelBottomInWindow = nextFigureLabel->mapTo(
      nextFigureWindow,
      QPoint((nextFigureLabel->width() - nextCellSize * 3) / 2,
             nextFigureLabel->height() / 2));

  painter.translate(nextFigureWindow->mapTo(this, labelBottomInWindow));

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      QRect cellRect(x * nextCellSize, (y * nextCellSize), nextCellSize - 3,
                     nextCellSize - 3);
      switch (currentState.next[y][x]) {
        case 1:
          painter.fillRect(cellRect, QRED);
          break;

        case 2:
          painter.fillRect(cellRect, QORANGE);
          break;

        case 3:
          painter.fillRect(cellRect, QYELLOW);
          break;

        case 4:
          painter.fillRect(cellRect, QPINK);
          break;

        case 5:
          painter.fillRect(cellRect, QGREEN);
          break;

        case 6:
          painter.fillRect(cellRect, QPURPLE);
          break;

        case 7:
          painter.fillRect(cellRect, QDARK_BLUE);
          break;

        case 8:
          painter.fillRect(cellRect, QWHITE);
          break;
      }
    }
  }
}

void QBrickGame::keyPressEvent(QKeyEvent *event) {
  UserAction_t current_action = get_signal(event);
  GameState_t current_state = SwitchState(current_action);

  switch (current_state) {
    case GAME_PLAY:
      userInput(current_action, true);
      if (current_action == Action) updateTimer(updateCurrentState().speed);

      break;

    case GAME_PAUSE:
      break;

    case GAME_END:
      showGameEnd();
      return;
  }
  update();
}

UserAction_t QBrickGame::get_signal(QKeyEvent *event) {
  static UserAction_t current_action = Start;
  switch (event->key()) {
    case Qt::Key_Left:
      current_action = Left;
      break;
    case Qt::Key_Right:
      current_action = Right;
      break;
    case Qt::Key_Down:
      current_action = Down;
      break;
    case Qt::Key_Up:
      current_action = Up;
      break;
    case Qt::Key_Space:
      current_action = Action;
      break;
    case Qt::Key_Escape:
    case Qt::Key_Q:
      current_action = Terminate;
      break;
    case Qt::Key_P:
      current_action = Pause;
      break;
    default:
      current_action = Start;
  }
  return current_action;
}

void QBrickGame::setupTimer() {
  gameTimer = new QTimer(this);
  connect(gameTimer, &QTimer::timeout, this, &QBrickGame::gameUpdate);
  timerStatusFSM = new QTimer(this);
  connect(timerStatusFSM, &QTimer::timeout, this, [this]() {
    GameState_t current_state = SwitchState(Start);
    if (current_state == GAME_END) {
      showGameEnd();
    }
  });
  displayTimer = new QTimer(this);
  connect(displayTimer, &QTimer::timeout, this, &QBrickGame::refreshDisplay);
  displayTimer->start(100);

  QTimer::singleShot(500, this, [this]() {
    int cli_speed_microsec = 33000 - 3000 * updateCurrentState().level;
    int game_speed_millisec = cli_speed_microsec / 1000;
    gameTimer->start(game_speed_millisec);  // Только запускаем
  });

  timerStatusFSM->start(16);
}

void QBrickGame::stopTimers() {
  if (gameTimer) gameTimer->stop();
  if (timerStatusFSM) timerStatusFSM->stop();
  if (displayTimer) displayTimer->stop();
}

void QBrickGame::resize(int width, int height) {  // width = 20, height = 40
  const qreal mmToInch = 25.4;
  int cof = 4;
  int width_px = (width / mmToInch) * dpi * cof + 18;
  int height_px = (height / mmToInch) * dpi * cof + 2;

  QWidget::resize(width_px + width_px / 2, height_px - width * cof);
}

void QBrickGame::updateFontSize() {
  // Получаем размер infoWindow
  QSize infoSize = infoWindow->size();

  // Вычисляем размер шрифта на основе ширины infoWindow
  int baseFontSize =
      infoSize.width() / 15;  // Подбирайте коэффициент под ваш случай
  baseFontSize =
      qMax(15, qMin(25, baseFontSize));  // Ограничиваем min=10, max=25

  font.setPointSize(baseFontSize);
  font.setFamily("Monaco, Menlo, Consolas, Monospace");

  scoreLabel->setFont(font);
  maxScoreLabel->setFont(font);
  levelLabel->setFont(font);
  nextFigureLabel->setFont(font);
}

void QBrickGame::showGameEnd() {
  stopTimers();
  QDialog *gameEndDialog = new QDialog(this);
  gameEndDialog->setModal(true);
  gameEndDialog->setFixedSize(400, 150);
  QVBoxLayout *gameEndlayout = new QVBoxLayout(gameEndDialog);

  QLabel *gameEnd = new QLabel("GAME OVER");
  gameEnd->setAlignment(Qt::AlignCenter);
  gameEnd->setStyleSheet("font-size: 25px");

  GameInfo_t state = updateCurrentState();
  QLabel *score = new QLabel("Final Score: " + QString::number(state.score));
  score->setAlignment(Qt::AlignCenter);
  score->setStyleSheet("font-size: 25px;");

  QPushButton *closeBtn = new QPushButton("Exit Game");
  connect(closeBtn, &QPushButton::clicked, qApp, &QApplication::quit);

  gameEndlayout->addWidget(gameEnd);
  gameEndlayout->addWidget(score);
  gameEndlayout->addWidget(closeBtn);

  gameEndDialog->show();
}

void QBrickGame::gameUpdate() {
  GameState_t current_state = SwitchState(Start);

  switch (current_state) {
    case GAME_PLAY:
      if (updateCurrentState().next) {
        userInput(Down, false);  // автоматическое падение
      } else {
        userInput(Start, false);  // автоматическое последнее движение
      }
      break;
      // updateTimer(updateCurrentState().speed);
    case GAME_PAUSE:
      break;

    case GAME_END:
      showGameEnd();
      return;
  }
}

}  // namespace s21
