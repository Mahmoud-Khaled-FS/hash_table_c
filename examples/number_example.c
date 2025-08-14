#define HASH_TABLE_IMPLEMENTATION
#include "../ht.h"
#include <stdio.h>
#include <assert.h>

int main()
{
  HashTable *ht = ht_init();

  int x = 1;
  int y = 2;
  int z = 3;

  ht_add(ht, "key1", &x);
  ht_add(ht, "key2", &y);
  ht_add(ht, "key3", &z);

  printf("key1: %d\n", *(int *)ht_get(ht, "key1"));
  printf("key2: %d\n", *(int *)ht_get(ht, "key2"));
  printf("key3: %d\n", *(int *)ht_get(ht, "key3"));

  printf("\ndelete element 'key2'\n");
  ht_delete(ht, "key2");

  if (!ht_has(ht, "key2"))
  {
    printf("delete 'key2' success\n");
  }

  uint32_t values_size;
  void **values = ht_values(ht, &values_size);
  printf("\nremaining values (%d):\n", values_size);
  for (uint32_t i = 0; i < values_size; i++)
  {
    printf("- %d\n", *(int *)values[i]);
  }
  ht_free(&ht);
  return 0;
}