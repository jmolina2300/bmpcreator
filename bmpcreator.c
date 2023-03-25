/**
 * @file bmpcreator.c
 * @author Jan Molina
 * @brief  Basic BMP generator
 * 
 * @date 2023-03-05
 * 
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "tinybmp.h"


// Convert a hex string to an integer
int xtoi(const char *hex)
{
  int num = 0;
  int digit;
  while ( *hex != '\0') 
  {
    num <<= 4;
    if (isdigit( *hex)) {
      digit = *hex - '0';
    } else if (isxdigit( *hex)) {
      digit = toupper( *hex) - 'A' + 10;
    } else {
      return -1;
    }
    num += digit;
    hex++;
  }
  return num;
}


int main(int argc, char * argv[]) 
{
  // Check arguments
  if (argc < 3) {
    printf("Usage:  ./bmpcreator [width] [height] [color: RRGGBB]\n\n\n");
    printf("For example, to create a RED image\n\n");
    printf("        ./bmpcreator 640 480 ff0000 \n\n");
    return 1;
  }
  char *fileName = "generated_image.bmp";

  size_t width = atoi(argv[1]);
  size_t height = atoi(argv[2]);
  int color = xtoi(argv[3]);

  
  // Create and fill a bitmap structure
  BmpImage image;
  bmp_create_header(&image, width, height, 24);
  

  printf("Creating BMP with...\n\n");
  printf("        Width: %d\n"
         "       Height: %d\n"
         "   Num Pixels: %d\n"
         "    File Size: %d bytes\n\n",

    image.info.width,
    image.info.height,
    image.info.width * image.info.height,
    image.header.filesize
     
  );

  bmp_create_canvas(&image, color);

  // Write the bitmap to a file
  bmp_write_to_file(&image, fileName);
  bmp_destroy_image(&image);

  return 0;
}