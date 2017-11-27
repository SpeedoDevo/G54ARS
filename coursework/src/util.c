#ifndef __SUB_UTIL__
#define __SUB_UTIL__

#include "header.h"


#define control(res, l, r)  do { res->left = (l); res->right = (r); res->active = true; } while(0)

#define inactive(res)  do { res->active = false; } while(0)

#define div(left, right)  ( ( (right) == 0 ) ? 0 : (left) / (right) )

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))

#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

#define clamp(v, low, high) do { if ((v) < (low)) { (v) = (low); } else if ((v) > (high)) { (v) = (high); } } while(0)

#endif //__SUB_UTIL__
