@echo off
bjam toolset=msvc-10.0 variant=debug,release threading=multi link=static, multi link=shared runtime-link=static, shared define=_BIND_TO_CURRENT_VCLIBS_VERSION