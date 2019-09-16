/*=========================================================================
 *
 *  Copyright SINAPSE: Scalable Informatics for Neuroscience, Processing and Software Engineering
 *            The University of Iowa
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __DemonsPreprocessor_h
#define __DemonsPreprocessor_h

#include "itkObject.h"

namespace itk
{
/** \class DemonsPreprocessor
 *
 * This component pre-processes the moving and fixed image before
 * registration.
 * If the fixed image dimensions are different from the moving image it will
 *    * resample the moving image to match the fixed image dimensions.
 * Histogram matching is done to solve the intensity mismatch problem.
 *
 * The preprocessor also called the skull-stripping filter itkBOBF
 * if an atlas and subject whole brain masks are specified.
 *
 * The preprocessing is activatived by method Execute().
 *
 * Inputs:
 *    - pointer to original fixed image
 *    - pointer original moving image
 *    - number of histogram levels
 *    - number of match points
 *
 * Outputs:
 *    - pointer to processed fixed image
 *    - pointer to processed moving image
 *    - the minimum value of original fixed image
 *    - the minimum value of original moving image
 *
 */
template <typename TInputImage, typename TOutputImage>
class DemonsPreprocessor : public Object
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(DemonsPreprocessor);

  /** Standard class type alias. */
  using Self = DemonsPreprocessor;
  using Superclass = Object;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Run-time type information (and related methods). */
  itkTypeMacro(DemonsPreprocessor, Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input Image Type. */
  using InputImageType = TInputImage;
  /** Output Image Type. */
  using OutputImageType = TOutputImage;

  /** Input image pixel type. */
  using InputPixelType = typename InputImageType::PixelType;
  using PixelType = typename OutputImageType::PixelType;
  using IndexType = typename OutputImageType::IndexType;
  using SizeType = typename OutputImageType::SizeType;

  /** Image dimension enumeration. */
  static constexpr unsigned int ImageDimension = TInputImage::ImageDimension;

  /** Set the input fixed image. */
  itkSetObjectMacro(InputFixedImage, InputImageType);

  /** Set the input moving image. */
  itkSetObjectMacro(InputMovingImage, InputImageType);

  /** Displacement field value type. */
  using FieldValueType = float;

  /** Displacement field pixel type. */
  using FieldPixelType = Vector<FieldValueType, Self::ImageDimension>;

  /** Displacement field type. */
  using TDisplacementField = Image<FieldPixelType, Self::ImageDimension>;

  /** Set the initial Displacement Field. */
  itkSetObjectMacro(InitialDisplacementField, TDisplacementField);
  itkGetModifiableObjectMacro(InitialDisplacementField, TDisplacementField);

  /** Set the number of histogram levels to use. */
  itkSetMacro(NumberOfHistogramLevels, unsigned long);

  /** Set the number of match points to use. */
  itkSetMacro(NumberOfMatchPoints, unsigned long);

  /** Method to execute the preprocessing. */
  virtual void
  Execute();

  /** Get the output fixed/moving image. */
  itkGetModifiableObjectMacro(OutputFixedImage, OutputImageType);
  itkGetModifiableObjectMacro(OutputMovingImage, OutputImageType);

  /** Get the output fixed/moving image. */
  itkGetModifiableObjectMacro(UnNormalizedMovingImage, OutputImageType);
  itkGetModifiableObjectMacro(UnNormalizedFixedImage, OutputImageType);

  /** Get minimum value of original fixed image. */
  itkGetMacro(FixedImageMinimum, InputPixelType);

  /** Get minimum value of original moving image. */
  itkGetMacro(MovingImageMinimum, InputPixelType);

  /* BOBF macros
   * Set Target Mask filename */
  itkSetStringMacro(FixedBinaryVolume);
  itkGetStringMacro(FixedBinaryVolume);

  /** Set Template Mask filename */
  itkSetStringMacro(MovingBinaryVolume);
  itkGetStringMacro(MovingBinaryVolume);

  /** Set/Get the lower threshold. The default is 0. */
  itkSetMacro(Lower, PixelType);
  itkGetMacro(Lower, PixelType);

  /** Set/Get the upper threshold. The default is 70 */
  itkSetMacro(Upper, PixelType);
  itkGetMacro(Upper, PixelType);

  itkSetMacro(DefaultPixelValue, PixelType);
  itkGetMacro(DefaultPixelValue, PixelType);

  itkSetMacro(MedianFilterSize, SizeType);
  itkGetMacro(MedianFilterSize, SizeType);

  /** Set the radius of the neighborhood used for a mask. */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of the neighborhood used to compute the median */
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Set the Seed of the neighborhood used for a mask. */
  itkSetMacro(Seed, IndexType);
  /** Get the radius of the neighborhood used to compute the median */
  itkGetConstReferenceMacro(Seed, IndexType);

  /**Set Debug mode*/
  itkSetMacro(OutDebug, bool);
  itkGetConstMacro(OutDebug, bool);

  /**Set histogram matching*/
  itkSetMacro(UseHistogramMatching, bool);
  itkGetConstMacro(UseHistogramMatching, bool);

protected:
  DemonsPreprocessor();
  ~DemonsPreprocessor() override {}

private:
  typename InputImageType::Pointer     m_InputFixedImage;
  typename InputImageType::Pointer     m_InputMovingImage;
  typename OutputImageType::Pointer    m_OutputFixedImage;
  typename OutputImageType::Pointer    m_OutputMovingImage;
  typename OutputImageType::Pointer    m_UnNormalizedMovingImage;
  typename OutputImageType::Pointer    m_UnNormalizedFixedImage;
  typename TDisplacementField::Pointer m_InitialDisplacementField;

  unsigned long m_NumberOfHistogramLevels;
  unsigned long m_NumberOfMatchPoints;

  InputPixelType m_FixedImageMinimum;
  InputPixelType m_MovingImageMinimum;

  std::string m_FixedBinaryVolume;
  std::string m_MovingBinaryVolume;
  IndexType   m_Seed;
  PixelType   m_Lower;
  PixelType   m_Upper;
  PixelType   m_DefaultPixelValue;
  SizeType    m_Radius;
  bool        m_OutDebug;
  SizeType    m_MedianFilterSize;
  using OutputImagePointer = typename OutputImageType::Pointer;
  using InputImagePointer = typename InputImageType::Pointer;

  bool m_UseHistogramMatching;

  /*MakeBOBF function takes in a brain image and a whole brain mask and strips
   * the skull of the image.*/
  OutputImagePointer
  MakeBOBFImage(OutputImagePointer input, std::string MaskName);
};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "DemonsPreprocessor.hxx"
#endif

#endif // _DemonsPreprocessor_h
