## Bmpcreator
A simple command line tool to generate 24-bit BMP files. 

I made this mainly to help me understand the Bitmap file format for another project, so its not very exciting or even useful, but here it is anyway.

## Note
The output file name is "generated_image.bmp" and there is no check for an existing file with the same name, so be careful not to overwrite anything important!


## Usage  
    ./bmpcreator [width] [height] [color: RRGGBB]


For example, to create a completely RED image...

    ./bmpcreator 640 480 ff0000
