CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Werror -MMD -D_DEFAULT_SOURCE
INCLUDE = -I include \
					-I thirdparty/dotenv-c/include \
					-I thirdparty/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64/include
LFLAGS = -lmysqlclient \
				 -ldotenv \
				 -L thirdparty/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64/lib \
				 -L thirdparty/dotenv-c/lib \
				 -Wl,-rpath=thirdparty/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64/lib \
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

