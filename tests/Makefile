CC          = clang
LD          = clang

EXEC        = test.out


TARGET      = $(shell uname -m)
TARGET_DEF  = __$(shell uname -m | tr [:lower:] [:upper:])__


DIR_SRC     = ./src
DIR_INC     = ./inc
DIR_PB_INC  = ../$(DIR_INC)
DIR_PB_LIB  = ../$(DIR_TARGET)/lib
DIR_MINUNIT = ./minunit
DIR_OUT     = ./out
DIR_TARGET  = $(DIR_OUT)/$(TARGET)
DIR_OBJ     = $(DIR_TARGET)/obj
DIR_DEP     = $(DIR_TARGET)/dep


$(shell mkdir -p $(DIR_SRC))


CFLAGS  += -W -Wall -Wextra -Wno-unused-function -fmessage-length=0 -D_REENTRANT -I $(DIR_INC) $(shell pkg-config --cflags json-c)
CFLAGS  += -I $(DIR_PB_INC)
LDFLAGS += -lcurl $(shell pkg-config --libs json-c) $(DIR_PB_LIB)/libpushbullet.a


SRC      = $(shell find $(DIR_SRC) -name '*.c' | sort)
OBJ      = $(foreach var,$(notdir $(SRC:.c=.o)),$(DIR_OBJ)/$(var))
OBJ_LIB  = $(filter-out $(DIR_OBJ)/main.o, $(OBJ))
DEP      = $(shell find $(DIR_DEP) -name '*.d')


# Which optimisation?
OPTIM   ?= DEBUG
ifeq ($(OPTIM),SIZE)
	CFLAGS   += -Os
else ifeq ($(OPTIM),SPEED)
	CFLAGS   += -Ofast
else ifeq ($(OPTIM),DEBUG)
	CFLAGS   += -g3 -O0 -D__DEBUG__
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


all: test


test: $(EXEC)

$(EXEC): $(OBJ)
	@ echo "\t\033[1;35m[LD]\t[$(OPTIM)]\t$@\033[0m"
	$(VERBOSE) $(LD) $^ -o $@ $(LDFLAGS)


# Include of the dependencies generated in %.o
-include $(DEP)


# Create every objects files in the same directory of the sources
# Create the dependency files in dep/%i
# .SECONDARY prevents make to delete intermediary files (here object files)
.SECONDARY: $(OBJ)
$(DIR_OBJ)/%.o: %.c
	@ mkdir -p $(DIR_OBJ) $(DIR_DEP)
	$(VERBOSE) $(CC) $< $(CFLAGS) -M -MT $@ -MF $(DIR_DEP)/$(notdir $(<:.c=.d))
	@ echo "\t\033[1;32m[CC]\t[$(OPTIM)]\t$<\033[0m"
	$(VERBOSE) $(CC) -c -fPIC -o $@ $< $(CFLAGS)


# clean : clean all objects files
clean:
	$(VERBOSE) [ ! -d "$(DIR_OBJ)" ] || find $(DIR_OBJ) -type f -name '*.o' -delete


# distclean : clean all objects files and the executable
d: distclean
distclean: clean
	$(VERBOSE) [ ! -d "$(DIR_DEP)" ] || find $(DIR_DEP) -type f -name '*.d' -delete
	$(VERBOSE) rm -rf $(DIR_OBJ) $(DIR_DEP) $(DIR_OUT)
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
