#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
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

AC_CHECK_HEADERS([arpa/inet.h netdb.h stdlib.h string.h sys/socket.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_SELECT_ARGTYPES

AC_CHECK_FUNCS([atexit gethostbyaddr gethostbyname gettimeofday memset select socket])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

