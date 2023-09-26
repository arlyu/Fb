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
#include "fakes.h"

char *base;
int w;
int h;
int foffset = 0;

int x_c;
int y_c;
int qsize = 300;

int car(int x, int y)
{
    return 4 * ((h - y) * w + x);
}

void pix(int x, int y, int color)
{
    int *real = (int *)(base + car(x, y));
    *real = color;
}

int is_safe(double x, double y)
{
    return (x < w && x >= 0 && y < h && y >= 0);
}

int is_inside(int x, int y)
{
    return (is_safe(x,y));
}

int main()
{
    int fb = open("/dev/fb0", O_RDWR);

    if (fb == -1)
    {
        printf("Shit\n");
        return -1;
    }

    struct fb_var_screeninfo vin;

    if (ioctl(fb, FBIOGET_VSCREENINFO, &vin))
    {
        printf("Fuck2\n");
        close(fb);
        return -1;
    }

    x_c = vin.xres / 2;
    y_c = vin.yres / 2;

    int ssize = vin.yres_virtual * vin.xres_virtual * vin.bits_per_pixel / 8;
    char *ptr = (char *)mmap(0, ssize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

    if (ptr == (char *) -1)
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
    delay.tv_nsec = 500;
    int dir = 0;

    struct timespec printDelay;
    printDelay.tv_nsec = 500;

//    for (int i = 0; i < 50; ++i)
//    {
//        for (double t = -3.141592; t < 1; t = (t >= 3.141592 ? t - 2 * 3.141592 + 0.25 : t + 0.25))
//        {
//            dir = ((foffset == 0 || foffset == 200) ? !dir : dir);
//            foffset = (int)(dir ? foffset + 1 : foffset - 1);
//            for (int y = 1-h / 2; y < h / 2; ++y)
//            {
//                for (int x = 1-w / 2; x < w / 2; ++x)
//                {
//                    if (is_inside(x, y))
//                    {
//                        //pix(x + offx, y + offy, 0x00ff0000 + (int)400 * sin(y));
//                        // nanosleep(&delay, NULL);
//                    }
//                    else
//                    {
//                        if (1)
//                        {
//                            pix(x + offx, y + offy, 0x0);
//                        }
//                        else
//                        {
//                            pix(x + offx, y + offy, 0x0);
//                        }
//                    }
//                }
//            }
//            
//
//            // nanosleep(&delay, NULL);
//        }
//       
//
//        nanosleep(&printDelay, NULL);
//    }

    int x = 30;
    int y = 30;
    for (double u = 0.01; u < 300; u += 0.10)
    {
        for (double t = 0.01; t<900; t += 0.025)
        {
            x = (4*t*cos(2*t+8*u))+offx;
            y = (4*t*sin(2*t+8*u))+offy;

            if(is_inside(x,y))
            {
                pix(x,y, 0x00f0f0f0 + 256*sin(u*400)+10*10*u);
                nanosleep(&delay,NULL);
            }
            //        printf("x: %lf,y: %lf, t: %lf\n", 50*fcos(t)+offx, 50*fsin(t)+offy, t);
        }
    }
    close(fb);
    munmap(base, ssize);

    return 0;
}
