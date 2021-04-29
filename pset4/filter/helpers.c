//Implement the functions in helpers.c such that a user can apply
//grayscale, sepia, reflection, or blur filters to their images.

#include <math.h>
#include "helpers.h"


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //loop though each row and then each pixel
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            //calculate average pixel value
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            //set each color value to avg value
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //loop through each pixel and calculate each new color value using SEPIA formula
    //ensure the result is int between 0-255 inclusive
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            //SEPIA formula
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            if(sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if(sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if(sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //swap pixels horizontally to oposite side

    RGBTRIPLE temp;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width/2; j++)
        {
            temp = image[i][j];
            image[i][j] =  image[i][(width-1) - j];
            image[i][(width-1) - j] = temp;

        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempimage[height][width];

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            //upper left corner
            if(i == 0 && j == 0)
            {
                tempimage[i][j].rgbtRed = round((image[i][j].rgbtRed
                                            + image[i][j+1].rgbtRed
                                            + image[i+1][j].rgbtRed
                                            + image[i+1][j+1].rgbtRed) / 4.0);

                tempimage[i][j].rgbtGreen = round((image[i][j].rgbtGreen
                                            + image[i][j+1].rgbtGreen
                                            + image[i+1][j].rgbtGreen
                                            + image[i+1][j+1].rgbtGreen) / 4.0);

                tempimage[i][j].rgbtBlue = round((image[i][j].rgbtBlue
                                            + image[i][j+1].rgbtBlue
                                            + image[i+1][j].rgbtBlue
                                            + image[i+1][j+1].rgbtBlue) / 4.0);
            }

            //upper right corner
            else if(i == 0 && j == width -1)
            {
                tempimage[i][j].rgbtRed = round((image[i][j].rgbtRed
                                            + image[i][j-1].rgbtRed
                                            + image[i+1][j].rgbtRed
                                            + image[i+1][j-1].rgbtRed) / 4.0);

                tempimage[i][j].rgbtGreen = round((image[i][j].rgbtGreen
                                            + image[i][j-1].rgbtGreen
                                            + image[i+1][j].rgbtGreen
                                            + image[i+1][j-1].rgbtGreen) / 4.0);

                tempimage[i][j].rgbtBlue = round((image[i][j].rgbtBlue
                                            + image[i][j-1].rgbtBlue
                                            + image[i+1][j].rgbtBlue
                                            + image[i+1][j-1].rgbtBlue) / 4.0);
            }

             //lower right corner
            else if(i == height - 1 && j == width -1)
            {
                tempimage[i][j].rgbtRed = round((image[i][j].rgbtRed
                                            + image[i][j-1].rgbtRed
                                            + image[i-1][j].rgbtRed
                                            + image[i-1][j-1].rgbtRed) / 4.0);

                tempimage[i][j].rgbtGreen = round((image[i][j].rgbtGreen
                                            + image[i][j-1].rgbtGreen
                                            + image[i-1][j].rgbtGreen
                                            + image[i-1][j-1].rgbtGreen) / 4.0);

                tempimage[i][j].rgbtBlue = round((image[i][j].rgbtBlue
                                            + image[i][j-1].rgbtBlue
                                            + image[i-1][j].rgbtBlue
                                            + image[i-1][j-1].rgbtBlue) / 4.0);
            }

             //lower left corner
            else if(i == height - 1 && j == 0)
            {
                tempimage[i][j].rgbtRed = round((image[i][j].rgbtRed
                                            + image[i][j+1].rgbtRed
                                            + image[i-1][j].rgbtRed
                                            + image[i-1][j+1].rgbtRed) / 4.0);

                tempimage[i][j].rgbtGreen = round((image[i][j].rgbtGreen
                                            + image[i][j+1].rgbtGreen
                                            + image[i-1][j].rgbtGreen
                                            + image[i-1][j+1].rgbtGreen) / 4.0);

                tempimage[i][j].rgbtBlue = round((image[i][j].rgbtBlue
                                            + image[i][j+1].rgbtBlue
                                            + image[i-1][j].rgbtBlue
                                            + image[i-1][j+1].rgbtBlue) / 4.0);
            }

            //upper edge
            else if(i == 0 && j != 0 && j != width - 1)
            {
                tempimage[i][j].rgbtRed = round((image[i][j].rgbtRed
                                            + image[i][j+1].rgbtRed
                                            + image[i][j-1].rgbtRed
                                            + image[i+1][j].rgbtRed
                                            + image[i+1][j-1].rgbtRed
                                            + image[i+1][j+1].rgbtRed) / 6.0);

                tempimage[i][j].rgbtGreen = round((image[i][j].rgbtGreen
                                            + image[i][j+1].rgbtGreen
                                            + image[i][j-1].rgbtGreen
                                            + image[i+1][j].rgbtGreen
                                            + image[i+1][j-1].rgbtGreen
                                            + image[i+1][j+1].rgbtGreen) / 6.0);

                tempimage[i][j].rgbtBlue = round((image[i][j].rgbtBlue
                                            + image[i][j+1].rgbtBlue
                                            + image[i][j-1].rgbtBlue
                                            + image[i+1][j].rgbtBlue
                                            + image[i+1][j-1].rgbtBlue
                                            + image[i+1][j+1].rgbtBlue) / 6.0);
            }

            //lower edge
            else if(i == height -1  && j != 0 && j != width - 1)
            {
                tempimage[i][j].rgbtRed = round((image[i][j].rgbtRed
                                            + image[i][j+1].rgbtRed
                                            + image[i][j-1].rgbtRed
                                            + image[i-1][j].rgbtRed
                                            + image[i-1][j-1].rgbtRed
                                            + image[i-1][j+1].rgbtRed) / 6.0);

                tempimage[i][j].rgbtGreen = round((image[i][j].rgbtGreen
                                            + image[i][j+1].rgbtGreen
                                            + image[i][j-1].rgbtGreen
                                            + image[i-1][j].rgbtGreen
                                            + image[i-1][j-1].rgbtGreen
                                            + image[i-1][j+1].rgbtGreen) / 6.0);

                tempimage[i][j].rgbtBlue = round((image[i][j].rgbtBlue
                                            + image[i][j+1].rgbtBlue
                                            + image[i][j-1].rgbtBlue
                                            + image[i-1][j].rgbtBlue
                                            + image[i-1][j-1].rgbtBlue
                                            + image[i-1][j+1].rgbtBlue) / 6.0);
            }

            //left edge
            else if(j == 0 && i != 0 && i != height - 1)
            {
                tempimage[i][j].rgbtRed = round((image[i][j].rgbtRed
                                            + image[i-1][j].rgbtRed
                                            + image[i-1][j+1].rgbtRed
                                            + image[i][j+1].rgbtRed
                                            + image[i+1][j+1].rgbtRed
                                            + image[i+1][j].rgbtRed) / 6.0);

                tempimage[i][j].rgbtGreen = round((image[i][j].rgbtGreen
                                            + image[i-1][j].rgbtGreen
                                            + image[i-1][j+1].rgbtGreen
                                            + image[i][j+1].rgbtGreen
                                            + image[i+1][j+1].rgbtGreen
                                            + image[i+1][j].rgbtGreen) / 6.0);

                tempimage[i][j].rgbtBlue = round((image[i][j].rgbtBlue
                                            + image[i-1][j].rgbtBlue
                                            + image[i-1][j+1].rgbtBlue
                                            + image[i][j+1].rgbtBlue
                                            + image[i+1][j+1].rgbtBlue
                                            + image[i+1][j].rgbtBlue) / 6.0);
            }

             //right edge
            else if(j == width - 1 && i != 0 && i != height - 1)
            {
                tempimage[i][j].rgbtRed = round((image[i][j].rgbtRed
                                            + image[i-1][j-1].rgbtRed
                                            + image[i-1][j].rgbtRed
                                            + image[i][j-1].rgbtRed
                                            + image[i+1][j-1].rgbtRed
                                            + image[i+1][j].rgbtRed) / 6.0);

                tempimage[i][j].rgbtGreen = round((image[i][j].rgbtGreen
                                            + image[i-1][j-1].rgbtGreen
                                            + image[i-1][j].rgbtGreen
                                            + image[i][j-1].rgbtGreen
                                            + image[i+1][j-1].rgbtGreen
                                            + image[i+1][j].rgbtGreen) / 6.0);

                tempimage[i][j].rgbtBlue = round((image[i][j].rgbtBlue
                                            + image[i-1][j-1].rgbtBlue
                                            + image[i-1][j].rgbtBlue
                                            + image[i][j-1].rgbtBlue
                                            + image[i+1][j-1].rgbtBlue
                                            + image[i+1][j].rgbtBlue) / 6.0);
            }

            //midle pixel case
            //count avg amount of each color from 3x3 grid around blur-to-be-pixel
            else
            {
                tempimage[i][j].rgbtRed = round((image[i-1][j-1].rgbtRed
                                            + image[i-1][j].rgbtRed
                                            + image[i-1][j+1].rgbtRed
                                            + image[i][j-1].rgbtRed
                                            + image[i][j].rgbtRed
                                            + image[i][j+1].rgbtRed
                                            + image[i+1][j-1].rgbtRed
                                            + image[i+1][j].rgbtRed
                                            + image[i+1][j+1].rgbtRed) / 9.0);

                tempimage[i][j].rgbtGreen = round((image[i-1][j-1].rgbtGreen
                                            + image[i-1][j].rgbtGreen
                                            + image[i-1][j+1].rgbtGreen
                                            + image[i][j-1].rgbtGreen
                                            + image[i][j].rgbtGreen
                                            + image[i][j+1].rgbtGreen
                                            + image[i+1][j-1].rgbtGreen
                                            + image[i+1][j].rgbtGreen
                                            + image[i+1][j+1].rgbtGreen) / 9.0);

                tempimage[i][j].rgbtBlue = round((image[i-1][j-1].rgbtBlue
                                            + image[i-1][j].rgbtBlue
                                            + image[i-1][j+1].rgbtBlue
                                            + image[i][j-1].rgbtBlue
                                            + image[i][j].rgbtBlue
                                            + image[i][j+1].rgbtBlue
                                            + image[i+1][j-1].rgbtBlue
                                            + image[i+1][j].rgbtBlue
                                            + image[i+1][j+1].rgbtBlue) / 9.0);
            }


        }
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            image[i][j] = tempimage[i][j];
        }
    }

    return;
}
