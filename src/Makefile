# Makefile

# 编译器
CC = gcc.exe
CPPC = g++.exe
WINDRES = windres.exe

# 编译选项
CFLAGS = -fdiagnostics-color=always -I"./"

# 源文件和目标文件
SRC = src/main.c
OBJ = \
build/build/main.o\
build/build/utils.o\
build/build/program_info.o\
build/build/version.o

# 目标可执行文件
TARGET = build/dest/CBJQ_XiaoKaiGuan.exe

# 目标规则
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJ)
	$(CC) -fdiagnostics-color=always $(OBJ) -o $(TARGET)

# 编译源文件生成目标文件
# build/%.o: src/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@
build/build/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

build/build/utils.o: src/utils.c
	$(CC) $(CFLAGS) -c $< -o $@

build/build/program_info.o: src/program_info.c
	$(CC) $(CFLAGS) -c $< -o $@
	
build/build/version.o: src/version.rc
	$(WINDRES) --preprocessor-arg="-DUNICODE" $< $@
#	$(WINDRES) $< $@

define clean_file
	del /Q /F "$(subst /,\\,$(1))"
endef

# 清理生成的文件
clean:
	$(foreach obj_i, $(OBJ), $(call clean_file,$(obj_i)))


define mkdir_withcheck
	@ echo "Creating directory $(subst /,\\,$(strip $1))"
	@ if not exist "$(subst /,\\,$(strip $1))" mkdir "$(subst /,\\,$(strip $1))" && echo "Directory $(subst /,\\,$(strip $1)) created"
endef

prepare_dir:
	$(call mkdir_withcheck,"build\build")
	$(call mkdir_withcheck,"build\dest")

.PHONY: all clean prepare_dir
