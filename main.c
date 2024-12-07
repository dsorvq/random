#include <stdio.h>
#include "s21_string.h"

int main() {
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "|%09.3d|", 12);
    sprintf    (s, "|%09.3d|", 12);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%.0d", 0);
    sprintf    (s, "%.0d", 0);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%+ 5d", 0);
    sprintf    (s, "%+ 5d", 0);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  /*
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%.5d", 0);
    sprintf    (s, "%.5d", 0);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%5d", 0);
    sprintf    (s, "%5d", 0);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "some%d", 7);
    sprintf    (s, "some%d", 7);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%.10d", 7);
    sprintf    (s, "%.10d", 7);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%+.0d", 0);
    sprintf    (s, "%+.0d", 0);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%+10.0d", 0);
    sprintf    (s, "%+10.0d", 0);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%#+010.0d", 0);
    sprintf    (s, "%+010.0d", 0);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%+010d", 0);
    sprintf    (s, "%+010d", 0);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%09.3d", 12);
    sprintf    (s, "%09.3d", 12);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "%09.3d", 12);
    sprintf    (s, "%09.3d", 12);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "|%#o|", 7u);
    sprintf(s, "|%#o|", 7u);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "|%#o|", 0u);
    sprintf(s, "|%#o|", 0u);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "|%#.10X|", 7u);
    sprintf(s, "|%#.10X|", 7u);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "|%#X|", 7u);
    sprintf(s, "|%#X|", 7u);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  {
    char m[100];
    char s[100];
    s21_sprintf(m, "|%#X|", 0u);
    sprintf(s, "|%#X|", 0u);
    printf("m:|%s|\n", m);
    printf("s:|%s|\n\n", s);
  }
  */
}
