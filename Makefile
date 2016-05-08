CC       = clang
LD       = clang

EXEC      = pushbullet.out


TARGET     = $(shell uname -m)
TARGET_DEF = __$(shell uname -m | tr [:lower:] [:upper:])__


DIR_SRC    = ./src
DIR_INC    = ./inc
DIR_OUT    = ./out
DIR_TARGET = $(DIR_OUT)/$(TARGET)
DIR_LIB    = $(DIR_TARGET)/lib
DIR_OBJ    = $(DIR_TARGET)/obj
DIR_DEP    = $(DIR_TARGET)/dep
DIR_PREPRO = $(DIR_TARGET)/prepro
DIR_LST    = $(DIR_TARGET)/lst
DIR_TESTS  = ./tests


LIB_SHARED = $(DIR_LIB)/lib$(EXEC:.out=.so)
LIB_STATIC = $(DIR_LIB)/lib$(EXEC:.out=.a)


$(shell mkdir -p $(DIR_SRC))


CFLAGS  += -W -Wall -Wextra -Wno-unused-function -fmessage-length=0 -D_REENTRANT -I $(DIR_INC)
LDFLAGS += -lpthread -lcurl


SRC      = $(shell find $(DIR_SRC) -name '*.c' | sort)
OBJ      = $(foreach var,$(notdir $(SRC:.c=.o)),$(DIR_OBJ)/$(var))
OBJ_LIB  = $(filter-out $(DIR_OBJ)/main.o, $(OBJ))
DEP      = $(shell find . -name '*.d')


# Which optimisation?
OPTIM   ?= DEBUG
ifeq ($(OPTIM),SIZE)
	CFLAGS   += -Os
else ifeq ($(OPTIM),SPEED)
	CFLAGS   += -Ofast
else ifeq ($(OPTIM),DEBUG)
	CFLAGS   += -g3 -O1 -D__DEBUG__
else ifeq ($(OPTIM),NONE)
	CFLAGS   +=
endif


# Include librairies directly in the program?
STATIC  ?= 0
ifeq ($(STATIC),1)
	CFLAGS   += -static
endif


# Verbosity
V            ?= 0
ifeq ($(V),1)
	VERBOSE	=
else
	VERBOSE = @
endif


# Look every source files in the directory SRC
vpath %.c $(DIR_SRC)


all: $(EXEC) lib


lib: $(LIB_SHARED) $(LIB_STATIC)


$(EXEC): $(OBJ)
	@ echo "\t\033[1;35m[LD]\t[$(OPTIM)]\t$@\033[0m"
	$(VERBOSE) $(LD) $^ -o $@ $(LDFLAGS)


$(LIB_SHARED): $(OBJ_LIB)
	@ mkdir -p $(DIR_LIB)
	@ echo "\t\033[1;35m[SO]\t[$(OPTIM)]\t$@\033[0m"
	$(VERBOSE) $(LD) -shared -o $@ $^  $(LDFLAGS)


$(LIB_STATIC): $(OBJ_LIB)
	@ mkdir -p $(DIR_LIB)
	@ echo "\t\033[1;35m[AR]\t[$(OPTIM)]\t$@\033[0m"
	$(VERBOSE) ar crs $@ $^


test:
	$(VERBOSE) $(MAKE) -C $(DIR_TESTS)


# Include of the dependencies generated in %.o
-include $(DEP)


# Create every objects files in the same directory of the sources
# Create the dependency files in dep/%i
# .SECONDARY prevents make to delete intermediary files (here object files)
.SECONDARY: $(OBJ)
$(DIR_OBJ)/%.o: %.c
	@ mkdir -p $(DIR_OBJ) $(DIR_DEP)
	$(VERBOSE) $(CC) $< $(CFLAGS) -M -MT $@ -MF $(DIR_DEP)/$(notdir $(<:.c=.d))
ifeq ($(PREPRO),1)
	@ mkdir -p $(DIR_PREPRO)
	@ echo "\t\033[0;33m[PP]\t[$(OPTIM)]\t$(DIR_PREPRO)/$(notdir $(<:.c=.i))\033[0m"
	$(VERBOSE) $(CC) -E $<  $(CFLAGS) -o $(DIR_PREPRO)/$(notdir $(<:.c=.i))
endif
ifeq ($(LISTING),1)
	@ mkdir -p $(DIR_LST)
	@ echo "\t\033[0;36m[LST]\t[$(OPTIM)]\t$(DIR_LST)/$(notdir $(<:.c=.lst))\033[0m"
	$(VERBOSE) $(CC) -S $<  $(CFLAGS) -o $(DIR_LST)/$(notdir $(<:.c=.lst))
endif
	@ echo "\t\033[1;32m[CC]\t[$(OPTIM)]\t$<\033[0m"
	$(VERBOSE) $(CC) -c -fPIC -o $@ $< $(CFLAGS)


# clean : clean all objects files
clean:
	$(VERBOSE) [ ! -d "$(DIR_OBJ)" ] || find $(DIR_OBJ) -type f -name '*.o' -delete
	$(VERBOSE) [ ! -d "$(DIR_PREPRO)" ] || find $(DIR_PREPRO) -type f -name '*.i' -delete
	$(VERBOSE) [ ! -d "$(DIR_LST)" ] || find $(DIR_LST) -type f -name '*.lst' -delete


# distclean : clean all objects files and the executable
d: distclean
distclean: clean
	$(VERBOSE) [ ! -d "$(DIR_DEP)" ] || find $(DIR_DEP) -type f -name '*.d' -delete
	$(VERBOSE) [ ! -d "$(DIR_LIB)" ] || find $(DIR_LIB) -type f -name '*.so' -delete
	$(VERBOSE) [ ! -d "$(DIR_LIB)" ] || find $(DIR_LIB) -type f -name '*.a' -delete
	$(VERBOSE) rm -rf $(DIR_OBJ) $(DIR_DEP) $(DIR_PREPRO) $(DIR_LST) $(DIR_OUT) $(DIR_LIB)
	$(VERBOSE) rm -f $(EXEC)


mrproper: distclean


# Display the help
help:
	@ echo "Usage :"
	@ echo "    $$ make [OPTIONS]"
	@ echo ""
	@ echo "Options available :"
	@ echo "    OPTIM=NONE|DEBUG|SIZE|SPEED   (dft : DEBUG)"
	@ echo "    STATIC=0|1                    (dft : 0)"
	@ echo "    V=0|1                         (dft : 0)"
