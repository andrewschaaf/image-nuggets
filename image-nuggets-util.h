
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    long x;
    long y;
    long w;
    long h;
} Rect;


void fatal_error(char *msg);
long num_digits(long n);

bool isRowThisColor(uint32_t **image, long y, Rect *r, uint32_t color);
bool isColThisColor(uint32_t **image, long x, Rect *r, uint32_t color);

long p6_size(long w, long h);
void write_subimage_chunk(FILE *f, uint32_t **image, Rect clip);
uint32_t** read_p6(FILE *f, Rect *bounds);
