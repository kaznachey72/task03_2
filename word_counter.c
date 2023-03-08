#include "word_counter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct item_t {
    char *key;
    size_t value;
    struct item_t *next;
}; 
typedef struct item_t item_t;

struct word_counter_t 
{
    size_t capacity;
    item_t **items;
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
    
    ret->capacity = capacity;
    ret->items = (item_t**) malloc(capacity * sizeof(item_t*));

    for (size_t i=0; i!=capacity; ++i) {
        ret->items[i] = NULL;
    }
    
    return ret;
}

void word_counter_add(word_counter_t *wc, const char *word)
{
    size_t word_len = strlen(word);
    size_t index = hash(word) % wc->capacity;
    item_t *item = wc->items[index];
    item_t *last = NULL;

    while (item) {
        if (strncmp(item->key, word, word_len) == 0) {
            item->value += 1;
            break;
        }
        last = item;
        item = item->next;
    }

    if (!item) {
        item = (item_t*) malloc(sizeof(item_t));
        if (last) {
            last->next = item;
        } else {
            wc->items[index] = item;
        }
        
        item->key = (char*) malloc(word_len * sizeof(char) + 1);
        strncpy(item->key, word, word_len);
        item->key[word_len] = '\0';
        item->value = 1;
        item->next = NULL;
    }
}

void word_counter_print(word_counter_t *wc)
{
    for (size_t i=0; i!=wc->capacity; ++i) {
        item_t *item = wc->items[i];
        while (item) {
            printf("%lu \t %s\n", item->value, item->key);
            item = item->next;
        }
    }
}

void word_counter_destroy(word_counter_t *wc)
{
    for (size_t i=0; i!=wc->capacity; ++i) {
        item_t *item = wc->items[i];
        while (item) {
            item_t *next = item->next;
            free(item->key);
            free(item);
            item = next;
        }
    }
    free(wc->items);
    free(wc);
}
