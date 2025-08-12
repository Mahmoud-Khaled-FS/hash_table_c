#ifndef HT_H
#define HT_H

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

HashTable *ht_init();

void ht_add(HashTable *ht, char *key, void *value);
void *ht_get(HashTable *ht, char *key);

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
  ht->items = (HT_Item **)malloc(sizeof(HT_Item) * 8);
  ht->len = 0;
  ht->cap = 8;
  return ht;
}

void ht_add(HashTable *ht, char *key, void *value)
{
  uint32_t hash = fnv1a_hash(key);
  uint32_t index;
  uint32_t i = 0;
  int32_t first_tombstone = -1;
  while (i < ht->cap)
  {
    index = (hash + i * i) % ht->cap;
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

// #endif

#endif
