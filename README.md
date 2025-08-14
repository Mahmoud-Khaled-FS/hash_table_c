# HashTable C Library

A simple **open-addressing hash table implementation** in C with double hashing for collision resolution. Supports dynamic resizing, key deletion, and retrieval of keys/values.

---

## Features

- Open addressing with **double hashing**
- Dynamic resizing based on **load factor** (`HT_MAX_LOAD = 0.7`)
- Supports **string keys** and generic `void*` values
- **Tombstone markers** for safe deletion without breaking probe sequences
- Functions to get all keys or all values
- Custom initial capacities and prime number growth for better distribution

---

## Installation

1. Copy `ht.h` into your project.
2. Define `HASH_TABLE_IMPLEMENTATION` in **one** `.c` file to include the implementation:

```c
#define HASH_TABLE_IMPLEMENTATION
#include "ht.h"
```

3. Include `ht.h` in other files where you want to use the hash table.

---

## Usage

```c
#include "ht.h"
#include <stdio.h>

int main() {
    // Initialize hash table
    HashTable *ht = ht_init();

    // Add items
    ht_add(ht, "key1", "value1");
    ht_add(ht, "key2", "value2");

    // Retrieve items
    char *val = (char *)ht_get(ht, "key1");
    if (val) printf("key1: %s\n", val);

    // Check existence
    if (ht_has(ht, "key2")) printf("key2 exists\n");

    // Delete a key
    ht_delete(ht, "key1");

    // Get all keys
    uint32_t size;
    char **keys = ht_keys(ht, &size);
    for (uint32_t i = 0; i < size; i++) {
        printf("Key: %s\n", keys[i]);
        free(keys[i]);
    }
    free(keys);

    // Free hash table
    ht_free(&ht);

    return 0;
}
```

---

## API Reference

| Function                                             | Description                                        |
| ---------------------------------------------------- | -------------------------------------------------- |
| `HashTable* ht_init()`                               | Create a new hash table                            |
| `void ht_add(HashTable *ht, char *key, void *value)` | Add or update a key-value pair                     |
| `void* ht_get(HashTable *ht, char *key)`             | Retrieve the value for a key                       |
| `bool ht_has(HashTable *ht, char *key)`              | Check if a key exists                              |
| `void ht_delete(HashTable *ht, char *key)`           | Delete a key-value pair                            |
| `char** ht_keys(HashTable *ht, uint32_t *size)`      | Get all keys (caller must free each key and array) |
| `void** ht_values(HashTable *ht, uint32_t *size)`    | Get all values (caller must free array)            |
| `void ht_free(HashTable **ht_ptr)`                   | Free the hash table and all stored keys/values     |

---

## Notes

- Capacity grows using predefined **prime numbers** to reduce clustering.
- Uses **double hashing** to handle collisions.
- Load factor is `0.7` — table resizes automatically when exceeded.
- Deleted items are replaced with a **TOMBSTONE** marker to maintain probe sequences.

---

## License

MIT License
