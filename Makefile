MAKEFILE:=$(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))

TARGET:=o/cpp_guideline_shared.md
TITLE=C++ Programming Epiphany($(VERSION))

MD_SEC_NUM:=
#MD_SEC_NUM:=--sec_num
INDEX_OPT:=--exclude sample_code.md:2 exercise_a.md:2

MD_GEN:=./md_gen/export/py

SHARED:=md/

MDS:=solid.md design_pattern.md template_meta_programming.md \
	 dynamic_memory_allocation.md term_explanation.md

include $(SHARED)../make/md.mk
