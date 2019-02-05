html_asciidoc_sources = doc/index.adoc\
			doc/installing-on-linux.adoc\
			doc/modules/installing-topdax.adoc

EXTRA_DIST += $(html_asciidoc_sources)

if HAVE_ASCIIDOCTOR
html-local: doc/index.html
maintainer-clean-local: maintainer-clean-local-asciidoc
install-html-local: install-html-asciidoc
uninstall-local: uninstall-html-asciidoc

.PHONY: maintainer-clean-local-asciidoc install-html-asciidoc uninstall-html-asciidoc
doc/index.html: doc/attributes.adoc $(html_asciidoc_sources)
	$(AM_V_GEN)$(ASCIIDOCTOR) -r asciidoctor-diagram doc/index.adoc

maintainer-clean-local-asciidoc:
	$(RM) doc/index.html

install-html-asciidoc: doc/index.html
	@echo " $(MKDIR_P) '$(DESTDIR)$(htmldir)'"; \
		$(MKDIR_P) "$(DESTDIR)$(htmldir)" || exit 1;
	@echo " $(INSTALL_DATA) doc/index.html '$(DESTDIR)$(htmldir)'"; \
		$(INSTALL_DATA) doc/index.html "$(DESTDIR)$(htmldir)" || exit $$?;

uninstall-html-asciidoc:
	@echo " ( cd '$(DESTDIR)$(htmldir)' && rm -f "index.html")"; \
	cd "$(DESTDIR)$(htmldir)" && rm -f index.html
endif
