IDIR=../include
ODIR=obj


CC=gcc
CFLAGS = -Wall -g -Og -std=gnu99 -Ofast   -I$(IDIR)

DEPS=$(IDIR)/$(wildcard *.h)
SOURCES=$(wildcard *.c)
MY_LIBS=$(wildcard $(LDIR)/*.c)
SOURCES_OBJ=$(patsubst %.c,$(ODIR)/%.o,$(SOURCES))
MY_LIBS_OBJ=$(foreach o, $(patsubst %.c,%.o,$(MY_LIBS)), $(ODIR)/$o)


print-% : ; @echo $* = $($*)

$(ODIR)/%.o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

notebook: $(SOURCES_OBJ) $(MY_LIBS_OBJ)
	$(CC) $(CFLAGS) $(wildcard $(ODIR)/*.o)  $(wildcard $(OLDIR)/*.o) -o notebook $(LIBS)

clean:
	rm obj/*.o
	rm notebook
