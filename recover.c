#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    char *inputuser = argv[1];
    //program accepts only one command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //open card
    FILE *card = fopen(inputuser, "r");
    
    //detects if the file opened
    if (card == NULL)
    {
        printf("Cannot open file\n");
        return 1;
    }
    
    //stores a byte of data
    typedef uint8_t BYTE;
    
    //size of the blocks
    BYTE block[512];
    
    //length of the jpeg file name
    char filename[8];
    
    //counter that will name the jpeg files
    int counter = 0;
    
    //pointer to the out file
    FILE *outimg = NULL;
    
    //read the card
    while (fread(block, sizeof(uint8_t), 512, card) == 512)
    {
        //looks for jpeg
        if ((block[0] == 0xff) && (block[1] == 0xd8) && (block[2] == 0xff) && (block[3] & 0xf0) == 0xe0)
        {
            //close the file if it is already open
            if (outimg != NULL)
            {
                fclose(outimg);
            }
            
            //creates a new jpeg and give rename it with the number of the counter
            sprintf(filename, "%03i.jpg", counter);
            outimg = fopen(filename, "w");
            counter++;
            
        }
        //writes data to the out file
        if (outimg != NULL)
        {
            fwrite(block, 512, 1, outimg);
        }
    }
    //closes the card
    fclose(card);
}
