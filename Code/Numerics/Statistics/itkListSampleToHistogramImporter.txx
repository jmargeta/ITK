/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkListSampleToHistogramImporter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkListSampleToHistogramImporter_txx
#define __itkListSampleToHistogramImporter_txx

namespace itk{
  namespace Statistics{

template< class TListSample, class THistogram >
ListSampleToHistogramImporter< TListSample, THistogram >
::ListSampleToHistogramImporter()
{
}

template< class TListSample, class THistogram >
void
ListSampleToHistogramImporter< TListSample, THistogram >
::Run()
{
  typename TListSample::Iterator iter = m_List->Begin() ;
  typename TListSample::Iterator last = m_List->End() ;
  typename THistogram::InstanceIdentifier id ;
  typename THistogram::IndexType index ;
  while (iter != last)
    {
      index = m_Histogram->GetIndex(iter.GetMeasurementVector()) ;
      if (!m_Histogram->IsIndexOutOfBound(index))
        {
          // if the measurement vector is out of bound then
          // the GetIndex method returns index with the sizes of each dimension
          // and doesn't increase the frequency
          id = m_Histogram->GetInstanceIdentifier(index) ;
          m_Histogram->IncreaseFrequency(id, 1) ;
        }
      ++iter ;
    }
}

  } // end of namespace Statistics 
} // end of namespace itk 

#endif
