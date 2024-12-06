#define s21_NULL ((void*)0)
#define S21_TEXTMAX 2048

typedef long unsigned s21_size_t;

s21_size_t s21_strlen(const char *str);

s21_size_t s21_sprintf(char *str, const char* format, ...);
