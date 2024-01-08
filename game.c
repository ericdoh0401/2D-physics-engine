#include "collision.h"
#include "color.h"
#include "constants.h"
#include "forces.h"
#include "list.h"
#include "polygon.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"
#include "text.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const rgb_color_t BLACK = (rgb_color_t){.r = 0.0, .g = 0.0, .b = 0.0};
const rgb_color_t WHITE = (rgb_color_t){.r = 1.0, .g = 1.0, .b = 1.0};
const rgb_color_t GREEN = (rgb_color_t){.r = 0, .g = 1.0, .b = 0};
const rgb_color_t RED = (rgb_color_t){.r = 1.0, .g = 0, .b = 0};

// window constants
const vector_t WINDOW = (vector_t){.x = 1000, .y = 500};
const vector_t CENTER = (vector_t){.x = 500, .y = 250};

const size_t STARTING_LEVEL = 1;

// weapon constants
// weapon is currently a rectangle
const size_t ANGLE_RANGE = 89;
const size_t WEAPON_ANGLE_MIN_RAD = 10;
const size_t WEAPON_ANGLE_MAX_RAD = 60;
const double WEAPON_HIT_TARGET_TIME = 4.0;
const double WEAPON_SPAWN_X = -150;
const double WEAPON_LAUNCH_TIME_INTERVAL = 0.2;
const double WEAPON_WIDTH = 25;
const double WEAPON_HEIGHT = 15;
const double WEAPON_MASS = 300;
const rgb_color_t WEAPON_COLOR = (rgb_color_t){.r = 1, .g = 0, .b = 0};
const double GRAVITY = -200;
const double WEAPON_RADIUS = 10;
const size_t CIRCLE_SIDES = 30;
const size_t TRIANGLE_SIDES = 3;
const size_t SQUARE_SIDES = 4;
const double CIRCLE_MASS = 50;
const double TRIANGLE_MASS = 100;
const double SQUARE_MASS = 200;

// egg constants
const vector_t EGG_CENTROID = (vector_t){.x = 381, .y = 99};
const double EGG_MINOR_AXIS = 28;
const double EGG_MAJOR_AXIS = 18;
const size_t EGG_LINE_SEGMENTS = 60;
const double EGG_HEALTH = 1000.0;
const rgb_color_t EGG_COLOR = (rgb_color_t){.r = 1, .g = 0.875, .b = 0};
const size_t EGG_BOTTOM_LEFT_GRID_ROW = 0;
const size_t EGG_BOTTOM_LEFT_GRID_COL = 14;
const size_t EGG_GRID_WIDTH = 2;
const size_t EGG_GRID_HEIGHT = 3;

// island constants
const double ISLAND_LEFT_MARGIN =
    50.0; // how far away the island is from the left side of the screen
const double ISLAND_HEIGHT = 70.0;
const double ISLAND_MASS = INFINITY;
const rgb_color_t ISLAND_COLOR = (rgb_color_t){
    .r = 0.494, .g = 0.784, .b = 0.314}; // dark green, emulating color of grass
const double LAVA_LEVEL = 20;            // how high the lava level is
const rgb_color_t LAVA_COLOR =
    (rgb_color_t){.r = 0.88, .g = 0.0, .b = 0.0}; // dark red

// menu constants
const double MENU_WIDTH = 200;
const rgb_color_t MENU_COLOR =
    (rgb_color_t){.r = 0.9, .g = 0.9, .b = 0.9}; // light grey
const double MENU_BORDER_WIDTH = 8;
const rgb_color_t MENU_BORDER_COLOR = (rgb_color_t){.r = 0, .g = 0, .b = 0};
const vector_t LEVEL_LABEL_LOC = (vector_t){.x = 827, .y = 163};
const vector_t CREDITS_LABEL_LOC = (vector_t){.x = 827, .y = 143};
const vector_t HEALTH_LABEL_LOC = (vector_t){.x = 827, .y = 123};
const double MENU_TEXT_SIZE = 25;
const double GAME_OVER_TEXT_SIZE = 70;

// selection constants
const size_t NUM_OF_SELECTIONS = 4;
const double SELECTION_SEPARATION = 20;
const double SELECTION_WIDTH = 160;
const double SELECTION_HEIGHT = 60;
const rgb_color_t SELECTION_BACKGROUND = (rgb_color_t){.r = 1, .b = 1, .g = 1};
const double SELECTION_CIRCLE_RADIUS = 10;
const size_t SELECTION_CIRCLE_SIDES = 20;
const rgb_color_t HAY_COLOR = (rgb_color_t){.r = 0.9, .g = 0.7, .b = 0.33};
const rgb_color_t WOOD_COLOR = (rgb_color_t){.r = 0.6, .g = 0.3, .b = 0.0};
const rgb_color_t STEEL_COLOR = (rgb_color_t){.r = 0.5, .g = 0.5, .b = 0.5};
const rgb_color_t DIAMOND_COLOR = (rgb_color_t){.r = 0.2, .g = 1.0, .b = 0.95};
const double HAY_HEALTH = 50;
const double WOOD_HEALTH = 100;
const double STEEL_HEALTH = 150;
const double DIAMOND_HEALTH = 200;
const size_t HAY_COST = 50;
const size_t WOOD_COST = 100;
const size_t STEEL_COST = 150;
const size_t DIAMOND_COST = 200;
const rgb_color_t START_COLOR = (rgb_color_t){.r = 0.0, .g = 1.0, .b = 0.0};
const rgb_color_t PAUSE_COLOR = (rgb_color_t){.r = 1.0, .g = 0.0, .b = 0.0};
const size_t MENU_BLOCK_DIM = 40;

// play_pause button constants
const vector_t PAUSE_PLAY_LOC = (vector_t){.x = 900, .y = 55};
const double PAUSE_PLAY_RADIUS = 15;
const size_t PAUSE_PLAY_SIDES = 30;
const double PAUSE_LINE_WIDTH = 3;
const double PAUSE_LINE_HEIGHT = 10;
const double PLAY_TRIANGLE_RADIUS = 7.5;

// building phase constants
const double GRID_SQUARE_WIDTH = 20;
const double GRID_SQUARE_HEIGHT = 20;
const size_t NUM_GRID_ROWS = 20;
const size_t NUM_GRID_COLS = 30;
const vector_t GRID_BOTTOM_LEFT =
    (vector_t){.x = ISLAND_LEFT_MARGIN + 30.0, .y = ISLAND_HEIGHT};
const double GRID_LINE_THICKNESS = 1;
const rgb_color_t GRID_LINE_COLOR = (rgb_color_t){.r = 0.5, .g = 0.5, .b = 0.5};
const rgb_color_t HOVER_SQUARE_COLOR =
    (rgb_color_t){.r = 0.678 * 1.1, .g = 0.847 * 1.1, .b = 0.9 * 1.1};
const double BUILDING_PHASE_DELAY =
    3.5; // delay for when shooting is done and building phase starts

// constants for damage calculations
const double DAMAGE_MULTIPLIER = 1e-5 * 0.5;
const double DAMAGE_MAGNITUDE = 2.0;

// 3 constants needed to define the credit function
const double CREDIT_EXPONENT = 0.8;
const double CREDIT_ADDITION = 400;
const double CREDIT_MULTIPLIER = 15;

// weapon formula constants
const size_t MIN_NUM_CIRCLES = 5;
const double CIRCLES_PER_LEVEL = 0.5;
const size_t MIN_NUM_TRIANGLES = 3;
const double TRIANGLES_PER_LEVEL = 0.4;
const double SQUARES_SUBTRACT = 0.5;
const double SQUARES_BASE = 1.1;

const double WOOD_INDEX = 1.0;
const double STEEL_INDEX = 2.0;
const double DIAMOND_INDEX = 3.0;

const double PI = 3.14;
const double TWO_PI = 2 * 3.14;

// y level for the game over message
const double GAME_OVER_MSG_Y = 400;

// background constants
const rgb_color_t SKY_COLOR = (rgb_color_t){.r = 0.725, .g = 0.96, .b = 1};

// x is the x coordinate of the top left corner. // y is the y coordinate of the
// top left corner
body_t *create_rectangle_body(double x, double y, double width, double height,
                              double mass, rgb_color_t color, size_t info,
                              free_func_t info_freer) {
  list_t *shape = list_init(4, free);
  vector_t *top_left = malloc(sizeof(vector_t));
  top_left->x = x;
  top_left->y = y;
  vector_t *top_right = malloc(sizeof(vector_t));
  top_right->x = x + width;
  top_right->y = y;
  vector_t *bottom_right = malloc(sizeof(vector_t));
  bottom_right->x = x + width;
  bottom_right->y = y - height;
  vector_t *bottom_left = malloc(sizeof(vector_t));
  bottom_left->x = x;
  bottom_left->y = y - height;
  list_add(shape, top_left);
  list_add(shape, top_right);
  list_add(shape, bottom_right);
  list_add(shape, bottom_left);
  role_t *body_info = malloc(sizeof(role_t));
  *body_info = info;
  body_t *body = body_init_with_info(shape, mass, color, body_info, info_freer);
  return body;
}

void create_background(scene_t *scene) {
  body_t *sky = create_rectangle_body(0, WINDOW.y, WINDOW.x, WINDOW.y, INFINITY,
                                      SKY_COLOR, BACKGROUND, free);
  scene_add_body(scene, sky);
}

void create_island(scene_t *scene) {
  body_t *lava =
      create_rectangle_body(0, LAVA_LEVEL, ISLAND_LEFT_MARGIN, LAVA_LEVEL,
                            INFINITY, LAVA_COLOR, LAVA, free);
  scene_add_body(scene, lava);

  // create the island's shape
  body_t *island = create_rectangle_body(
      ISLAND_LEFT_MARGIN, ISLAND_HEIGHT, WINDOW.x - ISLAND_LEFT_MARGIN,
      ISLAND_HEIGHT, ISLAND_MASS, ISLAND_COLOR, ISLAND, free);
  scene_add_body(scene, island);
}

void create_menu(scene_t *scene) {
  // create the menu shape
  body_t *menu =
      create_rectangle_body(WINDOW.x - MENU_WIDTH, WINDOW.y, MENU_WIDTH,
                            WINDOW.y, INFINITY, MENU_COLOR, MENU, free);
  scene_add_body(scene, menu);

  // create border
  body_t *border = create_rectangle_body(
      WINDOW.x - MENU_WIDTH - (MENU_BORDER_WIDTH / 2), WINDOW.y,
      MENU_BORDER_WIDTH, WINDOW.y, INFINITY, MENU_BORDER_COLOR, MENU, free);
  scene_add_body(scene, border);

  double spawn_x = WINDOW.x - (MENU_WIDTH / 2) - (SELECTION_WIDTH / 2);
  double spawn_y = WINDOW.y - SELECTION_SEPARATION;
  // create block selections
  for (size_t i = 0; i < NUM_OF_SELECTIONS; i++) {
    body_t *new_section = create_rectangle_body(
        spawn_x, spawn_y, SELECTION_WIDTH, SELECTION_HEIGHT, INFINITY,
        SELECTION_BACKGROUND, MENU, NULL);
    scene_add_body(scene, new_section);
    spawn_y -= (SELECTION_SEPARATION + SELECTION_HEIGHT);
  }

  // create bottom section
  body_t *last_section = create_rectangle_body(
      spawn_x, spawn_y, SELECTION_WIDTH, (spawn_y - SELECTION_SEPARATION),
      INFINITY, SELECTION_BACKGROUND, MENU, NULL);
  scene_add_body(scene, last_section);

  double menu_block_margin = (SELECTION_HEIGHT - MENU_BLOCK_DIM) / 2;
  // add blocks in the sections
  spawn_x = WINDOW.x - (MENU_WIDTH / 2) - (MENU_BLOCK_DIM / 2);
  spawn_y = WINDOW.y - SELECTION_SEPARATION - menu_block_margin;
  body_t *block1 =
      create_rectangle_body(spawn_x, spawn_y, MENU_BLOCK_DIM, MENU_BLOCK_DIM,
                            INFINITY, HAY_COLOR, MENU, NULL);
  scene_add_body(scene, block1);
  spawn_y -= ((menu_block_margin * 2) + MENU_BLOCK_DIM + SELECTION_SEPARATION);
  body_t *block2 =
      create_rectangle_body(spawn_x, spawn_y, MENU_BLOCK_DIM, MENU_BLOCK_DIM,
                            INFINITY, WOOD_COLOR, MENU, NULL);
  scene_add_body(scene, block2);
  spawn_y -= ((menu_block_margin * 2) + MENU_BLOCK_DIM + SELECTION_SEPARATION);
  body_t *block3 =
      create_rectangle_body(spawn_x, spawn_y, MENU_BLOCK_DIM, MENU_BLOCK_DIM,
                            INFINITY, STEEL_COLOR, MENU, NULL);
  scene_add_body(scene, block3);
  spawn_y -= ((menu_block_margin * 2) + MENU_BLOCK_DIM + SELECTION_SEPARATION);
  body_t *block4 =
      create_rectangle_body(spawn_x, spawn_y, MENU_BLOCK_DIM, MENU_BLOCK_DIM,
                            INFINITY, DIAMOND_COLOR, MENU, NULL);
  scene_add_body(scene, block4);
}

void create_egg(scene_t *scene) {
  assert(EGG_MAJOR_AXIS >= 0);
  assert(EGG_MINOR_AXIS >= 0);
  list_t *egg_points = list_init(EGG_LINE_SEGMENTS, NULL);
  double angle_increment = TWO_PI / EGG_LINE_SEGMENTS;
  for (size_t i = 0; i < EGG_LINE_SEGMENTS; i++) {
    vector_t *point = malloc(sizeof(vector_t));
    point->x = EGG_MAJOR_AXIS * cos(angle_increment * i);
    point->y = EGG_MINOR_AXIS * sin(angle_increment * i);
    list_add(egg_points, point);
  }
  role_t *body_info = malloc(sizeof(role_t));
  *body_info = EGG;
  body_t *ret = body_init_with_info(egg_points, 1, EGG_COLOR, body_info, free);
  body_set_health(ret, EGG_HEALTH);
  body_set_centroid(ret, EGG_CENTROID);
  scene_add_body(scene, ret);
}

void create_grid(scene_t *scene) {
  // create horizontal lines
  for (size_t i = 1; i < NUM_GRID_ROWS; i++) {
    body_t *line = create_rectangle_body(
        GRID_BOTTOM_LEFT.x,
        GRID_BOTTOM_LEFT.y + (i * GRID_SQUARE_HEIGHT) +
            (GRID_LINE_THICKNESS / 2),
        NUM_GRID_COLS * GRID_SQUARE_WIDTH, GRID_LINE_THICKNESS, INFINITY,
        GRID_LINE_COLOR, GRID_LINE, free);
    scene_add_body(scene, line);
  }

  // create vertical lines
  for (size_t i = 1; i < NUM_GRID_COLS; i++) {
    body_t *line = create_rectangle_body(
        GRID_BOTTOM_LEFT.x + (i * GRID_SQUARE_WIDTH) -
            (GRID_LINE_THICKNESS / 2),
        GRID_BOTTOM_LEFT.y + (NUM_GRID_ROWS * GRID_SQUARE_HEIGHT),
        GRID_LINE_THICKNESS, (NUM_GRID_ROWS * GRID_SQUARE_HEIGHT), INFINITY,
        GRID_LINE_COLOR, GRID_LINE, free);
    scene_add_body(scene, line);
  }
}

typedef struct state {
  scene_t *scene;
  game_state_t game_state;
  double last_weapon_time;
  double total_time_elapsed;
  size_t block_selected;
  text_t *text;
  list_t *weapon_queue;
  size_t level;
  size_t credits;
  double egg_health;
  bool game_over;
  bool is_paused;
  text_t *game_over_text;
  list_t *costs;
  list_t *egg_grid_spots_x;
  list_t *egg_grid_spots_y;
} state_t;

body_t *get_egg(scene_t *scene) {
  list_t *all_bodies = scene_get_all_bodies(scene);
  for (size_t i = 0; i < list_size(all_bodies); i++) {
    body_t *curr_body = list_get(all_bodies, i);
    if (*(role_t *)body_get_info(curr_body) == EGG) {
      return curr_body;
    }
  }
  return NULL;
}

// make it so that this does not get redrawn every single scene tick
void draw_pause_play(state_t *state) {
  list_t *all_bodies = scene_get_all_bodies(state->scene);
  for (size_t i = 0; i < list_size(all_bodies); i++) {
    body_t *curr_body = list_get(all_bodies, i);
    if (*(role_t *)body_get_info(curr_body) == PAUSE_PLAY) {
      body_remove(curr_body);
    }
  }
  // draw a pause button
  if (state->is_paused == false) {
    // make background
    double angle_inc = (TWO_PI) / PAUSE_PLAY_SIDES;
    double curr_angle = 0.0;
    list_t *sides = list_init(PAUSE_PLAY_SIDES, NULL);
    for (size_t i = 0; i < PAUSE_PLAY_SIDES; i++) {
      vector_t *new_vec = malloc(sizeof(vector_t));
      double new_x = (PAUSE_PLAY_RADIUS * cos(curr_angle)) + PAUSE_PLAY_LOC.x;
      double new_y = (PAUSE_PLAY_RADIUS * sin(curr_angle)) + PAUSE_PLAY_LOC.y;
      new_vec->x = new_x;
      new_vec->y = new_y;
      list_add(sides, new_vec);
      curr_angle += angle_inc;
    }
    role_t *body_info = malloc(sizeof(role_t));
    *body_info = (role_t)PAUSE_PLAY;
    body_t *body = body_init_with_info(sides, INFINITY, RED, body_info, free);
    scene_add_body(state->scene, body);

    // draw two lines
    body_t *left_rectangle = create_rectangle_body(
        PAUSE_PLAY_LOC.x - PAUSE_LINE_WIDTH - PAUSE_LINE_WIDTH,
        PAUSE_PLAY_LOC.y + (PAUSE_LINE_HEIGHT / 2), PAUSE_LINE_WIDTH,
        PAUSE_LINE_HEIGHT, INFINITY, WHITE, PAUSE_PLAY, free);
    body_t *right_triangle = create_rectangle_body(
        PAUSE_PLAY_LOC.x + PAUSE_LINE_WIDTH,
        PAUSE_PLAY_LOC.y + (PAUSE_LINE_HEIGHT / 2), PAUSE_LINE_WIDTH,
        PAUSE_LINE_HEIGHT, INFINITY, WHITE, PAUSE_PLAY, free);
    scene_add_body(state->scene, left_rectangle);
    scene_add_body(state->scene, right_triangle);
  }
  // draw a play button
  if (state->is_paused == true) {
    // make background
    double angle_inc = (TWO_PI) / PAUSE_PLAY_SIDES;
    double curr_angle = 0.0;
    list_t *sides = list_init(PAUSE_PLAY_SIDES, NULL);
    for (size_t i = 0; i < PAUSE_PLAY_SIDES; i++) {
      vector_t *new_vec = malloc(sizeof(vector_t));
      double new_x = (PAUSE_PLAY_RADIUS * cos(curr_angle)) + PAUSE_PLAY_LOC.x;
      double new_y = (PAUSE_PLAY_RADIUS * sin(curr_angle)) + PAUSE_PLAY_LOC.y;
      new_vec->x = new_x;
      new_vec->y = new_y;
      list_add(sides, new_vec);
      curr_angle += angle_inc;
    }
    role_t *body_info = malloc(sizeof(role_t));
    *body_info = (role_t)PAUSE_PLAY;
    body_t *body = body_init_with_info(sides, INFINITY, GREEN, body_info, free);
    scene_add_body(state->scene, body);

    // draw triangle
    list_t *points = list_init(3, NULL);
    angle_inc = (TWO_PI) / TRIANGLE_SIDES;
    curr_angle = 0.0;
    for (size_t i = 0; i < TRIANGLE_SIDES; i++) {
      vector_t *new_vec = malloc(sizeof(vector_t));
      double new_x =
          (PLAY_TRIANGLE_RADIUS * cos(curr_angle)) + PAUSE_PLAY_LOC.x;
      double new_y =
          (PLAY_TRIANGLE_RADIUS * sin(curr_angle)) + PAUSE_PLAY_LOC.y;
      new_vec->x = new_x;
      new_vec->y = new_y;
      list_add(points, new_vec);
      curr_angle += angle_inc;
    }
    role_t *traingle_info = malloc(sizeof(role_t));
    *traingle_info = (role_t)PAUSE_PLAY;
    body_t *traingle =
        body_init_with_info(points, INFINITY, WHITE, traingle_info, free);
    scene_add_body(state->scene, traingle);
  }
}

// get the row in the grid system
size_t get_local_row(vector_t loc) {
  if (loc.x > GRID_BOTTOM_LEFT.x && loc.y > GRID_BOTTOM_LEFT.y &&
      loc.x < GRID_BOTTOM_LEFT.x + (NUM_GRID_COLS * GRID_SQUARE_WIDTH) &&
      loc.y < GRID_BOTTOM_LEFT.y + (NUM_GRID_ROWS * GRID_SQUARE_HEIGHT)) {
    vector_t relative_loc = (vector_t){.x = loc.x - GRID_BOTTOM_LEFT.x,
                                       .y = loc.y - GRID_BOTTOM_LEFT.y};
    return (size_t)(relative_loc.y / GRID_SQUARE_HEIGHT);
  }
  return (size_t)(-1);
}

// get the column in the grid system
size_t get_local_col(vector_t loc) {
  if (loc.x > GRID_BOTTOM_LEFT.x && loc.y > GRID_BOTTOM_LEFT.y &&
      loc.x < GRID_BOTTOM_LEFT.x + (NUM_GRID_COLS * GRID_SQUARE_WIDTH) &&
      loc.y < GRID_BOTTOM_LEFT.y + (NUM_GRID_ROWS * GRID_SQUARE_HEIGHT)) {
    vector_t relative_loc = (vector_t){.x = loc.x - GRID_BOTTOM_LEFT.x,
                                       .y = loc.y - GRID_BOTTOM_LEFT.y};
    return (size_t)(relative_loc.x / GRID_SQUARE_WIDTH);
  }
  return (size_t)(-1);
}

// draw a square where the mouse is hovering
void hover_square(state_t *state, vector_t loc) {
  // check if colored in square already exists
  list_t *bodies = scene_get_all_bodies(state->scene);
  for (size_t i = 0; i < list_size(bodies); i++) {
    body_t *curr_body = list_get(bodies, i);
    if (*(role_t *)body_get_info(curr_body) == HIGHLIGHTED_SQUARE) {
      body_remove(curr_body);
    }
  }

  // check if egg is in spot
  for (size_t i = 0; i < list_size(state->egg_grid_spots_x); i++) {
    size_t curr_x = *((size_t *)(list_get(state->egg_grid_spots_x, i)));
    if (curr_x == get_local_col(loc)) {
      for (size_t j = 0; j < list_size(state->egg_grid_spots_y); j++) {
        size_t curr_y = *((size_t *)(list_get(state->egg_grid_spots_y, j)));
        if (curr_y == get_local_row(loc)) {
          return;
        }
      }
    }
  }

  if (state->game_state == BUILDING) {
    // check if mouse in grid area
    if (loc.x > GRID_BOTTOM_LEFT.x && loc.y > GRID_BOTTOM_LEFT.y) {
      if (loc.x < GRID_BOTTOM_LEFT.x + (NUM_GRID_COLS * GRID_SQUARE_WIDTH) &&
          loc.y < GRID_BOTTOM_LEFT.y + (NUM_GRID_ROWS * GRID_SQUARE_HEIGHT)) {
        size_t row_idx = get_local_row(loc);
        size_t col_idx = get_local_col(loc);
        // create blue square
        body_t *square = create_rectangle_body(
            GRID_BOTTOM_LEFT.x + (col_idx * GRID_SQUARE_WIDTH) +
                (GRID_LINE_THICKNESS / 2),
            GRID_BOTTOM_LEFT.y + ((row_idx + 1) * GRID_SQUARE_HEIGHT) -
                (GRID_LINE_THICKNESS / 2),
            GRID_SQUARE_WIDTH - GRID_LINE_THICKNESS,
            GRID_SQUARE_HEIGHT - GRID_LINE_THICKNESS, INFINITY,
            HOVER_SQUARE_COLOR, HIGHLIGHTED_SQUARE, free);
        scene_add_body(state->scene, square);
      }
    }
  }
}

vector_t calc_initial_weapon_vel(double angle) {
  double delta_x = (EGG_CENTROID.x - WEAPON_SPAWN_X) / 2;
  double magnitude = sqrt((-1 * delta_x * GRAVITY) / (sin(angle) * cos(angle)));
  return (vector_t){.x = cos(angle) * magnitude, .y = sin(angle) * magnitude};
}

// checks if the given body is a block
bool is_block(body_t *body) {
  if (*(role_t *)body_get_info(body) == HAY ||
      *(role_t *)body_get_info(body) == WOOD ||
      *(role_t *)body_get_info(body) == STEEL ||
      *(role_t *)body_get_info(body) == DIAMOND) {
    return true;
  }
  return false;
}

// checks to see if a block alr exists in that spot
// not super efficient but this function is rarely called
bool block_exists(state_t *state, vector_t loc) {
  list_t *all_bodies = scene_get_all_bodies(state->scene);
  for (size_t i = 0; i < list_size(all_bodies); i++) {
    body_t *curr_body = list_get(all_bodies, i);
    if (is_block(curr_body)) {
      list_t *points = body_get_shape(curr_body);
      vector_t top_left = *(vector_t *)(list_get(points, 0));
      vector_t bottom_right = *(vector_t *)(list_get(points, 2));
      if (loc.x < bottom_right.x && loc.x > top_left.x && loc.y < top_left.y &&
          loc.y > bottom_right.y) {
        return true;
      }
      list_free(points);
    }
  }
  // check if egg is in spot
  for (size_t i = 0; i < list_size(state->egg_grid_spots_x); i++) {
    size_t curr_x = *((size_t *)(list_get(state->egg_grid_spots_x, i)));
    if (curr_x == get_local_col(loc)) {
      for (size_t j = 0; j < list_size(state->egg_grid_spots_y); j++) {
        size_t curr_y = *((size_t *)(list_get(state->egg_grid_spots_y, j)));
        if (curr_y == get_local_row(loc)) {
          return true;
        }
      }
    }
  }
  return false;
}

// place a block on the grid
void place_block(state_t *state, vector_t loc) {
  if (loc.x > GRID_BOTTOM_LEFT.x && loc.y > GRID_BOTTOM_LEFT.y) {
    if (loc.x < GRID_BOTTOM_LEFT.x + (NUM_GRID_COLS * GRID_SQUARE_WIDTH) &&
        loc.y < GRID_BOTTOM_LEFT.y + (NUM_GRID_ROWS * GRID_SQUARE_HEIGHT)) {
      double block_health = 0;
      rgb_color_t block_color;
      size_t cost = 0;
      switch (state->block_selected) {
      case HAY:
        block_health = HAY_HEALTH;
        block_color = HAY_COLOR;
        cost = HAY_COST;
        break;
      case WOOD:
        block_health = WOOD_HEALTH;
        block_color = WOOD_COLOR;
        cost = WOOD_COST;
        break;
      case STEEL:
        block_health = STEEL_HEALTH;
        block_color = STEEL_COLOR;
        cost = STEEL_COST;
        break;
      case DIAMOND:
        block_health = DIAMOND_HEALTH;
        block_color = DIAMOND_COLOR;
        cost = DIAMOND_COST;
        break;
      }
      if (block_exists(state, loc) == false && state->credits > cost) {
        body_t *square = create_rectangle_body(
            GRID_BOTTOM_LEFT.x + (get_local_col(loc) * GRID_SQUARE_WIDTH) +
                (GRID_LINE_THICKNESS / 2),
            GRID_BOTTOM_LEFT.y +
                ((get_local_row(loc) + 1) * GRID_SQUARE_HEIGHT) -
                (GRID_LINE_THICKNESS / 2),
            GRID_SQUARE_WIDTH - GRID_LINE_THICKNESS,
            GRID_SQUARE_HEIGHT - GRID_LINE_THICKNESS, INFINITY, block_color,
            (role_t)(state->block_selected), free);
        body_set_health(square, block_health);
        scene_add_body(state->scene, square);
        state->credits -= cost;
      }
    }
  }
}

void remove_block(state_t *state, vector_t loc) {
  if (loc.x > GRID_BOTTOM_LEFT.x && loc.y > GRID_BOTTOM_LEFT.y) {
    if (loc.x < GRID_BOTTOM_LEFT.x + (NUM_GRID_COLS * GRID_SQUARE_WIDTH) &&
        loc.y < GRID_BOTTOM_LEFT.y + (NUM_GRID_ROWS * GRID_SQUARE_HEIGHT)) {
      list_t *all_bodies = scene_get_all_bodies(state->scene);
      for (size_t i = 0; i < list_size(all_bodies); i++) {
        body_t *curr_body = list_get(all_bodies, i);
        if (is_block(curr_body)) {
          list_t *points = body_get_shape(curr_body);
          vector_t top_left = *(vector_t *)(list_get(points, 0));
          vector_t bottom_right = *(vector_t *)(list_get(points, 2));
          if (loc.x < bottom_right.x && loc.x > top_left.x &&
              loc.y < top_left.y && loc.y > bottom_right.y) {
            body_remove(curr_body);
          }
          list_free(points);
        }
      }
    }
  }
}

void set_game_over(state_t *state) {
  state->game_over = true;
  state->egg_health = 0;
  state->is_paused = true;
}

// remove weapons that are out of bounds
void remove_weapons_out_of_bounds(state_t *state) {
  list_t *all_bodies = scene_get_all_bodies(state->scene);
  for (size_t i = 0; i < list_size(all_bodies); i++) {
    body_t *curr_body = list_get(all_bodies, i);
    if (*(role_t *)body_get_info(curr_body) == WEAPON) {
      list_t *shape = body_get_shape(curr_body);
      for (size_t j = 0; j < list_size(shape); j++) {
        if (((vector_t *)list_get(shape, j))->x > GRID_BOTTOM_LEFT.x) {
          if (((vector_t *)list_get(shape, j))->x >
                  WINDOW.x - MENU_WIDTH - (MENU_BORDER_WIDTH / 2) ||
              ((vector_t *)list_get(shape, j))->x < 0 ||
              ((vector_t *)list_get(shape, j))->y < ISLAND_HEIGHT) {
            body_remove(curr_body);
          }
        }
      }
      list_free(shape);
    }
  }
}

// calculate the number of credits given per round
size_t calc_credits(size_t level) {
  return (size_t)(
      round(((CREDIT_MULTIPLIER * pow((double)level, CREDIT_EXPONENT)) +
             CREDIT_ADDITION)));
}

body_t *create_weapon(size_t sides, double weapon_mass) {
  list_t *points = list_init(sides, free);
  double curr_angle = 0.0;
  double angle_inc = (TWO_PI) / sides;
  vector_t spawn_loc = (vector_t){.x = WEAPON_SPAWN_X, .y = EGG_CENTROID.y};
  for (size_t i = 0; i < sides; i++) {
    double x_val = (WEAPON_RADIUS * cos(curr_angle)) + spawn_loc.x;
    double y_val = (WEAPON_RADIUS * sin(curr_angle)) + spawn_loc.y;
    vector_t *new_point = malloc(sizeof(vector_t));
    new_point->x = x_val;
    new_point->y = y_val;
    list_add(points, new_point);
    curr_angle += angle_inc;
  }
  role_t *body_info = malloc(sizeof(role_t));
  *body_info = (role_t)(WEAPON);
  body_t *body =
      body_init_with_info(points, weapon_mass, WEAPON_COLOR, body_info, free);

  size_t launch_angle =
      (rand() % (WEAPON_ANGLE_MAX_RAD - WEAPON_ANGLE_MIN_RAD + 1)) +
      WEAPON_ANGLE_MIN_RAD;
  double launch_angle_rad = (launch_angle * PI) / 180;
  body_set_velocity(body, calc_initial_weapon_vel(launch_angle_rad));

  size_t random_angle = (rand() % (360));
  double random_angle_rad = (random_angle * PI) / 180;
  body_set_rotation(body, random_angle_rad);

  return body;
}

void calc_circles(state_t *state) {
  size_t curr_level = state->level;
  size_t num_objs = round(CIRCLES_PER_LEVEL * curr_level) + MIN_NUM_CIRCLES;
  for (size_t i = 0; i < num_objs; i++) {
    list_add(state->weapon_queue, create_weapon(CIRCLE_SIDES, CIRCLE_MASS));
  }
}

void calc_triangles(state_t *state) {
  size_t curr_level = state->level;
  size_t num_objs = round(TRIANGLES_PER_LEVEL * curr_level) + MIN_NUM_TRIANGLES;
  for (size_t i = 0; i < num_objs; i++) {
    list_add(state->weapon_queue, create_weapon(TRIANGLE_SIDES, TRIANGLE_MASS));
  }
}

void calc_squares(state_t *state) {
  size_t curr_level = state->level;
  size_t num_objs =
      round(pow(SQUARES_BASE, (double)curr_level) - SQUARES_SUBTRACT);
  for (size_t i = 0; i < num_objs; i++) {
    list_add(state->weapon_queue, create_weapon(SQUARE_SIDES, SQUARE_MASS));
  }
}

// draw circle to show which block is selected
void selected_block_circle(state_t *state) {
  // remove previous selection
  list_t *bodies = scene_get_all_bodies(state->scene);
  for (size_t i = 0; i < list_size(bodies); i++) {
    body_t *curr_body = list_get(bodies, i);
    if (*(role_t *)body_get_info(curr_body) == SELECTION_CIRCLE) {
      body_remove(curr_body);
    }
  }

  double spawn_loc_x = ((WINDOW.x - SELECTION_SEPARATION) +
                        (WINDOW.x - (MENU_WIDTH / 2) + (MENU_BLOCK_DIM / 2))) /
                       2;
  double spawn_loc_y = WINDOW.y - SELECTION_SEPARATION - (SELECTION_HEIGHT / 2);
  if (state->block_selected == WOOD) {
    spawn_loc_y -= (WOOD_INDEX * (SELECTION_HEIGHT + SELECTION_SEPARATION));
  }
  if (state->block_selected == STEEL) {
    spawn_loc_y -= (STEEL_INDEX * (SELECTION_HEIGHT + SELECTION_SEPARATION));
  }
  if (state->block_selected == DIAMOND) {
    spawn_loc_y -= (DIAMOND_INDEX * (SELECTION_HEIGHT + SELECTION_SEPARATION));
  }
  list_t *points = list_init(SELECTION_CIRCLE_SIDES, free);
  double curr_angle = 0.0;
  double angle_inc = (TWO_PI) / SELECTION_CIRCLE_SIDES;
  for (size_t i = 0; i < SELECTION_CIRCLE_SIDES; i++) {
    double x_val = (SELECTION_CIRCLE_RADIUS * cos(curr_angle)) + spawn_loc_x;
    double y_val = (SELECTION_CIRCLE_RADIUS * sin(curr_angle)) + spawn_loc_y;
    vector_t *new_point = malloc(sizeof(vector_t));
    new_point->x = x_val;
    new_point->y = y_val;
    list_add(points, new_point);
    curr_angle += angle_inc;
  }
  role_t *body_info = malloc(sizeof(role_t));
  *body_info = (role_t)(SELECTION_CIRCLE);
  body_t *body = body_init_with_info(points, INFINITY, BLACK, body_info, free);
  scene_add_body(state->scene, body);
}

// adds the appropriate number of each projectile
void start_shooting(state_t *state) {
  calc_circles(state);
  calc_triangles(state);
  calc_squares(state);
}

// restart the game
void restart(state_t *state) {
  // remove all bodies
  list_t *all_bodies = scene_get_all_bodies(state->scene);
  for (size_t i = 0; i < list_size(all_bodies); i++) {
    body_remove(list_get(all_bodies, i));
  }

  // reset other values
  state->game_state = BUILDING;
  state->last_weapon_time = 0.0;
  state->total_time_elapsed = 0.0;
  state->block_selected = HAY;
  state->weapon_queue = list_init(1, free);
  state->level = STARTING_LEVEL;
  state->credits = calc_credits(STARTING_LEVEL);
  state->egg_health = EGG_HEALTH;
  state->game_over = false;
  state->is_paused = true;

  create_background(state->scene);
  create_island(state->scene);
  create_menu(state->scene);
  create_egg(state->scene);
  create_grid(state->scene);
}

// convert smt on system where 0,0 is top right to system where 0,0 is bottom
// left
vector_t corrected_loc(vector_t loc) {
  return (vector_t){.x = loc.x, .y = WINDOW.y - loc.y};
}

void p_key_behavior(state_t *state) {
  if (state->game_over == true) {
    restart(state);
    return;
  }
  if (state->game_state == BUILDING && state->game_over == false) {
    state->game_state = SHOOTING;
    state->is_paused = false;
    start_shooting(state);
  } else if (state->game_state == SHOOTING) {
    if (state->is_paused == true) {
      state->is_paused = false;
    } else {
      state->is_paused = true;
    }
  }
}

void on_key(char key, key_event_type_t type, double held_time, state_t *state,
            vector_t loc, size_t button_type) {
  if (type == KEY_PRESSED) {
    // for some reason, the 0,0 is on the top left instead of buttom left when
    // mouse is clicked. reverse this so that 0,0 is the bottom left
    switch (key) {
    case MOUSE_CLICK:
      if (button_type == (role_t)(SDL_BUTTON_LEFT)) {
        if (state->game_state == BUILDING && state->game_over == false) {
          place_block(state, corrected_loc(loc));
        }
        if (vec_l2norm(corrected_loc(loc), PAUSE_PLAY_LOC) <=
            PAUSE_PLAY_RADIUS) {
          p_key_behavior(state);
        }
      }
      if (button_type == (role_t)(SDL_BUTTON_RIGHT)) {
        if (state->game_state == BUILDING && state->game_over == false) {
          remove_block(state, corrected_loc(loc));
        }
      }

      break;
    case MOUSE_MOVED:
      if (state->game_over == false) {
        hover_square(state, corrected_loc(loc));
      }
      break;
    case P_KEY:
      p_key_behavior(state);
      break;
    case ONE:
      state->block_selected = HAY;
      break;
    case TWO:
      state->block_selected = WOOD;
      break;
    case THREE:
      state->block_selected = STEEL;
      break;
    case FOUR:
      state->block_selected = DIAMOND;
      break;
    }
  }
}

state_t *emscripten_init() {
  srand(time(NULL));
  sdl_on_key(on_key);

  vector_t min = (vector_t){.x = 0, .y = 0};
  vector_t max = WINDOW;
  sdl_init(min, max);

  // assertions to ensure expected behavior of game
  assert(EGG_GRID_WIDTH > 0);
  assert(EGG_GRID_HEIGHT > 0);
  assert(STARTING_LEVEL > 0);
  assert(EGG_HEALTH > 0);

  state_t *state = malloc(sizeof(state_t));
  state->scene = scene_init();
  state->game_state = BUILDING;
  state->last_weapon_time = 0.0;
  state->total_time_elapsed = 0.0;
  state->block_selected = HAY;
  state->weapon_queue = list_init(1, free);
  state->level = STARTING_LEVEL;
  state->credits = calc_credits(STARTING_LEVEL);
  state->egg_health = EGG_HEALTH;
  state->game_over = false;
  state->is_paused = true;

  TTF_Font *font = TTF_OpenFont("assets/digital.ttf", MENU_TEXT_SIZE);
  text_t *text = text_init(font, free);
  state->text = text;

  TTF_Font *game_over_font =
      TTF_OpenFont("assets/digital.ttf", GAME_OVER_TEXT_SIZE);
  text_t *new_text = text_init(game_over_font, free);
  state->game_over_text = new_text;

  // add costs that will be displayed in the game
  size_t *ptr1 = malloc(sizeof(size_t));
  size_t *ptr2 = malloc(sizeof(size_t));
  size_t *ptr3 = malloc(sizeof(size_t));
  size_t *ptr4 = malloc(sizeof(size_t));
  *ptr1 = HAY_COST;
  *ptr2 = WOOD_COST;
  *ptr3 = STEEL_COST;
  *ptr4 = DIAMOND_COST;

  state->costs = list_init(NUM_OF_SELECTIONS, free);
  list_add(state->costs, ptr1);
  list_add(state->costs, ptr2);
  list_add(state->costs, ptr3);
  list_add(state->costs, ptr4);

  // add grid spots that the egg takes up
  state->egg_grid_spots_x = list_init(EGG_GRID_WIDTH, free);
  state->egg_grid_spots_y = list_init(EGG_GRID_HEIGHT, free);

  size_t *first_x = malloc(sizeof(size_t));
  *first_x = EGG_BOTTOM_LEFT_GRID_COL;
  list_add(state->egg_grid_spots_x, first_x);
  for (size_t i = 1; i < EGG_GRID_WIDTH; i++) {
    size_t *new_x = malloc(sizeof(size_t));
    *new_x = *first_x + i;
    list_add(state->egg_grid_spots_x, new_x);
  }
  size_t *first_y = malloc(sizeof(size_t));
  *first_y = EGG_BOTTOM_LEFT_GRID_ROW;
  list_add(state->egg_grid_spots_y, first_y);
  for (size_t i = 1; i < EGG_GRID_HEIGHT; i++) {
    size_t *new_y = malloc(sizeof(size_t));
    *new_y = *first_y + i;
    list_add(state->egg_grid_spots_y, new_y);
  }

  create_background(state->scene);
  create_island(state->scene);
  create_menu(state->scene);
  create_egg(state->scene);
  create_grid(state->scene);

  return state;
}

void emscripten_main(state_t *state) {
  sdl_clear();

  list_t *all_bodies = scene_get_all_bodies(state->scene);

  double dt = time_since_last_tick();
  state->last_weapon_time += dt;
  state->total_time_elapsed += dt;

  if (state->is_paused == false || state->game_state == BUILDING) {
    scene_tick(state->scene, dt);

    // apply gravity to the weapons
    for (size_t i = 0; i < list_size(all_bodies); i++) {
      body_t *curr_body = list_get(all_bodies, i);
      if (*(size_t *)body_get_info(curr_body) == WEAPON) {
        vector_t og_vel = body_get_velocity(curr_body);
        body_set_velocity(
            curr_body,
            (vector_t){.x = og_vel.x, .y = og_vel.y + (GRAVITY * dt)});
      }
    }
  }

  if (state->game_state == SHOOTING && list_size(state->weapon_queue) == 0 &&
      state->last_weapon_time >= BUILDING_PHASE_DELAY &&
      state->game_over == false) {
    state->game_state = BUILDING;
    state->level = state->level + 1;
    state->credits = state->credits + calc_credits(state->level);
    state->is_paused = true;
  }

  if (state->last_weapon_time >= WEAPON_LAUNCH_TIME_INTERVAL &&
      list_size(state->weapon_queue) > 0 && state->game_state == SHOOTING &&
      state->is_paused == false) {
    // spawn_weapon
    body_t *curr_weapon = list_remove(state->weapon_queue, 0);
    state->last_weapon_time = 0.0;
    scene_add_body(state->scene, curr_weapon);

    // create collisions between the block and the projectile created
    for (size_t i = 0; i < list_size(all_bodies); i++) {
      body_t *curr_body = list_get(all_bodies, i);
      if (is_block(curr_body) || *(role_t *)body_get_info(curr_body) == EGG) {
        create_block_collision(state->scene, curr_body, curr_weapon);
      }
    }
  }

  // draw all bodies
  if (state->game_state == BUILDING) {
    for (size_t i = 0; i < list_size(all_bodies); i++) {
      body_t *curr_body = list_get(all_bodies, i);
      list_t *curr_body_shape = body_get_shape(curr_body);
      sdl_draw_polygon(curr_body_shape, body_get_color(curr_body));
      list_free(curr_body_shape);
    }
  } else { // don't draw grid lines if not in building mode
    for (size_t i = 0; i < list_size(all_bodies); i++) {
      body_t *curr_body = list_get(all_bodies, i);
      if (*(role_t *)body_get_info(curr_body) != GRID_LINE) {
        list_t *curr_body_shape = body_get_shape(curr_body);
        sdl_draw_polygon(curr_body_shape, body_get_color(curr_body));
        list_free(curr_body_shape);
      }
    }
  }

  selected_block_circle(state);
  draw_pause_play(state);

  if (state->game_over == false) {
    remove_weapons_out_of_bounds(state);
  }

  // check and update stats based on the egg
  body_t *egg = get_egg(state->scene);
  if (body_get_health(egg) <= 0.0) {
    set_game_over(state);
    body_set_health(egg, 0.0);
    body_set_color(egg, BLACK);
  }
  state->egg_health = body_get_health(egg);

  double spawn_loc_x = ((WINDOW.x - MENU_WIDTH + SELECTION_SEPARATION) +
                        (WINDOW.x - (MENU_WIDTH / 2) - (MENU_BLOCK_DIM / 2))) /
                       2;
  double spawn_loc_y = WINDOW.y - SELECTION_SEPARATION - (SELECTION_HEIGHT / 2);
  SDL_Texture *msg = sdl_render_text(
      state, state->text, state->level, LEVEL_LABEL_LOC, state->credits,
      CREDITS_LABEL_LOC, round(state->egg_health), HEALTH_LABEL_LOC,
      state->game_over_text, state->game_over, GAME_OVER_MSG_Y, EGG_CENTROID,
      (vector_t){.x = spawn_loc_x, .y = spawn_loc_y},
      SELECTION_HEIGHT + SELECTION_SEPARATION, state->costs);
  sdl_show();

  SDL_DestroyTexture(msg);
}

void emscripten_free(state_t *state) {
  scene_free(state->scene);
  free(state);
}
