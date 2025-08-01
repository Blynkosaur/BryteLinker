

CC_FLAG = -I./include
OTHER_FLAGS = -v -g -Wall

LINK_TARGET = build/main

SRC_FILES = main.c debug.c chunk.c value.c

TARGET_OBJS = $(SRC_FILES:%.c=build/%.o)

vpath %.c src src/bytecode

vpath %.h include include/bytecode

build:
	mkdir -p build
all: $(LINK_TARGET)
	echo All done


$(LINK_TARGET): $(TARGET_OBJS)
	gcc -o $@ $^ $(CC_FLAG) $(OTHER_FLAGS)

build/%.o: %.c
	gcc -o $@ -c $<
# Automatic variables are set by make after a rule is matched. There include:
# $@: the target filename.
# $*: the target filename without the file extension.
# $<: the first prerequisite filename.
# $^: the filenames of all the prerequisites, separated by spaces, discard duplicates.
# $+: similar to $^, but includes duplicates.
# $?: the names of all prerequisites that are newer than the target, separated by spaces.
clean:
	rm -rf build/*
	echo cleaning done

main.c: common.h chunk.h debug.h
debug.c: debug.h
chunk.c: chunk.h memory.h
value.c: value.h memory.h
