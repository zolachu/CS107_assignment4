#include "samples/prototypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
/*
 * Function: binsert
 *-----------------
 * perform a binary search to search for the key and
 * if no matching element is found, it will insert 
 * the key into the proper position in the sorted array. 
 * 
 * return: a pointer to the key in the array
 */


void *binsert(const void *key, void *base, size_t *p_nelem, size_t width,
              int (*compar)(const void *, const void *)) {
    // TODO: implement this function (and remove the line below)

  char *base1 = (char*)base;
  //  assert(base1 != NULL);
  for(size_t nremain = *p_nelem; nremain != 0; nremain >>=1) {
    void *p = base1 + (nremain >> 1)* width;
    int sign = compar(key, p);
    
    if (sign  == 0) return p; /* if the key is found */
    if (sign > 0) {  /* key > p: move right */
      base1 = (char *)p + width;
      nremain--;
    }/* else move left */
  }

    ++(*p_nelem);
    //    printf("%lu", *p_nelem);

  size_t index = ((char*)base1 - (char*)base)/width;

  memmove((char*)base + (index + 1)*width, (char*)base + index*width, (*p_nelem - index + 1)* width);
  void* value = memcpy((char *)base + index * width, key, width);
  //  assert(value != NULL);
  return value;
}
