#ifndef BRICK_GAME
#define BRICK_GAME

#include <sys/wait.h>  // для waitpid()
#include <unistd.h>    // для fork()

#include <QApplication>
#include <QFont>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QObject>
#include <QPainter>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QThread>
#include <QTimer>
#include <QtWidgets>

/**
 * @namespace s21
 * @brief Пространство имен всего проекта змейки.
 */
namespace s21 {

/**
 * @class QGameLauncher
 *
 * @brief Главный класс лаунчера игр с графическим интерфейсом
 *
 * Предоставляет интерфейс для выбора игры (Тетрис/Змейка),
 * типа интерфейса (CLI/QT) и уровня сложности.
 */
class QGameLauncher : public QWidget {
  Q_OBJECT
 private:
  QStackedWidget *stack;  // Виджет для переключения между экранами
  QString buttonStyle;  // CSS стиль для элементов интерфейса
  QSpinBox *levelSpin = nullptr;  // Поле с цифрами где можно вводить значение
  QSlider *levelSlider = nullptr;  // Подзунок для смены уровня
  bool game;  // Флаг типа игры: false - тетрис, true - змейка
  bool view;  // Флаг типа интерфейса: false - CLI, true - QT
  short level;  // Уровень сложности в диапазоне 1-10
  bool game_loop;  // Флаг работы главного цикла приложения

 public:
  /**
   * @brief Конструктор класса QGameLauncher
   *
   * Инициализирует все элементы интерфейса, создает экраны меню
   * и настраивает графическое оформление.
   */
  QGameLauncher() : QWidget(nullptr) {
    stack = new QStackedWidget(this);

    buttonStyle =
        "* {"
        "    font-size: 17px;"
        "    font-weight: bold;"
        "}"
        "QLabel {"
        "    color: #333;"
        "    margin: 10px;"
        "    font-size: 25px;"
        "}"
        "QSpinBox {"
        "    font-size: 30px;"
        "    padding: 16px;"
        "    min-width: 300px;"
        "    min-height: 60px;"
        "    border: 2px solid #ccc;"
        "    border-radius: 5px;"
        "}"
        "QPushButton {"
        "    padding: 15px 30px;"
        "    min-width: 200px;"
        "    min-height: 50px;"
        "    background-color: #5CAF59;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "}";

    QWidget *setStart = creatreMainScreen();
    QWidget *setGame = creatreGameScreen();
    QWidget *setView = creatreViewScreen();
    QWidget *levelSpin = creatreLevelScreen();

    // Главный менеджер окон, отвечает за главное окно
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Создание и выбор стиля для внешней рамки
    QFrame *mainFrame = new QFrame();
    mainFrame->setFrameStyle(QFrame::Box | QFrame::Raised);  // Стиль рамки
    mainFrame->setLineWidth(3);     // Толщина линии
    mainFrame->setMidLineWidth(3);  // Толщина средней линии

    // Менеджер окон внутри рамки
    QVBoxLayout *frameLayout = new QVBoxLayout(mainFrame);

    // Создание и выбор стиля для рамки лейбла
    QFrame *lableFrame = new QFrame();
    lableFrame->setStyleSheet(
        "QFrame { border: 3px solid #5CAF59; border-radius: 10px; }");

    QVBoxLayout *wLableLayout = new QVBoxLayout(lableFrame);

    QLabel *nameLable = new QLabel(this);
    nameLable->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    nameLable->setText(QString(
        "<div style='line-height: 0.5; font-size: 50px;'>BRICK<br></div><div "
        "style='line-height: 0.5; font-size: 50px;'>GAME<br></div>"));

    wLableLayout->addWidget(nameLable);

    frameLayout->addWidget(lableFrame);
    frameLayout->addWidget(stack);

    mainLayout->addWidget(mainFrame);

    stack->addWidget(setStart);   // 0
    stack->addWidget(setGame);    // 1
    stack->addWidget(setView);    // 2
    stack->addWidget(levelSpin);  // 3
    game = false;
    view = false;
    level = 1;
    game_loop = true;

    stack->setCurrentIndex(0);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    resize();

    move(200, 100);
    QWidget::show();  // Явно показываем после установки позиции
  }
  /**
   * @brief Получить состояние игрового цикла
   *
   * @return true если приложение должно продолжать работу, false для выхода
   */
  bool get_game_loop() { return game_loop; }

  /**
   * @brief Создает главный экран меню
   *
   * Содержит кнопки: "Старт", "Уровень сложности", "Выход"
   *
   * @return Указатель на виджет главного экрана
   */
  QWidget *creatreMainScreen() {
    QWidget *Screen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(Screen);
    QPushButton *Start = new QPushButton("Старт");
    QPushButton *Level = new QPushButton("Уровень сложности");
    QPushButton *Exit = new QPushButton("Выход");

    Start->setStyleSheet(buttonStyle);
    Level->setStyleSheet(buttonStyle);
    Exit->setStyleSheet(buttonStyle);

    QObject::connect(Start, &QPushButton::clicked, this,
                     &QGameLauncher::pushStart);
    QObject::connect(Level, &QPushButton::clicked, this,
                     &QGameLauncher::pushLevel);
    QObject::connect(Exit, &QPushButton::clicked, this,
                     &QGameLauncher::pushExit);

    layout->addWidget(Start);
    layout->addWidget(Level);
    layout->addWidget(Exit);
    return Screen;
  }

  /**
   * @brief Создает экран выбора игры
   *
   * Содержит кнопки: "Тетрис", "Змейка", "Назад"
   *
   * @return Указатель на виджет экрана выбора игры
   */
  QWidget *creatreGameScreen() {
    QWidget *Screen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(Screen);
    QPushButton *Tetris = new QPushButton("Тетрис");
    QPushButton *Snake = new QPushButton("Змейка");
    QPushButton *Back = new QPushButton("Назад");

    Tetris->setStyleSheet(buttonStyle);
    Snake->setStyleSheet(buttonStyle);
    Back->setStyleSheet(buttonStyle);

    QObject::connect(Tetris, &QPushButton::clicked, this,
                     &QGameLauncher::pushTetris);
    QObject::connect(Snake, &QPushButton::clicked, this,
                     &QGameLauncher::pushSnake);
    QObject::connect(Back, &QPushButton::clicked, this,
                     &QGameLauncher::pushBack);

    layout->addWidget(Tetris);
    layout->addWidget(Snake);
    layout->addWidget(Back);
    return Screen;
  }

  /**
   * @brief Создает экран выбора интерфейса
   *
   * Содержит кнопки: "Консольный интерфейс (cli)", "Десктопный интерфейс (qt)",
   * "Назад"
   *
   * @return Указатель на виджет экрана выбора интерфейса
   *
   */
  QWidget *creatreViewScreen() {
    QWidget *Screen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(Screen);
    QPushButton *Console_cli = new QPushButton("Консольный интерфейс (cli)");
    QPushButton *Desctop_qt = new QPushButton("Десктопный интерфейс (qt)");
    QPushButton *Back = new QPushButton("Назад");

    Console_cli->setStyleSheet(buttonStyle);
    Desctop_qt->setStyleSheet(buttonStyle);
    Back->setStyleSheet(buttonStyle);

    QObject::connect(Console_cli, &QPushButton::clicked, this,
                     &QGameLauncher::pushCLI);
    QObject::connect(Desctop_qt, &QPushButton::clicked, this,
                     &QGameLauncher::pushQT);
    QObject::connect(Back, &QPushButton::clicked, this,
                     &QGameLauncher::pushBack);

    layout->addWidget(Console_cli);
    layout->addWidget(Desctop_qt);
    layout->addWidget(Back);

    return Screen;
  }

  /**
   * @brief Создает экран настройки уровня сложности
   *
   * Содержит QSpinBox для точного ввода и QSlider для визуальной настройки
   * уровня 1-10
   *
   * @return Указатель на виджет экрана настройки уровня
   */
  QWidget *creatreLevelScreen() {
    QWidget *Screen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(Screen);
    QPushButton *Back = new QPushButton("Назад");

    QLabel *title =
        new QLabel("Укажите уровень:\nМинимальный - 1\nМаксимальный - 10");
    title->setAlignment(Qt::AlignCenter);

    levelSpin = new QSpinBox();
    levelSpin->setRange(1, 10);
    levelSpin->setValue(1);
    levelSpin->setPrefix("Уровень: ");

    levelSlider = new QSlider(Qt::Horizontal);
    levelSlider->setRange(1, 10);
    levelSlider->setValue(1);

    Back->setStyleSheet(buttonStyle);
    title->setStyleSheet(buttonStyle);
    levelSpin->setStyleSheet(buttonStyle);

    QObject::connect(levelSlider, &QSlider::valueChanged, levelSpin,
                     &QSpinBox::setValue);
    QObject::connect(levelSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                     levelSlider, &QSlider::setValue);
    QObject::connect(Back, &QPushButton::clicked, this,
                     &QGameLauncher::pushBack);
    QObject::connect(levelSlider, &QSlider::valueChanged, this,
                     &QGameLauncher::setLevel);
    QObject::connect(levelSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                     this, &QGameLauncher::setLevel);

    layout->addWidget(title);
    layout->addSpacing(30);
    layout->addWidget(levelSpin);
    layout->addWidget(levelSlider);
    layout->addSpacing(30);
    layout->addWidget(Back);

    return Screen;
  }
 public slots:
  /**
   * @brief Переход к следующему экрану меню
   */
  void pushStart() { stack->setCurrentIndex(stack->currentIndex() + 1); }

  /**
   * @brief Возврат к предыдущему экрану меню
   *
   * При нахождении на экране уровня сложности возвращает на главный экран,
   * в остальных случаях - на предыдущий экран в цепочке.
   */
  void pushBack() {
    if (stack->currentIndex() == 3) {
      stack->setCurrentIndex(0);
    } else {
      stack->setCurrentIndex(stack->currentIndex() - 1);
    }
  }

  /**
   * @brief Выбор игры "Тетрис" и переход к выбору интерфейса
   */
  void pushTetris() {
    game = false;
    stack->setCurrentIndex(stack->currentIndex() + 1);
  }

  /**
   * @brief Выбор игры "Змейка" и переход к выбору интерфейса
   */
  void pushSnake() {
    game = true;
    stack->setCurrentIndex(stack->currentIndex() + 1);
  }

  /**
   * @brief Переход к экрану настройки уровня сложности
   */
  void pushLevel() {
    // Сбрасываем значения перед показом экрана
    if (levelSpin) levelSpin->setValue(1);
    if (levelSlider) levelSlider->setValue(1);
    setLevel(1);

    stack->setCurrentIndex(3);  // Переходим на экран уровня
  }

  /**
   * @brief Запуск игры с консольным интерфейсом (CLI)
   */
  void pushCLI() {
    view = false;
    startGame();
  }

  /**
   * @brief Запуск игры с десктопным интерфейсом (QT)
   */
  void pushQT() {
    view = true;
    startGame();
  }

  /**
   * @brief Завершение работы приложения
   *
   * Устанавливает флаг завершения игрового цикла и инициирует выход из
   * приложения.
   */
  void pushExit() {
    game_loop = false;
    QApplication::quit();
  }

  /**
   * @brief Установка уровня сложности
   *
   * @param newLevel Новый уровень сложности в диапазоне 1-10
   */
  void setLevel(int newLevel) { level = newLevel; }

  /**
   * @brief Запуск выбранной игры
   *
   * Формирует команду для запуска на основе выбранных настроек,
   * запускает игру в отдельном процессе и ожидает её завершения.
   * После завершения игры возвращает пользователя в главное меню
   * и сбрасывает уровень.
   */
  void startGame() {
    QString command;

    if (!game && !view) {
      command = "./tetris_cli";
    } else if (game && !view) {
      command = "./snake_cli";
    } else if (!game && view) {
      command = "./tetris_qt";
    } else {
      command = "./snake_qt";
    }
    if (level > 1) command += QString(" --level %1").arg(level);

    hide();

    // Запускаем процесс и ждём его завершения
    pid_t pid = fork();
    if (pid == 0) {
      // Дочерний процесс - запускаем игру
      execl("/bin/sh", "sh", "-c", command.toStdString().c_str(), NULL);
      exit(1);  // Если execl не сработал
    } else if (pid > 0) {
      // Родительский процесс - ждём завершения игры
      int status;
      waitpid(pid, &status, 0);  // Блокируемся пока игра не завершится
    }

    // Этот код выполнится только после завершения игры
    setLevel(1);
    show();
    raise();
    activateWindow();
    stack->setCurrentIndex(0);
  }

 public:
  /**
   * @brief Инициализация позиции окна на экране
   */
  void init() { QWidget::move(200, 100); }

  /**
   * @brief Установка размеров окна
   *
   * Вычисляет размеры окна на основе физических размеров (мм)
   * и DPI текущего экрана для обеспечения кроссплатформенного отображения.
   */
  void resize() {
    const qreal mmToInch = 25.4;
    int dpi = QApplication::primaryScreen()->logicalDotsPerInch();
    int hight = (200.0 / mmToInch) * dpi;
    int width = (100.0 / mmToInch) * dpi;

    QWidget::resize(width, hight);
    moveToCenter();
  }

  /**
   * @brief Центрирование окна на экране
   *
   * Вычисляет позицию для размещения окна по центру доступной области экрана.
   */
  void moveToCenter() {
    // Получаем геометрию доступного пространства на экране
    QScreen *screen = QGuiApplication::primaryScreen();

    QRect screenGeometry = screen->availableGeometry();
    // Вычисляем желаемую позицию для центрирования
    // Убедимся, что размер окна уже установлен
    if (width() > 0 && height() > 0) {
      int x = (screenGeometry.width() - width()) / 2;
      int y = (screenGeometry.height() - height()) / 2;

      // Устанавливаем позицию
      move(x, y);
    }
  }
};

}  // namespace s21

#endif  // BRICK_GAME
