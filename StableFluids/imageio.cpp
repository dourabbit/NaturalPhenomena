/* LIBRARY:   ImageIO Library v0.13
 * 
 * FILE:      imageio.cpp
 *
 * COPYRIGHT: Copyright (c) 2003-2004 Adrien Treuille
 * 
 * AUTHORS:   Adrien Treuille (treuille [AT] cs [DOT] washington [DOT] edu)
 *
 * LICENSE:   The ImageIO Library is supplied "AS IS".  The Authors
 *            disclaim all warranties, expressed or implied,
 *            including, without limitation, the warranties of
 *            merchantability and of fitness for any purpose. 
 *            The Authors assume no liability for direct, indirect,
 *            incidental, special, exemplary, or consequential
 *            damages, which may result from the use of the ImageIO
 *            Library, even if advised of the possibility of such
 *            damage.
 *
 *            Permission is hereby granted to use, copy, modify,
 *            and distribute this source code, or portions hereof,
 *            for any purpose, without fee, for non-commercial
 *            purposes. All rights reserved for commercial uses of
 *            this source code.
 */

#include "imageio.h"

// ***** generic internal functions ***** //

// Call this function if there was an error.
unsigned char *_loadImgError(int *width, int *height) {
  *width = *height = -1;
  return 0;
}

// Returns true iff the string s ends with postfix
bool _endsWith(char *s, const char *postfix) {
  int sLen = strlen(s);
  int postfixLen = strlen(postfix);
  if (postfixLen > sLen)
    return false;
  else
    return (!strcmp(s + sLen - postfixLen, postfix));
}

// ***** png related internal functions ***** //

unsigned char *_loadImageRGBApng(char *fileName, int *width, int *height) {
  // open the file
  FILE *fp = fopen(fileName, "rb");
  if (!fp)
    return _loadImgError(width, height);

  // read the header
  const int HEADER_LENGTH = 8;
  png_byte header[HEADER_LENGTH];
  fread(header, 1, HEADER_LENGTH, fp);
  if (png_sig_cmp(header, 0, HEADER_LENGTH))
    return _loadImgError(width, height);

  // try to create the loading structures
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png_ptr) {
    fclose(fp);
    return _loadImgError(width, height);
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, (png_infopp) 0, (png_infopp) 0);
    fclose(fp);
    return _loadImgError(width, height);
  }

  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info) {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);
    fclose(fp);
    return _loadImgError(width, height);
  }

  if (setjmp(png_ptr->jmpbuf)) {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return _loadImgError(width, height);
  }

  // start the io
  png_init_io(png_ptr, fp);

  // indicate that we have already read some of the hearder
  png_set_sig_bytes(png_ptr, HEADER_LENGTH);

  // read the image info, get some info
  png_read_info(png_ptr, info_ptr);
  *width = png_get_image_width(png_ptr, info_ptr);
  *height = png_get_image_height(png_ptr, info_ptr);
  int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  png_byte color_type = png_get_color_type(png_ptr, info_ptr);
  
  // force the image into RGBA, 8 bits per channel
  if (color_type != PNG_COLOR_TYPE_RGBA)
    png_set_expand(png_ptr);
  if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png_ptr);
  if (bit_depth < 8)
    png_set_packing(png_ptr);
  else if (bit_depth == 16)
    png_set_strip_16(png_ptr);
  if (color_type != PNG_COLOR_TYPE_RGBA)
    png_set_filler(png_ptr, 255, PNG_FILLER_AFTER);  
  png_read_update_info(png_ptr, info_ptr);

  // make sure we're actually in rgba mode
  if (png_get_rowbytes(png_ptr, info_ptr) != ((*width) * 4)) {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return _loadImgError(width, height);
  }

  // finally, read the file
  unsigned char *buffer = (unsigned char *) malloc((*width) * (*height) * 4);
  png_bytep *row_pointers = new png_bytep[*height];
  for (int y = 0 ; y < (*height) ; y++)
    row_pointers[y] = (png_byte *) (buffer + ((*height) - 1 - y) * (*width) * 4);
  png_read_rows(png_ptr, row_pointers, 0, (long unsigned int) (*height));

  // deallocate memory and return
  fclose(fp);
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  delete row_pointers;
  return buffer;
}

bool _saveImageRGBApng(char *fileName, unsigned char *buffer, int width, int height) {
  // open the file
  FILE *fp = fopen(fileName, "wb");
  if (!fp)
    return false;

  // create the needed data structures
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png_ptr) {
    fclose(fp);
    return false;
  }
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    fclose(fp);
    png_destroy_write_struct(&png_ptr, (png_infopp) 0);
    return false;
  }

  // do the setjmp thingy
  if (setjmp(png_ptr->jmpbuf)) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return false;
  }

  // set up the io
  png_init_io(png_ptr, fp);
  
  // write the header
  png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, 
	       PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png_ptr, info_ptr);
  png_set_flush(png_ptr, 10);

  // write the image
  png_bytep *row_pointers = new png_bytep[height];
  for (int y = 0 ; y < height ; y++)
    row_pointers[y] = (png_byte *) (buffer + (height - 1 - y) * width * 4);
  png_write_image(png_ptr, row_pointers);
  png_write_end(png_ptr, info_ptr);
  
  // clean up memory, and finish
  fclose(fp);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  delete row_pointers;
  return true;
}

// ***** external functions ***** //

// Sets tbe width and height to the appropriate values and mallocs
// a char *buffer loading up the values in row-major, RGBA format.
// The memory associated with the buffer can be deallocated with free().
// If there was an error reading file, then 0 is returned, and
// width = height = -1.
unsigned char *loadImageRGBA(char *fileName, int *width, int *height) {
	if (_endsWith(fileName, ".png"))
		return _loadImageRGBApng(fileName, width, height);
	else
		return _loadImgError(width, height);
}

// Saves image given by buffer with specicified with and height
// to the given file name, returns true on success, false otherwise.
// The image format is RGBA.
bool saveImageRGBA(char *fileName, unsigned char *buffer, int width, int height) {
	if (_endsWith(fileName, ".png"))
		return _saveImageRGBApng(fileName, buffer, width, height);
	else
		return false;
}
