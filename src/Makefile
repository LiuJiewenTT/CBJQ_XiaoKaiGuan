# Makefile

# 编译器
CC = gcc.exe
CPPC = g++.exe
WINDRES = windres.exe
MAKE = make

# 目录设置
SRC_DIR = src/
BUILD_DIR = build/build/
DEST_DIR = build/dest/


# 编译选项
CFLAGS = -fdiagnostics-color=always -I"./"

# 源文件和目标文件
SRC = $(SRC_DIR)main.c
OBJ = \
$(BUILD_DIR)main.o\
$(BUILD_DIR)version.o\
$(BUILD_DIR)ordinary_winutils.o\
$(BUILD_DIR)UAC_utils.o\
$(BUILD_DIR)program_info.o

# 目标可执行文件
TARGET = $(DEST_DIR)CBJQ_XiaoKaiGuan.exe

# 目标规则
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJ)
	@ echo Compiling $@
	$(CC) -fdiagnostics-color=always $(OBJ) -o $(TARGET)

# 编译源文件生成目标文件
# build/%.o: src/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)program_info.o: $(SRC_DIR)program_info.c flag_every_time
	@ echo Compiling $@
	$(CC) $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR)version.o: $(SRC_DIR)version.rc
	@ echo Compiling $@
	$(WINDRES) --preprocessor-arg="-DUNICODE" $< $@
#	$(WINDRES) $< $@

$(BUILD_DIR)main.o: $(SRC_DIR)main.c
	@ echo Compiling $@
	$(CC) $(CFLAGS) -c $< -o $@

# $(BUILD_DIR)utils.o: $(SRC_DIR)utils/utils.c $(SRC_DIR)utils/utils.h \
# 	$(SRC_DIR)utils/ordinaries.o \
# 	$(SRC_DIR)utils/my_winutils.o
# 	@ echo "Compiling $@"
# 	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)ordinaries.o: $(SRC_DIR)utils/ordinaries.c $(SRC_DIR)utils/ordinaries.h 
	@ echo Compiling $@
	$(CC) $(CFLAGS) -c $< -o $@

# $(BUILD_DIR)my_winutils.o: $(SRC_DIR)my_winutils.c
# 	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)ordinary_winutils.o: $(SRC_DIR)utils/ordinary_winutils.c $(SRC_DIR)utils/ordinary_winutils.h \
	$(SRC_DIR)utils/ordinaries.h $(SRC_DIR)utils/UAC_utils.h
	@ echo Compiling $@
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)UAC_utils.o: $(SRC_DIR)utils/UAC_utils.c $(SRC_DIR)utils/UAC_utils.h \
	$(SRC_DIR)utils/ordinaries.h $(SRC_DIR)utils/ordinary_winutils.h
	@ echo Compiling $@
	$(CC) $(CFLAGS) -c $< -o $@


define clean_file
	del /Q /F "$(subst /,\\,$(1))"
endef

# 清理生成的文件
clean:
	$(foreach obj_i, $(OBJ), $(call clean_file,$(obj_i)))


define mkdir_withcheck
	@ echo "Checking directory $(subst /,\\,$(strip $1))"
	@ if not exist "$(subst /,\\,$(strip $1))" mkdir "$(subst /,\\,$(strip $1))" && echo "Directory $(subst /,\\,$(strip $1)) created"
endef

prepare_dir:
	$(call mkdir_withcheck,"$(BUILD_DIR)")
	$(call mkdir_withcheck,"$(DEST_DIR)")

flag_every_time:
	@ REM "flag_every_time" # @ echo "flag_every_time"	# This will be executed every time
	

.PHONY: all clean prepare_dir flag_every_time
