%define name jinja2schema
%define version 0.1.4
%define unmangled_version 0.1.4
%define unmangled_version 0.1.4
%define release 1
%define _relstr 0contrail0
Summary: Type inference for Jinja2 templates.
Name: python-%{name}
Version: %{version}
Release: %{release}.%{_relstr}
Source0: https://files.pythonhosted.org/packages/60/6f/8db433c9b644654f77a5a85a251298b278b6fefe668c1d856ad34c61c811/jinja2schema-0.1.4.tar.gz
License: BSD
Group: Development/Libraries
Prefix: %{_prefix}
Vendor: Anton Romanovich <anthony.romanovich@gmail.com>
Url: https://jinja2schema.readthedocs.io

%description
jinja2schema: A library that provides a heuristic type inference algorithm for Jinja2 templates.
============

License
-------

`BSD license`_

.. _Jinja2: http://jinja.pocoo.org/docs/
.. _Demo: http://jinja2schema.aromanovich.ru/
.. _demo page: http://jinja2schema.aromanovich.ru/
.. _Documentation: https://jinja2schema.readthedocs.io/
.. _GitHub: https://github.com/aromanovich/jinja2schema
.. _PyPI: https://pypi.python.org/pypi/jinja2schema
.. _BSD license: https://github.com/aromanovich/jinja2schema/blob/master/LICENSE

Copyright (c) 2014 by Anton Romanovich.

Some rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the following
  disclaimer in the documentation and/or other materials provided
  with the distribution.

* The names of the contributors may not be used to endorse or
  promote products derived from this software without specific
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

%prep
%setup -n jinja2schema-0.1.4

%build
python setup.py build

%install
python setup.py install --single-version-externally-managed -O1 --root=$RPM_BUILD_ROOT --record=INSTALLED_FILES

%clean
rm -rf $RPM_BUILD_ROOT

%files -f INSTALLED_FILES
%defattr(-,root,root)

