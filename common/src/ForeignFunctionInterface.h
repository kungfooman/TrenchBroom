#pragma once

#include <ccall.h>

/*
	Any language supporting FFI can easily use these function

	Example for Julia (short function syntax):
		ffi_malloc(bytes) = ccall( :ffi_malloc, Ptr{Void}, (Int32,), bytes)
		ffi_free(ptr) = ccall(:ffi_free, Void, (Ptr{Void},) ptr)
		ffi_selected_brushes() = ccall( :ffi_selected_brushes, Ptr{Int}, ())
*/

CCALL void ffi_testbrush();
CCALL void ffi_julia_print(char *message);
CCALL void *ffi_malloc(int bytes);
CCALL void ffi_free(void *ptr);
CCALL int *ffi_selected_brushes();
CCALL int ffi_selected_brushes_count();
CCALL void ffi_update_views();
CCALL bool ffi_brush_rotate(int id, float roll, float pitch, float yaw);
CCALL int *ffi_selected_brushes();