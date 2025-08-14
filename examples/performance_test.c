#define HASH_TABLE_IMPLEMENTATION
#include "../ht.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ADD_COUNT 1000000
#define SEARCH_COUNT 1000000

// Simple cross-platform timing wrapper
double get_time_seconds()
{
  return (double)clock() / CLOCKS_PER_SEC;
}

int main()
{
  printf("=== Performance Test ===\n");

  HashTable *ht = ht_init();
  double start, end;

  // Test adding
  start = get_time_seconds();
  for (int i = 0; i < ADD_COUNT; i++)
  {
    char key[32], value[32];
    sprintf(key, "key_%d", i);
    sprintf(value, "value_%d", i);
    ht_add(ht, key, strdup(value));
  }
  end = get_time_seconds();
  printf("Adding %d elements: %.6f seconds\n", ADD_COUNT, end - start);

  // Test searching
  start = get_time_seconds();
  uint32_t missing = 0;
  for (int i = 0; i < SEARCH_COUNT; i++)
  {
    int idx = rand() % ADD_COUNT; // random key
    char key[32];
    sprintf(key, "key_%d", idx);
    char *val = ht_get(ht, key);
    if (!val)
    {
      // printf("Error: missing key %s\n", key);
      missing++;
    }
  }
  end = get_time_seconds();
  assert(missing == 0);
  printf("Searching %d elements: %.6f seconds\n", SEARCH_COUNT, end - start);

  printf("Final size: %zu\n", ht->len);
  printf("Final capacity: %zu\n", ht->cap);

  ht_free(&ht);

  return 0;
}
