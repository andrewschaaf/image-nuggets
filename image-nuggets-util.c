
#include "image-nuggets-util.h"

//### Misc. Functions

void fatal_error(char *msg) {
    fprintf(stderr, "******* OH NOES *******\n%s\n", msg);
    exit(1);
}

// Written non-generally so you can that you can verify,
// in a few seconds, that it won't be wrong or non-terminating:
long num_digits(long n) {
    if (n < 0)          {fatal_error("Invalid size.");}
    else if (n < 10)    {return 1;}
    else if (n < 100)   {return 2;}
    else if (n < 1000)  {return 3;}
    else if (n < 10000) {return 4;}
    else                {fatal_error("Size too big.");}
}

//### Analysis Functions

bool isRowThisColor(uint32_t **image, long y, Rect *r, uint32_t color) {
    long x;
    for (x = r->x; x < (r->x + r->w); x++) {
        if (image[y][x] != color) {
            return false;
        }
    }
    return true;
}

bool isColThisColor(uint32_t **image, long x, Rect *r, uint32_t color) {
    long y;
    for (y = r->y; y < (r->y + r->h); y++) {
        if (image[y][x] != color) {
            return false;
        }
    }
    return true;
}

//### PPM Functions

#define LINEBUF_MAXLEN  100

// <code>"P6\n%d %d\n255\n"</code> &rarr; 9 bytes plus the {w,h} digits
long p6_size(long w, long h) {
    return 9 + num_digits(w) + num_digits(h) + (3 * w * h);
}

void write_subimage_chunk(FILE *f, uint32_t **image, Rect r) {
    
    long x, y;
    int channel;
    uint8_t value;
    
    // Chunk header:
    fprintf(f, "%ld\n", p6_size(r.w, r.h));
    
    // PPM:
    fprintf(f, "P6\n%ld %ld\n255\n", r.w, r.h);
    for (y = r.y; y < (r.y + r.h); y++) {
        for (x = r.x; x < (r.x + r.w); x++) {
            for (channel = 0; channel < 3; channel++) {
                value = (image[y][x] >> (8 * channel)) % 256;
                fputc(value, f);
            }
        }
    }
}

// This implementation makes assumptions about the whitespace in the header.
// The PPM format is more general that that,
// but ImageMagick and image-nuggets produce PPMs of this specific form.
// If you don't like this special-casing, submit a patch.
uint32_t** read_p6(FILE *f, Rect *bounds) {
    
    char line[LINEBUF_MAXLEN + 1];
    char *tok;
    long w, h, x, y;
    int c, channel;
    
    bounds->x = 0;
    bounds->y = 0;
    
    // "P6\n"
    fgets(line, LINEBUF_MAXLEN, f);
    if (strcmp(line, "P6\n") != 0) {
        fatal_error("Invalid PPM file -- only P6 supported.");
    }
    
    // "%d %d\n"
    fgets(line, LINEBUF_MAXLEN, f);
    tok = strtok(line, " \n");
    bounds->w = w = atol(tok);
    tok = strtok(NULL, " \n");
    bounds->h = h = atol(tok);
    if ((w <= 0) || (h <= 0)) {
        fatal_error("Invalid image size.");
    } 
    
    // "255\n"
    fgets(line, LINEBUF_MAXLEN, f);
    if (strcmp(line, "255\n") != 0) {
        fatal_error("Invalid PPM file -- we only support P6 with 1-byte values.");
    }
    
    // Allocate image
    uint32_t **image = malloc(h * sizeof(uint32_t*));
    for (y = 0; y < h; y++) {
        image[y] = (uint32_t*)calloc(w, 4);
    }
    
    // Read pixels
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            for (channel = 0; channel < 3; channel++) {
                c = fgetc(f);
                if (c == EOF) {
                    fatal_error("Not enough pixels in PPM data!");
                }
                else {
                    image[y][x] |= (((uint32_t)c) << (8 * channel));
                }
            }
        }
    }
    
    return image;
}

