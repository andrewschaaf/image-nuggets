
## Introduction

**<code>image-nuggets</code>** extracts sub-images from an image (or from HTML/CSS).

![](https://github.com/andrewschaaf/image-nuggets/raw/master/docs/diagram.png)

If the input is HTML, image-nuggets will use first use <code>webkit2png</code> (included) to render the input image.

* This requires Mac OS 10.5 or later. Cross-platform patches welcome.
* It's a modern engine, so u can haz <code>border-radius</code> and other modern goodness.
* This can be really useful, especially if you're writing a script to
[test zillions of shades](http://stopdesign.com/archive/2009/03/20/goodbye-google.html)
of [teal and orange](http://theabyssgazes.blogspot.com/2010/03/teal-and-orange-hollywood-please-stop.html).

For now, <code>image-nuggets</code> uses this and only this nugget extraction method:

1. Consider all pixels at the edge. Assert that they have the same color and call that the background color.
2. Crop the image as tightly as possible without cropping a non-background color.
3. See which full-width horizontal lines contain only the background color and split the image on runs of k or more of those lines (k = 5).
4. Crop each subimage as in (2).

#### Prereqs

* gcc
* [ImageMagick](http://www.imagemagick.org/script/index.php)
(or an equivalent <code>convert</code> on your <code>PATH</code>)
* Optional: [OptiPNG](http://optipng.sourceforge.net/)
&mdash; it'll be used iff (<code>--to=png</code>) and (<code>--optipng-level=N</code> (e.g. N=2))

#### Building / Installing

* **Build it**: <code>./make.sh</code>
* **Optionally install it**: add this repo's <code>bin/</code> to your <code>PATH</code>

#### Kicking the Tires

* <code>cd example; ./make-example.sh</code>

## Command-line tools

<pre>cat buttons.html | image-nuggets
                            --from=html
                            --to=png
                            --dest-prefix=...
                            --dest-suffixes=landing/lorem.png,landing/lorem_hover.png,...</pre>

<code>image-nuggets</code> is a Python script which wraps this C program:

<pre>cat foo.ppm | image-nuggets-c > ppms.chunks</pre>

* PPM must be [a P6](http://netpbm.sourceforge.net/doc/ppm.html)
* ppms.chunks: <code>''.join(('%d\n%s' % (len(ppm), ppm)) for ppm in subppms)</code>

<!--
## Literate Programming / Annotated Source Code

* docs/<a href="TODO/image_nuggets.py">image_nuggets.py</a>

* docs/<a href="TODO/image_nuggets_util.py">image\_nuggets\_util.py</a>

* docs/<a href="TODO/image-nuggets.c">image-nuggets.c</a>

* docs/<a href="TODO/image-nuggets-util.c">image-nuggets-util.c</a>
-->
