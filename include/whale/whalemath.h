/*

      Whale mathematics include file. Defines certain basic mathematical
    constants and functions for working with single, double, and quad precision
    floating point numbers as well as complex single and double.

    This file is included by whalesys.h and should not be used directly.

*/

#if !defined(__WHALEMATH_H)
#define __WHALEMATH_H
#include <math.h>

/*

     Defines operations that are different for complex and real numbers;
   note that one cannot mix the use of complex and real in the same
   Whale program. All Whale objects in one program are built around the object
   WhaleScalar which is either always a real or a complex.

*/

#define WhaleExpPassiveScalar(a) WhaleExpScalar()
#if defined(WHALE_USE_REAL_SINGLE)
#define MPIU_REAL   MPI_FLOAT
typedef float WhaleReal;
#define WhaleSqrtReal(a)    sqrt(a)
#define WhaleExpReal(a)     exp(a)
#define WhaleLogReal(a)     log(a)
#define WhaleSinReal(a)     sin(a)
#define WhaleCosReal(a)     cos(a)
#define WhalePowReal(a,b)   pow(a,b)
#elif defined(WHALE_USE_REAL_DOUBLE)
#define MPIU_REAL   MPI_DOUBLE
typedef double WhaleReal;
#define WhaleSqrtReal(a)    sqrt(a)
#define WhaleExpReal(a)     exp(a)
#define WhaleLogReal(a)     log(a)
#define WhaleSinReal(a)     sin(a)
#define WhaleCosReal(a)     cos(a)
#define WhalePowReal(a,b)   pow(a,b)
#elif defined(WHALE_USE_REAL___FLOAT128)
#if defined(__cplusplus)
extern "C" {
#endif
#include <quadmath.h>
#if defined(__cplusplus)
}
#endif
WHALE_EXTERN MPI_Datatype MPIU___FLOAT128 WhaleAttrMPITypeTag(__float128);
#define MPIU_REAL MPIU___FLOAT128
typedef __float128 WhaleReal;
#define WhaleSqrtReal(a)    sqrtq(a)
#define WhaleExpReal(a)     expq(a)
#define WhaleLogReal(a)     logq(a)
#define WhaleSinReal(a)     sinq(a)
#define WhaleCosReal(a)     cosq(a)
#define WhalePowReal(a,b)   powq(a,b)
#endif /* WHALE_USE_REAL_* */

/*
    Complex number definitions
 */
#if defined(WHALE_CLANGUAGE_CXX) && defined(WHALE_HAVE_CXX_COMPLEX)
#if defined(WHALE_USE_COMPLEX) || defined(WHALE_DESIRE_COMPLEX)
#define WHALE_HAVE_COMPLEX 1
/* C++ support of complex number */
#if defined(WHALE_HAVE_CUSP)
#define complexlib cusp
#include <cusp/complex.h>
#else
#define complexlib std
#include <complex>
#endif

#define WhaleRealPartComplex(a)      (a).real()
#define WhaleImaginaryPartComplex(a) (a).imag()
#define WhaleAbsComplex(a)           complexlib::abs(a)
#define WhaleConjComplex(a)          complexlib::conj(a)
#define WhaleSqrtComplex(a)          complexlib::sqrt(a)
#define WhalePowComplex(a,b)         complexlib::pow(a,b)
#define WhaleExpComplex(a)           complexlib::exp(a)
#define WhaleLogComplex(a)           complexlib::log(a)
#define WhaleSinComplex(a)           complexlib::sin(a)
#define WhaleCosComplex(a)           complexlib::cos(a)

#if defined(WHALE_USE_REAL_SINGLE)
typedef complexlib::complex<float> WhaleComplex;
#elif defined(WHALE_USE_REAL_DOUBLE)
typedef complexlib::complex<double> WhaleComplex;
#elif defined(WHALE_USE_REAL___FLOAT128)
typedef complexlib::complex<__float128> WhaleComplex; /* Notstandard and not expected to work, use __complex128 */
#endif  /* WHALE_USE_REAL_ */
#endif  /* WHALE_USE_COMPLEX && WHALE_DESIRE_COMPLEX */

#elif defined(WHALE_CLANGUAGE_C) && defined(WHALE_HAVE_C99_COMPLEX)
/* Use C99 _Complex for the type. Do not include complex.h by default to define "complex" because of symbol conflicts in Hypre. */
/* Compilation units that can safely use complex should define WHALE_DESIRE_COMPLEX before including any headers */
#if defined(WHALE_USE_COMPLEX) || defined(WHALE_DESIRE_COMPLEX)
#define WHALE_HAVE_COMPLEX 1
#include <complex.h>

#if defined(WHALE_USE_REAL_SINGLE)
typedef float _Complex WhaleComplex;

#define WhaleRealPartComplex(a)      crealf(a)
#define WhaleImaginaryPartComplex(a) cimagf(a)
#define WhaleAbsComplex(a)           cabsf(a)
#define WhaleConjComplex(a)          conjf(a)
#define WhaleSqrtComplex(a)          csqrtf(a)
#define WhalePowComplex(a,b)         cpowf(a,b)
#define WhaleExpComplex(a)           cexpf(a)
#define WhaleLogComplex(a)           clogf(a)
#define WhaleSinComplex(a)           csinf(a)
#define WhaleCosComplex(a)           ccosf(a)

#elif defined(WHALE_USE_REAL_DOUBLE)
typedef double _Complex WhaleComplex;

#define WhaleRealPartComplex(a)      creal(a)
#define WhaleImaginaryPartComplex(a) cimag(a)
#define WhaleAbsComplex(a)           cabs(a)
#define WhaleConjComplex(a)          conj(a)
#define WhaleSqrtComplex(a)          csqrt(a)
#define WhalePowComplex(a,b)         cpow(a,b)
#define WhaleExpComplex(a)           cexp(a)
#define WhaleLogComplex(a)           clog(a)
#define WhaleSinComplex(a)           csin(a)
#define WhaleCosComplex(a)           ccos(a)

#elif defined(WHALE_USE_REAL___FLOAT128)
typedef __complex128 WhaleComplex;
WHALE_EXTERN MPI_Datatype MPIU___COMPLEX128 WhaleAttrMPITypeTag(__complex128);

#define WhaleRealPartComplex(a)      crealq(a)
#define WhaleImaginaryPartComplex(a) cimagq(a)
#define WhaleAbsComplex(a)           cabsq(a)
#define WhaleConjComplex(a)          conjq(a)
#define WhaleSqrtComplex(a)          csqrtq(a)
#define WhalePowComplex(a,b)         cpowq(a,b)
#define WhaleExpComplex(a)           cexpq(a)
#define WhaleLogComplex(a)           clogq(a)
#define WhaleSinComplex(a)           csinq(a)
#define WhaleCosComplex(a)           ccosq(a)
#endif /* WHALE_USE_REAL_* */
#elif defined(WHALE_USE_COMPLEX)
#error "Whale was configured --with-scalar-type=complex, but a language-appropriate complex library is not available"
#endif /* WHALE_USE_COMPLEX || WHALE_DESIRE_COMPLEX */
#endif /* (WHALE_CLANGUAGE_CXX && WHALE_HAVE_CXX_COMPLEX) else-if (WHALE_CLANGUAGE_C && WHALE_HAVE_C99_COMPLEX) */

#if defined(WHALE_HAVE_COMPLEX)
#if defined(WHALE_HAVE_MPI_C_DOUBLE_COMPLEX)
#define MPIU_C_DOUBLE_COMPLEX MPI_C_DOUBLE_COMPLEX
#define MPIU_C_COMPLEX MPI_C_COMPLEX
#else
# if defined(WHALE_CLANGUAGE_CXX) && defined(WHALE_HAVE_CXX_COMPLEX)
  typedef complexlib::complex<double> whale_mpiu_c_double_complex;
  typedef complexlib::complex<float> whale_mpiu_c_complex;
# elif defined(WHALE_CLANGUAGE_C) && defined(WHALE_HAVE_C99_COMPLEX)
  typedef double _Complex whale_mpiu_c_double_complex;
  typedef float _Complex whale_mpiu_c_complex;
# else
  typedef struct {double real,imag;} whale_mpiu_c_double_complex;
  typedef struct {float real,imag;} whale_mpiu_c_complex;
# endif
WHALE_EXTERN MPI_Datatype MPIU_C_DOUBLE_COMPLEX WhaleAttrMPITypeTagLayoutCompatible(whale_mpiu_c_double_complex);
WHALE_EXTERN MPI_Datatype MPIU_C_COMPLEX WhaleAttrMPITypeTagLayoutCompatible(whale_mpiu_c_complex);
#endif /* WHALE_HAVE_MPI_C_DOUBLE_COMPLEX */
#endif /* WHALE_HAVE_COMPLEX */

#if defined(WHALE_HAVE_COMPLEX)
#  if defined(WHALE_USE_REAL_SINGLE)
#    define MPIU_COMPLEX MPIU_C_COMPLEX
#  elif defined(WHALE_USE_REAL_DOUBLE)
#    define MPIU_COMPLEX MPIU_C_DOUBLE_COMPLEX
#  elif defined(WHALE_USE_REAL___FLOAT128)
#    define MPIU_COMPLEX MPIU___COMPLEX128
#  endif /* WHALE_USE_REAL_* */
#endif

#if defined(WHALE_USE_COMPLEX)
typedef WhaleComplex WhaleScalar;
#define WhaleRealPart(a)      WhaleRealPartComplex(a)
#define WhaleImaginaryPart(a) WhaleImaginaryPartComplex(a)
#define WhaleAbsScalar(a)     WhaleAbsComplex(a)
#define WhaleConj(a)          WhaleConjComplex(a)
#define WhaleSqrtScalar(a)    WhaleSqrtComplex(a)
#define WhalePowScalar(a,b)   WhalePowComplex(a,b)
#define WhaleExpScalar(a)     WhaleExpComplex(a)
#define WhaleLogScalar(a)     WhaleLogComplex(a)
#define WhaleSinScalar(a)     WhaleSinComplex(a)
#define WhaleCosScalar(a)     WhaleCosComplex(a)

#define MPIU_SCALAR MPIU_COMPLEX

/*
    real number definitions
 */
#else /* WHALE_USE_COMPLEX */
typedef WhaleReal WhaleScalar;
#define MPIU_SCALAR MPIU_REAL

#define WhaleRealPart(a)      (a)
#define WhaleImaginaryPart(a) ((WhaleReal)0.)
WHALE_STATIC_INLINE WhaleReal WhaleAbsScalar(WhaleScalar a) {return a < 0.0 ? -a : a;}
#define WhaleConj(a)          (a)
#if !defined(WHALE_USE_REAL___FLOAT128)
#define WhaleSqrtScalar(a)    sqrt(a)
#define WhalePowScalar(a,b)   pow(a,b)
#define WhaleExpScalar(a)     exp(a)
#define WhaleLogScalar(a)     log(a)
#define WhaleSinScalar(a)     sin(a)
#define WhaleCosScalar(a)     cos(a)
#else /* WHALE_USE_REAL___FLOAT128 */
#define WhaleSqrtScalar(a)    sqrtq(a)
#define WhalePowScalar(a,b)   powq(a,b)
#define WhaleExpScalar(a)     expq(a)
#define WhaleLogScalar(a)     logq(a)
#define WhaleSinScalar(a)     sinq(a)
#define WhaleCosScalar(a)     cosq(a)
#endif /* WHALE_USE_REAL___FLOAT128 */

#endif /* WHALE_USE_COMPLEX */

#define WhaleSign(a) (((a) >= 0) ? ((a) == 0 ? 0 : 1) : -1)
#define WhaleAbs(a)  (((a) >= 0) ? (a) : -(a))

/* --------------------------------------------------------------------------*/

/*
   Certain objects may be created using either single or double precision.
   This is currently not used.
*/
typedef enum { WHALE_SCALAR_DOUBLE,WHALE_SCALAR_SINGLE, WHALE_SCALAR_LONG_DOUBLE } WhaleScalarPrecision;

#if defined(WHALE_HAVE_COMPLEX)
/* WHALE_i is the imaginary number, i */
WHALE_EXTERN WhaleComplex WHALE_i;
#endif

/*MC
   WhaleMin - Returns minimum of two numbers

   Synopsis:
   #include "whalemath.h"
   type WhaleMin(type v1,type v2)

   Not Collective

   Input Parameter:
+  v1 - first value to find minimum of
-  v2 - second value to find minimum of


   Notes: type can be integer or floating point value

   Level: beginner


.seealso: WhaleMin(), WhaleClipInterval(), WhaleAbsInt(), WhaleAbsReal(), WhaleSqr()

M*/
#define WhaleMin(a,b)   (((a)<(b)) ?  (a) : (b))

/*MC
   WhaleMax - Returns maxium of two numbers

   Synopsis:
   #include "whalemath.h"
   type max WhaleMax(type v1,type v2)

   Not Collective

   Input Parameter:
+  v1 - first value to find maximum of
-  v2 - second value to find maximum of

   Notes: type can be integer or floating point value

   Level: beginner

.seealso: WhaleMin(), WhaleClipInterval(), WhaleAbsInt(), WhaleAbsReal(), WhaleSqr()

M*/
#define WhaleMax(a,b)   (((a)<(b)) ?  (b) : (a))

/*MC
   WhaleClipInterval - Returns a number clipped to be within an interval

   Synopsis:
   #include "whalemath.h"
   type clip WhaleClipInterval(type x,type a,type b)

   Not Collective

   Input Parameter:
+  x - value to use if within interval (a,b)
.  a - lower end of interval
-  b - upper end of interval

   Notes: type can be integer or floating point value

   Level: beginner

.seealso: WhaleMin(), WhaleMax(), WhaleAbsInt(), WhaleAbsReal(), WhaleSqr()

M*/
#define WhaleClipInterval(x,a,b)   (WhaleMax((a),WhaleMin((x),(b))))

/*MC
   WhaleAbsInt - Returns the absolute value of an integer

   Synopsis:
   #include "whalemath.h"
   int abs WhaleAbsInt(int v1)

   Not Collective

   Input Parameter:
.   v1 - the integer

   Level: beginner

.seealso: WhaleMax(), WhaleMin(), WhaleAbsReal(), WhaleSqr()

M*/
#define WhaleAbsInt(a)  (((a)<0)   ? -(a) : (a))

/*MC
   WhaleAbsReal - Returns the absolute value of an real number

   Synopsis:
   #include "whalemath.h"
   Real abs WhaleAbsReal(WhaleReal v1)

   Not Collective

   Input Parameter:
.   v1 - the double


   Level: beginner

.seealso: WhaleMax(), WhaleMin(), WhaleAbsInt(), WhaleSqr()

M*/
#define WhaleAbsReal(a) (((a)<0)   ? -(a) : (a))

/*MC
   WhaleSqr - Returns the square of a number

   Synopsis:
   #include "whalemath.h"
   type sqr WhaleSqr(type v1)

   Not Collective

   Input Parameter:
.   v1 - the value

   Notes: type can be integer or floating point value

   Level: beginner

.seealso: WhaleMax(), WhaleMin(), WhaleAbsInt(), WhaleAbsReal()

M*/
#define WhaleSqr(a)     ((a)*(a))

/* ----------------------------------------------------------------------------*/
/*
     Basic constants
*/
#if defined(WHALE_USE_REAL___FLOAT128)
#define WHALE_PI                 M_PIq
#elif defined(M_PI)
#define WHALE_PI                 M_PI
#else
#define WHALE_PI                 3.14159265358979323846264338327950288419716939937510582
#endif

#if !defined(WHALE_USE_64BIT_INDICES)
#define WHALE_MAX_INT            2147483647
#define WHALE_MIN_INT            (-WHALE_MAX_INT - 1)
#else
#define WHALE_MAX_INT            9223372036854775807L
#define WHALE_MIN_INT            (-WHALE_MAX_INT - 1)
#endif

#if defined(WHALE_USE_REAL_SINGLE)
#  define WHALE_MAX_REAL                3.40282346638528860e+38F
#  define WHALE_MIN_REAL                -WHALE_MAX_REAL
#  define WHALE_MACHINE_EPSILON         1.19209290e-07F
#  define WHALE_SQRT_MACHINE_EPSILON    3.45266983e-04F
#  define WHALE_SMALL                   1.e-5
#elif defined(WHALE_USE_REAL_DOUBLE)
#  define WHALE_MAX_REAL                1.7976931348623157e+308
#  define WHALE_MIN_REAL                -WHALE_MAX_REAL
#  define WHALE_MACHINE_EPSILON         2.2204460492503131e-16
#  define WHALE_SQRT_MACHINE_EPSILON    1.490116119384766e-08
#  define WHALE_SMALL                   1.e-10
#elif defined(WHALE_USE_REAL___FLOAT128)
#  define WHALE_MAX_REAL                FLT128_MAX
#  define WHALE_MIN_REAL                -FLT128_MAX
#  define WHALE_MACHINE_EPSILON         FLT128_EPSILON
#  define WHALE_SQRT_MACHINE_EPSILON    1.38777878078e-17
#  define WHALE_SMALL                   1.e-20
#endif

WHALE_EXTERN WhaleErrorCode WhaleIsInfOrNanScalar(WhaleScalar);
WHALE_EXTERN WhaleErrorCode WhaleIsInfOrNanReal(WhaleReal);

/* ----------------------------------------------------------------------------*/
#define PassiveReal   WhaleReal
#define PassiveScalar WhaleScalar

/*
    These macros are currently hardwired to match the regular data types, so there is no support for a different
    MatScalar from WhaleScalar. We left the MatScalar in the source just in case we use it again.
 */
#define MPIU_MATSCALAR MPIU_SCALAR
typedef WhaleScalar MatScalar;
typedef WhaleReal MatReal;

struct whale_mpiu_2scalar {WhaleScalar a,b;};
WHALE_EXTERN MPI_Datatype MPIU_2SCALAR WhaleAttrMPITypeTagLayoutCompatible(struct whale_mpiu_2scalar);
#if defined(WHALE_USE_64BIT_INDICES) || !defined(MPI_2INT)
struct whale_mpiu_2int {WhaleInt a,b;};
WHALE_EXTERN MPI_Datatype MPIU_2INT WhaleAttrMPITypeTagLayoutCompatible(struct whale_mpiu_2int);
#else
#define MPIU_2INT MPI_2INT
#endif

WHALE_STATIC_INLINE WhaleInt WhalePowInt(WhaleInt base,WhaleInt power) {
  WhaleInt result = 1;
  while (power) {
    if (power & 1) result *= base;
    power >>= 1;
    base *= base;
  }
  return result;
}
WHALE_STATIC_INLINE WhaleReal WhalePowRealInt(WhaleReal base,WhaleInt power) {
  WhaleReal result = 1;
  while (power) {
    if (power & 1) result *= base;
    power >>= 1;
    base *= base;
  }
  return result;
}

#endif
