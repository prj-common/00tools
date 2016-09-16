TARGET=issue_auto_parse

CC=gcc
CPP=g++
AS=as
STRIP=strip
CFLAGS=-g -I./libs/lib_char_code/lib_base64
OUT=out

ifeq ($(TARGET), issue_auto_parse)
    include issue_auto_parse.mk
else ifeq ($(TARGET), test1)
    $(info test1)
else ifeq ($(TARGET), test2)
    $(info test2)
else
    $(info null)
endif