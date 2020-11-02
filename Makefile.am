AM_CPPFLAGS = $(WX_CXXFLAGS) \
		-I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/sdk/wxscintilla/include

pluginlibdir = $(pkglibdir)/plugins

pluginlib_LTLIBRARIES = libMiniDoc.la

libMiniDoc_la_LDFLAGS = @MODULE_SHARED_LDFLAGS@ -version-info 0:1:0 -no-undefined -avoid-version

libMiniDoc_la_LIBADD = ../../../sdk/libcodeblocks.la $(WX_LIBS)

libMiniDoc_la_SOURCES = MiniDoc.cpp \
		MiniDocConfigPanel.cpp \
		MiniDocPanel.cpp \
		MiniStyledTextCtrl.cpp \
		MiniStyledTextCtrlEvent.cpp

noinst_HEADERS = MiniDoc.h \
		MiniDocConfigPanel.h \
		MiniDocPanel.h \
		MiniStyledTextCtrl.h \
		MiniStyledTextCtrlEvent.h

resources_forZIP = manifest.xml \
		MiniDocConfigPanel.xrc

EXTRA_DIST = $(srcdir)/*.cbp \
		$(srcdir)/wxsmith/MiniDocConfigPanel.wxs \
		$(resources_forZIP)

pkgdata_DATA = MiniDoc.zip

CLEANFILES = $(pkgdata_DATA)

MiniDoc.zip: $(resources_forZIP)
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/MiniDoc.zip $(resources_forZIP) > /dev/null
