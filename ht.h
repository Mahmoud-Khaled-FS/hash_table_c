#ifndef HT_H
#define HT_H

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
  size_t cap_index;
} HashTable;

static HT_Item TOMBSTONE = {.key = NULL, .value = NULL};

#define CAPSIZE 26

static uint32_t cap_size[CAPSIZE] = {
    53,
    97,
    193,
    389,
    769,
    1543,
    3079,
    6151,
    12289,
    24593,
    49157,
    98317,
    196613,
    393241,
    786433,
    1572869,
    3145739,
    6291469,
    12582917,
    25165843,
    50331653,
    100663319,
    201326611,
    402653189,
    805306457,
    1610612741,
};

#define HT_MAX_LOAD 0.7f

#define HT_INIT_CAP cap_size[0]

#define HASH2(hash, cap) (1 + (hash % (cap - 1)))

HashTable *ht_init();
void ht_add(HashTable *ht, char *key, void *value);
void ht_inc_cap(HashTable *ht);
void *ht_get(HashTable *ht, char *key);
bool ht_has(HashTable *ht, char *key);
char **ht_keys(HashTable *ht, uint32_t *size);
void **ht_values(HashTable *ht, uint32_t *size);
void ht_delete(HashTable *ht, char *key);
void ht_free(HashTable **ht);

#ifdef HASH_TABLE_IMPLEMENTATION

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
  ht->items = (HT_Item **)calloc(HT_INIT_CAP, sizeof(HT_Item *));
  ht->len = 0;
  ht->cap = HT_INIT_CAP;
  ht->cap_index = 0;
  return ht;
}

void ht_inc_cap(HashTable *ht)
{
  if (ht->cap_index + 1 == CAPSIZE)
  {
    fprintf(stderr, "Hash table capacity overflow\n");
    exit(1);
  }
  uint32_t new_cap = cap_size[++ht->cap_index];
  HT_Item **new_items_ptr = (HT_Item **)calloc(new_cap, sizeof(HT_Item *));

  for (uint32_t i = 0; i < ht->cap; i++)
  {
    HT_Item *item = ht->items[i];
    if (!ht->items[i] || ht->items[i] == &TOMBSTONE)
      continue;
    uint32_t hash = fnv1a_hash(item->key);
    uint32_t hash2 = HASH2(hash, new_cap);
    uint32_t index;
    uint32_t j = 0;
    while (j < new_cap)
    {
      index = (hash + j * hash2) % new_cap;

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
  uint32_t hash2 = HASH2(hash, ht->cap);
  uint32_t index;
  uint32_t i = 0;
  int32_t first_tombstone = -1;
  while (i < ht->cap)
  {
    index = (hash + i * hash2) % ht->cap;
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

  ht->items[index] = malloc(sizeof(HT_Item));
  ht->items[index]->key = strdup(key);
  ht->items[index]->value = value;
  ht->len++;
  return;
}

void *ht_get(HashTable *ht, char *key)
{
  uint32_t hash = fnv1a_hash(key);
  uint32_t hash2 = HASH2(hash, ht->cap);
  uint32_t index;
  uint32_t i = 0;

  while (i < ht->cap)
  {
    index = (hash + i * hash2) % ht->cap;
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
  uint32_t hash2 = HASH2(hash, ht->cap);
  uint32_t index;
  uint32_t i = 0;

  while (i < ht->cap)
  {
    index = (hash + i * hash2) % ht->cap;
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

void ht_delete(HashTable *ht, char *key)
{
  uint32_t hash = fnv1a_hash(key);
  uint32_t hash2 = HASH2(hash, ht->cap);
  uint32_t index;
  uint32_t i = 0;

  while (i < ht->cap)
  {
    index = (hash + i * hash2) % ht->cap;
    HT_Item *item = ht->items[index];
    if (item == NULL)
    {
      return;
    }

    if (item == &TOMBSTONE)
    {
      i++;
      continue;
    }
    if (strcmp(item->key, key) == 0)
    {
      free(ht->items[index]->key);
      free(ht->items[index]);
      ht->items[index] = &TOMBSTONE;
      return;
    }
    i++;
  }
  return;
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
}

#endif

#endif
