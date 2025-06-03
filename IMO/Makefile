PYTHON_VERSION ?= python3

UNAME_S := $(shell uname -s 2>/dev/null)

ifeq ($(OS),Windows_NT)
    PYTHON_EXE := $(PYTHON_VERSION)
    PYTHON_DIR := $(shell $(PYTHON_EXE) -c "import sys; print(sys.exec_prefix)")
    PYTHON_INCLUDE := $(shell $(PYTHON_EXE) -c "from sysconfig import get_paths; print(get_paths()['include'])")
    PYTHON_LIBDIR := $(shell $(PYTHON_EXE) -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))")
    PYTHON_LIB := $(shell $(PYTHON_EXE) -c "import sysconfig; print(sysconfig.get_config_var('LDLIBRARY') or '')" | sed "s/.lib//;s/.a//;s/^lib//")
    PYTHON_CFLAGS := -I$(PYTHON_INCLUDE)
    PYTHON_LDFLAGS := -L$(PYTHON_LIBDIR) -l$(PYTHON_LIB)
else ifeq ($(UNAME_S),Linux)
    PYTHON_CFLAGS  := $(shell $(PYTHON_VERSION)-config --includes)
    PYTHON_LDFLAGS := $(shell $(PYTHON_VERSION)-config --embed --ldflags 2>/dev/null || $(PYTHON_VERSION)-config --ldflags)
else ifeq ($(UNAME_S),Darwin)  # macOS
    PYTHON_CFLAGS  := $(shell $(PYTHON_VERSION)-config --includes)
    PYTHON_LDFLAGS := $(shell $(PYTHON_VERSION)-config --embed --ldflags 2>/dev/null || $(PYTHON_VERSION)-config --ldflags)
else
    $(error Unsupported OS)
endif

CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude -DWITHOUT_NUMPY $(PYTHON_CFLAGS) -O3
LDFLAGS  = $(PYTHON_LDFLAGS)

SRC_DIR = src
BUILD_DIR = build

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

tsp-study: $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) *.o tsp-study
