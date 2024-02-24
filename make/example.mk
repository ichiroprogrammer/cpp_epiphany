# This source codes can be compiled g++9.3.0 on ubuntu20.04.

ifeq ($(CXX),clang++)
COVERAGE:=-fprofile-instr-generate -fcoverage-mapping
COVERAGE_ENV:=LLVM_PROFILE_FILE=clang++/coverage.profraw
else
COVERAGE:=-coverage
COVERAGE_ENV:=
endif

# @@@ sample begin 0:0

WARN_ERROR:=-Werror
WARN:=-Wall -Wextra
# @@@ sample end

CCFLAGS:=-std=c++17 $(WARN) -Weffc++ $(WARN_ERROR) $(SUPPRESS_WARN) -g $(SANITIZER) \
	     $(CCFLAGS_ADD) $(COVERAGE) -D_GLIBCXX_USE_CXX11_ABI=0

CCFLAGS_C:=-std=c99 $(WARN) $(WARN_ERROR) $(SUPPRESS_WARN) -g $(SANITIZER) \
	     $(CCFLAGS_ADD) $(COVERAGE) -D_GLIBCXX_USE_CXX11_ABI=0

ifdef SRCS
ifdef SRCS2
all: $(EXE) $(EXE2)
else
all: $(EXE)
endif
else
all: $(EXE2)
endif

NOSUFIX = $(basename $(SRCS))
NOSUFIX2 = $(basename $(SRCS2))

DPDS:=$(addprefix $(D),$(addsuffix .d, $(NOSUFIX) $(NOSUFIX2)))

NOT_NEED_D:=$(patsubst clean%,true, $(MAKECMDGOALS))
ifeq ($(MAKECMDGOALS), help)
NOT_NEED_D=true
endif

ifneq ($(NOT_NEED_D), true)
-include $(DPDS)
endif

LINK_FLAGS:=$(COVERAGE) $(SANITIZER)
ifdef SANITIZER
CCFLAGS+=-DSANITIZER
endif

$(EXE): $(addprefix $(O),$(addsuffix .o, $(NOSUFIX))) $(GTEST_LIB) # ut
	$(CXX) $(LINK_FLAGS) -o $@ $^ -lstdc++fs -lpthread

$(EXE2): $(addprefix $(O),$(addsuffix .o, $(NOSUFIX2))) # ut以外
	$(CXX) $(LINK_FLAGS) -o $@ $^ -lstdc++fs -lpthread

$(O)%.o : %.cpp
	$(CXX) -I$(GTEST_INC) -I$(SHARED)h -I../h $(CCFLAGS) -c -o $@ $<

$(O)%.o : %.c
	$(CC) -I$(SHARED)h -I../h $(CCFLAGS_C) -c -o $@ $<

$(O)%.o : %.s
	$(CXX) -c -o $@ $<

$(D)%.d : %.cpp
	$(CXX) -I$(GTEST_INC) -I$(SHARED)h -I../h -E -MM -w $< | sed -e 's@^\([^ ]\)@$$(O)\1@g'  > $@

$(D)%.d : %.c
	$(CC) -I$(SHARED)h -I../h -E -MM -w $< | sed -e 's@^\([^ ]\)@$$(O)\1@g' > $@

$(EXE_DONE) : $(EXE) # 単体テストが成功すれば$(EXE_DONE)を作る
	$(COVERAGE_ENV) ./$< && touch $@

ifdef SRCS
ut:
	$(MAKE) $(EXE_DONE)
endif

$(O)cov.lcov: $(EXE_DONE)
ifeq ($(CXX),clang++)
	llvm-profdata merge --sparse clang++/coverage.profraw -o clang++/coverage2.profraw
	llvm-cov export --format=lcov --instr-profile clang++/coverage2.profraw $(EXE) > $@
else
	lcov -c -d $(O) -o $@
endif

.PHONY : cov
cov: $(O)cov.lcov
	lcov -r $(O)cov.lcov "*gtest*" -o $(O)cov1.lcov
	lcov -r $(O)cov1.lcov "*c++*" -o $(O)cov2.lcov
	lcov -r $(O)cov2.lcov "*_ut.cpp" -o $(O)cov1.lcov
	lcov -r $(O)cov1.lcov "*programming_convention*.cpp" -o $(O)cov.lcov
	@rm -rf $(O)coverage
	mkdir $(O)coverage
	genhtml $(O)cov.lcov -o $(O)coverage
	@rm -rf $(O)cov.lcov $(O)cov1.lcov $(O)cov2.lcov

CLEAN_DIRS=d/ g++/ clang++/ sanitizer/ scan-build/
.PHONY : clean clean_gtest
clean:
	rm -f $$(find $(CLEAN_DIRS) -type f ! -name ".gitignore")

clean_gtest:
	rm -f $$(find $(addprefix ../gtest/,$(CLEAN_DIRS)) -type f ! -name ".gitignore")

SCAN_BUILD_DIR=scan-build/

%can-build: force_scan-build
	scan-build -o $(SCAN_BUILD_DIR) $(MAKE) O=$(SCAN_BUILD_DIR) \
		2> $(SCAN_BUILD_DIR)scan-build.txt | tee $(SCAN_BUILD_DIR)scan-build_bugs.txt
	grep "scan-build: No bugs found." $(SCAN_BUILD_DIR)scan-build_bugs.txt > /dev/null;
	rm -f $(SCAN_BUILD_DIR)scan-build.txt $(SCAN_BUILD_DIR)scan-build_bugs.txt
force_scan-build: ;

# address, thread, leak incompatible
SANITIZER_NORMAL:=address,leak,undefined,float-divide-by-zero,float-cast-overflow
.PHONY : san
san:
	$(MAKE) SANITIZER=\"-fsanitize=$(SANITIZER_NORMAL)\" O=sanitizer/

%an-ut:
	$(MAKE) san
	$(MAKE) O=sanitizer/ ut
fource_san-ut: ;

.PHONY : clang clang-ut
clang:
	$(MAKE) CXX=clang++

clang-ut:
	$(MAKE) CXX=clang++ ut

clang-cov:
	$(MAKE) CXX=clang++ cov

.PHONY : doxy format complete
doxy:
	doxygen ../doxygen.config

format:
	clang-format -i $$(find . $(SHARED)h ../h -name "*.cpp" -o -name "*.h")

complete : 
	bear -- $(MAKE) --always-make
	sed -i -e "s@$$(pwd)@.@" compile_commands.json # 絶対パスの変換

%elp: force_help
	@echo "make [target] [MACRO=xxx]..."
	@echo "  all          : default target. To build all *.exe."
	@echo "  ut           : execute *_ut."
	@echo "  cov          : generate line coverage of ut."
	@echo "  clang        : generate all *.o by clang."
	@echo "  clang-ut     : execute ut using clang binary."
	@echo "  clang-cov    : generate line coverage of clang ut."
	@echo "  san          : generate all *.o by g++sanitizer."
	@echo "  san-ut       : execute ut using sanitizer binary."
	@echo "  scan-build   : static code analysis by scan-build."
	@echo "  clean        : call xxx_clean."
	@echo "  doxy         : generate doxygen docs."
	@echo "  format       : format *.cpp *.h by clang-formatter."
	@echo "  complete     : make compile_commands.json to complete by clang"
	@echo ""
	@echo "  Supported Macros"
	@echo "    CXX:         compiler(default:g++)"
	@echo "    WARN_ERROR:  compiler flang(default:-Werror)"
force_help: ;
