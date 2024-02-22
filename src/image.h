#pragma once

#include <linux/limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum allocation_type { NO_ALLOCATION, SELF_ALLOCATED, STB_ALLOCATED };

typedef struct {
  int width;
  int height;
  int channels;
  size_t size;
  uint8_t *data;
  enum allocation_type allocation_;
} Image;

typedef struct {
  unsigned int avg_color;
  unsigned int avg_BW;
  unsigned int histogramm_BW[255];
  unsigned int histogramm_red[255];
  unsigned int histogramm_green[255];
  unsigned int histogramm_blue[255];
  char img_path[PATH_MAX];
} Data_all_t;

void Image_load(Image *img, const char *fname);
void Image_create(Image *img, int width, int height, int channels, bool zeroed);
void Image_save(const Image *img, const char *fname);
void Image_free(Image *img);
void Image_to_gray(const Image *orig, Image *gray);
void Image_to_sepia(const Image *orig, Image *sepia);
void Image_to_Data_t(const Image *img, Data_all_t *data);
void Image_to_sections(const Image *img, const Image **sections,
                       unsigned int sections_x, unsigned int sections_y);
