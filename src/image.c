#include "image.h"
#include "utils.h"
#include <cstdlib>
#include <math.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

void Image_load(Image *img, const char *fname) {
  if ((img->data = stbi_load(fname, &img->width, &img->height, &img->channels,
                             0)) != NULL) {
    img->size = img->width * img->height * img->channels;
    img->allocation_ = STB_ALLOCATED;
  }
}

void Image_create(Image *img, int width, int height, int channels,
                  bool zeroed) {
  size_t size = width * height * channels;
  if (zeroed) {
    img->data = calloc(size, 1);
  } else {
    img->data = malloc(size);
  }

  if (img->data != NULL) {
    img->width = width;
    img->height = height;
    img->size = size;
    img->channels = channels;
    img->allocation_ = SELF_ALLOCATED;
  }
}

void Image_save(const Image *img, const char *fname) {
  // Check if the file name ends in one of the .jpg/.JPG/.jpeg/.JPEG or
  // .png/.PNG
  if (str_ends_in(fname, ".jpg") || str_ends_in(fname, ".JPG") ||
      str_ends_in(fname, ".jpeg") || str_ends_in(fname, ".JPEG")) {
    stbi_write_jpg(fname, img->width, img->height, img->channels, img->data,
                   100);
  } else if (str_ends_in(fname, ".png") || str_ends_in(fname, ".PNG")) {
    stbi_write_png(fname, img->width, img->height, img->channels, img->data,
                   img->width * img->channels);
  } else {
    ON_ERROR_EXIT(false, "");
  }
}

void Image_free(Image *img) {
  if (img->allocation_ != NO_ALLOCATION && img->data != NULL) {
    if (img->allocation_ == STB_ALLOCATED) {
      stbi_image_free(img->data);
    } else {
      free(img->data);
    }
    img->data = NULL;
    img->width = 0;
    img->height = 0;
    img->size = 0;
    img->allocation_ = NO_ALLOCATION;
  }
}

void Image_to_gray(const Image *orig, Image *gray) {
  ON_ERROR_EXIT(!(orig->allocation_ != NO_ALLOCATION && orig->channels >= 3),
                "The input image must have at least 3 channels.");
  int channels = orig->channels == 4 ? 2 : 1;
  Image_create(gray, orig->width, orig->height, channels, false);
  ON_ERROR_EXIT(gray->data == NULL, "Error in creating the image");

  for (unsigned char *p = orig->data, *pg = gray->data;
       p != orig->data + orig->size;
       p += orig->channels, pg += gray->channels) {
    *pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
    if (orig->channels == 4) {
      *(pg + 1) = *(p + 3);
    }
  }
}

void Image_to_sepia(const Image *orig, Image *sepia) {
  ON_ERROR_EXIT(!(orig->allocation_ != NO_ALLOCATION && orig->channels >= 3),
                "The input image must have at least 3 channels.");
  Image_create(sepia, orig->width, orig->height, orig->channels, false);
  ON_ERROR_EXIT(sepia->data == NULL, "Error in creating the image");

  // Sepia filter coefficients from
  // https://stackoverflow.com/questions/1061093/how-is-a-sepia-tone-created
  for (unsigned char *p = orig->data, *pg = sepia->data;
       p != orig->data + orig->size;
       p += orig->channels, pg += sepia->channels) {
    *pg = (uint8_t)fmin(0.393 * *p + 0.769 * *(p + 1) + 0.189 * *(p + 2),
                        255.0); // red
    *(pg + 1) = (uint8_t)fmin(0.349 * *p + 0.686 * *(p + 1) + 0.168 * *(p + 2),
                              255.0); // green
    *(pg + 2) = (uint8_t)fmin(0.272 * *p + 0.534 * *(p + 1) + 0.131 * *(p + 2),
                              255.0); // blue
    if (orig->channels == 4) {
      *(pg + 3) = *(p + 3);
    }
  }
}

void Image_to_Data_t(const Image *img, Data_all_t *data) {
  ON_ERROR_EXIT(!(img->allocation_ != NO_ALLOCATION),
                "The input image is not allocated");

  unsigned int red_bin = 0;
  unsigned int green_bin = 0;
  unsigned int blue_bin = 0;
  unsigned int BW_bin = 0;

  for (unsigned int i = 0; i < img->size; i += img->channels) {
    unsigned char pixel_R = img->data[i];
    unsigned char pixel_G = img->data[i + 1];
    unsigned char pixel_B = img->data[i + 2];

    data->histogramm_red[pixel_R]++;
    data->histogramm_green[pixel_G]++;
    data->histogramm_blue[pixel_B]++;

    unsigned char pixel_BW =
        (0.3 * pixel_R) + (0.59 * pixel_G) + (0.11 * pixel_B);
    data->histogramm_BW[pixel_BW]++;
  }

  for (int i = 0; i < 255; i++) {
    red_bin += data->histogramm_red[i];
    green_bin += data->histogramm_green[i];
    blue_bin += data->histogramm_blue[i];
    BW_bin += data->histogramm_BW[i];
  }

  red_bin /= img->size;
  green_bin /= img->size;
  blue_bin /= img->size;

  unsigned int rgb = red_bin;
  rgb = (rgb << 8) + green_bin;
  rgb = (rgb << 8) + blue_bin;

  data->avg_color = rgb;
  data->avg_BW = BW_bin / img->size;
}

void Image_to_sections(const Image *img, Image **sections,
                       unsigned int sections_x, unsigned int sections_y) {

  ON_ERROR_EXIT(!(img->allocation_ != NO_ALLOCATION),
                "The input image is not allocated");

  // calculate the average size of one split
  int split_size_x = 0;
  int split_size_y = 0;
  if (sections_x > 0)
    split_size_x = img->width / sections_x;
  if (sections_y > 0)
    split_size_y = img->height / sections_y;

  // calculate the rest since we use integer instead of float
  int rest_x = img->width - (split_size_x * sections_x);
  int rest_y = img->height - (split_size_y * sections_y);

  // initialise arrays for the redistribution of pixels
  int *sections_x_size = malloc(sizeof(int) * sections_x);
  int *sections_y_size = malloc(sizeof(int) * sections_y);

  // create arrays to use for creating unique indicies
  int *random_indices_x = malloc(sizeof(int) * sections_x);
  int *random_indices_y = malloc(sizeof(int) * sections_y);
  for (int i = 0; i < sections_x; i++) {
    random_indices_x[i] = i;
    sections_x_size[i] = split_size_x;
  }
  for (int i = 0; i < sections_y; i++) {
    random_indices_y[i] = i;
    sections_y_size[i] = split_size_y;
  }

  // randomize indicies
  shuffle_array(random_indices_x, sections_x);
  shuffle_array(random_indices_y, sections_y);

  // use the randomizes indicies to increase selected sections
  for (int i = 0; i < rest_x; i++) {
    sections_x_size[random_indices_x[i]]++;
  }
  for (int i = 0; i < rest_y; i++) {
    sections_y_size[random_indices_y[i]]++;
  }

  // free unused space
  free(random_indices_x);
  free(random_indices_y);

  // split the input image into multiple sections
  for (int i = 0; i < sections_y; i++) {
    int start_x = 0;
    for (int j = 0; j < sections_x; j++) {
      int start_y = 0;

      Image *curr_section = &sections[j][i];
      curr_section->height = sections_y_size[i];
      curr_section->width = sections_x_size[j];
      curr_section->channels = img->channels;
      curr_section->size = curr_section->height * curr_section->width;
      curr_section->data =
          malloc(sizeof(char) * curr_section->channels * curr_section->size);

      for (int y = 0; y < sections_y_size[i]; y++) {
        for (int x = 0; x < sections_x_size[j]; x++) {
          curr_section->data[4 * (y * sections_x_size[j] + x)] =
              img->data[4 *
                        ((y + start_y) * sections_x_size[j] + (x + start_x))];
        }
      }
    }
  }
}
