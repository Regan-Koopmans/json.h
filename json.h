/*
 ┌──────────────────────────────────────┐ 
 │                json.h                │
 │                                      │
 │       Simple and Efficient JSON      │
 │                                      │
 │            Regan Koopmans            │
 │            version 0.0.1             │
 └──────────────────────────────────────┘
*/


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////// MEMORY MANAGEMENT //////////////

struct JsonArena {
  size_t size;
  char *base;
  size_t offset;
};

/*
  Create an arena of initial_size bytes
*/
struct JsonArena *json_arena_create(size_t initial_size) {
  struct JsonArena *arena = malloc(sizeof(struct JsonArena));
  arena->base = malloc(initial_size);
  arena->size = initial_size;
  return arena;
}

void *json_arena_alloc(struct JsonArena *arena, size_t size) {
  if (arena->offset + size > arena->size) {
    // TODO: Resize here
    return NULL;
  }
  void *ptr = arena->base + arena->offset;
  arena->offset += size;
  return ptr;
}

void json_arena_grow(struct JsonArena *arena) {}

/*
  Release the memory for an arena.
 */
void json_arena_destroy(struct JsonArena *arena) {
  free(arena->base);
  free(arena);
}

void json_arena_reset(struct JsonArena *arena) { arena->offset = 0; }

//////////////// LEXING /////////////////////

enum JsonToken {
  Comma,
  BracketOpen,
  BracketClose,
  Colon,
  String,
  Number,
  BoolTrue,
  BoolFalse,
  SquareBracketOpen,
  SquareBracketClose
};

struct Token {
  enum JsonToken type;
  char *value;
};

struct Tokens {
  int count;
  struct Token *list;
};

struct Tokens tokenize(const char *source, int length) {
  struct JsonArena *token_buffer = json_arena_create(1024 * 16);
  bool in_string = false;
  bool in_number = false;

  int index = 0;
  int token_count = 0;
  struct Token *current = json_arena_alloc(token_buffer, sizeof(struct Token));
  while (index < length) {

    if (source[index] == '{') {

      token_count += 1;
      current->type = BracketOpen;
      current = json_arena_alloc(token_buffer, sizeof(struct Token));

    } else if (source[index] == '}') {

      token_count += 1;
      current->type = BracketClose;
      current = json_arena_alloc(token_buffer, sizeof(struct Token));

    } else if (source[index] == '[') {

      token_count += 1;
      current->type = SquareBracketOpen;
      current = json_arena_alloc(token_buffer, sizeof(struct Token));
    }

    index += 1;
  }

  struct Tokens tokens;
  tokens.count = token_count;

  json_arena_destroy(token_buffer);
  return tokens;
}

/////////////// PARSING //////////////////////

struct JSON {};

struct JSON json_parse(const char *json) {
  struct JSON response;
  tokenize(json, strlen(json));
  return response;
}
