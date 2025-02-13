#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    int distance = kernelSize / 2; 
    GrayscaleImage copyImg=image;
    
    for (int i = 0; i < copyImg.get_height(); i++)
    {
        for (int j = 0; j < copyImg.get_width(); j++)
        {
            int sum = 0;

            for (int di = -distance; di <= distance; di++) {
                for (int dj = -distance; dj <= distance; dj++) {
                    if (i+di < 0 || j+dj < 0 || i+di >= image.get_height() || j+dj >= image.get_width())
                        sum+=0; 
                    else
                        sum+=copyImg.get_pixel(i+di,j+dj);
                }
            }
            image.set_pixel(i,j,sum / (kernelSize*kernelSize));
        }
        
    }
    
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // Create a Gaussian kernel of size kernelSize x kernelSize using a dynamic array
    double** kernel = new double*[kernelSize];
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = new double[kernelSize];
    }

    int distance = kernelSize / 2;
    double sum = 0.0;  // To normalize the kernel values

    // Calculate the kernel values using the Gaussian function
    for (int i = -distance; i <= distance; i++) {
        for (int j = -distance; j <= distance; j++) {
            double exponent = -(i * i + j * j) / (2 * sigma * sigma);
            kernel[i + distance][j + distance] = exp(exponent) / (2 * M_PI * sigma * sigma);
            sum += kernel[i + distance][j + distance];
        }
    }

    // Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] /= sum;
        }
    }

    // Copy the original image
    GrayscaleImage copyImg=image;

    // Apply the Gaussian smoothing filter to each pixel in the image
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            double weightedSum = 0.0;

            // Apply the kernel to the neighborhood
            for (int di = -distance; di <= distance; di++) {
                for (int dj = -distance; dj <= distance; dj++) {
                    if (i+di < 0 || j+dj < 0 || i+di >= image.get_height() || j+dj >= image.get_width())
                        sum+=0;
                    else
                        weightedSum += copyImg.get_pixel(i+di, j+dj) * kernel[di + distance][dj + distance];

                    
                }
            }

            image.set_pixel(i, j, (int)weightedSum);
        }
    }

    for (int i = 0; i < kernelSize; i++) {
        delete[] kernel[i];
    }
    delete[] kernel;

    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    GrayscaleImage copyImg = image;
    apply_gaussian_smoothing(copyImg,kernelSize,1.0);
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            int originalValue = image.get_pixel(i, j);
            int blurredValue = copyImg.get_pixel(i, j);
            int newValue = static_cast<int>(originalValue + amount * (originalValue - blurredValue));
            
            if (newValue > 255) {
                newValue = 255;
            } else if (newValue < 0) {
                newValue = 0;
            }

            image.set_pixel(i, j, newValue);
        }
    }
}
