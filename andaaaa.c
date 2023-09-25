#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>

int main()
{
    int fb = open("/dev/fb0", O_RDWR);

    if(fb == -1)
    {
        printf("Shit\n");
        return -1;
    }

    struct fb_var_screeninfo vin;

    if(ioctl(fb, FBIOGET_VSCREENINFO, &vin))
    {
        printf("Fuck2\n");
        close(fb);
        return -1;
    }

    int qsize = 50;
    int x_c = (vin.xres - qsize)/2;
    int y_c = (vin.yres - qsize)/2;

    int ssize = vin.yres_virtual * vin.xres_virtual * vin.bits_per_pixel / 8;
    char *ptr = (char *) mmap(0, ssize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

    if((int)ptr == -1)
    {
        printf("Fuck3\n");
        close(fb);
        return -1;
    }

    printf("bpp: %d\tyv:%d\ty:%d\n", vin.bits_per_pixel, vin.yres_virtual, vin.yres);

    ptr += vin.xres * y_c * 4 + x_c * 4;

    for (int y = -50; y < qsize; ++y)
    {
        

        for (int x = -50; x < qsize; ++x)
        {
            ptr[x * 4] = 0;
            if(x*x+y*y <= 320)
            {
                ptr[x * 4+1] = 255;
            }
            ptr[x * 4+2] = 0;
            ptr[x * 4+3] = 0;
        }
        
        ptr += vin.xres_virtual*4;
    }




    return 0;
}
