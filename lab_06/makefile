# Paths
BINPATH=./bin/
DATAPATH=./data/
DOCSPATH=./docs/
INCPATH=./inc/
OUTPATH=./out/
SRCPATH=./src/


# Compilation variables
CC=gcc
CFLAGS=-std=c11 -I$(INCPATH)
LFLAGS=--coverage

# Files ( MODULES must not have "main" ):
MODULES := handling utils io_interface BinSearchTree AVLTree analyze hash

OBJS := $(addprefix ${OUTPATH}, $(addsuffix .o, ${MODULES}))
SRCS := $(addprefix ${SRCPATH}, $(addsuffix .c, ${MODULES}))
DPNDS := $(addprefix ${OUTPATH}, $(addsuffix .d, ${MODULES}))

DEL_EXT := o gcno gcda lastbuildstate

# Special targets: 
.PHONY: clean debug release
.NOTPARALLEL: release debug dirs


# Build mods
release: CFLAGS += -o2 -g0
release: LFLAGS = -o2
release: $(OUTPATH)release.lastbuildstate $(BINPATH)app

debug: CFLAGS += -ggdb --coverage -Wall -pedantic -Wextra -g3 -o0
debug: $(OUTPATH)debug.lastbuildstate $(BINPATH)app
debug: LFLAGS += -o0


$(BINPATH)app: $(OUTPATH)main.o $(OBJS)
	$(CC) $(LFLAGS) $^ -o $@ -lm -lrt

clean:
	rm -f *.exe ${OUTPATH}* ${BINPATH}*

dirs: 
	mkdir -p $(BINPATH)
	mkdir -p $(DATAPATH)
	mkdir -p $(DOCSPATH)
	mkdir -p $(INCPATH)
	mkdir -p $(OUTPATH)
	mkdir -p $(SRCPATH)

$(OUTPATH)%.o : $(SRCPATH)%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(OUTPATH)%.d : $(SRCPATH)%.c
	@if test -d $(OUTPATH); then \
		$(CC) -I$(INCPATH) -MM -MT ${subst .d,.o,$@} $< > $@; \
	else \
		mkdir $(OUTPATH); \
	fi

$(OUTPATH)debug.lastbuildstate $(OUTPATH)release.lastbuildstate:
	rm -f *.exe $(addprefix $(OUTPATH)*, ${DEL_EXT})
	touch $@

include $(DPNDS) $(OUTPATH)main.d