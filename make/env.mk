CXX=g++

ifeq ($(CXX), g++)
CC=gcc
else
CC=clang
endif

O:=$(CXX)/
D:=d/

EXE_NODIR=example.exe
EXE:=$(addprefix $(O),$(EXE_NODIR))
EXE_DONE:=$(addsuffix .done,$(EXE))

EXE2_NODIR=example2.exe
EXE2:=$(addprefix $(O),$(EXE2_NODIR))
EXE2_DONE:=$(addsuffix .done,$(EXE2))

GTEST_DIR:=../../googletest/googletest/
GTEST_DIR_ABSOLUTE=$(shell cd $(GTEST_DIR); pwd)

GTEST_INC:=$(GTEST_DIR)include/
GTEST_BIN_DIR:=../gtest/$(O)
GTEST_LIB:=$(GTEST_BIN_DIR)libgtest.a

