# Generic GNU Makefile with Native Language Support
# written by Henrik Carlqvist
# Possible targets are:
# make                    normal compile
# make all                normal compile, the same as above
# make install            installs compiled files
# make uninstall          removes installed files
# make clean              removes compiled files
# make mrproper           removes compiled files and dependencies
# make debug              compiles with flag -g for debuggers
# make messages           creates po/messages.po as a skeleton for NLS

# Name of program, compiled executable file
PACKAGE = ms-sys

# Add anything extra you might need when comipiling below
# Example: EXTRA_CFLAGS = -D MY_DEFINE=1
EXTRA_CFLAGS =

# Add anything extra you might need when linking below
# Example: EXTRA_LDFLAGS = -lm
EXTRA_LDFLAGS =

# Paths

# Installation path
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
LOCALEDIR = $(PREFIX)/share/locale

# Where your .c-files live
SRC = src
# Where your .h-files live
INC = inc
# Where .d-files will be created for dependencies
DEP = dep
# Where any .po-files live for native language support, eg sv_SE.po or de_DE.po
# A skeleton .po-file could be created by "make messages"
PO  = po
# Where .mo-files will be created
MO  = mo
# Where .o-files will be created
OBJ = obj
# Where your program will be created before installation
BIN = bin

# There is no need to change anything below this line
#***********************************************************************

# Used for Native Language Support, do not change!
MESSDIR = LC_MESSAGES

INCDIRS = $(INC)

CC = gcc
INCLUDES = $(INCDIRS:%=-I %)
CFLAGS = -O2 -ansi -pedantic -Wall -c $(INCLUDES) \
         -D PACKAGE=\"$(PACKAGE)\" -D LOCALEDIR=\"$(LOCALEDIR)\"
ifeq ($(MAKECMDGOALS),debug)
CFLAGS = -g -ansi -pedantic -Wall -c $(INCLUDES) \
         -D PACKAGE=\"$(PACKAGE)\" -D LOCALEDIR=\"$(LOCALEDIR)\" \
          $(EXTRA_CFLAGS)
endif
LDFLAGS = $(EXTRA_LDFLAGS)


SRC_FILES = $(wildcard $(SRC)/*.c)
INC_FILES = $(wildcard $(INC)/*.h)

MESSAGES = $(PO)/messages.po

PO_FILES = $(filter-out $(MESSAGES),$(wildcard $(PO)/*.po))
MO_FILES = $(PO_FILES:$(PO)/%.po=$(MO)/%.mo)
LANGUAGES = $(PO_FILES:$(PO)/%.po=%)
NLS_FILES = $(LANGUAGES:%=$(LOCALEDIR)/%/$(MESSDIR)/$(PACKAGE).mo)

FILES = $(SRC_FILES:$(SRC)/%.c=%)

OBJS = $(FILES:%=$(OBJ)/%.o)
DEPS = $(FILES:%=$(DEP)/%.d)


all debug: $(BIN)/$(PACKAGE) $(MO_FILES)

install: $(BINDIR)/$(PACKAGE) $(NLS_FILES)

uninstall:
	$(RM) $(BINDIR)/$(PACKAGE) $(NLS_FILES)

messages: $(MESSAGES)

mrproper: clean
	$(RM) $(DEP)/*.d
	$(RM) $(OS_CHECK)

clean:
	$(RM) $(MESSAGES)
	$(RM) $(MO)/*.mo
	$(RM) $(OBJ)/*.o
	$(RM) $(LIB)/*.a
	$(RM) $(filter-out $(BIN)/CVS,$(wildcard $(BIN)/*))

$(BINDIR)/%: $(BIN)/%
	install -m 755 $^ $@

$(LOCALEDIR)/%/$(MESSDIR)/$(PACKAGE).mo: $(MO)/%.mo
	mkdir -p $(LOCALEDIR)/$*/$(MESSDIR)
	install -m 644 $^ $@

$(BIN)/%: $(OBJS) 
	$(CC) -o $@ $^ $(LDFLAGS)

$(MESSAGES): $(SRC_FILES) $(INC_FILES)
	xgettext -k_ -o$@ $^

$(OBJS): $(OBJ)/%.o: $(SRC)/%.c $(DEP)/%.d
	$(CC) $(CFLAGS) -o $@ $<

$(MO_FILES): $(MO)/%.mo: $(PO)/%.po
	msgfmt -o $@ $<

$(DEPS): $(DEP)/%.d: $(SRC)/%.c
ifeq ($(MAKECMDGOALS),quiet)
	@if((printf "$@ $(OBJ)/";$(CC) -MM $(CFLAGS)  $< )> $@); then true; \
	else rm $@; false; fi
else
	if((printf "$@ $(OBJ)/";$(CC) -MM $(CFLAGS)  $< )> $@); then true; \
	else rm $@; false; fi
endif

ifneq ($(MAKECMDGOALS),mrproper)
ifneq ($(wildcard $(DEPS)),)
include $(wildcard $(DEPS))
endif
endif

# Used to force some rules to always be compiled
FORCE: ;


