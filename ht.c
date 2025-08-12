#define HASH_TABLE_IMPLEMENTATION
#include "ht.h"

int main()
{
  HashTable *ht = ht_init();
  ht_add(ht, "one", (void *)"1");
  ht_add(ht, "two", (void *)"2");
  ht_add(ht, "three", (void *)"3");
  ht_add(ht, "four", (void *)"4");
  ht_add(ht, "five", (void *)"5");
  ht_add(ht, "six", (void *)"6");
  ht_add(ht, "seven", (void *)"7");
  ht_add(ht, "eight", (void *)"8");
  // ht_add(ht, "nine", (void *)"9");
  ht_add(ht, "one", (void *)"10");
  printf("%s\n", (char *)ht_get(ht, "one"));
  printf("%s\n", (char *)ht_get(ht, "two"));
  printf("%s\n", (char *)ht_get(ht, "three"));
  printf("%s\n", (char *)ht_get(ht, "four"));
  printf("%s\n", (char *)ht_get(ht, "five"));
  printf("%s\n", (char *)ht_get(ht, "six"));
  printf("%s\n", (char *)ht_get(ht, "seven"));
  printf("%s\n", (char *)ht_get(ht, "eight"));
  // printf("%s\n", (char *)ht_get(ht, "nine"));
  return 0;
}