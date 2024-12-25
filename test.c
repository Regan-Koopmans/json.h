#include "json.h"
#include <stdio.h>

void case_one() {
  // The library should be able to handle a simple
  // unicode JSON
  printf("Test simple case\n");
  struct JSON json = json_parse("{'hello' : '안녕하세요'}");
}

int main() {
  case_one();
  return 0;
}
