/***
*qsort.c - quicksort algorithm; qsort() library function for sorting arrays
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   To implement the qsort() routine for sorting arrays.
*
*******************************************************************************/
typedef unsigned int size_t;

#include"qsort.h"
#include<memory.h>
/* Always compile this module for speed, not size */
#pragma optimize("t", on)

#if defined (_M_CEE)
#define __fileDECL  __clrcall
#else  /* defined (_M_CEE) */
#define __fileDECL  __cdecl
#endif  /* defined (_M_CEE) */

/* when building Managed Run time dll, we should be defining __cdecl
 * to __clrcall. Also note that when compiling for MRT, we are compiling
 * as C++ file.
 */

/* prototypes for local routines */
#ifdef __USE_CONTEXT
static void __fileDECL shortsort_s(char *lo, char *hi, size_t width,
                int (__fileDECL *comp)(void *, const void *, const void *), void *);
#define swap swap_c
#else  /* __USE_CONTEXT */
static void shortsort(char *lo, char *hi, size_t  width, size_t  element_size, size_t  element_interval_size, size_t  element_num,
                int (__fileDECL *comp)(void *, void *, size_t&, size_t&));
#endif  /* __USE_CONTEXT */

static void swap(char *p, char *q, const size_t element_size, const size_t element_interval_size, const size_t element_num);

/* this parameter defines the cutoff between using quick sort and
   insertion sort for arrays; arrays with lengths shorter or equal to the
   below value use insertion sort */

#define CUTOFF 8            /* testing shows that this is good value */

/* Note: the theoretical number of stack entries required is
   no more than 1 + log2(num).  But we switch to insertion
   sort for CUTOFF elements or less, so we really only need
   1 + log2(num) - log2(CUTOFF) stack entries.  For a CUTOFF
   of 8, that means we need no more than 30 stack entries for
   32 bit platforms, and 62 for 64-bit platforms. */
#define STKSIZ (8*sizeof(void*) - 2)

/***
*qsort(base, num, wid, comp) - quicksort function for sorting arrays
*
*Purpose:
*   quicksort the array of elements
*   side effects:  sorts in place
*   maximum array size is number of elements times size of elements,
*   but is limited by the virtual address space of the processor
*
*Entry:
*   char *base = pointer to base of array
*   size_t num  = number of elements in the array
*   size_t width = width in bytes of each array element
*   int (*comp)() = pointer to function returning analog of strcmp for
*           strings, but supplied by user for comparing the array elements.
*           it accepts 2 pointers to elements.
*           Returns neg if 1<2, 0 if 1=2, pos if 1>2.
*
*Exit:
*   returns void
*
*Exceptions:
*   Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

#ifdef __USE_CONTEXT
#define __COMPARE(context, p1, p2) comp(context, p1, p2)
#define __SHORTSORT(lo, hi, width, comp, context) shortsort_s(lo, hi, width, comp, context);
#else  /* __USE_CONTEXT */
#define __COMPARE(context, p1, p2, element_interval_size, element_num) comp(p1, p2, element_interval_size, element_num)
#define __SHORTSORT(lo, hi, width, element_size, element_interval_size, element_num , comp, context) shortsort(lo, hi, width, element_size, element_interval_size, element_num , comp);
#endif  /* __USE_CONTEXT */


void qsort (
    void *base,
    size_t num,//交换个数
    size_t width,//交换块的宽度
	size_t  element_size,//交换的元素大小
	size_t  element_interval_size,//交换的元素的间距
	size_t  element_num,//交换的元素的个数
    int (*comp)(void *, void *, size_t&, size_t&)
    )
{
    char *lo, *hi;              /* ends of sub-array currently sorting */
    char *mid;                  /* points to middle of subarray */
    char *loguy, *higuy;        /* traveling pointers for partition step */
    size_t size;                /* size of the sub-array */
    char *lostk[STKSIZ], *histk[STKSIZ];
    int stkptr;                 /* stack for saving sub-array to be processed */

    /* validation section */
    //_VALIDATE_RETURN_VOID(base != NULL || num == 0, EINVAL);
    //_VALIDATE_RETURN_VOID(width > 0, EINVAL);
    //_VALIDATE_RETURN_VOID(comp != NULL, EINVAL);

    if (num < 2)
        return;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = (char *)base;
    hi = (char *)base + width * (num-1);        /* initialize limits */

    /* this entry point is for pseudo-recursion calling: setting
       lo and hi and jumping to here is like recursion, but stkptr is
       preserved, locals aren't, so we preserve stuff on the stack */
recurse:

    size = (hi - lo) / width + 1;        /* number of el's to sort */

    /* below a certain size, it is faster to use a O(n^2) sorting method */
    if (size <= CUTOFF) {
        __SHORTSORT(lo, hi, width, element_size, element_interval_size, element_num, comp, context);
    }
    else {
        /* First we pick a partitioning element.  The efficiency of the
           algorithm demands that we find one that is approximately the median
           of the values, but also that we select one fast.  We choose the
           median of the first, middle, and last elements, to avoid bad
           performance in the face of already sorted data, or data that is made
           up of multiple sorted runs appended together.  Testing shows that a
           median-of-three algorithm provides better performance than simply
           picking the middle element for the latter case. */

        mid = lo + (size / 2) * width;      /* find middle element */

        /* Sort the first, middle, last elements into order */
        if (__COMPARE(context, lo, mid, element_interval_size, element_num) > 0) {
			swap(lo, mid, element_size, element_interval_size, element_num);
        }
        if (__COMPARE(context, lo, hi, element_interval_size, element_num) > 0) {
            swap(lo, hi,  element_size, element_interval_size, element_num);
        }
        if (__COMPARE(context, mid, hi, element_interval_size, element_num) > 0) {
            swap(mid, hi,  element_size, element_interval_size, element_num);
        }

        /* We now wish to partition the array into three pieces, one consisting
           of elements <= partition element, one of elements equal to the
           partition element, and one of elements > than it.  This is done
           below; comments indicate conditions established at every step. */

        loguy = lo;
        higuy = hi;

        /* Note that higuy decreases and loguy increases on every iteration,
           so loop must terminate. */
        for (;;) {
            /* lo <= loguy < hi, lo < higuy <= hi,
               A[i] <= A[mid] for lo <= i <= loguy,
               A[i] > A[mid] for higuy <= i < hi,
               A[hi] >= A[mid] */

            /* The doubled loop is to avoid calling comp(mid,mid), since some
               existing comparison funcs don't work when passed the same
               value for both pointers. */

            if (mid > loguy) {
                do  {
                    loguy += width;
                } while (loguy < mid && __COMPARE(context, loguy, mid, element_interval_size, element_num) <= 0);
            }
            if (mid <= loguy) {
                do  {
                    loguy += width;
                } while (loguy <= hi && __COMPARE(context, loguy, mid, element_interval_size, element_num) <= 0);
            }

            /* lo < loguy <= hi+1, A[i] <= A[mid] for lo <= i < loguy,
               either loguy > hi or A[loguy] > A[mid] */

            do  {
                higuy -= width;
            } while (higuy > mid && __COMPARE(context, higuy, mid, element_interval_size, element_num) > 0);

            /* lo <= higuy < hi, A[i] > A[mid] for higuy < i < hi,
               either higuy == lo or A[higuy] <= A[mid] */

            if (higuy < loguy)
                break;

            /* if loguy > hi or higuy == lo, then we would have exited, so
               A[loguy] > A[mid], A[higuy] <= A[mid],
               loguy <= hi, higuy > lo */
			if(loguy != higuy)
			{
				swap(loguy, higuy,  element_size, element_interval_size, element_num);
			}

            /* If the partition element was moved, follow it.  Only need
               to check for mid == higuy, since before the swap,
               A[loguy] > A[mid] implies loguy != mid. */

            if (mid == higuy)
                mid = loguy;

            /* A[loguy] <= A[mid], A[higuy] > A[mid]; so condition at top
               of loop is re-established */
        }

        /*     A[i] <= A[mid] for lo <= i < loguy,
               A[i] > A[mid] for higuy < i < hi,
               A[hi] >= A[mid]
               higuy < loguy
           implying:
               higuy == loguy-1
               or higuy == hi - 1, loguy == hi + 1, A[hi] == A[mid] */

        /* Find adjacent elements equal to the partition element.  The
           doubled loop is to avoid calling comp(mid,mid), since some
           existing comparison funcs don't work when passed the same value
           for both pointers. */

        higuy += width;
        if (mid < higuy) {
            do  {
                higuy -= width;
            } while (higuy > mid && __COMPARE(context, higuy, mid, element_interval_size, element_num) == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= width;
            } while (higuy > lo && __COMPARE(context, higuy, mid, element_interval_size, element_num) == 0);
        }

        /* OK, now we have the following:
              higuy < loguy
              lo <= higuy <= hi
              A[i]  <= A[mid] for lo <= i <= higuy
              A[i]  == A[mid] for higuy < i < loguy
              A[i]  >  A[mid] for loguy <= i < hi
              A[hi] >= A[mid] */

        /* We've finished the partition, now we want to sort the subarrays
           [lo, higuy] and [loguy, hi].
           We do the smaller one first to minimize stack usage.
           We only sort arrays of length 2 or more.*/

        if ( higuy - lo >= hi - loguy ) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }                           /* save big recursion for later */

            if (loguy < hi) {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }

            if (lo < higuy) {
                hi = higuy;
                goto recurse;           /* do small recursion */
            }
        }
    }

    /* We have sorted the array, except for any pending sorts on the stack.
       Check if there are any, and do them. */

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
        return;                 /* all subarrays done */
}


/***
*shortsort(hi, lo, width, comp) - insertion sort for sorting short arrays
*shortsort_s(hi, lo, width, comp, context) - insertion sort for sorting short arrays
*
*Purpose:
*       sorts the sub-array of elements between lo and hi (inclusive)
*       side effects:  sorts in place
*       assumes that lo < hi
*
*Entry:
*       char *lo = pointer to low element to sort
*       char *hi = pointer to high element to sort
*       size_t width = width in bytes of each array element
*       int (*comp)() = pointer to function returning analog of strcmp for
*               strings, but supplied by user for comparing the array elements.
*               it accepts 2 pointers to elements, together with a pointer to a context
*               (if present). Returns neg if 1<2, 0 if 1=2, pos if 1>2.
*       void *context - pointer to the context in which the function is
*               called. This context is passed to the comparison function.
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/

#ifdef __USE_CONTEXT
static void __fileDECL shortsort_s (
    char *lo,
    char *hi,
    size_t width,
    int (__fileDECL *comp)(void *, const void *, const void *),
    void * context
    )
#else  /* __USE_CONTEXT */
static void __fileDECL shortsort (
    char *lo,
    char *hi,
	size_t  width,
	size_t  element_size,//交换的元素大小
	size_t  element_interval_size,//交换的元素的间距
	size_t  element_num,//交换的元素的个数
    int (__fileDECL *comp)(void *, void *, size_t&, size_t&)
    )
#endif  /* __USE_CONTEXT */
{
    char *p, *max;

    /* Note: in assertions below, i and j are alway inside original bound of
       array to sort. */
    while (hi > lo) {
        /* A[i] <= A[j] for i <= j, j > hi */
        max = lo;
        for (p = lo+width; p <= hi; p += width) {
            /* A[i] <= A[max] for lo <= i < p */
            if (__COMPARE(context, p, max, element_interval_size, element_num) > 0) {
                max = p;
            }
            /* A[i] <= A[max] for lo <= i <= p */
        }

        /* A[i] <= A[max] for lo <= i <= hi */
		if(max != hi)
		{
			swap(max, hi,  element_size, element_interval_size, element_num);
		}

        /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

        hi -= width;

        /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
    }
    /* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
       so array is sorted */
}

/***
*swap(a, b, width) - swap two elements
*
*Purpose:
*       swaps the two array elements of size width
*
*Entry:
*       char *a, *b = pointer to two elements to swap
*       size_t width = width in bytes of each array element
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/

static void __fileDECL swap (
							 char *a,
							 char *b,
							 const size_t element_size,
							 const size_t element_interval_size,
							 const size_t element_num
							 )
{
	char *p_a , *p_b, temp;
	size_t i;
	size_t temp_element_size = element_size;
	for(i = 0; i < element_num; ++i)
	{
		p_a = a;
		p_b = b;
		if ( a != b )
			/* Do the swap one character at a time to avoid potential alignment
			problems. */
			temp_element_size = element_size;
			while ( temp_element_size-- ) {
				temp = *a;
				*a = *b;
				*b = temp;
				++a; 
				++b;
			}
			a = p_a + element_interval_size;
			b = p_b + element_interval_size;
	}
}

#undef __fileDECL
#undef __COMPARE
#undef __SHORTSORT
#undef swap
