CC    = gcc
FLAGS        = -std=gnu11
CFLAGS       = -fPIC -g -pedantic -Wall -Wextra -ggdb3 -shared
LDFLAGS      = -shared -ldl -lpthread

DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2

MODE ?= DEBUG

TARGET  = telnet.so
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
	@echo  $(FLAGS)
	$(CC) $(FLAGS) $(CFLAGS) $(INC) -o  $@ $^

clean:
	rm -rf $(OBJECTS) $(TARGET)

$(TARGET):$(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@
include $(SRCS:.c=.d)