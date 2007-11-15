#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
AC_CONFIG_SRCDIR([test/testgen.c])
AC_CONFIG_AUX_DIR([bin])
AC_CONFIG_HEADER([src/h5chk_config.h])

dnl AM_INIT_AUTOMAKE takes a list of options that should be applied to
dnl every Makefile.am when automake is run.
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

dnl ----------------------------------------------------------------------
dnl Is the GNU zlib present? It has a header file `zlib.h' and a library
dnl `-lz' and their locations might be specified with the `--with-zlib'
dnl command-line switch. The value is an include path and/or a library path.
dnl If the library path is specified then it must be preceded by a comma.
dnl
AC_ARG_WITH([zlib],
            [AC_HELP_STRING([--with-zlib=DIR],
                            [Use zlib library [default=yes]])],,
            withval=yes)

case $withval in
  yes)
    HAVE_ZLIB="yes"
    AC_CHECK_HEADERS([zlib.h], [HAVE_ZLIB_H="yes"])
    AC_CHECK_LIB([z], [compress2],, [unset HAVE_ZLIB])
    ;;
  no)
    HAVE_ZLIB="no"
    AC_MSG_CHECKING([for GNU zlib])
    AC_MSG_RESULT([suppressed])
    ;;
  *)
    HAVE_ZLIB="yes"
    case "$withval" in
      *,*)
        zlib_inc="`echo $withval |cut -f1 -d,`"
        zlib_lib="`echo $withval |cut -f2 -d, -s`"
        ;;
      *)
        if test -n "$withval"; then
          zlib_inc="$withval/include"
          zlib_lib="$withval/lib"
        fi
        ;;
    esac

    dnl Trying to include -I/usr/include and -L/usr/lib is redundant and
    dnl can mess some compilers up.
    if test "X$zlib_inc" = "X/usr/include"; then
      zlib_inc=""
    fi
    if test "X$zlib_lib" = "X/usr/lib"; then
      zlib_lib=""
    fi

    saved_CPPFLAGS="$CPPFLAGS"
    saved_LDFLAGS="$LDFLAGS"

    if test -n "$zlib_inc"; then
      CPPFLAGS="$CPPFLAGS -I$zlib_inc"
    fi

    AC_CHECK_HEADERS([zlib.h],
                     [HAVE_ZLIB_H="yes"],
                     [CPPFLAGS="$saved_CPPFLAGS"])

    if test -n "$zlib_lib"; then
      LDFLAGS="$LDFLAGS -L$zlib_lib"
    fi

    AC_CHECK_LIB([z], [compress2],,
                 [LDFLAGS="$saved_LDFLAGS"; unset HAVE_ZLIB])
    ;;
esac

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset pow strdup strerror strrchr strstr strtol])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
                 tool/Makefile])
AC_OUTPUT
