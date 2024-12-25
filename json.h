/*
 ┌──────────────────────────────────────┐
 │                json.h                │
 │                                      │
 │       Simple and Efficient JSON      │
 │                 SDG                  │
 │            Regan Koopmans            │
 │            version 0.0.1             │
 └──────────────────────────────────────┘
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
--- MEMORY MANAGEMENT -----

This library uses memory pools or "arena"s internally for memory
management. This helps with the overhead of many small allocations
when parsing large JSON.
*/
struct JsonArena {
  size_t size;
  char *base;
  size_t offset;
};

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

void json_arena_destroy(struct JsonArena *arena) {
  free(arena->base);
  free(arena);
}

void json_arena_reset(struct JsonArena *arena) { arena->offset = 0; }

//////////////// LEXING ///////////////////////////////////////////////

enum JsonToken {
  Comma = ',',
  BracketOpen = '{',
  BracketClose = '}',
  Colon = ':',
  String = 's',
  Number = 'n',
  BoolTrue = 't',
  BoolFalse = 'f',
  SquareBracketOpen = '[',
  SquareBracketClose = ']'
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
  int index = 0;
  int token_count = 0;
  struct Token *current = json_arena_alloc(token_buffer, sizeof(struct Token));
  while (index < length) {

    if (source[index] == '\'' || source[index] == '"') {
      char closing = source[index];
      current->type = String;
      int end = index + 1;
      while (end < length && source[end] != closing) { end += 1; }

      // TODO: Handle end

      int to_allocate = end - index - 1;
      char *extracted = malloc(sizeof(char) * to_allocate);
      strncpy(extracted, source + index + 1, to_allocate);
      free(extracted);
      index = end;
    
    } else {
      token_count += 1;
      current->type = source[index];
      current = json_arena_alloc(token_buffer, sizeof(struct Token));
    }
    index += 1;
  }

  struct Token *final_tokens = malloc(sizeof(struct Token) * token_count);
  memcpy(final_tokens, (struct Token *)token_buffer->base,
         sizeof(struct Token) * token_count);

  struct Tokens tokens;
  tokens.count = token_count;
  tokens.list = final_tokens;
  json_arena_destroy(token_buffer);
  return tokens;
}

/////////////// PARSING //////////////////////

struct JSON {};

struct JSON json_parse(const char *json) {
  struct JSON response;
  struct Tokens tokens = tokenize(json, strlen(json));
  free(tokens.list);
  return response;
}
