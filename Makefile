MAJOR = 0
MINOR = 1
NAME = whale
VERSION = $(MAJOR).$(MINOR)

TARGET_LIB_LINKER_NAME = lib${NAME}.so
TARGET_LIB_SONAME = lib$(NAME).so.$(MAJOR)
TARGET_LIB_REAL_NAME = lib$(NAME).so.$(VERSION)

CC = gcc
CFLAGS=-fPIC -Wall -g -UNDEBUG
#CFLAGS=-fPIC -Wall -O3 -march=native -DNDEBUG
LDFLAGS = -shared -Wl,-soname,$(TARGET_LIB_SONAME)
RM = rm -f

INSTALL_DIR=/opt/Work/FEniCS
HEADER_FILE=whale.h

DEPS = whale.h
SRCS = whale.c
OBJS = $(SRCS:.c=.o)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: lib$(NAME)
lib$(NAME): $(TARGET_LIB_LINKER_NAME)

run-test:
	LD_LIBRARY_PATH=. ./test

test: lib$(NAME).so
	$(CC) test.c -o $@ -L. -l$(NAME)

$(TARGET_LIB_REAL_NAME): $(NAME).o
	$(CC) ${LDFLAGS} $^ -o $@

.PHONY: $(TARGET_LIB_SONAME)
$(TARGET_LIB_SONAME): $(TARGET_LIB_REAL_NAME)
	ln -sf $(TARGET_LIB_REAL_NAME) $(TARGET_LIB_SONAME)

.PHONY: $(TARGET_LIB_LINKER_NAME)
$(TARGET_LIB_LINKER_NAME): $(TARGET_LIB_SONAME)
#	ldconfig -v -n .
	ln -sf $(TARGET_LIB_SONAME) $(TARGET_LIB_LINKER_NAME)

#$(SRCS:.c=.d):%.d:%.c
#	$(CC) $(CFLAGS) -MM $< >$@
#
#include $(SRCS:.c=.d)

.PHONY: all
all: lib$(NAME) test

install:
# install headers
	mkdir -p $(INSTALL_DIR)/include/$(NAME)
	cp $(HEADER_FILE) $(INSTALL_DIR)/include/$(NAME)

# install library in subdirectory $(NAME)
#	mkdir -p $(INSTALL_DIR)/lib/$(NAME)
#	cp -d $(TARGET_LIB_REAL_NAME) $(INSTALL_DIR)/lib/$(NAME)
#	cp -d $(TARGET_LIB_SONAME) $(INSTALL_DIR)/lib/$(NAME)
#	cp -d $(TARGET_LIB_LINKER_NAME) $(INSTALL_DIR)/lib/$(NAME)

# install library in lib/
	cp -d $(TARGET_LIB_REAL_NAME) $(INSTALL_DIR)/lib
	cp -d $(TARGET_LIB_SONAME) $(INSTALL_DIR)/lib
	cp -d $(TARGET_LIB_LINKER_NAME) $(INSTALL_DIR)/lib

uninstall:
	rm -rf $(INSTALL_DIR)/include/$(NAME)

#	rm -rf $(INSTALL_DIR)/lib/$(NAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_REAL_NAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_SONAME)
	rm -f $(INSTALL_DIR)/lib/$(TARGET_LIB_LINKER_NAME)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB_LINKER_NAME} ${TARGET_LIB_SONAME} ${TARGET_LIB_REAL_NAME} ${OBJS} $(SRCS:.c=.d) test *~ core

archive:
	git archive --format=zip master -o whale.zip
