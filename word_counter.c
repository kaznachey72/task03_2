#include "word_counter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

char *strdup(const char *s);

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

void swap(word_counter_t *lhs, word_counter_t *rhs) 
{
    item_t *tmp_items = lhs->items;
    lhs->items = rhs->items;
    rhs->items = tmp_items;

    size_t tmp_size = lhs->size;
    lhs->size = rhs->size;
    rhs->size = tmp_size;

    size_t tmp_capacity = lhs->capacity;
    lhs->capacity = rhs->capacity;
    rhs->capacity = tmp_capacity;
}


word_counter_t *word_counter_create(size_t capacity)
{
    word_counter_t *ret = (word_counter_t*) malloc(sizeof(word_counter_t));
   
    ret->size = 0;
    ret->capacity = capacity;
    ret->items = (item_t*) calloc(capacity, sizeof(item_t));

    return ret;
}

void rehash(word_counter_t *wc)
{
    word_counter_t *tmp_wc = word_counter_create(wc->capacity * 2);
    for (size_t i=0; i!=wc->capacity; ++i) {
        item_t *item = &wc->items[i];
        word_counter_add(tmp_wc, item->key, item->value);
    }  
    swap(wc, tmp_wc);
    word_counter_destroy(tmp_wc);
}

void word_counter_add(word_counter_t *wc, const char *word, size_t count)
{
    if (!word) {
        return;
    }

    size_t word_len = strlen(word);
    
    size_t offset = 0;
    bool is_collision = true;
    size_t hash_word = hash(word);
    while (is_collision) {
        size_t index = (hash_word + offset * 773) % wc->capacity;
        item_t *item = &wc->items[index];
        if (!item->key) {
            is_collision = false;
            item->key = strdup(word);
            item->value = count;
            wc->size += 1;
        }
        else if (strncmp(item->key, word, word_len) == 0) {
            is_collision = false;
            item->value += count;
        } 
        else {
            is_collision = true;
            ++offset;
        }
    }

    if (wc->size == wc->capacity) {
        rehash(wc);
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
