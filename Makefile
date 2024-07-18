# 指定编译器
CC=clang
# 指定编译选项
CFLAGS = -g -Wall
# 指定可执行文件的名字
TARGET = clox
# 指定源文件的目录
SRC_DIR = .
# 指定所有的源文件（以.c结尾）
SRCS = $(wildcard $(SRC_DIR)/*.c)
LNKS = -lm
# 指定默认的目标（all）
all: $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LNKS)
# 指定如何清理编译生成的文件
test: all
	./$(TARGET) test.lox
debug: all
	lldb ./clox
clean:
	rm -f $(TARGET)