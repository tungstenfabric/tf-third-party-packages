%define _sharedir /usr/share/grok

Summary: A powerful pattern matching system for parsing and processing text
Name: grok
Version: 1.20110708.1
Release: 1%{?dist}.srce
Group: System Environment/Utilities
License: BSD
Source0: https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/semicomplete/%{name}-%{version}.tar.gz
%if 0%{?rhel} >= 8
Patch0: grok_matchconf_macro_h.el8.patch
Patch1: grok_makefile.el8.patch
%endif
  
URL: https://www.semicomplete.com/projects/grok/
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Requires: libevent
Requires: pcre >= 7.6
Requires: tokyocabinet >= 1.4.9
BuildRequires: libevent-devel gperf tokyocabinet-devel pcre-devel
BuildRequires: libtirpc-devel
%if 0%{?rhel} >= 8
BuildRequires: rpcgen
%endif

# no longer needed -- modern jls-grok gem uses libffi
Obsoletes: grok-ruby

%description
A powerful pattern matching system for parsing and processing text data such
as log files.

%package devel
Group: Development Tools
Summary: Grok development headers

%description devel
Headers required for grok development.

%prep
%setup -q
%if 0%{?rhel} >= 8
%patch0 -p0
%patch1 -p0
%endif

%build
%if 0%{?rhel} >= 8
LDFLAGS="-ltirpc" C_INCLUDE_PATH=/usr/include/tirpc make
%else
make
%endif

%install
%{__rm} -rf %{buildroot}
%{__mkdir_p} %{buildroot}%{_bindir}
%{__mkdir_p} %{buildroot}%{_libdir}
%{__mkdir_p} %{buildroot}%{_includedir}
%{__mkdir_p} %{buildroot}%{_sharedir}/patterns
install -c grok %{buildroot}/%{_bindir}
install -c libgrok.so* %{buildroot}/%{_libdir}
install -c patterns/base %{buildroot}%{_sharedir}/patterns/base
for header in grok.h grok_pattern.h grok_capture.h grok_capture_xdr.h grok_match.h grok_logging.h grok_discover.h grok_version.h; do
 install -c $header %{buildroot}/%{_includedir}
done

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root)
%{_bindir}/grok
%{_libdir}/libgrok.so*
%dir %{_sharedir}
%dir %{_sharedir}/patterns
%{_sharedir}/patterns/base

%files devel
%{_includedir}

%post
/sbin/ldconfig

%changelog
* Fri Jan 24 2014 Jakov Sosic <jsosic@gmail.com> - 1.20110818.1-1
- bump to latest version

* Mon Oct 19 2009 Pete Fritchman <petef@databits.net> 20090928-1
- Initial packaging.
