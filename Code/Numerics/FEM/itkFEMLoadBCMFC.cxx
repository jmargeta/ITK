/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMLoadBCMFC.cxx
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

#include "itkFEMLoadBCMFC.h"
#include "itkFEMUtility.h"

namespace itk {
namespace fem {




/**
 * Fix a DOF to a prescribed value
 */
LoadBCMFC::LoadBCMFC(Node::ConstPointer node, int dof, vnl_vector<Node::Float> val)
{
  lhs.clear();

  /** Set the correct weight */
  lhs.push_back( MFCTerm(node, dof, 1.0) );
  rhs=val;
}




/** Read the LoadBCMFC object from input stream */
void LoadBCMFC::Read( std::istream& f, void* info )
{
  int nlhs, n;
  Node::Float d;
  /**
   * Convert the info pointer to a usable objects
   */
  Node::ArrayType::ConstPointer nodes=static_cast<ReadInfoType*>(info)->m_node;


  /** first call the parent's Read function */
  Superclass::Read(f,info);

  /** read number of terms in lhs of MFC equation */
  SkipWhiteSpace(f); f>>nlhs; if(!f) goto out;
  
  lhs.clear();
  for(int i=0; i<nlhs; i++) 
  {
    /** read and set pointer to node that we're applying the load to */
    SkipWhiteSpace(f); f>>n; if(!f) goto out;
    Node::ConstPointer node;
    try
    {
      node=dynamic_cast<const Node*>( &*nodes->Find(n));
    }
    catch ( FEMExceptionObjectNotFound e )
    {
      throw FEMExceptionObjectNotFound(__FILE__,__LINE__,"LoadBCMFC::Read()",e.m_baseClassName,e.m_GN);
    }

    /** read the number of dof within that node */
    SkipWhiteSpace(f); f>>n; if(!f) goto out;

    /** read weight */
    SkipWhiteSpace(f); f>>d; if(!f) goto out;

    /** add a new MFCTerm to the lhs */
    lhs.push_back( MFCTerm(node, n, d) );
  }

  /** read the rhs */
  SkipWhiteSpace(f); f>>n; if(!f) goto out;
  rhs.resize(n);
  SkipWhiteSpace(f); f>>rhs; if(!f) goto out;

out:

  if( !f )
  {
    throw FEMExceptionIO(__FILE__,__LINE__,"LoadBCMFC::Read()","Error reading FEM load!");
  }

}


/**
 * Write the LoadBCMFC object to the output stream
 */
void LoadBCMFC::Write( std::ostream& f, int ofid ) const 
{

  /** if not set already, se set the ofid */
  if (ofid<0) ofid=OFID;

  /** first call the parent's write function */
  Superclass::Write(f,ofid);

  /**
   * Write the actual Load data
   */

  /** write the number of DOFs affected by this MFC */
  f<<"\t"<<lhs.size()<<"\t% Number of DOFs in this MFC"<<"\n";

  /** write each term */
  f<<"\t  %==>\n";
  for(LhsType::const_iterator q=lhs.begin(); q!=lhs.end(); q++) 
  {
    f<<"\t  "<<q->node->GN<<"\t% GN of node"<<"\n";
    f<<"\t  "<<q->dof<<"\t% DOF# in node"<<"\n";
    f<<"\t  "<<q->value<<"\t% weight"<<"\n";
    f<<"\t  %==>\n";
  }

  /** write the rhs */
  f<<"\t"<<rhs.size();
  f<<" "<<rhs<<"\t% rhs of MFC"<<"\n";

  /** check for errors */
  if (!f)
  {
    throw FEMExceptionIO(__FILE__,__LINE__,"LoadBCMFC::Write()","Error writing FEM load!");
  }

}

FEM_CLASS_REGISTER(LoadBCMFC)




}} // end namespace itk::fem
