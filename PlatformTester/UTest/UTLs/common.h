#ifndef UTLS_COMMON_H_
#define UTLS_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

//typedef unsigned char  BOOLEAN

#ifndef PRIVATE
#define PRIVATE static
#endif

#ifdef PUBLIC
  #undef PUBLIC
#endif
#define PUBLIC

#endif /* UTLS_COMMON_H_ */
