#ifndef HT_H
#define HT_H

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef struct ht
{
  char *key;
  void *value;
} HT_Item;

typedef struct
{
  HT_Item **items;
  size_t len;
  size_t cap;
} HashTable;

static HT_Item TOMBSTONE = {.key = NULL, .value = NULL};
#define HT_MAX_LOAD 0.7f
#define HT_INIT_CAP 17

HashTable *ht_init();

void ht_add(HashTable *ht, char *key, void *value);
void *ht_get(HashTable *ht, char *key);
bool ht_has(HashTable *ht, char *key);
char **ht_keys(HashTable *ht, uint32_t *size);
void **ht_values(HashTable *ht, uint32_t *size);
void ht_free(HashTable **ht);

// #ifdef HASH_TABLE_IMPLEMENTATION

uint32_t fnv1a_hash(char *key)
{
  uint32_t hash = 2166136261u; // FNV offset basis
  while (*key)
  {
    hash ^= (uint8_t)*key; // XOR byte
    hash *= 16777619u;     // FNV prime
    key++;
  }
  return hash;
}

HashTable *ht_init()
{
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  ht->items = (HT_Item **)malloc(sizeof(HT_Item *) * HT_INIT_CAP);
  ht->len = 0;
  ht->cap = HT_INIT_CAP;
  return ht;
}

void ht_inc_cap(HashTable *ht)
{
  uint32_t new_cap = ht->cap * 2;
  HT_Item **new_items_ptr = (HT_Item **)malloc(sizeof(HT_Item *) * new_cap);

  for (uint32_t i = 0; i < ht->cap; i++)
  {
    HT_Item *item = ht->items[i];
    if (!ht->items[i] || ht->items[i] == &TOMBSTONE)
      continue;
    uint32_t hash = fnv1a_hash(item->key);
    uint32_t index;
    uint32_t j = 0;
    while (j < new_cap)
    {
      index = (hash + j * j) % new_cap;
      if (new_items_ptr[index] == NULL)
      {
        new_items_ptr[index] = item;
        break;
      }
      j++;
    }
  }
  free(ht->items);
  ht->items = new_items_ptr;
  ht->cap = new_cap;
}

void ht_add(HashTable *ht, char *key, void *value)
{
  if ((float)ht->len / ht->cap > HT_MAX_LOAD)
  {
    ht_inc_cap(ht);
  }
  uint32_t hash = fnv1a_hash(key);
  // TODO (MAHMOUD) - Use 2 Hash insted of Quadratic Probing
  uint32_t index;
  uint32_t i = 0;
  int32_t first_tombstone = -1;
  while (i < ht->cap)
  {
    index = (hash + i * i) % ht->cap;
    printf("---->%s: try = %d hash = %ld\n", key, index, hash);
    if (ht->items[index] == NULL)
    {
      break;
    }
    if (ht->items[index] == &TOMBSTONE)
    {
      if (first_tombstone == -1)
      {
        first_tombstone = index;
      }
      i++;
      continue;
    }
    if (strcmp(key, ht->items[index]->key) == 0)
    {
      ht->items[index]->value = value;
      return;
    }

    i++;
  }
  if (first_tombstone != -1)
  {
    index = first_tombstone;
  }
  printf("key: %s, index: %ld\n", key, index);
  ht->items[index] = malloc(sizeof(HT_Item));
  ht->items[index]->key = strdup(key);
  ht->items[index]->value = value;
  ht->len++;
  return;
}

void *ht_get(HashTable *ht, char *key)
{
  uint32_t hash = fnv1a_hash(key);
  uint32_t index;
  uint32_t i = 0;

  while (i < ht->cap)
  {
    index = (hash + i * i) % ht->cap;
    HT_Item *item = ht->items[index];
    if (item == NULL)
    {
      return NULL;
    }

    if (item == &TOMBSTONE)
    {
      i++;
      continue;
    }
    if (strcmp(item->key, key) == 0)
    {
      return item->value;
    }
    i++;
  }
  return NULL;
}

bool ht_has(HashTable *ht, char *key)
{
  uint32_t hash = fnv1a_hash(key);
  uint32_t index;
  uint32_t i = 0;

  while (i < ht->cap)
  {
    index = (hash + i * i) % ht->cap;
    HT_Item *item = ht->items[index];
    if (item == NULL)
    {
      return false;
    }

    if (item == &TOMBSTONE)
    {
      i++;
      continue;
    }
    if (strcmp(item->key, key) == 0)
    {
      return true;
    }
    i++;
  }
  return false;
}

char **ht_keys(HashTable *ht, uint32_t *size)
{
  char **keys = (char **)malloc(sizeof(char *) * ht->len);
  uint32_t items_index = 0;
  for (size_t i = 0; i < ht->cap; i++)
  {
    if (!ht->items[i] || ht->items[i] == &TOMBSTONE)
      continue;
    keys[items_index++] = strdup(ht->items[i]->key);
  }
  *size = items_index;
  return keys;
}

void **ht_values(HashTable *ht, uint32_t *size)
{
  void **values = malloc(sizeof(void *) * ht->len);
  uint32_t items_index = 0;
  for (size_t i = 0; i < ht->cap; i++)
  {
    if (!ht->items[i] || ht->items[i] == &TOMBSTONE)
      continue;
    values[items_index++] = ht->items[i]->value;
  }
  *size = items_index;
  return values;
}

void ht_free(HashTable **ht_ptr)
{
  if (!ht_ptr || !*ht_ptr)
    return;
  HashTable *ht = *ht_ptr;
  for (uint32_t i = 0; i < ht->cap; i++)
  {
    if (!ht->items[i] || ht->items[i] == &TOMBSTONE)
      continue;
    free(ht->items[i]->key);
    free(ht->items[i]);
  }
  free(ht->items);
  free(ht);
  *ht_ptr = NULL;
};

// #endif

#endif
