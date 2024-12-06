# USAGE:
#	lib:		make 
#	all tests:	make -s test
#	one test:	make -s test TEST_FILTER=test_name
#	some tests:	make -s test TEST_FILTER="test1 test2"
#	report:		make gcov_report

CC = gcc
AR = ar

ARFLAGS = rcs
CFLAGS = -Wall -Wextra -Werror -std=c11

CSRS := s21_sprintf.c 
TARGET_LIB := s21_string.a 

TEST_FILTER ?= $(CSRS:%.c=%)
COBJ := $(CSRS:.c=.o)
DEPS := $(COBJ:.o=.d)

NODEPS := clean
.PHONY: all test clean gcov_report add_coverage

all: $(TARGET_LIB)

$(TARGET_LIB): $(COBJ)
	$(AR) $(ARFLAGS) $@ $^

test: $(TARGET_LIB) 
	@$(MAKE) -C tests \
		CHKF="$(TEST_FILTER:%=%_test.check)" \
		CC=$(CC) \
		LDFLAGS="$(LDFLAGS) -L../" \
		LDLIBS="-l:$(TARGET_LIB)" \
		run 

gcov_report: clean add_coverage test
	gcovr -o gcov_report.html --html --html-self-contained

add_coverage:
	$(eval CFLAGS += --coverage)
	$(eval LDFLAGS += --coverage)

$(DEPS): %.d: %.c
	$(CC) -E $(CPPFLAGS) $< -MM -MT $(@:.d=.o) > $@

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif

clean:
	$(RM) $(COBJ) $(DEPS) $(TARGET_LIB) 
	$(RM) *.gc* gcov_report.html
	@$(MAKE) -C ./tests clean
