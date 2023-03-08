CC    = gcc
FLAGS        = -std=gnu11
CFLAGS       = -fPIC -g -pedantic -Wall -Wextra -ggdb
LDFLAGS      = -shared -ldl -lpthread

DEBUGFLAGS   = -O0 -DDEBUG
RELEASEFLAGS = -O2

MODE ?= DEBUG

TARGET  = libdebug.so
SOURCES = $(wildcard src/*.c)
SOURCES += $(wildcard external_libs/ring_buffer/*.c)
INC = -Iinc -Iexternal_libs
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(MODE), RELESE)
	# @echo "RELESE"
	FLAGS += $(RELEASEFLAGS)
else
	# @echo "DEBUG"
	FLAGS += $(DEBUGFLAGS)
endif

all: $(TARGET)

%.o : %.c
	$(CC) $(FLAGS) $(CFLAGS) $(INC) -c -o  $@ $^

clean:
	rm -rf $(OBJECTS) $(TARGET)

test: $(TARGET)
	rm -rf test/test_server.o test/test_server
	$(CC) $(FLAGS) $(CFLAGS) $(INC) -c -o test/test_server.o test/test_server.c -rdynamic
	$(CC) $(FLAGS) -L$(TARGET) -L./ -ldebug -o test/test_server test/test_server.o -rdynamic

$(TARGET):$(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@
include $(SRCS:.c=.d)