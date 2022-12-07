#include <cmath>
#include <iomanip>
#include <iostream>
#include <omp.h>

#include "lodepng.h"

#define BUF_SIZE 100

typedef struct MandelbrotConfig {
    double xmax;
    double xmin;
    double ymax;
    double ymin;
    unsigned max_iterations;
} MandelbrotConfig;

typedef struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;


// perform a linear interpolation of x ([0; 1]) between xmax and xmin
#define LINEAR_INTERP(x, xmax, xmin) ((x) * ((xmax) - (xmin)) + (xmin))

// convert from HSV color scheme to RGB
// this function was adapted from https://github.com/Helenjw/CSC317/blob/master/lab1/src/hsv_to_rgb.cpp
Color hsv2rgb(const double h, const double s, const double v){
    double r = 0, g = 0, b = 0;

    double c = v * s;
    double h_prime = h / 60.0;
    double x = c * (1.0 - abs(std::fmod(h_prime, 2.0) - 1.0));

    if (0 <= h_prime && h_prime < 1) {
        r = c;
        g = x;
        b = 0;
    }
    else if (1 <= h_prime && h_prime < 2) {
        r = x;
        g = c;
        b = 0;
    }
    else if (2 <= h_prime && h_prime < 3) {
        r = 0;
        g = c;
        b = x;
    }
    else if (3 <= h_prime && h_prime < 4) {
        r = 0;
        g = x;
        b = c;
    }
    else if (4 <= h_prime && h_prime < 5) {
        r = x;
        g = 0;
        b = c;
    }
    else if (5 <= h_prime && h_prime < 6) {
        r = c;
        g = 0;
        b = x;
    }

    double m = v - c;

    Color color = {};

    color.r = 255.0 * (r + m);
    color.g = 255.0 * (g + m);
    color.b = 255.0 * (b + m);

    return color;
}

// get a color in a pallette for a given scale
Color color_palette(double x) {
    // make value log of (x) to enhance colors
    // make hue vary between yellow and green
    return hsv2rgb(x * 120.0 + 60.0, 1.0, std::log2f(x));
}

// calculate pixel (xp, yp) and write it to image
void mandelbrot_pixel(
    unsigned char* image, unsigned width, unsigned height, 
    unsigned xp, unsigned yp, 
    MandelbrotConfig cfg
) {
    double xm = LINEAR_INTERP(xp / (double)width, cfg.xmax, cfg.xmin);
    double ym = LINEAR_INTERP(yp / (double)height, cfg.ymax, cfg.ymin);

    double r = 0, c = 0;

    unsigned iter = 0;
    while (r*r + c*c <= 4 && iter < cfg.max_iterations) {
        double rtmp = r*r - c*c + xm;
        c = 2*r*c + ym;
        r = rtmp;

        iter++;
    }

    Color color = color_palette(iter / (double)cfg.max_iterations);

    image[4 * width * yp + 4 * xp + 0] = color.r;
    image[4 * width * yp + 4 * xp + 1] = color.g;
    image[4 * width * yp + 4 * xp + 2] = color.b;
    image[4 * width * yp + 4 * xp + 3] = 255; // alpha channel
}

void mandelbrot_serial(unsigned char* image, unsigned width, unsigned height, MandelbrotConfig cfg) {
    for (unsigned yp = 0; yp < height; yp++) {
        for (unsigned xp = 0; xp < width; xp++) {
            mandelbrot_pixel(image, width, height, xp, yp, cfg);
        }
    }
}

void mandelbrot_parallel_for(unsigned char* image, unsigned width, unsigned height, MandelbrotConfig cfg) {
    #pragma omp parallel for
    for (unsigned yp = 0; yp < height; yp++) {
        for (unsigned xp = 0; xp < width; xp++) {
            mandelbrot_pixel(image, width, height, xp, yp, cfg);
        }
    }
}

void mandelbrot_tasks(unsigned char* image, unsigned width, unsigned height, MandelbrotConfig cfg) {
    for (unsigned yp = 0; yp < height; yp++) {
        for (unsigned xp = 0; xp < width; xp++) {
            // each pixel is an OpenMP task
            #pragma omp task
            mandelbrot_pixel(image, width, height, xp, yp, cfg);
        }
    }
    // wait until all pixels have completed
    #pragma omp taskwait
}

void (*mandelbrot)(unsigned char*, unsigned, unsigned, MandelbrotConfig) = 
    #ifdef P_FOR
        mandelbrot_parallel_for
    #elif P_TASK
        mandelbrot_tasks
    #else
        mandelbrot_serial
    #endif
    ;


int main(int argc, const char * argv[]) {
    unsigned width, height;
    MandelbrotConfig cfg;
    char image_path[BUF_SIZE];

    std::cout << "width: ";
    std::cin >> width;

    std::cout << "height: ";
    std::cin >> height;

    double real, complex, zoom;
    std::cout << "real complex zoom: ";
    std::cin >> real >> complex >> zoom;
    cfg.xmax = real + 1/zoom;
    cfg.xmin = real - 1/zoom;
    cfg.ymax = complex + 1/zoom;
    cfg.ymin = complex - 1/zoom;

    std::cout << "max_iterations: ";
    std::cin >> cfg.max_iterations;

    std::cout << "image path:";
    std::cin >> std::setw(BUF_SIZE) >> image_path;

    // We cannot use std::vector since that is thread-safe and would introduce locking
    // Size of array is (width * height) pixels with 4 (RGBA) channels
    unsigned char* image = new unsigned char[width * height * 4];

    mandelbrot(image, width, height, cfg);

    unsigned error = lodepng::encode(image_path, image, width, height);

    delete[] image;

    if (error) {
        std::cerr << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 1;
    }

    return 0;
}
