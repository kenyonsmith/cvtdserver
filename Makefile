OBJDIR := obj
BINDIR := bin
CC     := gcc
CFLAGS := 

# Object file groups
MOBJ := $(OBJDIR)/main.o
TPOBJ := $(OBJDIR)/mongoose/mongoose.o

# Depend groups
TPDEP := mongoose/mongoose.h

# Primary target
all: $(MOBJ) $(TPOBJ)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/cvtdserver $(MOBJ) $(TPOBJ)

# Target to build third party libraries
$(TPOBJ): mongoose/mongoose.c $(TPDEP)
	@mkdir -p $(OBJDIR)/mongoose
	$(CC) -c -o $@ $< $(CFLAGS)

# Target to build main file (which doesn't have an include file)
$(MOBJ): main.c $(TPDEP)
	@mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Phony target
clean:
	rm -rf obj
	rm -rf bin

