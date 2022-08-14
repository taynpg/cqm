#################### The C tqm Makefile #######################
ROOT_DIR = .
EXE_NAME = cqm
DEBUG_TARGET_UTF8 = calendar.o data.o qimenzpzr.o comline.o qimenyp.o qimenbasic.o
RELEASE_TARGET_UTF8 = utf8_calendar.o utf8_data.o utf8_qimenzpzr.o utf8_comline.o utf8_qimenyp.o utf8_qimenbasic.o
RELEASE_TARGET_GBK = gbk_calendar.o gbk_data.o gbk_qimenzpzr.o gbk_comline.o gbk_qimenyp.o utf8_qimenbasic.o
WIN_TARGET_DEBUG = wind-calendar.obj wind-data.obj wind-main.obj wind-qimenzpzr.obj wind-comline.obj wind-qimenyp.obj wind-qimenbasic.obj
WIN_TARGET_RELEASE = winr-calendar.obj winr-data.obj winr-main.obj winr-qimenzpzr.obj winr-comline.obj winr-qimenyp.obj winr-qimenbasic.obj
VSCC = cl
VSLINK = link
CC = gcc
MAKE = make
##################################
CFALGS = -g -Wall -std=c99
RCFALGS = -std=c99
WINCFLAGSD = /Zi /c /EHsc /source-charset:utf-8 /execution-charset:gbk /nologo
WINCFLAGS = /c /EHsc /source-charset:utf-8 /execution-charset:gbk /nologo
vpath %.h include
vpath %.c src
INCLUDE_DIR = $(ROOT_DIR)/include

# 默认编译 UTF8 环境下的 Release 版本
all: $(RELEASE_TARGET_UTF8)
	$(CC) $(RCFALGS) -o $(EXE_NAME) main.c $(RELEASE_TARGET_UTF8) -I$(INCLUDE_DIR)
utf8_calendar.o: calendar.h calendar.c define.h
	$(CC) $(RCFALGS) -o utf8_calendar.o -c src/calendar.c -I$(INCLUDE_DIR)
utf8_qimenzpzr.o: qimenzpzr.h qimenzpzr.c define.h data.c data.h calendar.c calendar.h
	$(CC) $(RCFALGS) -o utf8_qimenzpzr.o -c src/qimenzpzr.c -I$(INCLUDE_DIR)
utf8_data.o: data.h data.c define.h
	$(CC) $(RCFALGS) -o utf8_data.o -c src/data.c -I$(INCLUDE_DIR)
utf8_comline.o: calendar.c calendar.h comline.h comline.c
	$(CC) $(RCFALGS) -o utf8_comline.o -c src/comline.c -I$(INCLUDE_DIR)
utf8_qimenyp.o: qimenyp.h qimenyp.c define.h data.c data.h calendar.c calendar.h
	$(CC) $(RCFALGS) -o utf8_qimenyp.o -c src/qimenyp.c -I$(INCLUDE_DIR)
utf8_qimenbasic.o: qimenbasic.h qimenbasic.c
	$(CC) $(RCFALGS) -o utf8_qimenbasic.o -c src/qimenbasic.c -I$(INCLUDE_DIR)

# 编译 GBK 环境下的 Release 版本
rgbk: $(RELEASE_TARGET_GBK)
	$(CC) $(RCFALGS) -fexec-charset=GBK -o $(EXE_NAME) main.c $(RELEASE_TARGET_GBK) -I$(INCLUDE_DIR)
gbk_calendar.o: calendar.h calendar.c define.h
	$(CC) $(RCFALGS) -fexec-charset=GBK -o gbk_calendar.o -c src/calendar.c -I$(INCLUDE_DIR)
gbk_qimenzpzr.o: qimenzpzr.h qimenzpzr.c define.h data.c data.h calendar.c calendar.h
	$(CC) $(RCFALGS) -fexec-charset=GBK -o gbk_qimenzpzr.o -c src/qimenzpzr.c -I$(INCLUDE_DIR)
gbk_data.o: data.h data.c define.h
	$(CC) $(RCFALGS) -fexec-charset=GBK -o gbk_data.o -c src/data.c -I$(INCLUDE_DIR)
gbk_comline.o: calendar.c calendar.h comline.h comline.c
	$(CC) $(RCFALGS) -fexec-charset=GBK -o gbk_comline.o -c src/comline.c -I$(INCLUDE_DIR)
gbk_qimenyp.o: qimenyp.h qimenyp.c define.h data.c data.h calendar.c calendar.h
	$(CC) $(RCFALGS) -fexec-charset=GBK -o gbk_qimenyp.o -c src/qimenyp.c -I$(INCLUDE_DIR)
gbk_qimenbasic.o: qimenbasic.h qimenbasic.c
	$(CC) $(RCFALGS) -fexec-charset=GBK -o gbk_qimenbasic.o -c src/qimenbasic.c -I$(INCLUDE_DIR)

# UTF8 环境下的 debug 版本
debug: $(DEBUG_TARGET_UTF8)
	$(CC) $(CFALGS) -o $(EXE_NAME) main.c $(DEBUG_TARGET_UTF8) -I$(INCLUDE_DIR)
calendar.o: calendar.h calendar.c define.h
	$(CC) $(CFALGS) -o calendar.o -c src/calendar.c -I$(INCLUDE_DIR)
qimenzpzr.o: qimenzpzr.h qimenzpzr.c define.h data.c data.h calendar.c calendar.h
	$(CC) $(CFALGS) -o qimenzpzr.o -c src/qimenzpzr.c -I$(INCLUDE_DIR)
data.o: data.h data.c define.h
	$(CC) $(CFALGS) -o data.o -c src/data.c -I$(INCLUDE_DIR)
comline.o: calendar.c calendar.h comline.h comline.c
	$(CC) $(CFALGS) -o comline.o -c src/comline.c -I$(INCLUDE_DIR)
qimenyp.o: qimenyp.h qimenyp.c define.h data.c data.h calendar.c calendar.h
	$(CC) $(RCFALGS) -o qimenyp.o -c src/qimenyp.c -I$(INCLUDE_DIR)
qimenbasic.o: qimenbasic.h qimenbasic.c
	$(CC) $(RCFALGS) -o qimenbasic.o -c src/qimenbasic.c -I$(INCLUDE_DIR)

# GBK WIN 环境下的 debug 版本
win-debug : $(WIN_TARGET_DEBUG)
	$(VSLINK) /DEBUG /OUT:$(EXE_NAME).exe /SUBSYSTEM:CONSOLE $(WIN_TARGET_DEBUG)
wind-calendar.obj: calendar.h calendar.c define.h
	$(VSCC) $(WINCFLAGSD) /Fowind-calendar.obj /I$(INCLUDE_DIR) src/calendar.c
wind-data.obj: data.h data.c define.h
	$(VSCC) $(WINCFLAGSD) /Fowind-data.obj /I$(INCLUDE_DIR) src/data.c
wind-qimenzpzr.obj: qimenzpzr.h qimenzpzr.c define.h data.c data.h calendar.c calendar.h
	$(VSCC) $(WINCFLAGSD) /Fowind-qimenzpzr.obj -c src/qimenzpzr.c -I$(INCLUDE_DIR)
wind-main.obj: main.c
	$(VSCC) $(WINCFLAGSD) /Fowind-main.obj /I$(INCLUDE_DIR) main.c
wind-comline.obj: calendar.c calendar.h comline.h comline.c
	$(VSCC) $(WINCFLAGSD) /Fowind-comline.obj -c src/comline.c -I$(INCLUDE_DIR)
wind-qimenyp.obj: qimenyp.h qimenyp.c define.h data.c data.h calendar.c calendar.h
	$(VSCC) $(WINCFLAGSD) /Fowind-qimenyp.obj -c src/qimenyp.c -I$(INCLUDE_DIR)
wind-qimenbasic.obj: qimenbasic.h qimenbasic.c
	$(VSCC) $(WINCFLAGSD) /Fowind-qimenbasic.obj -c src/qimenbasic.c -I$(INCLUDE_DIR)

win-release : $(WIN_TARGET_RELEASE)
	$(VSLINK) /OUT:$(EXE_NAME).exe /SUBSYSTEM:CONSOLE $(WIN_TARGET_RELEASE)
winr-calendar.obj: calendar.h calendar.c define.h
	$(VSCC) $(WINCFLAGS) /Fowinr-calendar.obj /I$(INCLUDE_DIR) src/calendar.c
winr-data.obj: data.h data.c define.h data.c data.h calendar.c calendar.h
	$(VSCC) $(WINCFLAGS) /Fowinr-data.obj /I$(INCLUDE_DIR) src/data.c
winr-qimenzpzr.obj: qimenzpzr.h qimenzpzr.c define.h
	$(VSCC) $(WINCFLAGS) /Fowinr-qimenzpzr.obj -c src/qimenzpzr.c -I$(INCLUDE_DIR)
winr-main.obj: main.c
	$(VSCC) $(WINCFLAGS) /Fowinr-main.obj /I$(INCLUDE_DIR) main.c
winr-comline.obj: calendar.c calendar.h comline.h comline.c
	$(VSCC) $(WINCFLAGSD) /Fowinr-comline.obj -c src/comline.c -I$(INCLUDE_DIR)
winr-qimenyp.obj: qimenyp.h qimenyp.c define.h data.c data.h calendar.c calendar.h
	$(VSCC) $(WINCFLAGSD) /Fowinr-qimenyp.obj -c src/qimenyp.c -I$(INCLUDE_DIR)
winr-qimenbasic.obj: qimenbasic.h qimenbasic.c
	$(VSCC) $(WINCFLAGSD) /Fowinr-qimenbasic.obj -c src/qimenbasic.c -I$(INCLUDE_DIR)

clean:
	- rm -f *.o
	- rm -f $(EXE_NAME)
wclean:
	- del *.pdb
	- del *.ilk
	- del *.obj
	- del $(EXE_NAME).exe
mclean:
	- del *.o
	- del $(EXE_NAME).exe