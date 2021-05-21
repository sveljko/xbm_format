#define XBM_FORMAT_IMPLEMENTATION
#include "xbm_format.h"

#include <string.h>


#define X 'x', 'x', 'x', 'x',
#define _ ' ', ' ', ' ', ' ',
#define __() _

unsigned char const img_simp[] = {
    X _ X _ X _ X __() //
    X _ X _ X _ X __() //
    X _ X _ X _ X __() //
    X _ X _ X _ X __() //
    X _ X _ X _ X __() //
    X _ X _ X _ X __() //
    X _ X _ X _ X __() //
    X _ X _ X _ X __() //
};

unsigned char const xbm_simp[] = { 0x55, 0x55, 0x55, 0x55,
                                   0x55, 0x55, 0x55, 0x55 };

char const xmb_file_simp[] = { "#define simp_width 8\n"
                               "#define simp_height 8\n"
                               "static unsigned char simp_bits[] = {\n"
                               "    0x55,\n"
                               "    0x55,\n"
                               "    0x55,\n"
                               "    0x55,\n"
                               "    0x55,\n"
                               "    0x55,\n"
                               "    0x55,\n"
                               "    0x55};\n\n" };


unsigned char const img_med[] = {
    _ _ _ _ X _ X _ _ _ _ _ _ _ _ __() //
    _ _ X _ X _ X _ X _ _ _ _ _ _ __() //
    X X X X X X X X X X X X X _ _ __() //
    X X X X X X X X X X X X X X X __() //
    _ _ X _ X X X X X _ X _ _ _ _ __() //
    _ _ X _ X X _ _ X _ X _ _ _ _ __() //
    _ _ X _ X _ _ _ X _ X _ _ _ _ __() //
    _ _ X _ X _ _ _ X _ X _ _ _ _ __() //
};

unsigned char const xbm_med[] = { 0x50, 0x00, 0x54, 0x01, 0xff, 0x1f,
                                  0xff, 0x7f, 0xf4, 0x05, 0x34, 0x05,
                                  0x14, 0x05, 0x14, 0x05 };


char const xmb_file_med[] = { "#define med_width 16\n"
                              "#define med_height 8\n"
                              "static unsigned char med_bits[] = {\n"
                              "    0x50, 0x00,\n"
                              "    0x54, 0x01,\n"
                              "    0xff, 0x1f,\n"
                              "    0xff, 0x7f,\n"
                              "    0xf4, 0x05,\n"
                              "    0x34, 0x05,\n"
                              "    0x14, 0x05,\n"
                              "    0x14, 0x05};\n\n" };


void test_simp()
{
    unsigned char xbm[sizeof xbm_simp];
    assert(0 == img_to_xbm(img_simp, 8, 8, 4, xbm));
    assert(memcmp(xbm, xbm_simp, sizeof xbm) == 0);
}


int writef(char const* ovde, char const* ovo, size_t n)
{
    size_t w;
    FILE*  f = fopen(ovde, "w");
    if (NULL == f) {
        return -1;
    }
    w = fwrite(ovo, n, 1, f);
    fclose(f);
    return w - 1;
}


int compf(char const* ovo, char const* sovim)
{
    char  red_p[1024];
    char  d_red[1024];
    FILE* p = fopen(ovo, "r");
    FILE* d;

    if (NULL == p) {
        fprintf(stderr, "Failed to open  '%s' \n", ovo);
        return -1;
    }

    d = fopen(sovim, "r");
    if (NULL == d) {
        fprintf(stderr, "Failed to open  '%s' \n", sovim);
        fclose(p);
        return -1;
    }

    while (fgets(red_p, sizeof red_p, p)) {
        if (fgets(d_red, sizeof d_red, d)) {
            if (strcmp(red_p, d_red) != 0) {
                fprintf(stderr,
                        "Difference, in file '%s' : \n"
                        "%s\n"
                        "but in '%s':\n"
                        "%s\n",
                        ovo,
                        red_p,
                        sovim,
                        d_red);
                fclose(p);
                fclose(d);
                return -1;
            }
        }
    }

    fclose(p);
    fclose(d);
    return 0;
}


void test_simp_fname()
{
    assert(0 == img_to_xbm_filename(img_simp, 8, 8, 4, "simp", "simp.xbm"));
    assert(0 == writef("simp.xbm.expected", xmb_file_simp, sizeof xmb_file_simp - 1));
    assert(0 == compf("simp.xbm.expected", "simp.xbm"));
}


void test_med()
{
    unsigned char xbm[sizeof xbm_med];
    assert(0 == img_to_xbm(img_med, 16, 8, 4, xbm));
    assert(memcmp(xbm, xbm_med, sizeof xbm) == 0);
}


void test_med_fname()
{
    assert(0 == img_to_xbm_filename(img_med, 16, 8, 4, "med", "med.xbm"));
    assert(0 == writef("med.xbm.expected", xmb_file_med, sizeof xmb_file_med - 1));
    assert(0 == compf("med.xbm.expected", "med.xbm"));
}


int main()
{
    test_simp();
    test_simp_fname();
    test_med();
    test_med_fname();

    return 0;
}
