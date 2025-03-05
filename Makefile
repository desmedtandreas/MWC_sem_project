CC := gcc-14
SRC_DIR := src
BUILD_DIR := build

EXE := $(BUILD_DIR)/program.out

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

OPENMP := -fopenmp
CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall $(OPENMP)
LDLIBS   := $(OPENMP)

.PHONY: all clean

all: $(EXE)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJ) | $(BUILD_DIR)
	$(CC) $^ $(LDLIBS) -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	@$(RM) -rv $(BUILD_DIR)

-include $(OBJ:.o=.d)