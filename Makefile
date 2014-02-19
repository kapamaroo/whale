DEBUG = 1

MAJOR = 0
MINOR = 1
NAME = whale
VERSION = $(MAJOR).$(MINOR)

TARGET_LIB_LINKER_NAME = lib${NAME}.so
TARGET_LIB_SONAME = lib$(NAME).so.$(MAJOR)
TARGET_LIB_REAL_NAME = lib$(NAME).so.$(VERSION)

INSTALL_DIR=/opt/Work/FEniCS
INCLUDE_DIR=./include

CC = gcc
CXX = g++
CFLAGS = -Wall -g
CXXFLAGS = -I$(INCLUDE_DIR) -fPIC -Wall -MP -MMD -I/usr/include/lam

ifdef DEBUG
CXXFLAGS += -g -UNDEBUG
else
CXXFLAGS += -O3 -march=native -DNDEBUG
endif

LDFLAGS =-shared -Wl,-soname,$(TARGET_LIB_SONAME)
TEST_FLAGS = -Wall -g
RM = rm -f
TEST_FILE = test.c

DEPS = whale.h
SRCS = whale.cpp
OBJS = $(SRCS:.cpp=.o)

-include $(SRCS:.cpp=.d)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: lib$(NAME)
lib$(NAME): $(TARGET_LIB_LINKER_NAME)

run-test:
	LD_LIBRARY_PATH=. ./test

test: lib$(NAME).so
	$(CC) -I$(INCLUDE_DIR) $(TEST_FLAGS) $(TEST_FILE) -o $@ -L. -l$(NAME)

$(TARGET_LIB_REAL_NAME): $(OBJS)
	$(CXX) ${LDFLAGS} $^ -o $@

.PHONY: $(TARGET_LIB_SONAME)
$(TARGET_LIB_SONAME): $(TARGET_LIB_REAL_NAME)
	ln -sf $(TARGET_LIB_REAL_NAME) $(TARGET_LIB_SONAME)

.PHONY: $(TARGET_LIB_LINKER_NAME)
$(TARGET_LIB_LINKER_NAME): $(TARGET_LIB_SONAME)
#	ldconfig -v -n .
	ln -sf $(TARGET_LIB_SONAME) $(TARGET_LIB_LINKER_NAME)

.PHONY: all
all: lib$(NAME) test

install:
# install headers
	mkdir -p $(INSTALL_DIR)/include/$(NAME)
	cp -r $(INCLUDE_DIR)/*.h $(INSTALL_DIR)/include/$(NAME)

# install library in lib/
	cp -d $(TARGET_LIB_REAL_NAME) $(INSTALL_DIR)/lib
	cp -d $(TARGET_LIB_SONAME) $(INSTALL_DIR)/lib
	cp -d $(TARGET_LIB_LINKER_NAME) $(INSTALL_DIR)/lib

uninstall:
	rm -rf $(INSTALL_DIR)/include/$(NAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_REAL_NAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_SONAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_LINKER_NAME)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB_LINKER_NAME} ${TARGET_LIB_SONAME} ${TARGET_LIB_REAL_NAME} ${OBJS} $(SRCS:.cpp=.d) test *~ core

archive:
	git archive --format=zip master -o lib$(NAME)-$(VERSION).zip
