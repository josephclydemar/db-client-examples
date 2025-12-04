CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Werror -MMD -D_DEFAULT_SOURCE
INCLUDE = -I include \
					-I thirdparty/libpq/include \
					-I thirdparty/libpq \
					-I thirdparty/dotenv-c/include
LFLAGS = -lpq \
				 -ldotenv \
				 -L thirdparty/libpq \
				 -L thirdparty/dotenv-c/lib \
				 -Wl,-rpath=thirdparty/libpq \
				 -Wl,-rpath=thirdparty/dotenv-c/lib

SOURCE_FILES = $(shell find src -type f -name '*.c')
OBJECT_FILES = $(patsubst src/%.c,build/%.o,$(SOURCE_FILES))
DEPS_FILES = $(OBJECT_FILES:.o=.d)
TARGET_BIN = build/target.out

.PHONY: all run clean

all: debug

run:
	@$(TARGET_BIN)

clean:
	rm -rf $(wildcard build/*)


release: CFLAGS += -DNODEBUG
release: LFLAGS += -O3 -s
release: $(TARGET_BIN)

debug: CFLAGS += -g -fsanitize=address,undefined
debug: LFLAGS += -O0
debug: $(TARGET_BIN)


-include $(DEPS_FILES)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET_BIN): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

