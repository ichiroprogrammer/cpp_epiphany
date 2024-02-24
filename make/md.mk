#import TARGET
MDS:=$(addprefix md/,$(MDS))
MD_SC:=md/sample_code.md

MDS_DEPS:=$(addprefix md/o/,$(notdir $(MDS)))
MDS_DEPS:=$(MDS_DEPS:.md=.d)

MDS_C:=$(addprefix md/o/,$(notdir $(MDS)))
MDS_C_ALL:=$(MDS_C) $(addprefix md/o/,$(notdir $(MD_SC)))
MDS_DB:=md/o/db.json

MD_IND:=o/index.md
MDS_FINAL:=$(MD_IND) $(addprefix o/,$(notdir $(MDS_C_ALL)))
HTMLS:=$(MDS_FINAL:.md=.html)

TARGET_HTML:=$(TARGET:.md=.html)
TARGET_PDF:=$(TARGET:.md=.pdf)

TARGET_PU:=$(TARGET:.md=_pu.md)
TARGET_PU_HTML:=$(TARGET_PU:.md=.html)


.PHONY: md html pdf all
md: $(TARGET) $(MDS_FINAL)
html: $(TARGET_HTML) $(HTMLS)
pdf: html $(TARGET_PDF)
all: md html pdf

.PHONY: pu_md pu_html
pu_md: $(TARGET_PU)
pu_html: $(TARGET_PU_HTML)

CLEANS:=$(patsubst clean%,clean, $(MAKECMDGOALS))
ifneq ($(CLEANS), clean)
include $(MDS_DEPS)
endif

VERSION_TXT=version.txt
VERSION=$(shell cat $(VERSION_TXT))
AUTHOR=$(shell git config --local user.name)

MD_GEN=../md_gen/export/py

$(TARGET): $(MDS_FINAL)
	$(MD_GEN)/md_join.py -o $@ $^

$(TARGET_PDF) : $(TARGET_HTML)
	$(MD_GEN)/../sh/to_pdf.sh $<

$(TARGET_PU) : $(TARGET)
	$(MD_GEN)/md_enable_pu.py -o $@ $^

$(MD_IND) : $(MDS_DB)
	$(MD_GEN)/md_make_index.py $(MD_SEC_NUM) -o $@ $< $(INDEX_OPT)

$(MD_SC) : $(MAKEFILE) $(MDS)
	$(MD_GEN)/md_sample_section.py -o $@ $(MDS)

md/o/%.md : md/%.md
	$(MD_GEN)/md_compile.py -o $@ $< --mds $(MDS)

md/o/%.md: $(AUTOSAR)%.md
	$(MD_GEN)/md_compile.py -o $@ $< --mds $(MDS)

md/o/%.md: $(SHARED)%.md
	$(MD_GEN)/md_compile.py -o $@ $< --mds $(MDS)

md/o/%.md: $(ORG)%.md
	$(MD_GEN)/md_compile.py -o $@ $< --mds $(MDS)

md/o/sample_code.md : $(MD_SC)
	$(MD_GEN)/md_compile.py -o $@ $< --mds $(MDS) $<

md/o/%.d : md/%.md
	$(MD_GEN)/md_compile.py -D $(@:.d=.md) -o $@ $<

md/o/%.d: $(AUTOSAR)%.md
	$(MD_GEN)/md_compile.py -D $(@:.d=.md) -o $@ $< --mds $(MDS)

md/o/%.d: $(SHARED)%.md
	$(MD_GEN)/md_compile.py -D $(@:.d=.md) -o $@ $<

md/o/%.d: $(ORG)%.md
	$(MD_GEN)/md_compile.py -D $(@:.d=.md) -o $@ $<

$(MDS_DB) : $(MDS_C_ALL)
	$(MD_GEN)/md_make_db.py $@ --mds $^

o/%.md : md/o/%.md $(MDS_DB)
	$(MD_GEN)/md_link.py $(MD_SEC_NUM) -o $@ --db $(MDS_DB) $<

o/%.html: o/%.md $(VERSION_TXT)
	[ -n "$(AUTHOR)" ]
	$(MD_GEN)/md_to_html.py --author "$(AUTHOR)" --title "$(TITLE)" -o $@ $<

%.png: %.pu
	$(MD_GEN)/plant_uml_encode.py $< -o $@
	$(MD_GEN)/png_checker.py $@ || (rm $@; exit 1)

.PHONY: clean help
clean:
	rm -f $(MDS_DEPS)
	rm -f $(MD_SC) $(MD_IND) $(MDS_DB)
	rm -f $(MDS_C_ALL)
	rm -f $(MDS_FINAL)
	rm -f $(HTMLS)
	rm -f $(TARGET) $(TARGET_HTML) $(TARGET_PDF)

help:
	@echo "make                     :generate $(TARGET)"
	@echo "make md MD_SEC_NUM=      :generate $(TARGET) without section number"
	@echo "make html                :generate $(TARGET_HTML)"
	@echo "make pdf                 :generate $(TARGET_PDF)"
	@echo "make pu_md               :generate $(TARGET_PU)"
	@echo "make pu_html             :generate $(TARGET_PU_HTML)"
	@echo "make clean               :generated files are deleted"
	@echo "make help                :show this message"

