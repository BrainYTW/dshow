#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SRGUI_LIB)
#  define SRGUI_EXPORT Q_DECL_EXPORT
# else
#  define SRGUI_EXPORT Q_DECL_IMPORT
# endif
#else
# define SRGUI_EXPORT
#endif
