# image_processing.filters.c
Code is updated after 14 months with better knowledge and better programming skills. New goals has been set to turn this basic course project to bigger personal project.
---

Applying gauss, sobel and laplacian filters using convolution matrix

It is early stage of the project.

Things to be done:
  *optimize reading .pgm files
  *calculate the gaussian filter yourself
  *general optimization
  *there is problem with naming the file - sort it out
  
"Written for Image Processing Course."


Read .PGM files.

Apply Gaussian Filter for Noise Reduction                 -
Apply Sobel Filter for Edge Detection                     -   Use Convolution Matrix Method to apply filters

Apply Laplacian to the original image for Edge Detection  -

Write processed image to new .PGM file

Calculate the success of the filters in Edge Detection


** You need to use command line to run the program as photo_reader_V1.exe name_of_the_image

-----------------------------------------------------------------------------------------------
This program has developed in the beginning of third grade of Computer Engineering Bachelor Degree.
As it can be seen it is fairly basic program and it has too many mistakes. It will stay in my repository to allow people to see my progress.
New program is a 2 hour challenge for myself after 14 months.

- Program reads the directory and for each image it applies canny and laplacian filters then saves the images.
- All the filters can be calculated from scratch. It can generate any size of filters and apply it to the images.
- Whole program structure has changed. Easy use of functions in other applications too.
- Functions:
   * apply laplacian filter
   * apply sobel filter
   * apply gaussian filter
   * apply canny
   * apply convolution 
   * generate sobel filters Gx, Gy of any size
   * generate gaussian filter of any size and any sigma value
   * generate Laplacian of Gaussian filter of any size and any sigma value
   * image read from file with .pgm format
   * image write to file with .pgm format
   
Further Developement Goals:
   * programming more computer vision algorithms in c
   * programming functions to generate different filters
   * convert it to CUDA program
   * convert the program to a library ( main goal is to program a quite small version of opencv to understand the core of computer            vision)
   
