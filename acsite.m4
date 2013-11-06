dnl -------------------------------------------------------------------------
dnl -------------------------------------------------------------------------
dnl
dnl Copyright by The HDF Group.
dnl All rights reserved.
dnl
dnl This file is part of h5check. The full h5check copyright notice,
dnl including terms governing use, modification, and redistribution, is
dnl contained in the file COPYING, which can be found at the root of the
dnl source code distribution tree.  If you do not have access to this file,
dnl you may request a copy from help@hdfgroup.org.
dnl
dnl -------------------------------------------------------------------------
dnl -------------------------------------------------------------------------
dnl
dnl -------------------------------------------------------------------------
dnl _AC_SYS_LARGEFILE_MACRO_VALUE
dnl
dnl The following macro overrides the autoconf macro of the same name
dnl with this custom definition. This macro performs the same checks as 
dnl autoconf's native _AC_SYS_LARGEFILE_MACRO_VALUE, but will also set
dnl AM_CPPFLAGS with the appropriate -D defines so additional configure 
dnl sizeof checks do not fail.
dnl
# _AC_SYS_LARGEFILE_MACRO_VALUE(C-MACRO, VALUE,
#               CACHE-VAR,
#               DESCRIPTION,
#               PROLOGUE, [FUNCTION-BODY])
# ----------------------------------------------------------
m4_define([_AC_SYS_LARGEFILE_MACRO_VALUE],
[AC_CACHE_CHECK([for $1 value needed for large files], [$3],
[while :; do
  m4_ifval([$6], [AC_LINK_IFELSE], [AC_COMPILE_IFELSE])(
    [AC_LANG_PROGRAM([$5], [$6])],
    [$3=no; break])
  m4_ifval([$6], [AC_LINK_IFELSE], [AC_COMPILE_IFELSE])(
    [AC_LANG_PROGRAM([@%:@define $1 $2
$5], [$6])],
    [$3=$2; break])
  $3=unknown
  break
done])
case $$3 in #(
  no | unknown) ;;
  *) AC_DEFINE_UNQUOTED([$1], [$$3], [$4])
     AM_CPPFLAGS="-D$1=$$3 $AM_CPPFLAGS";;
esac
rm -rf conftest*[]dnl
])# _AC_SYS_LARGEFILE_MACRO_VALUE
