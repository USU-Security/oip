#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([OIP],[0.06],[rian dot shelley at usu dot edu])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([kdtree.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

dnl Check for SDL
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
	    :,
	    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
	    )

CFLAGS="$CFLAGS $SDL_CFLAGS"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lSDL':
AC_CHECK_LIB([SDL], [main])
# FIXME: Replace `main' with a function in `-lSDL_image':
AC_CHECK_LIB([SDL_image], [main])
# FIXME: Replace `main' with a function in `-lcryptopp':
AC_CHECK_LIB([cryptopp], [main])
# FIXME: Replace `main' with a function in `-lpcap':
AC_CHECK_LIB([pcap], [main])

AC_CHECK_LIB([fontconfig], [FcInit])

#check for freetype
PKG_CHECK_MODULES([FT2], [freetype2])

#ac_oip_LIBS="$LIBS"
#LIBS="-lcryptopp -lpcap"

if test "$ac_cv_lib_SDL_main" == no
then
	AC_MSG_ERROR([the SDL development libraries must be installed])
fi

if test "$ac_cv_lib_SDL_image_main" == no
then
	AC_MSG_WARN([if you want to compile oipgui, you need to install the SDL_image development libraries])
fi

if test "$ac_cv_lib_cryptopp_main" == no
then
	AC_MSG_ERROR([the cryptopp development libraries must be installed])
fi

# Checks for header files.
m4_warn([obsolete],
[The preprocessor macro `STDC_HEADERS' is obsolete.
  Except in unusual embedded environments, you can safely include all
  ISO C90 headers unconditionally.])dnl
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_CHECK_HEADERS([arpa/inet.h netdb.h stdlib.h string.h sys/socket.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
m4_warn([obsolete],
[Update your code to rely only on HAVE_SYS_TIME_H,
then remove this warning and the obsolete code below it.
All current systems provide time.h; it need not be checked for.
Not all systems provide sys/time.h, but those that do, all allow
you to include it and time.h simultaneously.])dnl
AC_CHECK_HEADERS_ONCE([sys/time.h])
# Obsolete code to be removed.
if test $ac_cv_header_sys_time_h = yes; then
  AC_DEFINE([TIME_WITH_SYS_TIME],[1],[Define to 1 if you can safely include both <sys/time.h>
	     and <time.h>.  This macro is obsolete.])
fi
# End of obsolete code.


# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_SELECT_ARGTYPES
m4_warn([obsolete],
[your code may safely assume C89 semantics that RETSIGTYPE is void.
Remove this warning and the `AC_CACHE_CHECK' when you adjust the code.])dnl
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[AC_COMPILE_IFELSE(
[AC_LANG_PROGRAM([#include <sys/types.h>
#include <signal.h>
],
		 [return *(signal (0, 0)) (0) == 1;])],
		   [ac_cv_type_signal=int],
		   [ac_cv_type_signal=void])])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[$ac_cv_type_signal],[Define as the return type of signal handlers
		    (`int' or `void').])

AC_CHECK_FUNCS([atexit gethostbyaddr gethostbyname gettimeofday memset select socket])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

