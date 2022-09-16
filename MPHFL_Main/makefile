TARGET = out
OBJS = $(wildcard *.c 2D/*.c Utils/*.c)

INCDIR =
CFLAGS = -O1 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS = -lpng -lz -lm -lpspgu -lpsprtc -lpspumd -lpspusb -lpspusbstor

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
