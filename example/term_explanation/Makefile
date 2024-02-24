SRCS:=\
    aaa.cpp argument.cpp class_layout_ut.cpp constexpr_ut.cpp constructor_ut.cpp decltype_ut.cpp \
	deep_shallow_copy_ut.cpp func_try_block.cpp interface_class.cpp \
	implicit_conversion_ut.cpp incomplete_type_ut.cpp integral_promotion_ut.cpp \
	generic_lambda_ut.cpp lack_of_cohesion_ut.cpp lambda.cpp pod_ut.cpp nsdmi.cpp \
    name_hiding.cpp name_lookup_adl_ut.cpp name_lookup_ut.cpp namespace_ut.cpp \
    override_overload_ut.cpp range_for_ut.cpp ref_collapsing_ut.cpp rtti_ut.cpp \
    rvalue_lvalue_ut.cpp semantics_ut.cpp slice_ut.cpp spurious_wakeup_ut.cpp \
    two_phase_name_lookup_ut.cpp uniform_initialization_ut.cpp \
    ambiguous_ownership_ut.cpp unique_ptr_ownership_ut.cpp shared_ptr_ownership_ut.cpp \
    universal_ref_ut.cpp user_defined_literal_ut.cpp dominance_ut.cpp diamond_inheritance_ut.cpp \
    virtual_inheritance_ut.cpp underlying_type_ut.cpp most_vexing_parse_ut.cpp \
	simple_declaration_ut.cpp dangling_ut.cpp

SHARED:=../../
include $(SHARED)make/env.mk

ifeq ($(CXX), g++)
# @@@ sample begin 0:0

CCFLAGS_ADD:=-fdump-lang-class
# @@@ sample end
endif

include $(SHARED)make/example.mk
include $(SHARED)make/gtest.mk
