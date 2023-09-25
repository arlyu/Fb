#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>

char *base;
int w;
int h;

int x_c;
int y_c;

int car(int x, int y)
{
    return 4*((h-x)*w + y);
}

void pix(int x, int y, int color)
{
    int *real = (int *) (base + car(x,y));
    *real = color;
}

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

    int qsize = 200;
    x_c = vin.xres/2;
    y_c = vin.yres/2;

    int ssize = vin.yres_virtual * vin.xres_virtual * vin.bits_per_pixel / 8;
    char *ptr = (char *) mmap(0, ssize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

    if((int)ptr == -1)
    {
        printf("Fuck3\n");
        close(fb);
        return -1;
    }

    base = ptr;
    w = vin.xres;
    h = vin.yres;

    printf("bpp: %d\tyv:%d\ty:%d\n", vin.bits_per_pixel, vin.yres_virtual, vin.yres);

    ptr += car(20,20);
    //pix( 20, 20, 0x00ff0000);

    for (int y = -200; y < 200; ++y)
    {
        

        for (int x = -200; x < 200; ++x)
        {
            if(x*x+y*y <= 4000)
            {
                ptr[x * 4] = 0;
                ptr[x * 4+1] = 255;  
                ptr[x * 4+2] = 0;
                ptr[x * 4+3] = 0;
            }
            else
            {
                ptr[x * 4] = 255;
                ptr[x * 4+1] = 0;  
                ptr[x * 4+2] = 0;
                ptr[x * 4+3] = 0;

            }
        }
        
        ptr += vin.xres*4;
    }
    return 0;
}
