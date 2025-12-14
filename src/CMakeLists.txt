cmake_minimum_required(VERSION 3.16)
project(BrickGame)

# Настройка Qt6
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
# Автоматически генерирует moc_*.cpp файлы из заголовков с Q_OBJECT
set(CMAKE_AUTOMOC ON)
# Автоматически конвертирует .ui файлы в ui_*.h (Преобразует XML из Qt Designer в C++ классы)
set(CMAKE_AUTORCC ON)
# компилирует .qrc файлы (Встраивает ресурсы (иконки, переводы) в исполняемый файл)
set(CMAKE_AUTOUIC ON)

add_compile_options(-Wall -Werror -Wextra)

# поиск pkg-config(Утилита, которая знает где лежат библиотеки и их флаги компиляции)
find_package(PkgConfig REQUIRED)

if(APPLE)
    set(IS_MACOS 1)
    message(STATUS "Building for macOS")
elseif(UNIX AND NOT APPLE)
    set(IS_LINUX 1)
    message(STATUS "Building for Linux")
endif()

# Поиск GTest с проверкой ОС
find_package(GTest)
if(NOT GTest_FOUND)
    if(IS_MACOS)
        # Для macOS часто нужно указывать другие пути
        find_library(GTEST_LIB gtest)
        if(GTEST_LIB)
            set(GTEST_LIBRARIES ${GTEST_LIB} pthread)
            # Homebrew обычно ставит в /usr/local/include
            set(GTEST_INCLUDE_DIRS /usr/local/include)
        endif()
    else()
        set(GTEST_LIBRARIES gtest gtest_main pthread)
        set(GTEST_INCLUDE_DIRS /usr/include)
    endif()
endif()

# Поиск Check с проверкой ОС
pkg_check_modules(Check REQUIRED check)
if(NOT Check_FOUND)
    find_library(CHECK_LIB check)
    if(CHECK_LIB)
        if(IS_MACOS)
            # На macOS check может требовать другие библиотеки
            set(Check_LIBRARIES ${CHECK_LIB})
        else()
            set(Check_LIBRARIES ${CHECK_LIB} pthread)
        endif()
        # Убираем жесткий путь /usr/include
        set(Check_INCLUDE_DIRS "")
    endif()
endif()

# Поиск ncurses с проверкой ОС
if(IS_MACOS)
    find_library(CURSES_LIBRARY ncurses)
    # На macOS часто только ncurses, без отдельной curses
    set(NCURSES_LIBRARY ${CURSES_LIBRARY})
else()
    find_library(CURSES_LIBRARY curses)
    find_library(NCURSES_LIBRARY ncurses)
endif()


# Поиск Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
if(NOT Qt6_FOUND AND IS_MACOS)
    # Типичные пути Homebrew на macOS
    set(possible_paths
        "/usr/local/opt/qt/lib/cmake/Qt6"
        "/opt/homebrew/opt/qt/lib/cmake/Qt6"  # Apple Silicon
    )

    foreach(path ${possible_paths})
        if(EXISTS ${path})
            set(Qt6_DIR ${path})
            find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
            break()
        endif()
    endforeach()
endif()

# Включение тестирования
include(CTest)

# Включение директорий
include_directories(
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/tetris
    ${CMAKE_SOURCE_DIR}/headers/snake
    ${GTEST_INCLUDE_DIRS}
    ${Check_INCLUDE_DIRS}
)

# Группирует файлы в макрос ${RELATIVE_PATHS}
set(RELATIVE_PATHS
    brick_game/snake/backend.cpp
    brick_game/tetris/backend.c
    brick_game/brick_game.cpp
    brick_game/fsm.cpp
    gui/cli/frontend.cpp
    gui/desktop/frontend.cpp
    headers/snake/backend.hpp
    headers/snake/frontend.hpp
    headers/tetris/backend.h
    headers/tetris/frontend.hpp
    headers/brick_game.hpp
    headers/defines.h
    headers/fsm.hpp
    tests/test.c
    tests/test.cpp
)

# Подставляет перед одним макросом другой
set(ALL_PROJECT_FILES "")
foreach(file ${RELATIVE_PATHS})
    list(APPEND ALL_PROJECT_FILES ${CMAKE_SOURCE_DIR}/${file})
endforeach()


# ============================================================================
# ЛАУНЧЕР (МЕНЮ)
# ============================================================================
set(LAUNCHER_SOURCES
    brick_game/brick_game.cpp
)

set(LAUNCHER_HEADERS
    headers/brick_game.hpp
)

# Создаёт исполняемый файл brick_game_launcher из файлов с исходным кодом
add_executable(brick_game_launcher ${LAUNCHER_SOURCES} ${LAUNCHER_HEADERS})

target_compile_options(brick_game_launcher PRIVATE -g)
target_link_libraries(brick_game_launcher Qt6::Core Qt6::Widgets)

# ============================================================================
# ДЕСКТОПНАЯ ВЕРСИЯ ТЕТРИСА(qt)
# ============================================================================
set(TETRIS_QT_SOURCES
    brick_game/tetris/backend.c
    brick_game/fsm.cpp
    gui/desktop/frontend.cpp
)

set(TETRIS_QT_HEADERS
    headers/tetris/backend.h
    headers/fsm.hpp
    headers/snake/frontend.hpp
)

add_executable(tetris_qt ${TETRIS_QT_SOURCES} ${TETRIS_QT_HEADERS})

target_compile_options(tetris_qt PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>: -g -Wall -Werror -Wextra>
    $<$<COMPILE_LANGUAGE:C>: -g -Wall -Werror -Wextra>
)
target_link_libraries(tetris_qt Qt6::Core Qt6::Widgets)
target_compile_definitions(tetris_qt PRIVATE TETRIS_QT)

# ============================================================================
# ДЕСКТОПНАЯ ВЕРСИЯ ЗМЕЙКИ(qt)
# ============================================================================
set(SNAKE_QT_SOURCES
    brick_game/snake/backend.cpp
    brick_game/fsm.cpp
    gui/desktop/frontend.cpp
)

set(SNAKE_QT_HEADERS
    headers/snake/backend.hpp
    headers/fsm.hpp
    headers/snake/frontend.hpp
)

set(BRICK_GAME_AND_DEFINES
    brick_game/brick_game.cpp
    headers/brick_game.hpp
    headers/defines.h
)

add_executable(snake_qt ${SNAKE_QT_SOURCES} ${SNAKE_QT_HEADERS})

target_compile_definitions(snake_qt PRIVATE SNAKE_QT)
target_compile_options(snake_qt PRIVATE -g)
target_link_libraries(snake_qt Qt6::Core Qt6::Widgets)

# ============================================================================
# КОНСОЛЬНАЯ ВЕРСИЯ ТЕТРИСА(ncurses)
# ============================================================================
set(TETRIS_CLI_SOURCES
    brick_game/tetris/backend.c
    brick_game/fsm.cpp
    gui/cli/frontend.cpp
)

set(TETRIS_CLI_HEADERS
    headers/tetris/backend.h
    headers/fsm.hpp
    headers/tetris/frontend.hpp
)

find_library(CURSES_LIBRARY curses)
find_library(NCURSES_LIBRARY ncurses)

add_executable(tetris_cli ${TETRIS_CLI_SOURCES} ${TETRIS_CLI_HEADERS})

target_compile_definitions(tetris_cli PRIVATE TETRIS_CLI)
target_compile_options(tetris_cli PRIVATE -g)
target_link_libraries(tetris_cli ${CURSES_LIBRARY} ${NCURSES_LIBRARY})


# ============================================================================
# КОНСОЛЬНАЯ ВЕРСИЯ ЗМЕЙКИ(ncurses)
# ============================================================================
set(SNAKE_CLI_SOURCES
    brick_game/snake/backend.cpp
    brick_game/fsm.cpp
    gui/cli/frontend.cpp
)

set(SNAKE_CLI_HEADERS
    headers/snake/backend.hpp
    headers/fsm.hpp
    headers/tetris/frontend.hpp
)

add_executable(snake_cli ${SNAKE_CLI_SOURCES} ${SNAKE_CLI_HEADERS})

target_compile_definitions(snake_cli PRIVATE SNAKE_CLI)
target_compile_options(snake_cli PRIVATE -g)
target_link_libraries(snake_cli ${CURSES_LIBRARY} ${NCURSES_LIBRARY})

# ============================================================================
# TEST_C (для Tetris на C)
# ============================================================================
if(Check_FOUND)
    add_executable(test_c
        tests/test.c
        brick_game/tetris/backend.c
    )

    target_include_directories(test_c PRIVATE
        ${CMAKE_SOURCE_DIR}/headers
        ${Check_INCLUDE_DIRS}
    )

    target_compile_options(test_c PRIVATE -fprofile-arcs -g -ftest-coverage)
    target_compile_definitions(test_c PRIVATE TETRIS_CLI)
    target_link_options(test_c PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_libraries(test_c ${Check_LIBRARIES})

    add_test(NAME test_c COMMAND test_c)
endif()

# ============================================================================
# TEST_CPP (для Snake на C++)
# ============================================================================
if(GTest_FOUND)
    add_executable(test_cpp
        tests/test.cpp
        brick_game/snake/backend.cpp
    )

    target_include_directories(test_cpp PRIVATE
        ${CMAKE_SOURCE_DIR}/headers
        ${GTEST_INCLUDE_DIRS}
    )

    if(TARGET GTest::gtest AND TARGET GTest::gtest_main)
        target_link_libraries(test_cpp GTest::gtest GTest::gtest_main)
    else()
        target_link_libraries(test_cpp ${GTEST_LIBRARIES})
    endif()

    target_compile_definitions(test_cpp PRIVATE SNAKE_QT)
    target_compile_options(test_cpp PRIVATE -g -fprofile-arcs -ftest-coverage)
    target_link_options(test_cpp PRIVATE -fprofile-arcs -ftest-coverage)

    add_test(NAME test_cpp COMMAND test_cpp)
endif()

# ============================================================================
# INSTALL - Команда для создания исполняемых файлов игры и запуска меню
# ============================================================================
install(TARGETS
    brick_game_launcher
    tetris_qt
    snake_qt
    tetris_cli
    snake_cli
)

# ============================================================================
# ОБЩАЯ КОМАНДА TEST - запускает все тесты обычным способом
# ============================================================================
add_custom_target(test_all

    DEPENDS test_c test_cpp
)

# ============================================================================
# CLANG-FORMAT - запуск clang-format для всего проекта
# ============================================================================
add_custom_target(clang_i

    COMMAND clang-format --style=file:${CMAKE_SOURCE_DIR}/../materials/linters/.clang-format -i ${ALL_PROJECT_FILES}
)

# ============================================================================
# CLANG-FORMAT - запуск clang-format для всего проекта
# ============================================================================
add_custom_target(clang_n

    COMMAND clang-format --style=file:${CMAKE_SOURCE_DIR}/../materials/linters/.clang-format -n ${ALL_PROJECT_FILES}
)

# ============================================================================
# VALGRIND TEST - запускает оба теста с Valgrind
# ============================================================================
add_custom_target(valgrind_test
    COMMAND echo "=== Running C tests with Valgrind ==="
    COMMAND valgrind --tool=memcheck --leak-check=full --track-origins=yes -q ./test_c || echo "C Valgrind tests failed"
    COMMAND echo "=== Running C++ tests with Valgrind ==="
    COMMAND valgrind --tool=memcheck --leak-check=full --track-origins=yes -q ./test_cpp || echo "C++ Valgrind tests failed"
    DEPENDS test_c test_cpp
)

# ============================================================================
# TEST-ALL - общая команда которая запускает обычные тесты И Valgrind тесты
# ============================================================================
add_custom_target(test-all
    COMMAND echo "=== Running standard tests ==="
    COMMAND ./test_c || echo "C tests failed"
    COMMAND ./test_cpp || echo "C++ tests failed"
    COMMAND echo "=== Running Valgrind tests ==="
    COMMAND valgrind --tool=memcheck --leak-check=full --track-origins=yes -q ./test_c || echo "C Valgrind tests failed"
    COMMAND valgrind --tool=memcheck --leak-check=full --track-origins=yes -q ./test_cpp || echo "C++ Valgrind tests failed"
    DEPENDS test_c test_cpp
)

# Для gcov_report добавляем авто-открытие с проверкой ОС
find_program(GCOV gcov)
find_program(LCOV lcov)
find_program(GENHTML genhtml)

if(GCOV AND LCOV AND GENHTML)
    add_custom_target(gcov_report
        COMMAND ${CMAKE_COMMAND} -E make_directory report
        COMMAND ${CMAKE_COMMAND} --build . --target test_c
        COMMAND ${CMAKE_COMMAND} --build . --target test_cpp
        COMMAND ./test_c || echo "C tests completed"
        COMMAND ./test_cpp || echo "C++ tests completed"
        COMMAND ${LCOV} --directory . --capture --output-file ./report/report.info --ignore-errors gcov,mismatch
        COMMAND ${LCOV} --remove ./report/report.info '*/tests/*' '*/usr/*' '*/usr/local/*' --output-file ./report/report.info --ignore-errors unused
        COMMAND ${GENHTML} ./report/report.info --output-directory ./report --ignore-errors unmapped,unused,mismatch
        COMMAND echo "Coverage report generated in report/index.html"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()


# ============================================================================
# DOXYGEN - формирует документацию к коду на основе комментариев
# ============================================================================
find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HAVE_DOT YES)  # Для диаграмм классов
    add_custom_target(dvi
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation"
    )
else()
    add_custom_target(dvi
        COMMAND echo "Doxygen not found - install with: sudo apt install doxygen"
        COMMENT "Doxygen not available"
    )
endif()

# ============================================================================
# DIST - Создаёт архив с проектом
# ============================================================================
add_custom_target(dist
    COMMAND ${CMAKE_COMMAND} -E make_directory dist
    COMMAND tar -czvf dist/BrickGame.tar.gz
        brick_game/
        gui/
        headers/
        tests/
        CMakeLists.txt
        Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Creating distribution package BrickGame.tar.gz"
)
