GCC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -c
LIB = -lSDL2 -lSDL2_ttf -lm

SOURCEDIR = Code
BUILDDIR = Build

EXE = Raycast.exe

SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

all: dir $(BUILDDIR)/$(EXE)

rebuild: clean all
	./$(BUILDDIR)/$(EXE)
	
dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(EXE): $(OBJECTS)
	$(GCC) $^ -o $@ $(LIB)
	
$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	$(GCC) $(CFLAGS) $< -o $@

run:
	./$(BUILDDIR)/$(EXE)

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/$(EXE)
	rm -f *~ $(SOURCEDIR)/*~
