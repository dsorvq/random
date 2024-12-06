#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include <stdio.h>

#include "s21_string.h"

typedef enum {
  TYPE_MODIFICATION_DEFAULT,
  TYPE_MODIFICATION_h,
  TYPE_MODIFICATION_l,
  TYPE_MODIFICATION_L
} type_modification_t;

typedef enum {
  TYPE_DEFAULT,
  TYPE_c,
  TYPE_d,
  TYPE_i,
  TYPE_f,
  TYPE_s,
  TYPE_u,
  TYPE_PERCENT,
  TYPE_g,
  TYPE_G,
  TYPE_e,
  TYPE_E,
  TYPE_x,
  TYPE_X,
  TYPE_o,
  TYPE_p
} type_t;

typedef struct {
  bool left_justify;
  bool always_sign;
  bool blank_positive;
  bool use_prefix;
  bool zero_padding;
} flags_t;

typedef struct {
  unsigned precision;
  bool is_precision_set;
} precision_t;

typedef struct {
  flags_t flags;
  unsigned width;
  precision_t precision;
  type_modification_t type_modification;
  type_t type;
} format_specification_t;

static flags_t parse_flags(const char* format, s21_size_t* i) {
  flags_t flags = {0};

  for (int flags_parsed = 0; !flags_parsed;) {
    char flag = format[*i];
    if (flag == '-') {
      flags.left_justify = true;
    } else if (flag == '+') {
      flags.always_sign = true;
    } else if (flag == ' ') {
      flags.blank_positive = true;
    } else if (flag == '#') {
      flags.use_prefix = true;
    } else if (flag == '0') {
      flags.zero_padding = true;
    } else {
      flags_parsed = true;
    }
    *i = flags_parsed ? *i : *i + 1;
  }

  return flags;
}

static unsigned parse_width(const char* format, s21_size_t* i, va_list args) {
  unsigned width = 0;

  if (isdigit(format[*i])) {
    char* endp;
    width = strtol(format + *i, &endp, 10);
    *i = endp - format;
  } else if (format[*i] == '*') {
    width = va_arg(args, int);
    ++(*i);
  }

  return width;
}

// TODO: behaviour should be different depending on the type, in case of
// no prevision modification is provided
// current problem: i don't know the type yet
static precision_t parse_precision(const char* format, s21_size_t* i, va_list args) {
  precision_t precision = {.precision = 0, .is_precision_set = false};

  if (format[*i] == '.') {
    ++(*i);
    if (isdigit(format[*i])) {
      char* endp;
      precision.precision = strtol(format + *i, &endp, 10);
      *i = endp - format;
    } else if (format[*i] == '*') {
      precision.precision = va_arg(args, int);
      ++(*i);
    }
    precision.is_precision_set = true;
  }

  return precision;
}

static type_modification_t parse_type_modification(const char* format, s21_size_t* i) {
  type_modification_t type_modification = TYPE_MODIFICATION_DEFAULT;

  if (format[*i] == 'l') {
      type_modification = TYPE_MODIFICATION_l;
  } else if (format[*i] == 'L') {
      type_modification = TYPE_MODIFICATION_L;
  } else if (format[*i] == 'h') {
      type_modification = TYPE_MODIFICATION_h;
  }

  if (type_modification != TYPE_MODIFICATION_DEFAULT) {
    ++(*i);
  }
  return type_modification;
}

static type_t parse_type(const char* format, s21_size_t* i) {
  type_t type = TYPE_DEFAULT;

  if (format[*i] == 'c') {
        type = TYPE_c;
    } else if (format[*i] == 'd') {
        type = TYPE_d;
    } else if (format[*i] == 'i') {
        type = TYPE_i;
    } else if (format[*i] == 'f') {
        type = TYPE_f;
    } else if (format[*i] == 's') {
        type = TYPE_s;
    } else if (format[*i] == 'u') {
        type = TYPE_u;
    } else if (format[*i] == '%') {
        type = TYPE_PERCENT;
    } else if (format[*i] == 'g') {
        type = TYPE_g;
    } else if (format[*i] == 'G') {
        type = TYPE_G;
    } else if (format[*i] == 'e') {
        type = TYPE_e;
    } else if (format[*i] == 'E') {
        type = TYPE_E;
    } else if (format[*i] == 'x') {
        type = TYPE_x;
    } else if (format[*i] == 'X') {
        type = TYPE_X;
    } else if (format[*i] == 'o') {
        type = TYPE_o;
    } else if (format[*i] == 'p') {
        type = TYPE_p;
    }

  if (type != TYPE_DEFAULT) {
    ++(*i);
  }
  return type;
}

static format_specification_t parse_format_specifications(const char* format,
                                                          s21_size_t* i,
                                                          va_list args) {
  format_specification_t specs = {0};

  specs.flags = parse_flags(format, i);
  specs.width = parse_width(format, i, args);
  // TODO: read parse_precision TODO
  specs.precision = parse_precision(format, i, args);
  specs.type_modification = parse_type_modification(format, i);
  specs.type = parse_type(format, i);

  return specs;
}

static void print_char(char* dst, s21_size_t* i, char ch,
                      format_specification_t* specs) {
  memset(dst + *i, ' ', specs->width);
  if (specs->flags.left_justify || specs->width == 0) {
    dst[*i] = ch;
  } else {
    dst[*i + specs->width - 1u] = ch;
  }
  *i += specs->width == 0 ? 1 : specs->width;
}

static void print_string(char* dst, s21_size_t* i, const char* string,
                         format_specification_t* specs) {
  memset(dst + *i, ' ', specs->width);
  s21_size_t string_len = strlen(string);
  if (specs->precision.is_precision_set) {
    string_len = string_len > specs->precision.precision ? specs->precision.precision : string_len;
  }
  if (string_len < specs->width) {
    s21_size_t blank_len = specs->width - string_len;
    if (specs->flags.left_justify) {
      strncpy(dst + *i, string, string_len);
    } else {
      strncpy(dst + *i + blank_len, string, string_len);
    }
    *i += specs->width;
  } else {
    strncpy(dst + *i, string, string_len);
    *i += string_len;
  }
}

static void reverse_string(char* buff, s21_size_t begin, s21_size_t end) {
  if (end <= begin) {
    return ;
  }
  --end;

  for (; begin < end; ++begin, --end) {
    char tmp = buff[begin];
    buff[begin] = buff[end];
    buff[end] = tmp;
  }
}

static s21_size_t integer_to_string(long long src, char* dst,
                                    long long base,
                                    char positive_sign_symbol) {
  static const char digits[] = "0123456789ABCDEF";
  s21_size_t len = 0;
  if (src < 0) {
    positive_sign_symbol = '-';
  } 

  do {
    dst[len++] = digits[src % base];
    src /= base;
  } while (src != 0);
  if (positive_sign_symbol != 0) {
    dst[len++] = positive_sign_symbol;
  }

  reverse_string(dst, 0, len);
  return len;
}

// TODO: Add # flag
static s21_size_t unsigned_to_string(long unsigned src, char* dst,
                                     long unsigned base, 
                                     bool prefix, type_t type) {
  static const char digits[] = "0123456789ABCDEF";
  s21_size_t len = 0;
  do {
    dst[len++] = digits[src % base];
    src /= base;
  } while (src != 0);

  if (prefix && (type == TYPE_x || type == TYPE_X)) {
    dst[len++] = '0'; 
    dst[len++] = 'X'; 
  } else if (prefix && (type == TYPE_o)) {
     
  }

  reverse_string(dst, 0, len);
  return len;
}

static void print_number(char* dst, s21_size_t* i, char* number,
                         s21_size_t number_len,
                         format_specification_t* specs) {
  s21_size_t number_len_with_precision = number_len;
  if (specs->precision.is_precision_set) {
    s21_size_t additional = isdigit(number[0]) ? 0 : 1;
    if (number_len < specs->precision.precision + additional) {
      number_len_with_precision = specs->precision.precision + additional;
    }
  }

  s21_size_t i_temp = *i;
  if (specs->width > number_len_with_precision) {
    char fill_char = ' ';
    if (specs->flags.zero_padding && !specs->flags.left_justify 
        && !specs->precision.is_precision_set) {
      fill_char = '0';
      if (!isdigit(number[0])) {
        dst[i_temp++] = *(number++);
        --number_len;
      }
    }
    memset(dst + i_temp, fill_char, specs->width);
  }

  if (!specs->flags.left_justify) {
    s21_size_t begin_offset = 0;
    if (specs->width > number_len_with_precision) {
      begin_offset = specs->width - number_len_with_precision;
    }
    i_temp += begin_offset;
  }

  if (!isdigit(number[0])) {
    dst[i_temp++] = *(number++);
    --number_len;
  }
  s21_size_t precision_lacking = 0;
  if (specs->precision.is_precision_set && 
      specs->precision.precision > number_len) {
    precision_lacking = specs->precision.precision - number_len;
    memset(dst + i_temp, '0', precision_lacking);
    i_temp += precision_lacking;
  }
  strncpy(dst + i_temp, number, number_len);

  *i += (specs->width > number_len_with_precision ? 
      specs->width : number_len_with_precision);
}

static void print_integer(char* dst, s21_size_t* i, long value,
                         format_specification_t* specs) {
  char number[100] = {0};
  s21_size_t number_len = 0;

  if (specs->flags.always_sign) {
    number_len = integer_to_string(value, number, 10, '+');
  } else if (specs->flags.blank_positive) {
    number_len = integer_to_string(value, number, 10, ' ');
  } else {
    number_len = integer_to_string(value, number, 10, 0);
  }

  print_number(dst, i, number, number_len, specs);
}

static void print_unsigned(char* dst, s21_size_t* i, long unsigned value,
                         format_specification_t* specs) {
  char number[100] = {0};
  s21_size_t number_len = 0;

  long unsigned base = 10;
  if (specs->type == TYPE_o) {
    base = 8;
  } else if (specs->type == TYPE_x || specs->type == TYPE_X) {
    base = 16;
  }
  // TODO: add # flag
  number_len = unsigned_to_string(value, number, base, false, specs->type);
  // TODO: if specs->type == TYPE-x: to_lower(number)
  // currently to_lower is not implemented
  print_number(dst, i, number, number_len, specs);
}

#include <stdio.h>

void print_format_specification(const format_specification_t *spec) {
    printf("Flags:\n");
    printf("  Left Justify: %s\n", spec->flags.left_justify ? "true" : "false");
    printf("  Always Sign: %s\n", spec->flags.always_sign ? "true" : "false");
    printf("  Blank Positive: %s\n", spec->flags.blank_positive ? "true" : "false");
    printf("  Use Prefix: %s\n", spec->flags.use_prefix ? "true" : "false");
    printf("  Zero Padding: %s\n", spec->flags.zero_padding ? "true" : "false");

    printf("Width: %u\n", spec->width);
    printf("Precision is set: %s\n", spec->precision.is_precision_set ? "true" : "false");
    printf("Precision: %u\n", spec->precision.precision);

    printf("Type Modification: ");
    switch (spec->type_modification) {
        case TYPE_MODIFICATION_DEFAULT: printf("DEFAULT\n"); break;
        case TYPE_MODIFICATION_h: printf("h\n"); break;
        case TYPE_MODIFICATION_l: printf("l\n"); break;
        case TYPE_MODIFICATION_L: printf("L\n"); break;
    }

    printf("Type: ");
    switch (spec->type) {
        case TYPE_DEFAULT: printf("DEFAULT\n"); break;
        case TYPE_c: printf("c\n"); break;
        case TYPE_d: printf("d\n"); break;
        case TYPE_i: printf("i\n"); break;
        case TYPE_f: printf("f\n"); break;
        case TYPE_s: printf("s\n"); break;
        case TYPE_u: printf("u\n"); break;
        case TYPE_PERCENT: printf("%%\n"); break;
        case TYPE_g: printf("g\n"); break;
        case TYPE_G: printf("G\n"); break;
        case TYPE_e: printf("e\n"); break;
        case TYPE_E: printf("E\n"); break;
        case TYPE_x: printf("x\n"); break;
        case TYPE_X: printf("X\n"); break;
        case TYPE_o: printf("o\n"); break;
        case TYPE_p: printf("p\n"); break;
    }
}

s21_size_t s21_sprintf(char* dst, const char* format, ...) {
  va_list args;
  va_start(args, format);

  s21_size_t len = 0;

  for (s21_size_t i = 0; format[i]; ) {
    if (format[i] != '%') {
      dst[len++] = format[i++];
    } else {
      ++i;
      format_specification_t specs =
          parse_format_specifications(format, &i, args);
      // print_format_specification(&specs);
      if (specs.type == TYPE_c) {
        print_char(dst, &len, va_arg(args, int), &specs);
      } else if (specs.type == TYPE_s) {
        print_string(dst, &len, va_arg(args, char*), &specs);
      } else if (specs.type == TYPE_d || specs.type == TYPE_i) {
        if (specs.type_modification == TYPE_MODIFICATION_DEFAULT 
            || specs.type_modification == TYPE_MODIFICATION_h) {
          print_integer(dst, &len, va_arg(args, int), &specs);
        } else {
          print_integer(dst, &len, va_arg(args, long), &specs);
        }
      } else if (specs.type == TYPE_X || specs.type == TYPE_o
            || specs.type == TYPE_x || specs.type == TYPE_u) {
        if (specs.type_modification == TYPE_MODIFICATION_DEFAULT 
            || specs.type_modification == TYPE_MODIFICATION_h) {
          print_unsigned(dst, &len, va_arg(args, int), &specs);
        } else {
          print_unsigned(dst, &len, va_arg(args, long), &specs);
        }
      }
    }
  }

  dst[len] = '\0';
  va_end(args);
  return len;
}
