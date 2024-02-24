GTEST_SRCS_BASE:=gtest-all.cc gtest_main.cc
GTEST_SRCS:=$(addprefix $(GTEST_DIR)src/,$(GTEST_SRCS_BASE))
GTEST_OBJS:=$(addprefix $(GTEST_BIN_DIR),$(GTEST_SRCS_BASE:.cc=.o))

$(GTEST_LIB): $(GTEST_OBJS)
	ar cr $@ $^

$(GTEST_BIN_DIR)%.o : $(GTEST_DIR)src/%.cc
	$(CXX) -I$(GTEST_INC) -I $(GTEST_DIR) \
		   -D_GLIBCXX_USE_CXX11_ABI=0 -c -o $@ $<

