# Компиляторы и флаги
CC = gcc
CXX = g++
CFLAGS = -std=c11 -Wall -Werror -Wextra -g -fPIC
CXXFLAGS = -std=c++17 -Wall -Werror -Wextra -g -fPIC

# Директории с заголовками
INCLUDES = -Iheaders -Iheaders/tetris -Iheaders/snake

# Определение ОС
UNAME_S := $(shell uname -s)

# Qt флаги
QT_CFLAGS = $(shell pkg-config --cflags Qt6Core Qt6Widgets Qt6Gui 2>/dev/null)
QT_LIBS = $(shell pkg-config --libs Qt6Core Qt6Widgets Qt6Gui 2>/dev/null)

ifneq ($(QT_CFLAGS),)
    CXXFLAGS += $(QT_CFLAGS)
else
    ifeq ($(UNAME_S), Linux)
        QT_LIBS = -lQt6Core -lQt6Widgets -lQt6Gui
    else ifeq ($(UNAME_S), Darwin)
        QT_LIBS = -framework QtCore -framework QtWidgets -framework QtGui
    endif
endif

ifeq ($(UNAME_S), Linux)
    CLI_FLAGS = -lncurses
    TEST_C_FLAGS = -lcheck -lsubunit -lm -lrt -lpthread
    TEST_CPP_FLAGS = -lgtest -lgtest_main -lpthread
    OPEN_CMD = xdg-open
endif
ifeq ($(UNAME_S), Darwin)
    CLI_FLAGS = -lncurses
    TEST_C_FLAGS = -lcheck -lm
    TEST_CPP_FLAGS = -lgtest -lgtest_main
    OPEN_CMD = open
endif

# Исходные файлы
TETRIS_BACKEND_C = brick_game/tetris/backend.c
SNAKE_BACKEND_CPP = brick_game/snake/backend.cpp
FSM_CPP = brick_game/fsm.cpp
BRICK_GAME_CPP = brick_game/brick_game.cpp
FRONTEND_CLI_CPP = gui/cli/frontend.cpp
FRONTEND_QT_CPP = gui/desktop/frontend.cpp
FRONTEND_QT_HEADER = headers/snake/frontend.hpp
TEST_C = tests/test.c
TEST_CPP = tests/test.cpp

# Объектные файлы
TETRIS_BACKEND_O_CLI = build/tetris_backend_cli.o
TETRIS_BACKEND_O_QT = build/tetris_backend_qt.o
SNAKE_BACKEND_O = build/snake_backend.o
FSM_TETRIS_CLI_O = build/fsm_tetris_cli.o
FSM_SNAKE_CLI_O = build/fsm_snake_cli.o
FSM_TETRIS_QT_O = build/fsm_tetris_qt.o
FSM_SNAKE_QT_O = build/fsm_snake_qt.o
BRICK_GAME_O = build/brick_game.o
FRONTEND_CLI_O = build/frontend_cli.o
FRONTEND_QT_O = build/frontend_qt.o
TEST_C_O = build/test_c.o
TEST_CPP_O = build/test_cpp.o

# MOC файлы
MOC = moc
MOC_FRONTEND_CPP = build/moc_frontend.cpp
MOC_BRICK_GAME_CPP = build/moc_brick_game.cpp
MOC_FRONTEND_O = build/moc_frontend.o
MOC_BRICK_GAME_O = build/moc_brick_game.o

# Исполняемые файлы
TETRIS_CLI = build/tetris_cli
SNAKE_CLI = build/snake_cli
TETRIS_QT = build/tetris_qt
SNAKE_QT = build/snake_qt
LAUNCHER = build/brick_game_launcher
TEST_C_BIN = build/test_c
TEST_CPP_BIN = build/test_cpp

# Цели по умолчанию
.PHONY: all install run test gcov_report valgrind_test dvi dist clean uninstall clang

all: install

# ============================================================================
# КОМПИЛЯЦИЯ ОБЪЕКТНЫХ ФАЙЛОВ
# ============================================================================
# C файлы
$(TETRIS_BACKEND_O_CLI): $(TETRIS_BACKEND_C)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -DTETRIS_CLI -c $< -o $@

$(TETRIS_BACKEND_O_QT): $(TETRIS_BACKEND_C)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -DTETRIS_QT -c $< -o $@

$(TEST_C_O): $(TEST_C)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# C++ файлы
$(SNAKE_BACKEND_O): $(SNAKE_BACKEND_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# FSM с разными макросами
$(FSM_TETRIS_CLI_O): $(FSM_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DTETRIS_CLI -c $< -o $@

$(FSM_SNAKE_CLI_O): $(FSM_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DSNAKE_CLI -c $< -o $@

$(FSM_TETRIS_QT_O): $(FSM_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DTETRIS_QT -c $< -o $@

$(FSM_SNAKE_QT_O): $(FSM_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DSNAKE_QT -c $< -o $@

$(BRICK_GAME_O): $(BRICK_GAME_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(FRONTEND_CLI_O): $(FRONTEND_CLI_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(FRONTEND_QT_O): $(FRONTEND_QT_CPP) $(MOC_FRONTEND_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(QT_CFLAGS) -c $< -o $@

$(TEST_CPP_O): $(TEST_CPP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# MOC генерация
$(MOC_FRONTEND_CPP): headers/snake/frontend.hpp
	@mkdir -p build
	$(MOC) $< -o $@

$(MOC_BRICK_GAME_CPP): headers/brick_game.hpp
	@mkdir -p build
	$(MOC) $< -o $@

$(MOC_FRONTEND_O): $(MOC_FRONTEND_CPP)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(QT_CFLAGS) -c $< -o $@

$(MOC_BRICK_GAME_O): $(MOC_BRICK_GAME_CPP)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(QT_CFLAGS) -c $< -o $@

# ============================================================================
# УСТАНОВКА И ЗАПУСК
# ============================================================================
install: $(TETRIS_CLI) $(SNAKE_CLI) $(TETRIS_QT) $(SNAKE_QT) $(LAUNCHER)
	cd build && ./brick_game_launcher

# ============================================================================
# ЛИНКОВКА ИСПОЛНЯЕМЫХ ФАЙЛОВ
# ============================================================================
# Консольные версии (ncurses)
$(TETRIS_CLI): $(TETRIS_BACKEND_O_CLI) $(FSM_TETRIS_CLI_O) $(FRONTEND_CLI_O)
	$(CXX) $^ -o $@ $(CLI_FLAGS)

$(SNAKE_CLI): $(SNAKE_BACKEND_O) $(FSM_SNAKE_CLI_O) $(FRONTEND_CLI_O)
	$(CXX) $^ -o $@ $(CLI_FLAGS)

# QT версии (с MOC)
$(TETRIS_QT): $(TETRIS_BACKEND_O_QT) $(FSM_TETRIS_QT_O) $(FRONTEND_QT_O) $(MOC_FRONTEND_O)
	$(CXX) $^ -o $@ $(QT_LIBS)

$(SNAKE_QT): $(SNAKE_BACKEND_O) $(FSM_SNAKE_QT_O) $(FRONTEND_QT_O) $(MOC_FRONTEND_O)
	$(CXX) $^ -o $@ $(QT_LIBS)

$(LAUNCHER): $(BRICK_GAME_O) $(MOC_BRICK_GAME_O)
	$(CXX) $^ -o $@ $(QT_LIBS)
# Тесты
$(TEST_C_BIN): $(TEST_C_O) $(TETRIS_BACKEND_O_CLI)
	$(CC) $^ -o $@ $(TEST_C_FLAGS) -fprofile-arcs -ftest-coverage

$(TEST_CPP_BIN): $(TEST_CPP_O) $(SNAKE_BACKEND_O)
	$(CXX) $^ -o $@ $(TEST_CPP_FLAGS) -fprofile-arcs -ftest-coverage

# ============================================================================
# ТЕСТИРОВАНИЕ
# ============================================================================
test: $(TEST_C_BIN) $(TEST_CPP_BIN)
	./$(TEST_C_BIN)
	./$(TEST_CPP_BIN)

valgrind_test: $(TEST_C_BIN) $(TEST_CPP_BIN)
	valgrind --trace-children=yes --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=definite,possible -q ./$(TEST_C_BIN)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes -q ./$(TEST_CPP_BIN)

# ============================================================================
# ОСТАЛЬНЫЕ ЦЕЛИ
# ============================================================================
gcov_report: clean_coverage
	@mkdir -p build
	# Перекомпилируем всё с gcov флагами
	$(CC) $(CFLAGS) $(INCLUDES) -DTETRIS_CLI -fprofile-arcs -ftest-coverage -c $(TETRIS_BACKEND_C) -o build/tetris_backend_gcov.o
	$(CC) $(CFLAGS) $(INCLUDES) -fprofile-arcs -ftest-coverage -c $(TEST_C) -o build/test_c_gcov.o
	$(CC) build/test_c_gcov.o build/tetris_backend_gcov.o -o build/test_c_gcov $(TEST_C_FLAGS) -fprofile-arcs -ftest-coverage

	$(CXX) $(CXXFLAGS) $(INCLUDES) -fprofile-arcs -ftest-coverage -c $(SNAKE_BACKEND_CPP) -o build/snake_backend_gcov.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fprofile-arcs -ftest-coverage -c $(TEST_CPP) -o build/test_cpp_gcov.o
	$(CXX) build/test_cpp_gcov.o build/snake_backend_gcov.o -o build/test_cpp_gcov $(TEST_CPP_FLAGS) -fprofile-arcs -ftest-coverage

	./build/test_c_gcov
	./build/test_cpp_gcov

	@mkdir -p report
	lcov --capture --directory . --output-file report/report.info --ignore-errors gcov,mismatch,empty
	lcov --remove report/report.info '*/tests/*' '*/usr/*' --output-file report/report.info
	genhtml report/report.info --output-directory report

clean_coverage:
	rm -rf build/*_gcov* report/ *.gcda *.gcno *.gcov

dvi:
	doxygen Doxyfile
	@echo "Documentation generated"

dist:
	@mkdir -p dist
	tar -czvf dist/BrickGame.tar.gz \
		brick_game/ gui/ headers/ tests/ \
		Makefile Doxyfile CMakeLists.txt \
		--transform 's,^,BrickGame/,'
	@echo "Distribution package created: dist/BrickGame.tar.gz"

clang:
	clang-format --style=Google -i \
		brick_game/*.cpp brick_game/tetris/*.c brick_game/snake/*.cpp \
		gui/cli/*.cpp gui/desktop/*.cpp \
		headers/*.h headers/*.hpp headers/tetris/*.h headers/snake/*.hpp \
		tests/*.c tests/*.cpp

style:
	clang-format --style=Google -n \
		brick_game/*.cpp brick_game/tetris/*.c brick_game/snake/*.cpp \
		gui/cli/*.cpp gui/desktop/*.cpp \
		headers/*.h headers/*.hpp headers/tetris/*.h headers/snake/*.hpp \
		tests/*.c tests/*.cpp

clean:
	rm -rf build/ report/ dist/ doxygen/ *.gcda *.gcno *.gcov max_score_snake.txt Score.txt

uninstall:
	rm -rf build/ max_score_snake.txt Score.txt

tetris_cli: $(TETRIS_CLI)
	./$(TETRIS_CLI)

snake_cli: $(SNAKE_CLI)
	./$(SNAKE_CLI)

tetris_qt: $(TETRIS_QT)
	./$(TETRIS_QT)

snake_qt: $(SNAKE_QT)
	./$(SNAKE_QT)

brick_game: $(LAUNCHER)
	./$(LAUNCHER)
