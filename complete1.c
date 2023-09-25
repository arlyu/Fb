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
#include <math.h>

char *base;
int w;
int h;
int foffset = 0;

int x_c;
int y_c;
int qsize = 300;

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
    return (40*sin((double) x/40+foffset)<=y);
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

    printf("bpp: %d\tx:%d\ty:%d\n", vin.bits_per_pixel, w, h);

    int offx = x_c;
    int offy = y_c;
    struct timespec delay;
    delay.tv_nsec = 10000;

    for (double t = -3.141592; t < 10 ; t = (t >= 3.141592 ? t-2*3.141592+0.25 : t+0.25))
    {
        foffset = (int) (foffset <= w ? foffset + 1 : 0);
        for (int y = -h/2; y < h/2; ++y)
        {
            for (int x = -w/2; x < w/2; ++x)
            {
                if(is_inside(x,y))
                {
                    pix(x+offx,y+offy,0x00ff0000 +(int) 400*sin(y));
                    //nanosleep(&delay, NULL);
                }
                else
                {
                    if (1)
                    {
                        pix(x+offx,y+offy,0x0);
                    }
                    else
                    {
                        pix(x+offx, y+offy, 0x0);
                    }
                }
            }
        }
        //nanosleep(&delay, NULL);
    }

    return 0;
}
