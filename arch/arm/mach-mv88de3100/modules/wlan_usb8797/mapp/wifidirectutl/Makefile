# File : wifidirectutl/Makefile
#
# Copyright (C) 2008-2011, Marvell International Ltd. All Rights Reserved
 
# Path to the top directory of the wlan distribution
PATH_TO_TOP = ../..

# Determine how we should copy things to the install directory
ABSPATH := $(filter /%, $(INSTALLDIR))
RELPATH := $(filter-out /%, $(INSTALLDIR))
INSTALLPATH := $(ABSPATH)
ifeq ($(strip $(INSTALLPATH)),)
INSTALLPATH := $(PATH_TO_TOP)/$(RELPATH)
endif

# Override CFLAGS for application sources, remove __ kernel namespace defines
CFLAGS := $(filter-out -D__%, $(EXTRA_CFLAGS))


CFLAGS += -Wall
LIBS = -lrt

.PHONY: default tags all

OBJECTS = wifidirectutl.o
HEADERS = wifidirectutl.h

TARGET = wifidirectutl

build default: $(TARGET)
	@cp -f $(TARGET) $(INSTALLPATH)
	@cp -rf config $(INSTALLPATH)


all : tags default

$(TARGET): $(OBJECTS) $(HEADERS)
	$(ECHO)$(CC) $(LIBS) -o $@ $(OBJECTS)

%.o: %.c $(HEADERS)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $<

tags:
	ctags -R -f tags.txt

distclean clean:
	$(ECHO)$(RM) $(OBJECTS) $(TARGET)
	$(ECHO)$(RM) tags.txt 

