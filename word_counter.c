#include "word_counter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct item_t {
    char *key;
    size_t value;
}; 
typedef struct item_t item_t;

struct word_counter_t 
{
    size_t size;
    size_t capacity;
    item_t *items;
};
typedef struct word_counter_t word_counter_t;


size_t hash(const char *word)
{
    const unsigned char *str = (const unsigned char*)word;
    size_t ret = 5381;
    unsigned char c;
    while ((c = *str++)) {
        ret = ((ret << 5) + ret) + c; // (ret * 33) + c
    }   
    return ret;
}


word_counter_t *word_counter_create(size_t capacity)
{
    word_counter_t *ret = (word_counter_t*) malloc(sizeof(word_counter_t));
   
    ret->size = 0;
    ret->capacity = capacity;
    ret->items = (item_t*) malloc(capacity * sizeof(item_t));

    for (size_t i=0; i!=capacity; ++i) {
        ret->items[i].key = NULL;
        ret->items[i].value = 0;
    }
    
    return ret;
}

void word_counter_add(word_counter_t *wc, const char *word)
{
    size_t word_len = strlen(word);
    
    size_t offset = 0;
    bool is_collision = true;
    while (is_collision && ((wc->capacity - wc->size) > 0)) {
        size_t index = (hash(word) + offset) % wc->capacity;
        item_t *item = &wc->items[index];
        if (!item->key) {
            is_collision = false;
            item->key = (char*) malloc(word_len * sizeof(char) + 1);
            strncpy(item->key, word, word_len);
            item->key[word_len] = '\0';
            item->value = 1;
            wc->size += 1;
        }
        else if (strncmp(item->key, word, word_len) == 0) {
            is_collision = false;
            item->value += 1;
        } 
        else {
            is_collision = true;
            ++offset;
        }
    }

    if (is_collision) {
        fprintf(stderr, "ERROR: collision\n");
        abort();
    }
}

void word_counter_print(word_counter_t *wc)
{
    for (size_t i=0; i!=wc->capacity; ++i) {
        item_t *item = &wc->items[i];
        if (item->key) {
            printf("%lu \t %s\n", item->value, item->key);
        }
    }
}

void word_counter_destroy(word_counter_t *wc)
{
    for (size_t i=0; i!=wc->capacity; ++i) {
        item_t *item = &wc->items[i];
        if (item->key) {
            free(item->key);
        }
    }
    free(wc->items);
    free(wc);
}
