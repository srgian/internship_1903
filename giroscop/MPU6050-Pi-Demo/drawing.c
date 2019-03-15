#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>


char *device_map = 0;
struct fb_fix_screeninfo fixed_info;
struct fb_var_screeninfo var_info;

void pixel_plotter(int x, int y, int c)
{
    unsigned int pix_offset = x + y * fixed_info.line_length;...
