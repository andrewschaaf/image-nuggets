
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "image-nuggets-util.h"


uint32_t check_borders(uint32_t **image, Rect r);
void crop(uint32_t **image, Rect *r, uint32_t backgroundColor);

