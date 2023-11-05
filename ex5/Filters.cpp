
#include <iostream>
#include "Matrix.h"

/**
 * helper for convolution
 * @param small
 * @param smallMiddleX
 * @param smallMiddleY
 * @param r
 * @param c
 * @param image
 * @param res
 */
void
calcConvVal(const Matrix &small, int smallMiddleX, int smallMiddleY, int r, int c, const Matrix &image, Matrix &res)
{
    for (int rs = 0; rs < small.getRows(); ++rs)  // go over rows of small
    {
        // i-1,j-1 ->i+1,j+1 image. 0,0->2,2

        int rowBound = small.getRows() - 1 - rs;  // row bound index
        for (int cs = 0; cs < small.getCols(); ++cs) // go over cols of small
        {
            int colBound = small.getCols() - 1 - cs;  // col bound index
            int iNew = r + (smallMiddleY - rowBound); // row index new (to check bounds)
            int jNew = c + (smallMiddleX - colBound); // col index new (to check bounds)
            // only if new index in bound - add:
            if (iNew >= 0 && jNew >= 0 && iNew < image.getRows() && jNew < image.getCols())
            {
                res(r, c) += (image(iNew, jNew) * small(rs, cs));
            }
        }
    }
}

/**
 * this func does the convolution process
 * @param image
 * @param small
 * @return matrix after convolution
 */
Matrix convolution(const Matrix &image, const Matrix &small)
{
    // create result matrix :
    Matrix res(image.getRows(), image.getCols());
    // find center of small:
    int smallMiddleX = small.getCols() / 2;
    int smallMiddleY = small.getRows() / 2;
    for (int r = 0; r < image.getRows(); ++r)  //go over rows of image
    {
        for (int c = 0; c < image.getCols(); ++c)  // go over cols of image
        {
            calcConvVal(small, smallMiddleX, smallMiddleY, r, c, image, res);
            res(r, c) = rintf(res(r, c));
        }
    }
    return res;
}

/**
 * Operator Quantization
 * Performs quantization on the input image by the given number of levels.
 * Returns new matrix which is the result of running the operator on the image
 * @param image
 * @param levels
 * @return
 */
Matrix quantization(const Matrix &image, int levels)
{

    // 256 is the highest number - MAX_PXLS
    // to get the lower bound:
    // Divide MAX_PXLS by levels to get rangeInLevel.
    // Divide each pixel(place in mat) by rangeInLevel, and throw away the remainder.
    // to get the lower bound:
    // 200, level =8
    // range in level = 32
    // lower = 200/32 =6.25
    // upper = 6.25 +32 -1 = 37.25 -> 21
    Matrix res(image.getRows(), image.getCols());
    int rangeInLevel = 256 / levels; //assuming there would be no remainder

    int *rangeArr = new int[levels];
    int *averageArr = new int[levels];
    for (int i = 0; i < levels; ++i)
    {
        rangeArr[i] = i * rangeInLevel;
    }
    for (int i = 0; i < levels - 1; ++i)
    {
        averageArr[i] = (rangeArr[i + 1] + rangeArr[i] - 1) / 2;
    }
    averageArr[levels - 1] = (int) ((256 + rangeArr[levels - 1] - 1) / 2);

    for (int i = 0; i < image.getRows() * image.getCols(); ++i)
    {
        int idx = (floorf(image[i] / rangeInLevel));  //todo check if there's a mistake here !!!
        res[i] = averageArr[idx];
    }

    for (int i = 0; i < image.getRows() * image.getCols(); ++i)
    {
        res[i] = rintf(res[i]);
    }
    delete[] rangeArr;
    delete[] averageArr;
    return res;
}

/**
 *  make sure mat vals are in range
 * @param numCells
 * @param res
 * @return
 */
Matrix &makeMatrixInBounds(const int numCells, Matrix &res)
{

    for (int i = 0; i < numCells; ++i)
    {
        if (res[i] > 255)
        {
            res[i] = 255;
        }
        else if (res[i] < 0)
        {
            res[i] = 0;
        }
    }
    return res;
}

/**
 * Gaussian Blurring
 * Performs gaussian blurring on the input image.
 * Returns new matrix which is the result of running the operator on the image
 * @param image
 * @return
 */
Matrix blur(const Matrix &image)
{
    Matrix temp(3, 3);
    float valTemp[] = {1 / 16.0, 2 / 16.0, 1 / 16.0, 2 / 16.0, 4 / 16.0, 2 / 16.0, 1 / 16.0, 2 / 16.0, 1 / 16.0};

    for (int i = 0; i < 9; ++i)
    {
        temp[i] = valTemp[i];
    }
    Matrix res;
    res = convolution(image, temp);
    int numCells = image.getRows() * image.getCols();
    res = makeMatrixInBounds(numCells, res);
    return res;
}

/**
 * Sobel operator (edge detection)
 * Performs sobel edge detection on the input image.
 * Returns new matrix which is the result of running the operator on the image.
 * @param image
 * @return
 */
Matrix sobel(const Matrix &image)
{
    // set first mat
    Matrix mat1(3, 3);
    float valMat1[] = {1 / 8.0, 0, -1 / 8.0, 2 / 8.0, 0, -2 / 8.0, 1 / 8.0, 0, -1 / 8.0};

    for (int i = 0; i < 9; ++i)
    {
        mat1[i] = valMat1[i];
    }
    // set second mat
    Matrix mat2(3, 3);
    float valMat2[] = {1.0 / 8.0, 2 / 8.0, 1 / 8.0, 0, 0, 0, -1 / 8.0, -2 / 8.0, -1 / 8.0};

    for (int j = 0; j < 9; ++j)
    {
        mat2[j] = valMat2[j];
    }

    // calc convolution for each mat:
    mat1 = convolution(image, mat1);

    mat2 = convolution(image, mat2);

    // calc convolution res:
    Matrix res;
//    res = convolution(image, mat1) +convolution(image, mat2);
    res = mat1 + mat2;
    int numCells = image.getRows() * image.getCols();
    res = makeMatrixInBounds(numCells, res);
    return res;
}
