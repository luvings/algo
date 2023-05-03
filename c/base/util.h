
#ifndef __COMMON_H__
#define __COMMON_H__

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* [0, 1) */
#define RANDOM()        (random() / (RAND_MAX + 1.0))

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

/*
 * fault:
 *     1) param type must be consistent.
 *     2) param can't have side effects, e.g. ++, --
 */
#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

/*
 * ..and if you can't take the strict
 * types, you can specify one yourself.
 *
 * Or not use min/max at all, of course.
 */
#define min_t(type, x, y)   \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define max_t(type, x, y)   \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

/* Indirect macros required for expanded argument pasting, eg. __LINE__. */
#define ___PASTE(a, b)   a##b
#define __PASTE(a, b)    ___PASTE(a, b)

/*
 * The typeof keyword is disabled by default when building with -std=c99
 * as it's a GNU extension, make it known that it's an extension by using
 * the underscore-enclosed variant.
 *
 * The underscore-encosed keyword is accepted by GCC without requesting
 * extensions to the C99 standard, while the simpler typeof() keyword
 * requires GNU extensions to the C99 standard to be explicitly requested
 * (e.g.: by using the -fasm option, or -std=gnu11).
 *
 * ICC supports the __typeof__ keyword as well as typeof.
 *
 */

/*
 * min()/max() macros that also do
 * strict type-checking.. See the
 * "unnecessary" pointer comparison.
 */
#define __min(x, y) ({		\
	typeof(x) _x = (x);     \
	typeof(y) _y = (y);     \
	(void) (&_x == &_y);    \
	_x < _y ? _x : _y;      \
})

#define __max(x, y) ({		\
	typeof(x) _x = (x);     \
	typeof(y) _y = (y);     \
	(void) (&_x == &_y);    \
	_x > _y ? _x : _y;      \
})

extern void swap(int *data, int i, int j);

extern void print_array_data(int *data, int n);

extern int create_random_array(int max_size, int max_value, int *array, int *actual_length);

extern int cmp(const void *a, const void *b);

extern bool is_equal(int *a, int *b, int n);

#endif /* __COMMON_H__ */
