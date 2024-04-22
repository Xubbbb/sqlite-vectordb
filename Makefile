CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lpthread -ldl  # SQLite需要这些标准库

# 项目的目标文件名
TARGET=app

# 包括所有的源文件
SRCS=main.c sqlite3.c
OBJS=$(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
	rm -rf test.db