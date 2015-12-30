#include "num2words.h"
#include "string.h"
//#include <stdio.h>

const char* const HOURS_GL[] = {
  // AM hours
  "doce",
  "unha",
  "duas",
  "tres",
  "catro",
  "cinco",
  "seis",
  "sete",
  "oito",
  "nove",
  "dez",
  "once",

  // PM hours
  "doce",
  "unha",
  "duas",
  "tres",
  "catro",
  "cinco",
  "seis",
  "sete",
  "oito",
  "nove",
  "dez",
  "once"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_GL[] = {
  "*$1 en punto",
  "*$1 e cinco",
  "*$1 e dez",
  "*$1 e cuarto",
  "*$1 e vinte",
  "*$1 e vinte- cinco",
  "*$1 e media",
  "*$2 menos vinte- cinco",
  "*$2 menos vinte",
  "*$2 menos cuarto",
  "*$2 menos dez",
  "*$2 menos cinco"
};


size_t min(const size_t a, const size_t b) {
  return a < b ? a : b;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

static size_t interpolate_and_append(char* buffer, const size_t length,
    const char* parent_str, const char* first_placeholder_str, const char* second_placeholder_str) {
  const char* placeholder_str;
  char* insert_ptr = strstr(parent_str, "$1");

  if (insert_ptr) {
    placeholder_str = first_placeholder_str;
  }
  else {
    insert_ptr = strstr(parent_str, "$2");
    placeholder_str = second_placeholder_str;
  }

  size_t parent_len = strlen(parent_str);
  size_t insert_offset = insert_ptr ? (size_t) insert_ptr - (size_t) parent_str : parent_len;

  size_t remaining = length;

  remaining -= append_string(buffer, min(insert_offset, remaining), parent_str);
  remaining -= append_string(buffer, remaining, placeholder_str);
  if (insert_ptr) {
    remaining -= append_string(buffer, remaining, insert_ptr + 2);
  }

  return remaining;
}

/* simple base 10 only itoa, found: http://stackoverflow.com/questions/20435527 */
char * itoa10(int value, char *result)
{
  char const digit[] = "0123456789";
  char *p = result;
  if (value < 0) {
    *p++ = '-';
    value *= -1;
  }

  /* move number of required chars and null terminate */
  int shift = value;
  do {
    ++p;
    shift /= 10;
  } while (shift);
  *p = '\0';

  /* populate result in reverse order */
  do {
    *--p = digit [value % 10];
    value /= 10;
  } while (value);

  return result;
}

const char* get_hour(int index) {
  return HOURS_GL[index];
}

const char* get_rel(int index) {
  return RELS_GL[index];
}

void time_to_words(int hours, int minutes, int seconds, char* words, size_t buffer_size) {

  size_t remaining = buffer_size;
  memset(words, 0, buffer_size);

  // We want to operate with a resolution of 30 seconds.  So multiply
  // minutes and seconds by 2.  Then divide by (2 * 5) to carve the hour
  // into five minute intervals.
  int half_mins  = (2 * minutes) + (seconds / 30);
  int rel_index  = ((half_mins + 5) / (2 * 5)) % 12;
  int hour_index;

  if (rel_index == 0 && minutes > 30) {
    hour_index = (hours + 1) % 24;
  }
  else {
    hour_index = hours % 24;
  }

  const char* hour = get_hour(hour_index);
  const char* next_hour = get_hour((hour_index + 1) % 24);
  const char* rel  = get_rel(rel_index);

  remaining -= interpolate_and_append(words, remaining, rel, hour, next_hour);

  // Leave one space at the end
  remaining -= append_string(words, remaining, " ");

}