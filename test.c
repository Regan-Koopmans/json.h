#include "json.h"
#include <stdio.h>

void case_one() {
  printf("Test simple case");
  struct JSON json = json_parse("{}");
  printf(" ✅\n");
}

int main() {
  case_one();
  return 0;
}
