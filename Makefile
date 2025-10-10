
BINDIR = bin
PROGRAM = cavs_tester
FLAGS = -Wall

REL_DIR = $(BINDIR)/release
DEB_DIR = $(BINDIR)/debug

ifeq "$(findstring release, $(MAKECMDGOALS))" ""
	BUILD = $(DEB_DIR)
	FLAGS += -O3 -ggdb -D DEBUG
else
	BUILD = $(REL_DIR)
	FLAG += -O3
endif

debug: all
release: all

all: md $(BUILD)/$(PROGRAM)

$(BUILD)/$(PROGRAM): *.c
					$(CC) -o $@ $^ $(FLAGS)

md:
	mkdir -p $(BUILD)
	cp config $(BUILD)

clean:
					rm -rf $(BINDIR)/*
