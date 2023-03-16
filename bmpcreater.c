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

#define SIZE_BMP_HEADER 14
#define SIZE_DIB_HEADER 40
#define SIZE_PIXEL 3



// BMP header structure
typedef struct {
  uint8_t   signature[2];
  uint32_t  filesize;
  uint16_t  reserved1;
  uint16_t  reserved2;
  uint32_t  offset;
} BmpFileHeader;

// DIB header structure
typedef struct {
  uint32_t  size;
  uint32_t  width;
  uint32_t  height;
  uint16_t  planes;
  uint16_t  bits;
  uint32_t  compression;
  uint32_t  imagesize;
  uint32_t  xresolution;
  uint32_t  yresolution;
  uint32_t  ncolors;
  uint32_t  importantcolors; 
} DibHeader;

// BMP pixel structure
typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} Pixel;

// BMP complete file structure
typedef struct {
  BmpFileHeader  header;
  DibHeader      info;
  Pixel         *data;
}  BmpImage;


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

void bmp_create_header(BmpImage *image, int width, int height, int bits)
{
  image->header.signature[0] = 'B';
  image->header.signature[1] = 'M';
  image->header.reserved1 = 0;
  image->header.reserved2 = 0;
  image->header.offset = SIZE_BMP_HEADER + SIZE_DIB_HEADER;

  image->info.size = SIZE_DIB_HEADER;
  image->info.width = width;
  image->info.height = height;
  image->info.planes = 1;
  image->info.bits = 24;
  image->info.compression = 0;
  image->info.imagesize = (width * height) * SIZE_PIXEL;
  image->info.xresolution = 0;
  image->info.yresolution = 0;
  image->info.ncolors = 0;
  image->info.importantcolors = 0;

  // Now, compute the full file size 
  image->header.filesize = image->header.offset + image->info.imagesize;
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

  // Create a new file
  FILE *file = fopen(fileName, "wb");
  if (!file) {
    printf("Error: could not create file '%s'\n", fileName);
    return 1;
  }

  // Write the header (manually)
  fwrite( &image.header.signature, 2, 1, file);
  fwrite( &image.header.filesize, 4, 1, file);
  fwrite( &image.header.reserved1, 2, 1, file);
  fwrite( &image.header.reserved2, 2, 1, file);
  fwrite( &image.header.offset, 4, 1, file);

  // Write info (manually)
  fwrite( &image.info.size, 4, 1, file);
  fwrite( &image.info.width, 4, 1, file);
  fwrite( &image.info.height, 4, 1, file);
  fwrite( &image.info.planes, 2, 1, file);
  fwrite( &image.info.bits, 2, 1, file);
  fwrite( &image.info.compression, 4, 1, file);
  fwrite( &image.info.imagesize, 4, 1, file);
  fwrite( &image.info.xresolution, 4, 1, file);
  fwrite( &image.info.yresolution, 4, 1, file);
  fwrite( &image.info.ncolors, 4, 1, file);
  fwrite( &image.info.importantcolors, 4, 1, file);

  fseek(file, image.header.offset, SEEK_SET);


  // Create one pixel to paint the entire image
  Pixel pixel;
  pixel.b = color & 0xFF;
  pixel.g = (color >> 8) & 0xFF;
  pixel.r = (color >> 16) & 0xFF;
  
  
  
  // Create an array of pixels the same size as the image
  size_t numPixels = image.info.width * image.info.height;
  Pixel *pixelArray = malloc(SIZE_PIXEL * numPixels);
  if (pixelArray == NULL) {
    printf("Error: could not allocate memory for pixel array\n");
    return 1;
  }

  // Fill the pixel array
  size_t i;
  for (i = 0; i < numPixels; i++) {
    pixelArray[i] = pixel;
  }

  // Write the pixel array
  fwrite(pixelArray, SIZE_PIXEL, numPixels, file);
  free(pixelArray);

  fclose(file);

  return 0;
}