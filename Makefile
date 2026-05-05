# ─────────────────────────────────────────────────────────
#  DevTools — Game Calculator Suite
#  Makefile  (Linux / macOS)
# ─────────────────────────────────────────────────────────

TARGET   := devtools_calc
SRC_DIR  := src
IMGUI    := libs/imgui
BUILD    := build

CXX      := g++
SDL2_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null || echo "-I/usr/include/SDL2")
SDL2_LIBS   := $(shell sdl2-config --libs   2>/dev/null || echo "-lSDL2")

CXXFLAGS := -std=c++17 -O2 -Wall \
            -I$(SRC_DIR) \
            -I$(IMGUI) \
            -I$(IMGUI)/backends \
            $(SDL2_CFLAGS)

LIBS     := $(SDL2_LIBS) -lGL -ldl -lpthread

IMGUI_SRC := \
    $(IMGUI)/imgui.cpp \
    $(IMGUI)/imgui_draw.cpp \
    $(IMGUI)/imgui_tables.cpp \
    $(IMGUI)/imgui_widgets.cpp \
    $(IMGUI)/backends/imgui_impl_sdl2.cpp \
    $(IMGUI)/backends/imgui_impl_opengl3.cpp

APP_SRC := $(SRC_DIR)/main.cpp

IMGUI_OBJ := $(patsubst %.cpp, $(BUILD)/%.o, $(notdir $(IMGUI_SRC)))
APP_OBJ   := $(BUILD)/main.o

.PHONY: all clean run

all: $(BUILD) $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

# Compile ImGui sources
$(BUILD)/%.o: $(IMGUI)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/%.o: $(IMGUI)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile app
$(BUILD)/main.o: $(APP_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link
$(TARGET): $(IMGUI_OBJ) $(APP_OBJ)
	$(CXX) -o $@ $^ $(LIBS)
	@echo ""
	@echo "  ✓  Built: $(TARGET)"
	@echo "     Run:   ./$(TARGET)"
	@echo ""

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)
