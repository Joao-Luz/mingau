LINK :=
CFLAGS := -g
INCLUDE_PATHS := -Icompiler
CXX := gcc
TEST := tests/tokenizer.c
TARGET := $(notdir $(basename $(TEST)))

all: $(TARGET)

SRC = compiler/tokenizer.c

HEADERS = compiler/tokenizer.h

OBJECTS := $(SRC:%.c=build/%.o)
TEST_OBJECT := $(TEST:%.c=build/%.o)

OBJECTS += $(TEST_OBJECT)

build/compiler/%.o: compiler/%.c $(HEADERS)
	@mkdir -p build/compiler/
	$(CXX) $(INCLUDE_PATHS) $(CFLAGS) -c -o $@ $<

$(TEST_OBJECT): $(TEST)
	@mkdir -p build/tests/
	$(CXX) $(INCLUDE_PATHS) $(CFLAGS) -c -o $(TEST_OBJECT) $(TEST) 

$(TARGET): $(OBJECTS)
	$(CXX) $(INCLUDE_PATHS) $(OBJECTS) $(LINK) -o $@

clean:
	-rm -f -r build
	-rm -f *.o
	-rm -f $(TARGET)

test: $(TARGET)
	@./$(TARGET)