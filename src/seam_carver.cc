#include "seam_carver.hpp"

const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::GetEnergy(int row, int col) {
  int cr = -1, ccr = -1, cb = -1, ccb = -1, cg = -1, ccg = -1, rr = -1,
      rrr = -1, rb = -1, rrb = -1, rg = -1, rrg = -1;
  int en = 0;
  if (col == 0) {
    cr = (image_.GetPixel(row, width_ - 1).GetRed());
    cg = (image_.GetPixel(row, width_ - 1).GetGreen());
    cb = (image_.GetPixel(row, width_ - 1).GetBlue());
  }
  if (row == 0) {
    rr = (image_.GetPixel(height_ - 1, col).GetRed());
    rg = (image_.GetPixel(height_ - 1, col).GetGreen());
    rb = (image_.GetPixel(height_ - 1, col).GetBlue());
  }
  if (col == width_ - 1) {
    ccr = (image_.GetPixel(row, 0).GetRed());
    ccg = (image_.GetPixel(row, 0).GetGreen());
    ccb = (image_.GetPixel(row, 0).GetBlue());
  }
  if (row == height_ - 1) {
    rrr = (image_.GetPixel(0, col).GetRed());
    rrb = (image_.GetPixel(0, col).GetBlue());
    rrg = (image_.GetPixel(0, col).GetGreen());
  }
  cr = Getred(cr, row, col);
  cb = Getblue(cb, row, col);
  cg = Getgreen(cg, row, col);
  ccr = Ccr(ccr, row, col);
  ccb = Ccb(ccb, row, col);
  ccg = Ccg(ccg, row, col);
  rr = Rr(rr, row, col);
  rb = Rb(rb, row, col);
  rg = Rg(rg, row, col);
  rrr = Rrr(rrr, row, col);
  rrb = Rrb(rrb, row, col);
  rrg = Rrg(rrg, row, col);
  en = (cr - ccr) * (cr - ccr) + (cb - ccb) * (cb - ccb) +
       (cg - ccg) * (cg - ccg) + (rr - rrr) * (rr - rrr) +
       (rb - rrb) * (rb - rrb) + (rg - rrg) * (rg - rrg);
  return en;
}
int SeamCarver::Getred(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row, col - 1).GetRed());
    return a;
  }
  return a;
}
int SeamCarver::Getblue(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row, col - 1).GetBlue());
    return a;
  }
  return a;
}
int SeamCarver::Getgreen(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row, col - 1).GetGreen());
    return a;
  }
  return a;
}
int SeamCarver::Ccr(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row, col + 1).GetRed());
    return a;
  }
  return a;
}
int SeamCarver::Ccb(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row, col + 1).GetBlue());
    return a;
  }
  return a;
}
int SeamCarver::Ccg(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row, col + 1).GetGreen());
    return a;
  }
  return a;
}
int SeamCarver::Rr(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row - 1, col).GetRed());
    return a;
  }
  return a;
}
int SeamCarver::Rb(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row - 1, col).GetBlue());
    return a;
  }
  return a;
}
int SeamCarver::Rg(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row - 1, col).GetGreen());
    return a;
  }
  return a;
}
int SeamCarver::Rrr(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row + 1, col).GetRed());
    return a;
  }
  return a;
}
int SeamCarver::Rrb(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row + 1, col).GetBlue());
    return a;
  }
  return a;
}
int SeamCarver::Rrg(int a, int row, int col) {
  if (a == -1) {
    a = (image_.GetPixel(row + 1, col).GetGreen());
    return a;
  }
  return a;
}
int** SeamCarver::MakeArray() {
  int** array = new int*[height_];
  for (int i = 0; i < height_; i++) {
    array[i] = new int[width_];
  }
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      array[i][j] = GetEnergy(i, j);
    }
  }

  return array;
}
int* SeamCarver::GetHorizontalSeam() {
  int** array = SeamCarver::MakeArray();
  array = GetHorizontalSeam1(array);
  int min = array[0][0];
  int* hor_arr = new int[width_];
  hor_arr[0] = 0;
  for (int i = 0; i < height_; i++) {
    if (min > array[i][width_ - 1]) {
      min = array[i][width_ - 1];
      hor_arr[0] = i;
    }
  }
  hor_arr = GetHorizontalSeam2(hor_arr, array);
  return hor_arr;
}
int** SeamCarver::GetHorizontalSeam1(int** array) const {
  int num1 = 0;
  int num2 = 0;
  int num3 = 0;
  for (int i = width_ - 2; i > 0; i--) {
    for (int j = 0; j < height_; j++) {
      if (j == 0) {
        num2 = array[j][i + 1];
        num3 = array[j + 1][i + 1];
        if (num2 > num3) {
          array[j][i] = array[j + 1][i] + array[j][i];
        } else {
          array[j][i] = array[j][i + 1] + array[j][i];
        }
      } else if (j == height_ - 1) {
        num1 = array[i + 1][j - 1];
        num2 = array[i + 1][j];
        if (num1 > num2) {
          array[j][i] = array[j][i + 1] + array[j][i];
        } else {
          array[j][i] = array[j - 1][i + 1] + array[j][i];
        }
      } else {
        num1 = array[j - 1][i + 1];
        num2 = array[j][i + 1];
        num3 = array[j + 1][i + 1];
        int minimum = num1;
        if (minimum > num2) {
          minimum = num2;
        }
        if (minimum > num3) {
          minimum = num3;
        }
        array[j][i] += minimum;
      }
    }
  }
  return array;
}
int* SeamCarver::GetHorizontalSeam2(int* hor_arr, int** array) const {
  for (int i = 1; i < width_; i++) {
    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    if (hor_arr[i - 1] == 0) {
      num2 = array[0][i];
      num3 = array[1][i];
      if (num2 > num3) {
        hor_arr[i] = 1;
      } else {
        hor_arr[i] = 0;
      }
    } else if (hor_arr[i - 1] == height_ - 1) {
      num1 = array[height_ - 2][i];
      num2 = array[height_ - 1][i];
      if (num1 > num2) {
        hor_arr[i] = height_ - 1;
      } else {
        hor_arr[i] = height_ - 2;
      }
    } else {
      num1 = array[hor_arr[i - 1] - 1][i];
      num2 = array[hor_arr[i - 1]][i];
      num3 = array[hor_arr[i - 1] + 1][i];
      int minimum = num1;
      int min_idx = hor_arr[i - 1] - 1;
      if (minimum >= num2) {
        minimum = num2;
        min_idx = hor_arr[i - 1];
      }
      if (minimum > num3) {
        minimum = num3;
        min_idx = hor_arr[i];
      }
      hor_arr[i] = min_idx;
    }
  }
  return hor_arr;
}

int* SeamCarver::GetVerticalSeam() {
  int** array = SeamCarver::MakeArray();
  array = GetVerticalSeam1(array);

  int min = array[0][0];
  int* vert_arr = new int[height_];
  vert_arr[0] = 0;
  for (int i = 0; i < width_; i++) {
    if (min > array[0][i]) {
      min = array[0][i];
      vert_arr[0] = i;
    }
  }
  vert_arr = GetVerticalSeam2(vert_arr, array);
  return vert_arr;
}
int** SeamCarver::GetVerticalSeam1(int** array) const {
  int num1 = 0;
  int num2 = 0;
  int num3 = 0;
  for (int i = height_ - 2; i > 0; i--) {
    for (int j = 0; j < width_; j++) {
      if (j == 0) {
        num2 = array[i + 1][j];
        num3 = array[i + 1][j + 1];
        if (num2 > num3) {
          array[i][j] = array[i][j + 1] + array[i][j];
        } else {
          array[i][j] = array[i + 1][j] + array[i][j];
        }
      } else if (j == width_ - 1) {
        num1 = array[i + 1][j - 1];
        num2 = array[i + 1][j];
        if (num1 > num2) {
          array[i][j] = array[i + 1][j] + array[i][j];
        } else {
          array[i][j] = array[i + 1][j - 1] + array[i][j];
        }
      } else {
        num1 = array[i + 1][j - 1];
        num2 = array[i + 1][j];
        num3 = array[i + 1][j + 1];
        int minimum = num1;
        if (minimum > num2) {
          minimum = num2;
        }
        if (minimum > num3) {
          minimum = num3;
        }
        array[i][j] += minimum;
      }
    }
  }
  return array;
}

int* SeamCarver::GetVerticalSeam2(int* vert_arr, int** array) const {
  for (int i = 1; i < height_; i++) {
    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    if (vert_arr[i - 1] == 0) {
      num2 = array[i][0];
      num3 = array[i][1];
      if (num2 > num3) {
        vert_arr[i] = 1;
      } else {
        vert_arr[i] = 0;
      }
    } else if (vert_arr[i - 1] == width_ - 1) {
      num1 = array[i][width_ - 2];
      num2 = array[i][width_ - 1];
      if (num1 > num2) {
        vert_arr[i] = width_ - 1;
      } else {
        vert_arr[i] = width_ - 2;
      }
    } else {
      num1 = array[i][vert_arr[i - 1] - 1];
      num2 = array[i][vert_arr[i - 1]];
      num3 = array[i][vert_arr[i - 1] + 1];
      int minimum = num1;
      int min_idx = vert_arr[i - 1] - 1;
      if (minimum >= num2) {
        minimum = num2;
        min_idx = vert_arr[i - 1];
      }
      if (minimum > num3) {
        minimum = num3;
        min_idx = vert_arr[i];
      }
      vert_arr[i] = min_idx;
    }
  }
  return vert_arr;
}

void SeamCarver::RemoveHorizontalSeam() {
  int** array = SeamCarver::MakeArray();
  int* hor_arr = GetHorizontalSeam();
  image_.RemoveHoriz(/*array, */ hor_arr);
  delete[] array;
  array = nullptr;
  delete[] hor_arr;
  hor_arr = nullptr;
  height_ -= 1;
}

void SeamCarver::RemoveVerticalSeam() {
  int* vert_arr = GetVerticalSeam();
  image_.RemoveVert(vert_arr);
  width_ -= 1;
}

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

// int FindMinPathDynamicProgramming(int row, int col) {}