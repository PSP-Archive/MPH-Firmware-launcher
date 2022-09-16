TARGET = mphfwl
SRCS = $(wildcard *.c 2D/*.c Utils/*.c)
OBJS=$(SRCS:.c=.o)

INCDIR =
CFLAGS = -O1 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS = -lpng -lz -lm -lpspgu -lpsprtc -lpspumd -lpspusb -lpspusbstor

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = MPH Firmware launcher 1.4.0
PSP_EBOOT_ICON = PBP/ICON0.PNG
PSP_EBOOT_PIC1 = PBP/PIC1.PNG

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
