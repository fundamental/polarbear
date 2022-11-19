bin_PROGRAMS = polarbear_alsa polarbear_jack polarbear

polarbear_common_SOURCES = \
AmplitudeResponse.cxx \
AmplitudeResponse.hxx \
Apply.cxx \
CoefArray.hxx \
Complex.cxx \
Complex.hxx \
ComplexArray.hxx \
ComplexExt.hxx \
ComplexExtArray.cxx \
ComplexExtArray.hxx \
Defines.hxx \
EMode.hxx \
Filter.hxx \
PhaseResponse.cxx \
PhaseResponse.hxx \
Polar.cxx \
Polar.hxx \
PoleZeroEditor.cxx \
PoleZeroEditorGUI.cxx \
PoleZeroEditorGUI.hxx \
PoleZeroList.cxx \
PoleZeroList.hxx \
PoleZeroPlane.cxx \
PoleZeroPlane.hxx \
Selection.hxx \
ToolTipButton.cxx \
ToolTipButton.hxx \
VERSION.hxx

polarbear_SOURCES = $(polarbear_common_SOURCES) main.cxx alsaio.cxx jackio.cxx

polarbear_alsa_SOURCES = $(polarbear_common_SOURCES) main_alsa.cxx alsaio.cxx

polarbear_jack_SOURCES = $(polarbear_common_SOURCES) main_jack.cxx jackio.cxx

polarbear_LDADD = @LIBS@ @ALSA_LIBS@ @JACK_LIBS@

polarbear_alsa_LDADD = @LIBS@ @ALSA_LIBS@

polarbear_jack_LDADD = @LIBS@ @ALSA_LIBS@ @JACK_LIBS@

install-exec-hook:
	chown root $(DESTDIR)$(bindir)/polarbear
	chmod u+s $(DESTDIR)$(bindir)/polarbear
	chown root $(DESTDIR)$(bindir)/polarbear_alsa
	chmod u+s $(DESTDIR)$(bindir)/polarbear_alsa
	chown root $(DESTDIR)$(bindir)/polarbear_jack
	chmod u+s $(DESTDIR)$(bindir)/polarbear_jack

DISTCLEANFILES = Makefile.in

EXTRA_DIST = \
arrow.xbm \
movemag.xbm \
play.xbm \
pole.xbm \
zero.xbm \
moveang.xbm \
movexy.xbm \
polarbear.xpm \
stop.xbm \
PoleZeroEditorGUI-1_0.cxx \
PoleZeroEditorGUI-1_1.cxx \
PoleZeroEditorGUI.fl

