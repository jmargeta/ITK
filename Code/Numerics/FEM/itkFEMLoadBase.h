/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLoadBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMLoadBase_h
#define __itkFEMLoadBase_h

#include "itkFEMElementBase.h"
#include "itkFEMNodeBase.h"
#include "itkFEMPArray.h"

namespace itk {
namespace fem {




/**
 * \class Load
 * \brief General abstract load base class.
 *
 * All other load classes that can be used in a FEM system are defined by deriving this one.
 * The load class defines an external load that acts on the system. For each specific subtype 
 * of load, a separate load abstract class should be derived. For example we have LoadElement,
 * which defines the base for all loads that act on a specific element in a system.
 */
class Load : public FEMLightObject
{
FEM_CLASS_SP(Load,FEMLightObject)
public:

  /** Array class that holds special pointers to the load objects */
  typedef FEMPArray<Self> ArrayType;

  /**
   * \class ReadInfoType
   * \brief Additional information that is required when reading load
            objects from stream.
   *
   * When the load object is to be read from the input stream, we must provide
   * pointers to the array of nodes and elements. Construct this class and
   * pass a pointer to it when calling the Read member function for loads.
   */
  class ReadInfoType {
  public:
    Node::ArrayType::ConstPointer m_node;   /**< Pointer to an array nodes */
    Element::ArrayType::ConstPointer m_el;  /**< Pointer to an array of elements */
    /** Constructor for simple object creation. */
    ReadInfoType(  Node::ArrayType::ConstPointer node_,
            Element::ArrayType::ConstPointer el_) :
      m_node(node_), m_el(el_) {}
  };

};




}} // end namespace itk::fem

#endif // #ifndef __itkFEMLoadBase_h
