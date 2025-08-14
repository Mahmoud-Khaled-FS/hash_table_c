#define HASH_TABLE_IMPLEMENTATION
#include "../ht.h"
#include <stdio.h>

int main()
{
  HashTable *ht = ht_init();
  if (ht == NULL)
  {
    printf("Failed to initialize hash table\n");
    return 1;
  }
  ht_add(ht, "key1", "value1");
  ht_add(ht, "key2", "value2");
  ht_add(ht, "key3", "value3");

  printf("key1: %s\n", (char *)ht_get(ht, "key1"));
  printf("key2: %s\n", (char *)ht_get(ht, "key2"));
  printf("key3: %s\n", (char *)ht_get(ht, "key3"));
  
  if(ht_has(ht, "key1"))
  {
    printf("key1 exists\n");
  }

  ht_delete(ht, "key1");
  if(!ht_has(ht, "key1"))
  {
    printf("key1 deleted\n");
  }

  uint32_t size;
  char **keys = ht_keys(ht, &size);
  for(uint32_t i = 0; i < size; i++)
  {
    printf("key%d: %s\n", i + 1, keys[i]);
  }

  ht_free(&ht);
  return 0;
}