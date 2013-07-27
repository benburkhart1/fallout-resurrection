VERSION = "1.0.0"

AC_SUBST(VERSION)

AC_INIT(src/dat1_file.c)
AM_INIT_AUTOMAKE(FOResurrection,1.0.0)

AC_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE


AC_PROG_CC
AM_PROG_CC_C_O

dnl test C Compiler
AC_LANG_C

AC_C_CONST

AM_PATH_SDL

AC_STRUCT_TM

AC_CHECK_HEADERS([time.h])
AC_CHECK_HEADERS([sys/time.h])

dnl set HAVE_WORKING_FORK if system supports forking
AC_FUNC_FORK

AC_TYPE_PID_T

AC_PROG_MAKE_SET

AC_HEADER_STDC

AC_OUTPUT(Makefile 
          src/Makefile)

