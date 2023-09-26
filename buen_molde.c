#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <time.h>

char *base;
int w;
int h;

int x_c;
int y_c;

int car(int x, int y)
{
    return 4*((h-y)*w + x);
}

void pix(int x, int y, int color)
{
    int *real = (int *) (base + car(x,y));
    *real = color;
}

int is_inside(int x, int y)
{
    return (x*x+y*y<= 100*100 && x*x+y*y>=50*50);
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
    int offx = x_c;
    int offy = y_c;
    struct timespec delay;
    delay.tv_nsec = 500000;


    for (int y = -100; y < 100; ++y)
    {
        for (int x = -100; x < 100; ++x)
        {
            if(is_inside(x,y))
            {
                pix(x+offx,y+offy,0x00ff0000);
                //nanosleep(&delay, NULL);
            }
            else
            {
                if (x == 0 || y == 0)
                {
                    pix(x+offx,y+offy,0x000000f1);
                }
            }
        }
    }
    
    return 0;
}
