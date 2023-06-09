/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library

  Copyright (c) 2006-2011 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmImageApplyLookupTable.h"

#include <limits>

namespace gdcm
{

struct ImageApplyLookupTable::impl
{
  bool rgb8;
};

ImageApplyLookupTable::ImageApplyLookupTable():pimpl(new impl)
{
  pimpl->rgb8 = false;
}

ImageApplyLookupTable::~ImageApplyLookupTable()
{
  delete pimpl;
}


bool ImageApplyLookupTable::Apply()
{
  Output = Input;
  const Bitmap &image = *Input;

  PhotometricInterpretation pi = image.GetPhotometricInterpretation();
  if( pi != PhotometricInterpretation::PALETTE_COLOR )
    {
    gdcmDebugMacro( "Image is not palettized" );
    return false;
    }
  const LookupTable &lut = image.GetLUT();
  int bitsample = lut.GetBitSample();
  if( !bitsample ) return false;
  if( pimpl->rgb8 && !lut.IsRGB8() )
    {
    gdcmDebugMacro( "LUT is not compatible with RGB8" );
    return false;
    }

  const unsigned long len = image.GetBufferLength();
  std::vector<char> v;
  v.resize( len );
  char *p = v.data();
  image.GetBuffer( p );
  std::stringstream is;
  if( !is.write( p, len ) )
    {
    gdcmErrorMacro( "Could not write to stringstream" );
    return false;
    }

  DataElement &de = Output->GetDataElement();
#if 0
  std::ostringstream os;
  lut.Decode(is, os);
  const std::string str = os.str();
  VL::Type strSize = (VL::Type)str.size();
  de.SetByteValue( str.c_str(), strSize);
#else
  std::vector<char> v2;
  v2.resize( len * 3 );
  if( pimpl->rgb8 )
    lut.Decode8(v2.data(), v2.size(), v.data(), v.size());
  else
    lut.Decode(v2.data(), v2.size(), v.data(), v.size());
  assert( v2.size() < (size_t)std::numeric_limits<uint32_t>::max() );
  if( pimpl->rgb8 )
    de.SetByteValue( v2.data(), (uint32_t)v2.size() / 2);
  else
    de.SetByteValue( v2.data(), (uint32_t)v2.size() );
#endif
  Output->GetLUT().Clear();
  Output->SetPhotometricInterpretation( PhotometricInterpretation::RGB );
  Output->GetPixelFormat().SetSamplesPerPixel( 3 );
  if( pimpl->rgb8 )
    Output->GetPixelFormat().SetBitsAllocated(8);
  Output->SetPlanarConfiguration( 0 ); // FIXME OT-PAL-8-face.dcm has a PlanarConfiguration while being PALETTE COLOR...
  const TransferSyntax &ts = image.GetTransferSyntax();
  //assert( ts == TransferSyntax::RLELossless );
  if( ts.IsExplicit() )
    {
    Output->SetTransferSyntax( TransferSyntax::ExplicitVRLittleEndian );
    }
  else
    {
    assert( ts.IsImplicit() );
    Output->SetTransferSyntax( TransferSyntax::ImplicitVRLittleEndian );
    }
  // I do not have access to the DataSet:
#if 0
    {
    // usual tags:
    ds.Remove( Tag(0x0028, 0x1101) );
    ds.Remove( Tag(0x0028, 0x1102) );
    ds.Remove( Tag(0x0028, 0x1103) );

    ds.Remove( Tag(0x0028, 0x1201) );
    ds.Remove( Tag(0x0028, 0x1202) );
    ds.Remove( Tag(0x0028, 0x1203) );

    // Don't forget the segmented one:
    ds.Remove( Tag(0x0028, 0x1221) );
    ds.Remove( Tag(0x0028, 0x1222) );
    ds.Remove( Tag(0x0028, 0x1223) );

    // PaletteColorLookupTableUID ??
    ds.Remove( Tag(0x0028, 0x1199) );
    }
#endif


  bool success = true;
  return success;
}

void ImageApplyLookupTable::SetRGB8(bool b)
{
  pimpl->rgb8 = b;
}

} // end namespace gdcm
