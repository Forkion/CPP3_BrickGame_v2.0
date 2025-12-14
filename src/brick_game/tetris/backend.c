#include "../../headers/tetris/backend.h"

#include "../../headers/defines.h"

// /**
//  * @param action_figure Подсказка для падающей фигуры
//  */
// matrix_figure action_figure;

void init_game_backend(GameInfo *field) {
  srand(time(NULL));
  init_field(field);
  init_matrix_figure(&field->figure, SIZE_NEXT, SIZE_NEXT);
  init_matrix_figure(&field->next_figure, SIZE_NEXT, SIZE_NEXT);
  init_next_figure(&field->next, SIZE_NEXT, SIZE_NEXT);
  // if (hint)
  //   init_matrix_figure(&action_figure, SIZE_NEXT, SIZE_NEXT);
  create_figure(&field->next_figure);
  rewrite_figure(&field->figure, field->next_figure);
  create_figure(&field->next_figure);
  rewrite_next(field->next_figure, &field->next);
  write_figure_in_field(field);
}

bool init_field(GameInfo *field) {
  bool res = true;
  field->matrix = NULL;
  field->matrix =
      (int **)malloc(HEIGHT * sizeof(int *) + HEIGHT * WIDTH * sizeof(int));
  if (field->matrix) {
    for (int i = 0; i < HEIGHT; ++i) {
      field->matrix[i] = (int *)(field->matrix + HEIGHT) + (i * WIDTH);
      for (int j = 0; j < WIDTH; ++j) field->matrix[i][j] = 0;
    }
    field->score = 0;
    field->max_score = 0;
    field->level = 1;
    field->speed = START_SPEED - SPEED_STEP * field->level;
  } else
    res = false;

  open_read(field);

  return res;
}

bool init_matrix_figure(matrix_figure *figure, int x, int y) {
  bool res = true;
  figure->matrix = NULL;
  figure->matrix = (int **)malloc(y * sizeof(int *) + y * x * sizeof(int));
  if (figure->matrix) {
    for (int i = 0; i < y; ++i) {
      figure->matrix[i] = (int *)(figure->matrix + y) + (i * x);
      for (int j = 0; j < x; ++j) figure->matrix[i][j] = 0;
    }
  } else
    res = false;

  return res;
}

bool init_next_figure(int ***next, int x, int y) {
  bool res = true;

  (*next) = NULL;
  (*next) = (int **)malloc(y * sizeof(int *) + y * x * sizeof(int));
  if ((*next)) {
    for (int i = 0; i < y; ++i) {
      (*next)[i] = (int *)((*next) + y) + (i * x);
      for (int j = 0; j < x; ++j) (*next)[i][j] = 0;
    }
  } else
    res = false;

  return res;
}

void create_figure(matrix_figure *figure) {
  int rand_figure = rand() % 7 + 1;

  clean_matrix_figure(figure);
  while (figure->number == rand_figure) rand_figure = rand() % 7 + 1;
  set_figure(figure, rand_figure);
}

void set_figure(matrix_figure *figure, int rand_figure) {
  switch (rand_figure) {  // rand()%7+1
    case FIGUR_l:
      for (int i = 0; i < 1; ++i)
        for (int j = 0; j < SIZE_NEXT; ++j) figure->matrix[i][j] = FIGUR_l;
      figure->number = FIGUR_l;
      break;
    case FIGUR_L_MIRRORED:
      for (int i = 0; i < 1; ++i)
        for (int j = 0; j < SIZE_NEXT - 1; ++j) {
          figure->matrix[i + 1][j] = FIGUR_L_MIRRORED;
          if (j >= SIZE_NEXT / 2) figure->matrix[i][j] = FIGUR_L_MIRRORED;
        }
      figure->number = FIGUR_L_MIRRORED;
      break;
    case FIGUR_L:
      for (int i = 0; i < 1; ++i)
        for (int j = 0; j < SIZE_NEXT - 1; ++j) {
          figure->matrix[i + 1][j] = FIGUR_L;
          if (j < 1) figure->matrix[i][j] = FIGUR_L;
        }
      figure->number = FIGUR_L;
      break;
    case FIGUR_O:
      for (int i = 0; i < SIZE_NEXT / 2; ++i)
        for (int j = 0; j < SIZE_NEXT / 2; ++j) figure->matrix[i][j] = FIGUR_O;
      figure->number = FIGUR_O;
      break;

    case FIGUR_S:
      for (int i = 0; i < SIZE_NEXT / 2; ++i)
        for (int j = 0; j < SIZE_NEXT / 2; ++j)
          if (i < 1)
            figure->matrix[i][j + 1] = FIGUR_S;
          else
            figure->matrix[i][j] = FIGUR_S;
      figure->number = FIGUR_S;
      break;
    case FIGUR_Z:
      for (int i = 0; i < SIZE_NEXT / 2; ++i)
        for (int j = 0; j < SIZE_NEXT / 2; ++j)
          if (i < 1)
            figure->matrix[i][j] = FIGUR_Z;
          else
            figure->matrix[i][j + 1] = FIGUR_Z;
      figure->number = FIGUR_Z;
      break;
    case FIGUR_T:
      for (int i = 0; i < 1; ++i)
        for (int j = 0; j < SIZE_NEXT - 1; ++j) {
          figure->matrix[i + 1][j] = FIGUR_T;
          if (j >= 1 && j < SIZE_NEXT / 2) figure->matrix[i][j] = FIGUR_T;
        }
      figure->number = FIGUR_T;
      break;
  }
  figure->x = WIDTH / 2 - 1;
  figure->y = 0;
}

void offset_field(GameInfo *field) {
  short int score = 0;
  int offset = 0;
  for (int y = HEIGHT - 1; y >= 0; y -= 1) {
    for (int x = 0; x < WIDTH; x += 1) {
      if (field->matrix[y][x]) score += 1;
      if (score == WIDTH) {
        for (int i = y; i - 1 >= 0; i--)
          for (int j = 0; j < WIDTH; j++)
            field->matrix[i][j] = field->matrix[i - 1][j];
        offset++;
        x = -1;
        score = 0;
      }
    }
    score = 0;
  }
  add_score(field, offset);
}

void up_level(GameInfo *field) {
  int score_level = field->score;
  int level = field->level;
  while (score_level >= 600 * level && field->level < 10) {
    score_level -= 600;
    field->level++;
  }

  if (level != field->level && field->level <= 10)
    field->speed -= (field->level * SPEED_STEP - level * SPEED_STEP);
}

bool check_down_touch(GameInfo *field) {
  bool res = true;
  delete_figure_out_field(field);

  for (int x = 0; x < SIZE_NEXT && res; x++)
    for (int y = 0; y < SIZE_NEXT && res; y++) {
      if ((field->figure.matrix[y][x] &&
           (y + field->figure.y + 1 == HEIGHT ||
            field->matrix[y + field->figure.y + 1][x + field->figure.x]))) {
        write_figure_in_field(field);
        field->game_end = check_defeat(field);
        rewrite_figure(&field->figure,
                       field->next_figure);  // спавн новой фигуры
        create_figure(&field->next_figure);
        rewrite_next(field->next_figure, &field->next);
        res = false;
        check_status_lines(field);
        write_figure_in_field(field);
        // if (hint){
        //   rewrite_figure(&action_figure, figure);
        // }
      }
    }
  return res;
}

bool check_defeat(GameInfo *field) {
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 4; x++) {
      if (field->matrix[y][x + field->next_figure.x] &&
          field->next_figure.matrix[y][x])
        return true;
    }
  }

  return false;
}

bool check_left_touch(GameInfo *field) {
  bool res = true;

  delete_figure_out_field(field);

  for (int x = 0; x < SIZE_NEXT && res; x += 1)
    for (int y = 0; y < SIZE_NEXT && res; y += 1)
      if (field->figure.matrix[y][x] &&
          (!(field->figure.x + x) ||
           field->matrix[field->figure.y + y][field->figure.x + x - 1]))
        res = false;

  write_figure_in_field(field);

  return res;
}

bool check_right_touch(GameInfo *field) {
  bool res = true;

  delete_figure_out_field(field);

  for (int x = SIZE_NEXT - 1; x >= 0 && res; x -= 1)
    for (int y = 0; y < SIZE_NEXT && res; y += 1)
      if (field->figure.matrix[y][x] &&
          (!(field->figure.x + x + 1 < WIDTH) ||
           field->matrix[field->figure.y + y][field->figure.x + x + 1]))
        res = false;

  write_figure_in_field(field);

  return res;
}

void check_status_lines(GameInfo *field) {
  offset_field(field);
  up_level(field);
}

void add_score(GameInfo *field, int offset) {
  switch (offset) {
    case 1 * 1:
      field->score += 100;
      break;
    case 2 * 1:
      field->score += 300;
      break;
    case 3 * 1:
      field->score += 700;
      break;
    case 4 * 1:
      field->score += 1500;
      break;
  }
  if (field->score >= field->max_score) field->max_score = field->score;
}

void rewrite_figure(matrix_figure *figure, matrix_figure next_figure) {
  clean_matrix_figure(figure);
  for (int i = 0; i < SIZE_NEXT; ++i)
    for (int j = 0; j < SIZE_NEXT; ++j)
      figure->matrix[i][j] = next_figure.matrix[i][j];
  figure->x = next_figure.x;
  figure->y = next_figure.y;
  figure->number = next_figure.number;
}

void rewrite_next(matrix_figure next_figure, int ***next) {
  for (int i = 0; i < SIZE_NEXT; ++i)
    for (int j = 0; j < SIZE_NEXT; ++j) {
      (*next)[i][j] = 0;
      (*next)[i][j] = next_figure.matrix[i][j];
    }
}

void clean_matrix_figure(matrix_figure *figure) {
  memset(figure->matrix + SIZE_NEXT, 0, SIZE_NEXT * SIZE_NEXT * sizeof(int));
}

void move_up(GameInfo *field) {
  delete_figure_out_field(field);

  matrix_figure rotate;

  init_matrix_figure(&rotate, SIZE_NEXT, SIZE_NEXT);
  rewrite_figure(&rotate, field->figure);

  if (rotate.number != FIGUR_O) {
    rotate_figure(field, rotate);
    check_layering_figure_in_field(field, rotate);
  }

  free(rotate.matrix);

  write_figure_in_field(field);
}

bool move_down(GameInfo *field) {
  bool res = true;

  delete_figure_out_field(field);

  if (check_down_touch(field))
    field->figure.y += 1;
  else
    res = false;

  if (res) write_figure_in_field(field);

  return res;
}

void move_right(GameInfo *field) {
  if (check_right_touch(field)) {
    delete_figure_out_field(field);

    field->figure.x += 1;

    write_figure_in_field(field);
  }
}

void move_left(GameInfo *field) {
  if (check_left_touch(field)) {
    delete_figure_out_field(field);

    field->figure.x -= 1;

    write_figure_in_field(field);
  }
}

void write_figure_in_field(GameInfo *field) {
  for (int y = 0; y < SIZE_NEXT; ++y)
    for (int x = 0; x < SIZE_NEXT; ++x)
      if (field->figure.matrix[y][x])
        field->matrix[y + field->figure.y][x + field->figure.x] =
            field->figure.number;
}

void delete_figure_out_field(GameInfo *field) {
  for (int y = 0; y < SIZE_NEXT; ++y)
    for (int x = 0; x < SIZE_NEXT; ++x) {  // (y  + figure.y) < HEIGHT ??????
      if (field->figure.matrix[y][x]) {
        field->matrix[y + field->figure.y][x + field->figure.x] = 0;
      }
    }
}

void rotate_figure(GameInfo *field, matrix_figure rotate) {
  clean_matrix_figure(&field->figure);

  if (field->figure.number == FIGUR_l) {  // весь if под фигуру палки
    if (rotate.matrix[0][0] == FIGUR_l) {
      for (int m = 0; m < 1; m++)
        for (int j = m * SIZE_NEXT, l = 0; j < SIZE_NEXT + m * SIZE_NEXT; ++j)
          for (int n = 0; n < 1; ++n, ++l)
            field->figure.matrix[j][SIZE_NEXT / 2] = rotate.matrix[m][l];
    } else if (rotate.matrix[0][SIZE_NEXT / 2] == FIGUR_l) {
      for (int m = 1; m < SIZE_NEXT / 2; m++)
        for (int j = (m - 1) * SIZE_NEXT, l = 0;
             j < SIZE_NEXT + (m - 1) * SIZE_NEXT; ++j)
          for (int n = 0; n < 1; ++n, ++l)
            field->figure.matrix[m][l] = rotate.matrix[j][SIZE_NEXT / 2];
    } else if (rotate.matrix[1][0] == FIGUR_l) {
      for (int m = 1; m < SIZE_NEXT / 2; m++)
        for (int j = (m - 1) * SIZE_NEXT, l = 0;
             j < SIZE_NEXT + (m - 1) * SIZE_NEXT; ++j)
          for (int n = 0; n < 1; ++n, ++l)
            field->figure.matrix[j][1] = rotate.matrix[m][l];
    } else if (rotate.matrix[0][1] == FIGUR_l) {
      for (int m = 0; m < 1; m++)
        for (int j = m * SIZE_NEXT, l = 0; j < SIZE_NEXT + m * SIZE_NEXT; ++j)
          for (int n = 0; n < 1; ++n, ++l)
            field->figure.matrix[m][l] = rotate.matrix[j][1];
    }
  } else {  // остальные фигуры, кроме палки
    for (int i = SIZE_NEXT / 2, m = 0; i >= 0; i -= 1, m += 1)
      for (int k = 0; k < 1; ++k)
        for (int j = 0, l = 0; j < SIZE_NEXT - 1; j += 1)
          for (int n = 0; n < 1; ++n, ++l)
            field->figure.matrix[j + k][i + n] = rotate.matrix[m + k][l];
  }
}

void check_layering_figure_in_field(GameInfo *field, matrix_figure rotate) {
  bool res = true;  // Становится false, если поворот невозможен
  short int count = 0;

  for (int x = 0; x < SIZE_NEXT && res; x += 1) {
    for (int y = 0; y < SIZE_NEXT && res; y += 1)
      if (field->figure.matrix[y][x]) {
        if ((x < SIZE_NEXT / 2) && (y + field->figure.y) < HEIGHT) {
          if ((x + field->figure.x < 0 ||
               (field->figure.matrix[y][x] &&
                field->matrix[y + field->figure.y][x + field->figure.x]))) {
            if (count >= 2) {
              res = false;
              rewrite_figure(&field->figure, rotate);
            } else {
              field->figure.x += 1;
              y = 0;
              x = 0;
              count++;
            }
          }
        } else if ((x >= SIZE_NEXT / 2) && (y + field->figure.y) < HEIGHT) {
          if ((x + field->figure.x >= WIDTH ||
               (field->figure.matrix[y][x] &&
                field->matrix[y + field->figure.y][x + field->figure.x]))) {
            if (count >= 2) {
              res = false;
              rewrite_figure(&field->figure, rotate);
            } else {
              field->figure.x -= 1;
              y = 0;
              x = 0;
              count++;
            }
          }
        } else if (y + field->figure.y >= HEIGHT) {
          res = false;
          rewrite_figure(&field->figure, rotate);
        }
      }
  }
}

void close_game_backend(GameInfo *field) {
  int max_score_file = 0;
  FILE *out = fopen("build/Score.txt", "rb");

  if (out) {
    fread(&max_score_file, sizeof(max_score_file), 1, out);
    fclose(out);
  }

  if (max_score_file < field->max_score) open_wright(field);

  free(field->figure.matrix);
  // if (hint)
  //   free(action_figure.matrix);
  free(field->next_figure.matrix);
  free(field->matrix);
  free(field->next);
}

bool open_read(GameInfo *field) {
  bool res = true;
  FILE *out = fopen("Score.txt", "rb");

  if (out) {
    fread(&field->max_score, sizeof(field->max_score), 1, out);
    fclose(out);
  } else
    res = false;

  return res;
}

bool open_wright(GameInfo *field) {
  bool res = true;
  FILE *in = fopen("Score.txt", "wb");

  if (in) {
    fwrite(&field->max_score, sizeof(field->max_score), 1, in);
    fclose(in);
  } else
    res = false;

  return res;
}

// bool action_logic(GameInfo *field) {
//   bool res = true;

//   for (int x = 0; x < SIZE_NEXT && res; x += 1)
//     for (int y = 0; y < SIZE_NEXT && res; y += 1)
//       if (action_figure.matrix[y][x] &&
//           (y + action_figure.y + 1 == HEIGHT ||
//            field->matrix[y + action_figure.y + 1]
//                         [x + action_figure.x]))
//         res = false;

//   return res;
// }
