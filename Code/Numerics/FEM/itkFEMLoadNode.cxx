/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLoadNode.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEMLoadNode.h"
#include "itkFEMUtility.h"

namespace itk {
namespace fem {




/**
 * Read the LoadNode object from input stream
 */
void LoadNode::Read( std::istream& f, void* info )
{
  int n;
  /**
   * Convert the info pointer to a usable objects
   */
  Node::ArrayType::ConstPointer nodes=static_cast<ReadInfoType*>(info)->m_node;


  /** first call the parent's read function */
  Superclass::Read(f,info);

  /** read and set pointer to node that we're applying the load to */
  SkipWhiteSpace(f); f>>n; if(!f) goto out;
  try
  {
    node=dynamic_cast<const Node*>( &*nodes->Find(n));
  }
  catch ( FEMExceptionObjectNotFound e )
  {
    throw FEMExceptionObjectNotFound(__FILE__,__LINE__,"LoadNode::Read()",e.m_baseClassName,e.m_GN);
  }
  
  /** read and set the number of elements inside a force vector */
  SkipWhiteSpace(f); f>>n; if(!f) goto out;
  F.resize(n);  

  /** read the force vector itself */
  SkipWhiteSpace(f); f>>F; if(!f) goto out;

out:

  if( !f )
  {
    throw FEMExceptionIO(__FILE__,__LINE__,"LoadNode::Read()","Error reading FEM load!");
  }

}




/**
 * Write the LoadNode to the output stream
 */
void LoadNode::Write( std::ostream& f, int ofid ) const {

  /** if not set already, se set the ofid */
  if (ofid<0) ofid=OFID;

  /** first call the parent's write function */
  Superclass::Write(f,ofid);

  /** write the actual Load data */
  f<<"\t"<<node->GN<<"\t% GN of node on which the load acts"<<"\n";
  f<<"\t"<<F.size()<<" "<<F<<"\t% Force vector (first number is the size of a vector)\n";

  /** check for errors */
  if (!f)
  {
    throw FEMExceptionIO(__FILE__,__LINE__,"LoadNode::Write()","Error writing FEM load!");
  }

}

FEM_CLASS_REGISTER(LoadNode)




}} // end namespace itk::fem
