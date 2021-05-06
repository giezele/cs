#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

//Implement a program that recovers JPEGs from a forensic image, e.g. which were saved on a
//digital camera as JPEGs on a memory card.

//Pseudocode
  //open memory card
  //repeat untill end of card:
    //read 512 bytes into buffer
    //if start of new JPEG - look at header if it's jpeg 4 bytes signature
       //if first JPEG
         //...write into it starting from 000.jpeg
       //else
         //...stop writing in first and open up new file to write to
    //else
       //if already found JPEG
         //keep writing to it - it's next 512 block of your jpeg
  //close any remaining files


int main(int argc, char *argv[])
{
    //check for invalid usage
    if (argc < 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    uint8_t buffer[512];  // Buffer to store data you're reading
    // int count;            //fread returns count of read elements
    int fileCounter = 0;  //for the new jpg files to name;
    FILE *card;           //FILE * to read from
    FILE *img;            //File * to write 512 chunks to
    char title[16];

    //Open memory card.    FILE *f = fopen(filename, "r");
    card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("Error opening file\n");
        exit(2);
    }

    //repeat untill end of card: read 512 bytes into buffer
    while (fread(buffer, sizeof(uint8_t), 512, card) == 512)
    {
        //Look for beginning of JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            //Open a new JPEG file
            sprintf(title, "%03i.jpg", fileCounter);

            if (fileCounter == 0) //if first JPEG
            {
                img = fopen(title, "w");
                fwrite(buffer, sizeof(uint8_t), 512, img);
            }
            else //if JPEG already found
            {
                fclose(img); //pries tai esantis titlefileCounter -1.jpg
                fopen(title, "w"); //title su jau padidintu caunteriu is ifo
                fwrite(buffer, sizeof(uint8_t), 512, img);
            }
            fileCounter++;

      }
      else
      {
        //Write 512 bytes untill a new JPEG is found
        if (img != NULL)
            fwrite(buffer, sizeof(uint8_t), 512, img);
      }
    }
    //Stop at end of file
    fclose(img);
    fclose(card);
    return 0;
}