/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMultiResolutionImagePyramid.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __itkMultiResolutionImagePyramid_h
#define __itkMultiResolutionImagePyramid_h

#include "itkImageToImageFilter.h"
#include "vnl/vnl_matrix.h"

namespace itk
{

/**
 * \class MultiResolutionImagePyramid
 * \brief Framework for creating images in a multi-resolution
 * pyramid.
 *
 * MultiResolutionImagePyramid provides a generic framework to
 * to create a image pryamid according to a users defined 
 * multi-resolution schedule.
 * 
 * The multi-resolution schedule is specified in terms for
 * shrink factors at each multi-resolution level for each dimension.
 * 
 * A user can either use the default schedules or specify 
 * each factor in the schedules directly.
 *
 * The schedule is stored as an unsigned int matrix.
 * An element of the table can be access via the double bracket
 * notation: table[resLevel][dimension]
 *
 * For example:
 *   8 4 4
 *   4 4 2
 *
 * is a schedule for two computation level. In the first (coarest)
 * level the image is reduce by a factor of 8 in the column dimension,
 * factor of 4 in the row dimension and factor of 4 in the slice dimension.
 * In the second level, the image is reduce by a factor of 4 in the column
 * dimension, 4 is the row dimension and 2 in the slice dimension.
 * 
 * The method SetNumberOfLevels() set the number of
 * computation levels in the pyramid. This method will
 * allocate memory for the multi-resolution schedule table.
 * This method generates defaults tables with the starting
 * shrink factor for all dimension set to  NumberOfLevel - 1)^2. 
 * All factors are halved for all subsequent levels. 
 * For example if the number of levels was set to 4, the default table is:
 *
 * 8 8 8
 * 4 4 4
 * 2 2 2
 * 1 1 1
 *
 * The user can get a copy of the schedule via GetSchedule()
 * They may make alteration and reset it using SetSchedule().
 *
 * A user can create a default table by specifying the starting
 * shrink factors via methods SetStartingShrinkFactors()
 * The factors for subsequent level is generated by 
 * halving the factor or setting to one, depending on which is larger.
 *
 * For example, for 4 levels and starting factors of 8,8,4
 * the default table would be:
 *
 * 8 8 4
 * 4 4 2
 * 2 2 1
 * 1 1 1
 *
 * To generate the output image, Gaussian smoothing is first performed using
 * RecursiveGaussianImageFilter and standard deviation of (shrink factor / 2). 
 * The smoothed image is then downsampled using ShrinkImageFiter.
 *
 * This class is templated over the input image type and the output image type.
 *
 * \todo
 * Smoothing has been skipped for the time being until RecursiveGaussianImageFilter
 * has been fixed. The filter currently zero pads the image which causes large 
 * gradient at the image boundary causes registration algorithms to be unstable. 
 *
 * \ingroup ImageRegistration
 *
 */
template <
class TInputImage, 
class TOutputImage
>
class ITK_EXPORT MultiResolutionImagePyramid : 
  public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /**
   * Standard "Self" typedef
   */
  typedef MultiResolutionImagePyramid  Self;

  /**
   * Standard "Superclass" typedef
   */
  typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;

  /**
   * SmartPointer typedef support
   */
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /**
   * Method for creation through the object factory
   */
  itkNewMacro(Self);

  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(MultiResolutionImagePyramid, ImageToImageFilter);

  /**
   * ScheduleType typedef support
   */
  typedef vnl_matrix<unsigned int>  ScheduleType;

  /**
   * ImageDimension enumeration
   */
  enum{ ImageDimension = TInputImage::ImageDimension };

  /**
   * Inherit types from Superclass
   */
  typedef typename Superclass::InputImageType InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::InputImagePointer InputImagePointer;
  typedef typename Superclass::OutputImagePointer OutputImagePointer;

  /**
   * Set the number of multi-resolution levels. The matrix 
   * containing the schedule will be resized accordingly.
   * The schedule is populated with default values. 
   * At the coarset (0) level, the shrink factors are set
   * (nlevel - 1)^2 for all dimension. These shrink factors
   * are halved for subsequent levels.
   * The number of levels is clamped to a minimum value of 1.
   * All shrink factors are also clamped to a minimum value of 1.
   */
  void SetNumberOfLevels(unsigned int num);

  /**
   * Get the number of multi-resolution levels
   */
  itkGetMacro(NumberOfLevels, unsigned int);

  /**
   * Get the multi-resolution schedule
   */
  itkGetConstReferenceMacro(Schedule, ScheduleType);

  /**
   * Set the current resolution level. When this filter is updated,
   * the result is an image corresponding to the shrink factors
   * of the current level. The current level is clamped to be between
   * 0 and (NumberOfLevels - 1)
   */
  void SetCurrentLevel( unsigned int level );

  /**
   * Get the current resolution level.
   */
  itkGetMacro(CurrentLevel, unsigned int);

  /**
   * Set the starting shrink factor for the coarset (0) resolution
   * level. The schedule is then populated with defaults values
   * obtained by halving the factors at the previous level.
   * All shrink factors are clamped to a minimum value of 1.
   */
  void SetStartingShrinkFactors( unsigned int factor );
  void SetStartingShrinkFactors( unsigned int* factors );

  /**
   * MultiResolutionPyramid produces an image which is a different resolution and
   * with a different pixel spacing than its input image.  As such,
   * MultiResolutionPyramid needs to provide an implementation for
   * UpdateOutputInformation() in order to inform the pipeline execution model.
   * The original documentation of this method is below.
   *
   * \sa ProcessObject::UpdateOutputInformaton()
   */
  virtual void UpdateOutputInformation();

  /**
   * MultiResolutionPyramid requires all of the input to be in the
   * buffer. As such, MultiResolutionPyramid needs to provide an
   * implemenation for GenerateInputRequestedRegion().
   * The original documenation of this method is below.
   *
   * \sa ProcessObject::GenerateInputRequestedRegion()
   */
  virtual void GenerateInputRequestedRegion();

  /**
   * MultiResolutionPyramid requires all of the output to be in the
   * buffer. As such, MultiResolutionPyramid needs to provide an
   * implemenation for EnlargeOutputRequestedRegion();
   * The original documenation of this method is below.
   *
   * \sa ProcessObject::EnlargeOutputRequestedRegion();
   */
  virtual void EnlargeOutputRequestedRegion(DataObject * output);


protected:
  MultiResolutionImagePyramid();
  ~MultiResolutionImagePyramid() {};
  MultiResolutionImagePyramid(const Self&) {};
  void operator=(const Self&) {};
  void PrintSelf(std::ostream&os, Indent indent) const;

  void GenerateData();

private:
  unsigned int    m_NumberOfLevels;
  unsigned int    m_CurrentLevel;
  ScheduleType    m_Schedule;

  /**
   * Copies the input image region to output image region.
   * Useful for copying the output of the internal mini-pipeline to the
   * actual output of this filter
   */
  static void ImageRegionCopy(InputImageType *, OutputImageType *);
  static void ImageRegionCopy2(OutputImageType *, OutputImageType * );

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMultiResolutionImagePyramid.txx"
#endif

#endif


