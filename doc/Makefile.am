html_asciidoc_sources = doc/index.adoc\
			doc/installing-on-linux.adoc\
			doc/modules/installing-topdax.adoc

EXTRA_DIST += $(html_asciidoc_sources)

if HAVE_ASCIIDOCTOR
html-local: html-local-asciidoc
maintainer-clean-local: maintainer-clean-local-asciidoc
install-html-local: install-html-asciidoc

.PHONY: html-local-asciidoc maintainer-clean-local-asciidoc install-html-asciidoc
html-local-asciidoc: doc/attributes.adoc $(html_asciidoc_sources)
	$(AM_V_GEN)$(ASCIIDOCTOR) -r asciidoctor-diagram doc/index.adoc

maintainer-clean-local-asciidoc:
	$(RM) doc/index.html

install-html-asciidoc:
	@echo Target not implemented
endif
