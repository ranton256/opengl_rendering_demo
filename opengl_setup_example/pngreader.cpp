//
//  pngreader.cpp
//  oramaxc
//
//  Created by Richard Anton on 6/6/22.
//

#include "pngreader.h"

#include "dbgutils.h"

#include "png.h"

#include <iostream>
#include <cstdio>
#include <exception>

// TODO: this is duplicated from PNGWriter and should be shared.

typedef enum {
    pw_NoError = 0,
    pw_FileOpenError,
    pw_MemFull,
    pw_Unknown
} PNGErrorCode;


typedef struct {
    PNGErrorCode errorCode;

    FILE* fp;
} PNGState;

/////////////////////////////////////////////////////////////////////////
// error and warning handling functions.
static void LibPNGErrorProc(png_structp png_ptr, png_const_charp ptr)
{
    PNGState *statePtr = (PNGState *)png_get_error_ptr(png_ptr);//->error_ptr;
    // throw an error
    DbgAssert( statePtr );
    // set it to unknown error if we don't know what it is.
    if( statePtr->errorCode == pw_NoError )
        statePtr->errorCode = pw_Unknown;
    
    throw( statePtr->errorCode );
}

static void LibPNGWarningProc(png_structp png_ptr, png_const_charp ptr)
{
    // assert so that we can tell when debugging that we had
    // a warning, but ignore it otherwise.
    DbgAssert(false);
}


// our allocation and freeing functions.
static png_voidp LibPNGMallocProc(png_structp png_ptr, png_size_t size)
{
    uint8_t *ptr = new uint8_t[size];
    if(!ptr)
    {
        // save error for error handler.
        PNGState *statePtr = (PNGState *)png_get_mem_ptr(png_ptr);//->mem_ptr;
        DbgAssert( statePtr );
        statePtr->errorCode = pw_MemFull;
    }
    return (png_voidp)ptr;
}

static void LibPNGFreeProc(png_structp png_ptr, png_voidp ptr)
{
    DbgAssert(ptr);
    
    uint8_t *bptr = (uint8_t *)ptr;
    
    delete[] bptr;
}

class PNGReadException : public std::exception {
public:
    virtual const char* what() const noexcept {
        return "Error reading PNG file";
    }
};

bool ReadPNGImage(const char* path, uint8_t** outPixels, uint32_t* outWidth, uint32_t* outHeight)
{
    DbgAssert(path != nullptr);
    
    
    DbgAssert(outPixels != nullptr);
    
    DbgAssert(outWidth != nullptr);
    DbgAssert(outHeight != nullptr);
    
    *outPixels = nullptr;
    
    uint8_t *pixelPtr = nullptr;
    
    PNGState state;
    state.errorCode = pw_NoError;
    state.fp = fopen(path, "r");
    if(!state.fp) {
        std::cerr << "Unable to open PNG file " << path << "\n";
        return false;
    }
    
    try {
        
        
        // our row pointers.
        png_bytepp rowPointers = NULL;
        
//
//    write_ptr = png_create_write_struct_2(PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
//        (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL,
//        LibPNGMallocProc,LibPNGFreeProc);
//
        // we need to create the png and info structures
        png_structp png_ptr = png_create_read_struct
           (PNG_LIBPNG_VER_STRING, (png_voidp)&state,
            LibPNGErrorProc, LibPNGWarningProc);
        if (!png_ptr)
        {
            std::cerr << "Error allocating memory for PNG read struct\n";
            throw PNGReadException();
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            std::cerr << "Error allocating memory for PNG info\n";
            png_destroy_read_struct(&png_ptr,
               (png_infopp)NULL, (png_infopp)NULL);
            
            throw PNGReadException();
        }

        png_set_mem_fn(png_ptr, (png_voidp)&state, LibPNGMallocProc,LibPNGFreeProc);
        
        
        png_infop end_info = png_create_info_struct(png_ptr);
        if (!end_info)
        {
            std::cerr << "Error allocating memory for PNG end info\n";
            png_destroy_read_struct(&png_ptr, &info_ptr,
              (png_infopp)NULL);
           
            throw PNGReadException();
        }
        
        // error handling functions are already set.
        // Setup error callbacks so we don't have to do setjmp and longjmp
        png_set_error_fn(png_ptr, (png_voidp)NULL, LibPNGErrorProc, LibPNGWarningProc);
        
        
        png_init_io(png_ptr, state.fp);
        
        // read header info.
        png_read_info(png_ptr, info_ptr);
        
        // get the IHDR information
        png_uint_32 width, height;
        int bitDepth, colorType, interlaceType;//, compressionType, filterType;
        
        png_get_IHDR(
            png_ptr, info_ptr,
            &width, &height,
            &bitDepth, &colorType, &interlaceType,
            NULL, NULL
            //&compressionType, &filterType
               );

        *outWidth = width;
        *outHeight = height;
       

        // Extract multiple pixels with bit depths of 1, 2, and 4 from a single
        // byte into separate bytes (useful for paletted and grayscale images).
        png_set_packing(png_ptr);
        
        // if we are dealing with 16 bit channels, convert them to 8 bit
        if( bitDepth == 16 )
            png_set_strip_16(png_ptr);
        
        // Need to deal with background color if there is a transparency
        // or alpha of any kind.
        png_color_16 my_background, *image_background;
        
        // our default background colors.
        my_background.index = 0;
        my_background.red = 0xFFFF;
        my_background.green = 0xFFFF;
        my_background.blue = 0xFFFF;
        my_background.gray = 0xFFFF;
        
        if (png_get_bKGD(png_ptr, info_ptr, &image_background))
            png_set_background(png_ptr, image_background,
                PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
        else
            png_set_background(png_ptr, &my_background,
                PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
        
        // NOTE: we should actually deal with gamma
        
        // update the info structures
        png_read_update_info(png_ptr, info_ptr);
        
        uint32_t rowBytes = (uint32_t)png_get_rowbytes( png_ptr,info_ptr );
        
        if( colorType != PNG_COLOR_TYPE_RGB ) {
            std::cerr << "Unsupported PNG color type\n";
            throw PNGReadException();
        }
        
        // setup the row pointers.
        rowPointers = new png_bytep[ height ];
        DbgAssert(rowPointers);
        
        // lock the image.
        uint32_t imageRowBytes = width * 3;
        pixelPtr = new uint8_t[imageRowBytes * height];
        DbgAssert(pixelPtr);
        
        // check that our row bytes is not less than theirs(theirs
        // doesn't include padding, so we have messed up if ours is less)
        DbgAssert( imageRowBytes >= rowBytes );
        
        // set the row pointers to actually point to the rows.
        for( int rc = 0; rc < height; rc++ )
            rowPointers[rc] = pixelPtr + imageRowBytes*rc;
        
        // call png_read_image to read it at once.
        png_read_image( png_ptr, rowPointers );
        
        // finish up
        png_read_end(png_ptr, end_info);
        
        // free the row pointers.
        delete[] rowPointers;
        
        // free the read structures.
        png_destroy_read_struct(&png_ptr, &info_ptr,
               &end_info);


        // Close the file
        fclose(state.fp);

        *outPixels = pixelPtr;
        
        
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        
        fclose(state.fp);
        if(pixelPtr)
            delete pixelPtr;
        return false;
    }
    
    return true;
}

