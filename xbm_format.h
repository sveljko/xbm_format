/* xbm_format - public domain XBM format writer
   no warranty implied; use at your own risk

   This is a "single header" C library, so you need to

#define XBM_FORMAT_IMPLEMENTATION

    before you include this header file in one of C/C++ files.

    This simple library is designed for working with monocrhome
    displays (these days, those are usually small ones used in
    embedded systems).  One would load an icon that is to be
    displayed on such a screen from some usual format (PNG, GIF...)
    and then use this library to convert to XBM, either off-line
    (writing to a file) or on-line (writing to a memory buffer).

    There is no support for "hotspots", which are rarely used in
    embedded systems and which have nothing to do with the actual
    pixel data. One could add them, if need be, by using one of the
    APIs that doesn't write the whole file.

    It has several options on how to decide whether a pixel is to be
    "on" or "off".

    Since handling of images with sizes that are not a multiple of 8
    is tricky and they are rarely used, we assume (precondition) that
    all the sizes we get are multiples of 8. If that turns out to be
    too restrictive, a future version might introduce support for
    other sizes.

    There is no support for scaling the image, Often one needs to
    scale down an image to make it fit on the small display, but,
    scaling can be tricky and would make this library much more
    complex _and_ complicated. If you need to scale, please do it
    beforehand.

    The library is easy to use with `stb_image`, for example:

        int x, y, n;
        unsigned char* img = stbi_load(filename, &x, &y, &n, 0);
        if (img) {
            img_to_xbm_filename(img, x, y, n, "my", "my.xbm");
        }

    It never uses malloc() or any such memory-allocation functions,
    expecting to caller to allocate memory. Of course, using the
    `FILE*` for writing to file might imply some allocations "behind
    the scenes", but, that's out of our control.

    There is currently no support for loading an XBM image from a file,
    though it's not very hard and can be added if the need arises.

    See the bottom of this header file for license information.
*/
#if !defined(INC_XBM_FORMAT)
#define INC_XBM_FORMAT


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>


#if !defined(XBM_FORMAT_THRESHOLD_ALPHA)
/** The default threshold for alpha channel to be considered "large enough"
    to warrant converting to `1` in the monochrome bitmap. */
#define XBM_FORMAT_THRESHOLD_ALPHA 0.2f
#endif

#if !defined(XBM_FORMAT_THRESHOLD_COLOR)
/** The default threshold for color to be considered "large enough"
    to warrant converting to `1` in the monochrome bitmap. */
#define XBM_FORMAT_THRESHOLD_COLOR 0.3f
#endif


/** Returns the number of bytes needed to encode a monochrome bitmap
    in XBM format for the given dimensions @p x (width) and @p y
    (height).

    This is the number of bytes in _memory_, not the number of bytes
    in an XBM file, which is a C source file with a specific format.

    Essentially, if you have an image with dimensions x and y and
    need to allocate memory for its monochrome XBM presentation,
    call this function, like:

        unsigned char* xbm = malloc(xbm_bytes_for_dimensions(x, y)) ;
        if (0 == img_to_xbm(data, x, y, n, xbm)) {

        }
        free(xbm);

    @precondition x%8 == 0
    @precondition y%8 == 0
    */
size_t xbm_bytes_for_dimensions(int x, int y);

/** Converts an colorful bitmap image, represented with @p data with dimensions 
    @p x (width) and @p y (height), with @p n components (where
    3 means RGB and 4 means RGBA) into a monochrome bitmap
    according to XBM format writing it to @p xbm.

    This is the "simple" API, using the default parameters to do
    this conversion. If you want to set all the parameters, use
    img_to_xbm_ex().

    @precondition x%8 == 0
    @precondition y%8 == 0

    @return 0: OK, otherwise error, failed to convert.
    */
int img_to_xbm(unsigned char const* data, int x, int y, int n, unsigned char* xbm);

/** Possible ways of deciding should we convert the colorful pixel
    to `1` or `0` in the resulting monochrome bitmap.
    */
enum img_to_xbm_option {
    /** Both the color(s) and the alpha channel should be
        "large enough" to put `1` in the result.
        */
    img_to_xbm_color_and_alpha,
    /** Either the color(s) or the alpha channel should be
        "large enough" to put `1` in the result.
        */
    img_to_xbm_color_or_alpha,
    /** Disregard color, only the alpha channel should be
        "large enough" to put `1` in the result.
        */
    img_to_xbm_only_alpha,
    /** Disregard the alpha channel (it need not be present at all), 
        only the color should be
        "large enough" to put `1` in the result.
        */
    img_to_xbm_ignore_alpha
};

/** Converts an colorful bitmap image, represented with @p data with dimensions 
    @p x (width) and @p y (height), with @p n components (where
    3 means RGB and 4 means RGBA) into a monochrome bitmap
    according to XBM format writing it to @p xbm.

    Use the @p opt to choose the algorithm for "deciding" how to
    convert the image pixels to monochrome result. Use the @p color_threshold
    and @p alpha_threshold to choose the level at which color and/or
    alpha channel are "large enough" to decide on `1`, rather than `0`,
    as the resulting pixel.

    The thresholds are values between 0 and 1.

    For the color, the average "intensity" of the three colors is used
    as the threshold. So, for example, if you want a "pure red" to be
    converted to `1`, your threshold needs to be at least `0.33`.

    @precondition x%8 == 0
    @precondition y%8 == 0

    @return 0: OK, otherwise error, failed to convert.
    */
int img_to_xbm_ex(unsigned char const*   data,
                  int                    x,
                  int                    y,
                  int                    n,
                  unsigned char*         xbm,
                  enum img_to_xbm_option opt,
                  float                  color_threshold,
                  float                  alpha_threshold);

/** Convert a colorful bitmap to XBM monochrome bitmap file. 
    Similar to img_to_xbm(), but, instead of writing to a memory buffer,
    this will write to a file, named @p filename, which it will create.
    This file will be in the XBM (11) format, with the bytes (octets)
    contents written in the C language source format.

    The @p  imgame will be used in the name array and the `_width` and 
    `_height` macros in the created file.

    This is the "simple" API, which uses the default parameters to do
    this conversion. To specify all the parameters, use 
    img_to_xbm_filename_ex().

    @precondition x%8 == 0
    @precondition y%8 == 0

    @return 0: OK, otherwise error, failed to convert/create the file.
    */
int img_to_xbm_filename(unsigned char const* data,
                        int                  x,
                        int                  y,
                        int                  n,
                        char const*          imgname,
                        char const*          filename);

/** Convert a colorful bitmap to XBM monochrome bitmap file. 
    This is the "extended" version of img_to_xbm_filename(), with
    pretty much the same semantics, it just lets you specify all
    the options in making the XBM bitmap.

    The options have the same meaning as for img_to_xbm_ex().

    @precondition x%8 == 0
    @precondition y%8 == 0

    @return 0: OK, otherwise error, failed to convert/create the file.
  */
int img_to_xbm_filename_ex(unsigned char const*   data,
                           int                    x,
                           int                    y,
                           int                    n,
                           char const*            imgname,
                           enum img_to_xbm_option opt,
                           float                  color_threshold,
                           float                  alpha_threshold,
                           char const*            filename);

/** Writes a colorful bitmap converted to XBM monochrome bitmap to the 
    given @p f file.

    This is very similar to img_to_xbm_filename(), the only difference is
    that this expects an already open file @p f. This enables you to write
    more than one bitmap to the same file, which is OK for XBM format.

    This will _not_ close the given file, you need to do it yourself
    some time after calling this function.

    This is the "simple" API, which uses the default parameters to do
    this conversion. To specify all the parameters, use 
    img_to_xbm_file_ex().

    @precondition x%8 == 0
    @precondition y%8 == 0

    @return 0: OK, otherwise error, failed to convert/write to the file.
*/
int img_to_xbm_file(unsigned char const* data,
                    int                  x,
                    int                  y,
                    int                  n,
                    char const*          imgname,
                    FILE*                f);

/** Writes a colorful bitmap converted to XBM monochrome bitmap to the 
    given @p f file.

    This is the extended version of img_to_xbm_file(), which allows you
    to set all the parameters of the conversion. Otherwise the semantics
    are the same as for img_to_xbm_file(). The meaning of the parameters
    is the same as for img_to_xbm_ex().

    @precondition x%8 == 0
    @precondition y%8 == 0

    @return 0: OK, otherwise error, failed to convert/write to the file.
    */
int img_to_xbm_file_ex(unsigned char const*   data,
                       int                    x,
                       int                    y,
                       int                    n,
                       char const*            imgname,
                       enum img_to_xbm_option opt,
                       float                  color_threshold,
                       float                  alpha_threshold,
                       FILE*                  f);

#ifdef __cplusplus
}
#endif

#endif /* !defined(INC_XBM_FORMAT) */


#if defined(XBM_FORMAT_IMPLEMENTATION)

#include <assert.h>


size_t xbm_bytes_for_dimensions(int x, int y)
{
    assert(x > 0);
    assert(y > 0);
    assert(x % 8 == 0);
    assert(y % 8 == 0);
    return (x / 8) * y;
}


int img_to_xbm(unsigned char const* data, int x, int y, int n, unsigned char* xbm)
{
    return img_to_xbm_ex(data,
                         x,
                         y,
                         n,
                         xbm,
                         img_to_xbm_color_or_alpha,
                         XBM_FORMAT_THRESHOLD_COLOR,
                         XBM_FORMAT_THRESHOLD_ALPHA);
}


static int img_to_xbm_decide_by_color(unsigned char const* data,
                                      int                  offset,
                                      int                  n,
                                      float                color_thold)
{
    assert(n >= 3);
    return data[offset] + data[offset + 1] + data[offset + 2] > 255 * 3 * color_thold;
}


static int img_to_xbm_decide_by_alpha(unsigned char const* data,
                                      int                  offset,
                                      int                  n,
                                      float                alpha_thold)
{
    assert(n == 4);
    return data[offset + 3] > 255 * alpha_thold;
}

static int img_to_xbm_decide_bit(unsigned char const*   data,
                                 int                    offset,
                                 int                    n,
                                 enum img_to_xbm_option opt,
                                 float                  color_thold,
                                 float                  alpha_thold)
{
    switch (opt) {
    case img_to_xbm_color_and_alpha:
        return img_to_xbm_decide_by_color(data, offset, n, color_thold)
               && img_to_xbm_decide_by_alpha(data, offset, n, alpha_thold);
    case img_to_xbm_color_or_alpha:
        return img_to_xbm_decide_by_color(data, offset, n, color_thold)
               || img_to_xbm_decide_by_alpha(data, offset, n, alpha_thold);
    case img_to_xbm_only_alpha:
        return img_to_xbm_decide_by_alpha(data, offset, n, alpha_thold);
    case img_to_xbm_ignore_alpha:
        return img_to_xbm_decide_by_color(data, offset, n, color_thold);
    default:
        return 0;
    }
}


int img_to_xbm_ex(unsigned char const*   data,
                  int                    x,
                  int                    y,
                  int                    n,
                  unsigned char*         xbm,
                  enum img_to_xbm_option opt,
                  float                  color_threshold,
                  float                  alpha_threshold)
{
    int iy;
    assert(data != NULL);
    assert(xbm != NULL);
    for (iy = 0; iy < y; ++iy) {
        int ix;
        for (ix = 0; ix < x; ix += 8) {
            unsigned char byte = 0;
            int           pos;
            for (pos = 0; pos < 8; ++pos) {
                const int offset = (iy * x + ix + pos) * n;
                const int bit    = img_to_xbm_decide_bit(
                    data, offset, n, opt, color_threshold, alpha_threshold);
                byte |= bit << pos;
            }
            xbm[(iy * x + ix) / 8] = byte;
        }
    }
    return 0;
}


int img_to_xbm_filename(unsigned char const* data,
                        int                  x,
                        int                  y,
                        int                  n,
                        char const*          imgname,
                        char const*          filename)
{
    return img_to_xbm_filename_ex(data,
                                  x,
                                  y,
                                  n,
                                  imgname,
                                  img_to_xbm_color_and_alpha,
                                  XBM_FORMAT_THRESHOLD_COLOR,
                                  XBM_FORMAT_THRESHOLD_ALPHA,
                                  filename);
}


int img_to_xbm_filename_ex(unsigned char const*   data,
                           int                    x,
                           int                    y,
                           int                    n,
                           char const*            imgname,
                           enum img_to_xbm_option opt,
                           float                  color_threshold,
                           float                  alpha_threshold,
                           char const*            filename)
{
    int   rslt = -1;
    FILE* f    = fopen(filename, "w");
    if (f != NULL) {
        rslt = img_to_xbm_file_ex(
            data, x, y, n, imgname, opt, color_threshold, alpha_threshold, f);
        fclose(f);
    }
    return rslt;
}


int img_to_xbm_file(unsigned char const* data,
                    int                  x,
                    int                  y,
                    int                  n,
                    char const*          imgname,
                    FILE*                f)
{
    return img_to_xbm_file_ex(data,
                              x,
                              y,
                              n,
                              imgname,
                              img_to_xbm_color_and_alpha,
                              XBM_FORMAT_THRESHOLD_COLOR,
                              XBM_FORMAT_THRESHOLD_ALPHA,
                              f);
}


int img_to_xbm_file_ex(unsigned char const*   data,
                       int                    x,
                       int                    y,
                       int                    n,
                       char const*            imgname,
                       enum img_to_xbm_option opt,
                       float                  color_threshold,
                       float                  alpha_threshold,
                       FILE*                  f)
{
    int iy;
    assert(data != NULL);
    assert(imgname != NULL);
    assert(f != NULL);
    assert(x % 8 == 0);
    assert(y % 8 == 0);

    fprintf(f, "#define %s_width %d\n", imgname, x);
    fprintf(f, "#define %s_height %d\n", imgname, y);
    fprintf(f, "static unsigned char %s_bits[] = {", imgname);

    for (iy = 0; iy < y; ++iy) {
        int ix;
        fprintf(f, "%s\n    ", (0 == iy) ? "" : ",");
        for (ix = 0; ix < x; ix += 8) {
            unsigned char byte = 0;
            int           pos;
            for (pos = 0; pos < 8; ++pos) {
                const int offset = (iy * x + ix + pos) * n;
                const int bit    = img_to_xbm_decide_bit(
                    data, offset, n, opt, color_threshold, alpha_threshold);
                byte |= bit << pos;
            }
            fprintf(f, "%s0x%02x", (0 == ix) ? "" : ", ", byte);
        }
    }
    fputs("};\n", f);
    return 0;
}


#endif /* defined(XBM_FORMAT_IMPLEMENTATION) */

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2021 Srdjan Veljkovic
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
