DEBUG = 1

MAJOR = 0
MINOR = 1
NAME = whale
VERSION = $(MAJOR).$(MINOR)

TARGET_LIB_LINKER_NAME = lib${NAME}.so
TARGET_LIB_SONAME = lib$(NAME).so.$(MAJOR)
TARGET_LIB_REAL_NAME = lib$(NAME).so.$(VERSION)

INSTALL_DIR=~/Work/FEniCS
SRC_DIR=src
BUILD_DIR=build
TEST_FILE = test.cpp

CXX = g++
RM = rm -rf

ifdef DEBUG
CXXFLAGS = -g -UNDEBUG
else
CXXFLAGS = -O3 -march=native -DNDEBUG
endif

CXXFLAGS += -DHAS_WHALE -fPIC -Wall -MP -MMD -I./include -I/usr/include/lam -I$(INSTALL_DIR)/include
LDFLAGS =-shared -Wl,-soname,$(TARGET_LIB_SONAME)
TEST_FLAGS = -DHAS_WHALE -Wall -g -I./include -I$(INSTALL_DIR)/include

DEPS = $(wildcard include/*.h)
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o, $(SRCS))

-include $(SRCS:.cpp=.d)

.PHONY: all
all: $(BUILD_DIR) lib lib$(NAME) test

.PHONY: lib$(NAME)
lib$(NAME): lib/$(TARGET_LIB_LINKER_NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

lib:
	mkdir -p lib

run-test:
	LD_LIBRARY_PATH=lib:$(LD_LIBRARY_PATH) $(BUILD_DIR)/test

$(BUILD_DIR)/test.o:
	$(CXX) $(TEST_FLAGS) -c -o $@ $(TEST_FILE)

$(BUILD_DIR)/test: $(BUILD_DIR)/test.o lib$(NAME)
	$(CXX) -o $@ -Wall -Llib $(BUILD_DIR)/test.o -l$(NAME)

.PHONY: test
test: $(BUILD_DIR)/test

lib/$(TARGET_LIB_REAL_NAME): $(OBJS)
	$(CXX) ${LDFLAGS} $^ -o $@

lib/$(TARGET_LIB_SONAME): lib/$(TARGET_LIB_REAL_NAME)
	ln -sf $(TARGET_LIB_REAL_NAME) lib/$(TARGET_LIB_SONAME)

lib/$(TARGET_LIB_LINKER_NAME): lib/$(TARGET_LIB_SONAME)
#	ldconfig -v -n lib/
	ln -sf $(TARGET_LIB_SONAME) lib/$(TARGET_LIB_LINKER_NAME)

install:
# install headers
	mkdir -p $(INSTALL_DIR)/include/$(NAME)
	cp -r ./include/$(NAME)/*.h $(INSTALL_DIR)/include/$(NAME)

# install library in lib/
	cp -d lib/* $(INSTALL_DIR)/lib

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

uninstall:
	rm -rf $(INSTALL_DIR)/include/$(NAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_REAL_NAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_SONAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_LINKER_NAME)

.PHONY: clean
clean:
	-${RM} $(SRC_DIR)/*~ core $(BUILD_DIR) lib

archive:
	git archive --format=zip master -o lib$(NAME)-$(VERSION).zip
