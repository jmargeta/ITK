/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMembershipSampleGenerator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMembershipSampleGenerator_txx
#define __itkMembershipSampleGenerator_txx

namespace itk{
  namespace Statistics{

template< class TInputSample, class TClassMaskSample >
MembershipSampleGenerator< TInputSample, TClassMaskSample >
::MembershipSampleGenerator()
{
}

template< class TInputSample, class TClassMaskSample >
void
MembershipSampleGenerator< TInputSample, TClassMaskSample >
::SetInput(InputPointer sample)
{
  m_Input = sample ;
}
  
template< class TInputSample, class TClassMaskSample >
MembershipSampleGenerator< TInputSample, TClassMaskSample >::InputPointer
MembershipSampleGenerator< TInputSample, TClassMaskSample >
::GetInput()
{
  return m_Input ;
}

template< class TInputSample, class TClassMaskSample >
void
MembershipSampleGenerator< TInputSample, TClassMaskSample >
::SetClassMask(ClassMaskPointer classMask)
{
  m_ClassMask = classMask ;
}

template< class TInputSample, class TClassMaskSample >
MembershipSampleGenerator< TInputSample, TClassMaskSample >::ClassMaskPointer
MembershipSampleGenerator< TInputSample, TClassMaskSample >
::GetClassMask()
{
  return m_ClassMask ;
}

template< class TInputSample, class TClassMaskSample >
MembershipSampleGenerator< TInputSample, TClassMaskSample >::OutputPointer
MembershipSampleGenerator< TInputSample, TClassMaskSample >
::GetOutput()
{
  return m_Output ;
}

template< class TInputSample, class TClassMaskSample >
void
MembershipSampleGenerator< TInputSample, TClassMaskSample >
::GenerateData()
{
  unsigned int classLabel ;
  m_Output = OutputType::New() ;
  m_Output->SetSample(m_Input) ;
  typename TClassMaskSample::Iterator iter = m_ClassMask->Begin() ;
  while (iter != m_ClassMask->End())
    {
      classLabel = iter.GetMeasurement(0) ;
      m_Output->AddInstance(classLabel, iter.GetInstanceIdentifier()) ;
      ++iter ;
    }
}

template< class TInputSample, class TClassMaskSample >
void
MembershipSampleGenerator< TInputSample, TClassMaskSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Input: " << m_Input << std::endl;
  os << indent << "Output: " << m_Output << std::endl;
  os << indent << "ClassMask: " << m_ClassMask << std::endl ;
}
  } // end namespace Statistics
} // end namespace itk

#endif
