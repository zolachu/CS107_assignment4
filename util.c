#include "samples/prototypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *binsert(const void *key, void *base, size_t *p_nelem, size_t width,
              int (*compar)(const void *, const void *)) {
    // TODO: implement this function (and remove the line below)

  char *base1 = (char*)base;

  for(size_t nremain = *p_nelem; nremain != 0; nremain >>=1) {
    void *p = base1 + (nremain >> 1)* width;
    int sign = compar(key, p);

    if (sign == 0) return p; // if matching element is found
    if (sign > 0) {
      base1 = (char *)p + width;
      nremain--;
    }
  }

  ++(*p_nelem);

  size_t index = ((char*)base1 - (char*)base)/width;

  memmove((char*)base + (index + 1)* width,
	  (char*)base + index* width, (*p_nelem - index + 1)* width);

  return  memcpy((char *)base + index * width, key, width);
}
