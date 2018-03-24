#ifndef ITKIMAGETYPE_H
#define ITKIMAGETYPE_H
/*
 *
 * define the medical iamge type and itk image to vtk image type
 */
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>

typedef signed short DicomPixelType;
const unsigned int DicomDimension = 3;

typedef itk::Image< DicomPixelType, DicomDimension > DicomImageType;
typedef itk::ImageFileReader< DicomImageType > DicomReaderType;
typedef itk::ImageToVTKImageFilter<DicomImageType>  ConnectorType;
#endif // ITKIMAGETYPE_H
