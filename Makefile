CC = gcc
CFLAGS = -g -Wall -Wvla -Werror -Wno-error=unused-variable
LIBS = -lpthread

TARGET=gol

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIBS)

clean:
	$(RM) $(TARGET)
