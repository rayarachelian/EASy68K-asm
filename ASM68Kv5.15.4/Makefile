# lame Makefile that's just enough for what I need, feel free to
# add CMake or automake/conf to this.
TARGET ?= asy68k
PREFIX = /usr/local

CXX = g++
SRCS = *.CPP

.PHONY: clean

all:    $(TARGET)
	@echo  $(TARGET) has been built

.CPP.o:	
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SRCS) $(LFLAGS) $(LIBS)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

distclean:
	$(RM) $(TARGET)

install:
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin

-include $(DEPS)