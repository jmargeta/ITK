/*==========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLevelSetNeighborhoodExtractor.txx
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
#ifndef _itkLevelSetNeighborhoodExtractor_txx
#define _itkLevelSetNeighborhoodExtractor_txx

#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

#include <algorithm>

namespace itk
{

/**
 *
 */
template <class TLevelSet>
LevelSetNeighborhoodExtractor<TLevelSet>
::LevelSetNeighborhoodExtractor()
{
  m_InputLevelSet = NULL;  
  m_LevelSetValue = 0.0;

  typedef typename LevelSetImageType::PixelType PixelType;
  m_LargeValue = NumericTraits<PixelType>::max();
  m_NodesUsed.resize( SetDimension );

  m_NarrowBanding = false;
  m_NarrowBandwidth = 12.0;
  m_InputNarrowBand = NULL;

}

/**
 *
 */
template <class TLevelSet>
void
LevelSetNeighborhoodExtractor<TLevelSet>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Input level set: " << m_InputLevelSet.GetPointer();
  os << std::endl;
  os << indent << "Level set value: " << m_LevelSetValue << std::endl;
  os << indent << "Narrow bandwidth: " << m_NarrowBandwidth;
  os << std::endl;
  os << indent << "Narrowbanding: " << m_NarrowBanding;
  os << indent << "Input narrow band: ";
  os << m_InputNarrowBand.GetPointer() << std::endl;
}

/**
 *
 */
template <class TLevelSet>
void
LevelSetNeighborhoodExtractor<TLevelSet>
::SetInputNarrowBand(
NodeContainer * ptr )
{
  if( m_InputNarrowBand != ptr )
    {
    m_InputNarrowBand = ptr;
    this->Modified();
    }
}


/**
 *
 */
template <class TLevelSet>
void
LevelSetNeighborhoodExtractor<TLevelSet>
::Locate()
{
  this->GenerateData();
}


/**
 *
 */
template <class TLevelSet>
void
LevelSetNeighborhoodExtractor<TLevelSet>
::Initialize()
{
  // create new emtpy points containers
  m_InsidePoints = NodeContainer::New();
  m_OutsidePoints = NodeContainer::New();

  typename TLevelSet::SizeType size =
    m_InputLevelSet->GetBufferedRegion().GetSize();

  for( int j = 0; j < SetDimension; j++ )
    {
    m_ImageSize[j] = (signed long) size[j];
    }

}

/**
 *
 */
template <class TLevelSet>
void
LevelSetNeighborhoodExtractor<TLevelSet>
::GenerateData()
{
  if( !m_InputLevelSet ) 
    { 
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "GenerateData" );
    err.SetDescription( "Input level set in NULL" );
    throw err;
    }

  this->Initialize();


  if( m_NarrowBanding )
    {
    this->GenerateDataNarrowBand();
    }
  else
    {
    this->GenerateDataFull();
    }

  itkDebugMacro(<< "No. inside points: " << m_InsidePoints->Size());
  itkDebugMacro(<< "No. outside points: " << m_OutsidePoints->Size());

}


/**
 *
 */
template <class TLevelSet>
void
LevelSetNeighborhoodExtractor<TLevelSet>
::GenerateDataFull()
{

  typedef ImageRegionIterator<LevelSetImageType> InputIterator;

  InputIterator inIt ( m_InputLevelSet,
                       m_InputLevelSet->GetBufferedRegion() );

  IndexType inputIndex;

  for( inIt = inIt.Begin(); !inIt.IsAtEnd(); ++inIt )
    {
    inputIndex = inIt.GetIndex();
    this->CalculateDistance( inputIndex );
    }

}

/**
 *
 */
template <class TLevelSet>
void
LevelSetNeighborhoodExtractor<TLevelSet>
::GenerateDataNarrowBand()
{
  if( !m_InputNarrowBand )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation("GenerateDataNarrowBand");
    err.SetDescription("InputNarrowBand has not been set" );
    throw err;
    }

  typename NodeContainer::ConstIterator pointsIter;
  typename NodeContainer::ConstIterator pointsEnd;

  pointsIter = m_InputNarrowBand->Begin();
  pointsEnd = m_InputNarrowBand->End();
  NodeType node;
  double maxValue = m_NarrowBandwidth / 2.0;

  for( ; pointsIter != pointsEnd; ++pointsIter )
    {
    node = pointsIter.Value();
    if( vnl_math_abs( node.GetValue() ) <= maxValue )
      {
      this->CalculateDistance( node.GetIndex() );
      }
    }

}

/**
 *
 */
template <class TLevelSet>
double
LevelSetNeighborhoodExtractor<TLevelSet>
::CalculateDistance(
IndexType& index)
{
  typename LevelSetImageType::PixelType centerValue;
  PixelType inputPixel;

  inputPixel = m_InputLevelSet->GetPixel( index );
  centerValue = (double) inputPixel ;
  centerValue -= m_LevelSetValue;

  NodeType centerNode;
  centerNode.SetIndex( index );

  if( centerValue == 0.0 )
    { 
    centerNode.SetValue( 0.0 );
    m_InsidePoints->InsertElement( m_InsidePoints->Size(), centerNode );
    return 0.0;
    }

  bool inside = ( centerValue <= 0.0 );

  IndexType neighIndex = index;
  typename LevelSetImageType::PixelType neighValue;
  NodeType  neighNode;
  double distance;

  // In each dimension, find the distance to the zero set
  // by linear interpolating along the grid line.
  for( int j = 0; j < SetDimension; j++ )
    {
    neighNode.SetValue( m_LargeValue );

    for( int s = -1; s < 2; s = s + 2 )
      {
      neighIndex[j] = index[j] + s;
      
      if( neighIndex[j] > m_ImageSize[j] - 1 || 
          neighIndex[j] < 0 )
        {
        continue;
        }
          
      inputPixel = m_InputLevelSet->GetPixel( neighIndex );
      neighValue = inputPixel;
      neighValue -= m_LevelSetValue;

      if( ( neighValue > 0 && inside ) ||
          ( neighValue < 0 && !inside ) )
        {
         distance = centerValue / ( centerValue - neighValue );

         if( neighNode.GetValue() > distance )
          {
            neighNode.SetValue( distance );
            neighNode.SetIndex( neighIndex );
          }
        }

      } // end one dim loop

    // put the minimum distance neighbor onto the heap
    m_NodesUsed[j] = neighNode;

    // reset neighIndex
    neighIndex[j] = index[j];

  } // end dimension loop

  // sort the neighbors according to distance
  std::sort( m_NodesUsed.begin(), m_NodesUsed.end() );

 // The final distance is given by the minimum distance to the plane
 // crossing formed by the zero set crossing points.
  distance = 0.0;
  for( int j = 0; j < SetDimension; j++ )
    {
    neighNode = m_NodesUsed[j];

    if( neighNode.GetValue() >= m_LargeValue )
      { 
      break;
      }

    distance += 1.0 / vnl_math_sqr( neighNode.GetValue() );
    }

  if( distance == 0.0 )
    {
    return m_LargeValue;
    }

  distance = vnl_math_sqrt( 1.0 / distance );
  centerNode.SetValue( distance );

  if( inside )
    {
    m_InsidePoints->InsertElement( m_InsidePoints->Size(), centerNode );
    }
  else
    {
    m_OutsidePoints->InsertElement( m_OutsidePoints->Size(), centerNode );
    }

  return distance;

}

} // namespace itk

#endif
