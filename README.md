## BrickGame_v2.0
### Реализация двух игр консоли BrickGame

1. Тетриса на языке СИ для консольного интерфейса (ncurses.h) и для десктопного интерфейса (qt)

2. Змейка на языке С++ для консольного интерфейса (ncurses.h) и для десктопного интерфейса (qt)

## Сборка:
Сборка проекта с запуском меню: make

Тесты: make test

Покрытие: make gcov_report

Утечки: make valgrind_test

Форматирование кода: make clang

Создание архива .gz: make dist

Создание документации к коду: make dvi

## Структура
```
src/
├── CMakeLists.txt		# Создание Makefile с помощью Cmake (создавать в папке build)
├── Doxyfile			# Конфигурация Doxygen
├── FSM.jpg   			# диаграмма состояний конечного автомата (FSM)
├── Makefile
├── brick_game
│   ├── brick_game.cpp	# файл с запуском игрового меню
│   ├── fsm.cpp			# логика конечного автомата (FSM)
│   ├── snake			# логика змейки (С++)
│   └── tetris			# логика тетриса (СИ)
├── gui
│   ├── cli				# консольный интерфейс
│   └── desktop			# десктопный интерфейс
├── headers				# заголовочные файлы
│   ├── brick_game.hpp  # код к игровому меню
│   ├── defines.h
│   ├── fsm.hpp
│   ├── snake
│   └── tetris
└── tests				# unit-тесты
```
