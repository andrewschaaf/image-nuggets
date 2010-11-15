
#include "image-nuggets.h"


int main() {
    
    Rect rect, subrect;
    uint32_t **image;
    uint32_t backgroundColor;
    long y, runLength, k;
    
    // Load the image
    image = read_p6(stdin, &rect);
    if ((rect.w <= 1) || (rect.h <= 1)) {
        fatal_error("Invalid image size -- must be at least 2x2.");
    }
    
    // Consider all pixels at the edge. Assert that they have the same color and call that the background color.
    backgroundColor = check_borders(image, rect);
    
    // Crop the image as tightly as possible without cropping a non-background color.
    crop(image, &rect, backgroundColor);
    if ((rect.w <= 1) || (rect.h <= 1)) {
        fatal_error("Invalid image -- it must be at least 2x2 after cropping.");
    }
    
    // See which full-width horizontal lines contain only the background color and split the image on runs of k or more of those lines (k = 5).
    k = 5;
    runLength = 0;
    subrect.x = rect.x;
    subrect.w = rect.w;
    subrect.y = rect.y;
    for (y = rect.y; y < rect.h; y++) {
        if (isRowThisColor(image, y, &rect, backgroundColor)) {
            if (runLength == 0) {
                // We might have a subrect. If so, we know its h now.
                subrect.h = y - subrect.y;
            }
            runLength++;
        }
        else {
            if (runLength >= k) {
                
                // We haz a subrect! Crop it.
                crop(image, &subrect, backgroundColor);
                
                // Write it
                write_subimage_chunk(stdout, image, subrect);
                
                // Prep subrect the next one
                subrect.x = rect.x;
                subrect.w = rect.w;
                subrect.y = y;
                
                runLength = 0;
            }
        }
    }
    
    // Last one
    subrect.h = (rect.y + rect.h) - subrect.y;
    if (subrect.h > 0) {
        crop(image, &subrect, backgroundColor);
        write_subimage_chunk(stdout, image, subrect);
    }
    
    // We're exiting, so need to free image and its rows.
    
    return EXIT_SUCCESS;
}


uint32_t check_borders(uint32_t **image, Rect r) {
    
    long x, y;
    uint32_t backgroundColor = image[0][0];
    
    // Check the top, bottom
    for (y = 0; y < r.h; y += r.h - 1) {// Yes, h > 1
        if (!isRowThisColor(image, y, &r, backgroundColor)) {
            fatal_error("The border pixels are not all of the same color.");
        }
    }
    
    // Check the left, right
    for (x = 0; x < r.w; x += r.w - 1) {// // Yes, w > 1
        if (!isColThisColor(image, x, &r, backgroundColor)) {
            fatal_error("The border pixels are not all of the same color.");
        }
    }
    
    return backgroundColor;
}


void crop(uint32_t **image, Rect *r, uint32_t backgroundColor) {
    
    long x, y;
    
    // [Bring the roof down](http://www.theonion.com/articles/roof-on-fire-claims-lives-of-43-party-people,1584/)
    for (y = r->y; y < (r->y + r->h); y++) {
        if (!isRowThisColor(image, y, r, backgroundColor)) {
            break;
        }
        r->y = r->y + 1;
        r->h = r->h - 1;
    }
    
    // Bring the floor up
    for (y = (r->y + r->h - 1); y >= r->y; y--) {
        if (!isRowThisColor(image, y, r, backgroundColor)) {
            break;
        }
        r->h = r->h - 1;
    }
    
    // Bring the left in
    for (x = r->x; x < (r->x + r->w); x++) {
        if (!isColThisColor(image, x, r, backgroundColor)) {
            break;
        }
        r->x = r->x + 1;
        r->w = r->w - 1;
    }
    
    // Bring the right in
    for (x = (r->x + r->w - 1); x >= r->x; x--) {
        if (!isColThisColor(image, x, r, backgroundColor)) {
            break;
        }
        r->w = r->w - 1;
    }
}

