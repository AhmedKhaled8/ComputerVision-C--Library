#include<iostream>
#include<fstream>
#include"bmp.h"
using namespace std;

struct imagefile
{
  BITMAPFILEHEADER bf;
  BITMAPINFOHEADER bi;
  RGBTRIPLE ** image;
};

imagefile read_bmp(string filepath)
{
  cout<<"Attempting to open file"<<endl;
  ifstream file;
  file.open(filepath, ios::in | ios::binary);
  if(!file)
  {
    cout<< "Could not open file."<<endl;
    // reutn unintiallized variable
    imagefile imgfile1;
    return imgfile1;
  }
  cout<< "File opened"<<endl;

  // Read BITMAPFILEHEADER
  BITMAPFILEHEADER bf;

  file.read((char *) &bf, sizeof(BITMAPFILEHEADER));


  // Read BITMAPINFOHEADER
  BITMAPINFOHEADER bi;

  file.read((char *) &bi, sizeof(BITMAPINFOHEADER));

  if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
    bi.biBitCount != 24 || bi.biCompression != 0)
    {
      file.close();
      cout<<"unsupported file format"<<endl;
      imagefile imgfile2;
      return imgfile2;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    RGBTRIPLE **image = new RGBTRIPLE*[height];
    for(int i = 0;i< height; ++i)
    {
      image[i] = new RGBTRIPLE[width];
    }

    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    for (int i = 0; i < height; ++i)
    {
        // Read row into pixel array
        for (int k = 0; k < width; ++k)
        {
          file.read((char *) &image[i][k], sizeof(RGBTRIPLE));
        }
        // Skip over padding
        file.seekg(padding, ios_base::cur);
    }

    file.close();
    imagefile returnfile;

    returnfile.bf = bf;
    returnfile.bi = bi;
    returnfile.image = image;

    return returnfile;
  }

int save_bmp(imagefile &imgfile, string filepath)
{
  ofstream file;
  file.open(filepath, ios::out | ios::binary);
  if(!file)
  {
    cout<<"Could not create file"<<endl;
    return 1;
  }
  cout<<"File created, attempring to write"<<endl;

  int height = abs(imgfile.bi.biHeight);
  int width = imgfile.bi.biWidth;

  int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

  file.write((char *) &imgfile.bf, sizeof(BITMAPFILEHEADER));
  file.write((char *) &imgfile.bi, sizeof(BITMAPINFOHEADER));
  for(int i = 0; i < height; ++i)
  {
    for (int k = 0; k < width; ++k)
    {
      file.write((char *) &imgfile.image[i][k], sizeof(RGBTRIPLE));
    }
    for (int m = 0; m < padding; ++m)
    {
      file.write((char *) 0x00, sizeof(char));
    }
  }
  cout<<"File created successfully"<<endl;

  file.close();

  return 0;


}
