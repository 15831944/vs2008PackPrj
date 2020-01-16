/**
 @file mem_image.cpp
 @author Kevin Lynx
 @brief to load/save images from/to memory based on GDI+
*/
#include "StdAfx.h"
#include "mem_image.h"
#include <string.h>

static int GetEncoderClsid( const WCHAR *format, CLSID *pClsid )
{
	UINT num = 0, size = 0;
	Gdiplus::ImageCodecInfo *pImageCodecInfo = NULL; 
	Gdiplus::GetImageEncodersSize( &num, &size );
	if( size == 0 )
	{
		return -1;
	}
	pImageCodecInfo = (Gdiplus::ImageCodecInfo*) malloc( size );
	Gdiplus::GetImageEncoders( num, size, pImageCodecInfo );
	for( UINT i = 0; i < num; ++ i )
	{
		if( wcscmp( pImageCodecInfo[i].MimeType, format ) == 0 )
		{
			*pClsid = pImageCodecInfo[i].Clsid;
			free( pImageCodecInfo );
			return i;
		}
	}
	free( pImageCodecInfo );
	return -1;
}

static bool mem_to_global( const void *buf, size_t size, HGLOBAL global )
{
	void *dest = ::GlobalLock( global );
	if( dest == NULL )
	{
		return false;
	}
	memcpy( dest, buf, size );
	::GlobalUnlock( global );
	return true;
}

static bool stream_to_mem( IStream *stream, void **outbuf, size_t *size )
{
	ULARGE_INTEGER ulnSize;
	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	/* get the stream size */
	if( stream->Seek( lnOffset, STREAM_SEEK_END, &ulnSize ) != S_OK )
	{
		return false;
	}
	if( stream->Seek( lnOffset, STREAM_SEEK_SET, NULL ) != S_OK )
	{
		return false;
	}

	/* read it */
	*outbuf = malloc( (size_t)ulnSize.QuadPart );
	*size = (size_t) ulnSize.QuadPart;
	ULONG bytesRead;
	if( stream->Read( *outbuf, (ULONG)ulnSize.QuadPart, &bytesRead ) != S_OK )
	{
		free( *outbuf );
		return false;
	}

	return true;
}

Gdiplus::Image *mi_from_memory( const void *buf, size_t size )
{
	IStream *stream = NULL;
	HGLOBAL global = ::GlobalAlloc( GMEM_MOVEABLE, size );
	if( global == NULL )
	{
		return NULL;
	}
	/* copy the buf content to the HGLOBAL */
	if( !mem_to_global( buf, size, global ) )
	{
		::GlobalFree( global );
		return NULL;
	}
	/* get the IStream from the global object */
	if( ::CreateStreamOnHGlobal( global, TRUE, &stream ) != S_OK )
	{
		::GlobalFree( global );
		return NULL;
	}
	/* create the image from the stream */
	Gdiplus::Image *image = Gdiplus::Image::FromStream( stream );

	stream->Release();
	/* i suppose when the reference count for stream is 0, it will 
	GlobalFree automatically. The Image maintain the object also.*/	
	return image;
}

void *mi_to_memory( Gdiplus::Image *image, void **outbuf, size_t *size, bool bFillWhite, int nImageDPI)
{
	if (!image)
	{
		return NULL;
	}

	if (nImageDPI <= 0)
	{
		nImageDPI = 96;
	}

	IStream *stream = NULL;
	if( ::CreateStreamOnHGlobal( NULL, TRUE, &stream ) != S_OK )
	{
		return NULL;
	}

// 	format = L"image/jpeg";
// 	format = L"image/tiff";
// 	format = L"image/png";
// 	format = L"image/bmp";
// 	format = L"image/gif";
	/* get the image encoder PNG */
	::CLSID imageClsid;
	GetEncoderClsid( L"image/png", &imageClsid );

	// 
	if (bFillWhite)
	{
		Gdiplus::Bitmap *bmp = NULL;
		Gdiplus::Graphics *graphics = NULL;
		float fRatio = image->GetHorizontalResolution() / nImageDPI/*96.0*/;
		int nBitmapWidth = image->GetWidth() / fRatio;
		int nBitmapHeight = image->GetHeight() / fRatio;
		bmp = new Gdiplus::Bitmap(nBitmapWidth, nBitmapHeight);
		bmp->SetResolution(96.0, 96.0);
		graphics = new Gdiplus::Graphics(bmp);
		Gdiplus::SolidBrush WhiteBrush(Gdiplus::Color::White);
		graphics->FillRectangle(&WhiteBrush, 0, 0, nBitmapWidth, nBitmapHeight);
		Gdiplus::Rect rc(0, 0, nBitmapWidth, nBitmapHeight);
		graphics->DrawImage(image, rc, 0, 0, image->GetWidth(), image->GetHeight(), Gdiplus::Unit::UnitPixel);

		/* save the image to stream */
		Gdiplus::Status save_s = bmp->Save( stream, &imageClsid );
		delete bmp;
		delete graphics;
		graphics=NULL;
		bmp=NULL;

		if( save_s != Gdiplus::Ok )
		{
			stream->Release();
			return NULL;
		}
	}
	else
	{
		/* save the image to stream */
		Gdiplus::Status save_s = image->Save( stream, &imageClsid );
		if( save_s != Gdiplus::Ok )
		{
			stream->Release();
			return NULL;
		}
	}

	/* read the stream to buffer */
	if( !stream_to_mem( stream, outbuf, size ) )
	{
		stream->Release();
		return NULL;
	}
	stream->Release();
	return *outbuf;
}
