
OUT_EXTENSION = 
OUT_DIR = release
OUT = spine
LIB_EXTENSION = .dylib

OBJ_DIR = objects
SRC_DIR = .

INCLUDES = -I common
LIBRARIES =

OBJS = \
	$(OBJ_DIR)/src/medula.o \
	$(OBJ_DIR)/src/modulemanager.o \

MODULE_OBJS = \
	$(OBJ_DIR)/src/module.o \

OS_OBJS = \
	$(OBJ_DIR)/src/os.o \

DO_CC = g++ $(INCLUDES) -o $@ -c $<

OUT_FINAL = $(OUT_DIR)/$(OUT)$(OUT_EXTENSION)
MODULE_FINAL = $(OUT_DIR)/module$(LIB_EXTENSION)
OS_FINAL = $(OUT_DIR)/os$(LIB_EXTENSION)

all: $(OBJ_DIR) $(OBJS) $(MODULE_OBJ) $(OUT_DIR) $(OUT_FINAL) $(MODULE_FINAL) $(OS_FINAL)

clean:
	-rm -rf $(OBJ_DIR)
	-rm -f $(OUT_FINAL)

$(OUT_DIR):
	mkdir $(OUT_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)
	mkdir $(OBJ_DIR)/src

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(DO_CC)

$(OUT_FINAL): $(OBJS)
	gcc -lstdc++ -o $@ $(DEBUG) $(OBJS) $(LIBRARIES)

$(MODULE_FINAL): $(MODULE_OBJS)
	gcc -dynamiclib -undefined suppress -flat_namespace -lstdc++ -o $@ $(DEBUG) $(MODULE_OBJS) $(LIBRARIES)

$(OS_FINAL): $(OS_OBJS)
	gcc -dynamiclib -undefined suppress -flat_namespace -lstdc++ -o $@ $(DEBUG) $(OS_OBJS) $(LIBRARIES)


