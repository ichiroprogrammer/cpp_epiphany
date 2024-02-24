MAKEFILE:=$(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))

TARGET:=o/cpp_guideline_shared.md
TITLE=C++ Programming Guide Shared($(VERSION))

MD_SEC_NUM:=
INDEX_OPT:=--exclude sample_code.md:2 exercise_a.md:2

SHARED:=../shared/md/

MDS:=solid.md design_pattern.md template_meta_programming.md \
	 dynamic_memory_allocation.md term_explanation.md

include $(SHARED)../make/md.mk
