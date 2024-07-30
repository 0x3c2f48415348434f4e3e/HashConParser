# set up some varibales
cc:=cc
CFLAGS := -Wall -Wextra -I./include

SRC_DIR := ./test
OBJ_DIR := ./obj

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET := example

$(TARGET):$(OBJ)
	$(cc) $(CFLAGS) $? -o $(@)
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(cc) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
