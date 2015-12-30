#pragma once
#include "string.h"

const char* const HOURS_GL[24];
const char* const RELS_GL[12];

void time_to_words(int hours, int minutes, int seconds, char* words, size_t length);
void date_to_words(int day, int date, int month, char* words, size_t length);

char * itoa10(int value, char *result);