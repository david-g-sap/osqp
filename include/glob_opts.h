#ifndef GLOB_OPTS_H
# define GLOB_OPTS_H

# ifdef __cplusplus
extern "C" {
# endif /* ifdef __cplusplus */

/*
   Define OSQP compiler flags
 */

// cmake generated compiler flags
#include "osqp_configure.h"

/* DATA CUSTOMIZATIONS (depending on memory manager)-----------------------   */

// We do not need memory allocation functions if EMBEDDED is enabled
# ifndef EMBEDDED

/* define custom printfs and memory allocation (e.g. matlab/python) */
#  ifdef MATLAB
    #   include "mex.h"
static void* c_calloc(size_t num, size_t size) {
  void *m = mxCalloc(num, size);
  mexMakeMemoryPersistent(m);
  return m;
}

static void* c_malloc(size_t size) {
  void *m = mxMalloc(size);
  mexMakeMemoryPersistent(m);
  return m;
}

static void* c_realloc(void *ptr, size_t size) {
  void *m = mxRealloc(ptr, size);
  mexMakeMemoryPersistent(m);
  return m;
}

    #   define c_free mxFree
#  elif defined PYTHON
// Define memory allocation for python. Note that in Python 2 memory manager
// Calloc is not implemented
    #   include <Python.h>
    #   define c_malloc PyMem_Malloc
    #   if PY_MAJOR_VERSION >= 3
    #    define c_calloc PyMem_Calloc
    #   else  /* if PY_MAJOR_VERSION >= 3 */
static void* c_calloc(size_t num, size_t size) {
  void *m = PyMem_Malloc(num * size);
  memset(m, 0, num * size);
  return m;
}
    #   endif /* if PY_MAJOR_VERSION >= 3 */
    #   define c_free PyMem_Free
    #   define c_realloc PyMem_Realloc

# elif !defined OSQP_CUSTOM_MEMORY
/* If no custom memory allocator defined, use
 * standard linux functions. Custom memory allocator definitions
 * appear in the osqp_configure.h generated file. */
    #  include <stdlib.h>
    #  define c_malloc  malloc
    #  define c_calloc  calloc
    #  define c_free    free
    #  define c_realloc realloc
#  endif /* ifdef MATLAB */

# endif // end ifndef EMBEDDED


/* Use customized number representation -----------------------------------   */
# ifdef DLONG            // long integers
typedef long long c_int; /* for indices */
# else // standard integers
typedef int c_int;       /* for indices */
# endif /* ifdef DLONG */


# ifndef DFLOAT         // Doubles
typedef double c_float; /* for numerical values  */
# else                  // Floats
typedef float c_float;  /* for numerical values  */
# endif /* ifndef DFLOAT */


/* Use customized operations */

# ifndef c_absval
#  define c_absval(x) (((x) < 0) ? -(x) : (x))
# endif /* ifndef c_absval */

# ifndef c_max
#  define c_max(a, b) (((a) > (b)) ? (a) : (b))
# endif /* ifndef c_max */

# ifndef c_min
#  define c_min(a, b) (((a) < (b)) ? (a) : (b))
# endif /* ifndef c_min */

// Round x to the nearest multiple of N
# ifndef c_roundmultiple
#  define c_roundmultiple(x, N) ((x) + .5 * (N)-c_fmod((x) + .5 * (N), (N)))
# endif /* ifndef c_roundmultiple */


/* Use customized functions -----------------------------------------------   */

# if EMBEDDED != 1

#  include <math.h>
#  ifndef DFLOAT // Doubles
#   define c_sqrt sqrt
#   define c_fmod fmod
#  else          // Floats
#   define c_sqrt sqrtf
#   define c_fmod fmodf
#  endif /* ifndef DFLOAT */

# endif // end EMBEDDED

/* informational print function */
#  ifdef MATLAB
#   define c_print mexPrintf
#  elif defined PYTHON
#   include <Python.h>
#   define c_print PySys_WriteStdout
#  elif defined R_LANG
#   include <R_ext/Print.h>
#   define c_print Rprintf
#  else  /* ifdef MATLAB */
#   define c_print printf
#  endif /* c_print configuration */

/* error printing function */
#  ifdef R_LANG
#   define c_eprint Rprintf
#  else
#   define c_eprint(...) c_print("ERROR in %s: ", __FUNCTION__); \
            c_print(__VA_ARGS__); c_print("\n");
#  endif /* c_eprint configuration */

# endif  /* PRINTING */


# ifdef __cplusplus
}
# endif /* ifdef __cplusplus */

#endif /* ifndef GLOB_OPTS_H */
