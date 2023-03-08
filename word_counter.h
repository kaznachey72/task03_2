#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <stddef.h>

struct word_counter_t;

struct word_counter_t *word_counter_create(size_t capacity);
void word_counter_add(struct word_counter_t *wc, const char *word);
void word_counter_print(struct word_counter_t *wc);
void word_counter_destroy(struct word_counter_t *wc);

#endif
