# careful with wildcard
CHKF ?= $(wildcard *.check)
CSRS = $(CHKF:.check=.c)
COBJ = $(CSRS:.c=.o)
EXES = $(CSRS:.c=.x)

override LDLIBS += $(shell pkg-config --cflags --libs check)

.PHONY: clean tests run

run: $(EXES)
	@for exe in $(EXES); do \
		./$$exe; \
	done

$(EXES): %.x: %.o FORCE
	$(CC) $(LDFLAGS) $< $(LDLIBS) -o $@
FORCE:

$(CSRS): %.c: %.check
	checkmk clean_mode=1 $< > $@	

clean:
	$(RM) $(COBJ) $(EXES) $(CSRS)
	$(RM) *.gc*
