/*

Header for PLY polygon files.

- Greg Turk, March 1994

A PLY file contains a single polygonal _object_.

An object is composed of lists of _elements_.  Typical elements are
vertices, faces, edges and materials.

Each type of element for a given object has one or more _properties_
associated with the element type.  For instance, a vertex element may
have as properties three floating-point values x,y,z and three unsigned
chars for red, green and blue.

---------------------------------------------------------------

Copyright (c) 1994 The Board of Trustees of The Leland Stanford
Junior University.  All rights reserved.

Permission to use, copy, modify and distribute this software and its
documentation for any purpose is hereby granted without fee, provided
that the above copyright notice and this permission notice appear in
all copies of this software and that you do not sell the software.

THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND,
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

*/

#ifndef __PLY_H__
#define __PLY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>

#define PLY_ASCII      1        /* ascii PLY file */
#define PLY_BINARY_BE  2        /* binary PLY file, big endian */
#define PLY_BINARY_LE  3        /* binary PLY file, little endian */

#define PLY_OKAY    0           /* ply routine worked okay */
#define PLY_ERROR  -1           /* error in ply routine */

	/* scalar data types supported by PLY format */

#define PLY_START_TYPE 0
#define PLY_CHAR       1
#define PLY_SHORT      2
#define PLY_INT        3
#define PLY_UCHAR      4
#define PLY_USHORT     5
#define PLY_UINT       6
#define PLY_FLOAT      7
#define PLY_DOUBLE     8
#define PLY_END_TYPE   9

#define  PLY_SCALAR  0
#define  PLY_LIST    1


typedef struct PlyProperty {    /* description of a property */

	char* name;                           /* property name */
	int external_type;                    /* file's data type */
	int internal_type;                    /* program's data type */
	int offset;                           /* offset bytes of prop in a struct */

	int is_list;                          /* 1 = list, 0 = scalar */
	int count_external;                   /* file's count type */
	int count_internal;                   /* program's count type */
	int count_offset;                     /* offset byte for list count */

} PlyProperty_t;

typedef struct PlyElement {     /* description of an element */
	char* name;                   /* element name */
	int num;                      /* number of elements in this object */
	int size;                     /* size of element (bytes) or -1 if variable */
	int nprops;                   /* number of properties for this element */
	PlyProperty_t** props;          /* list of properties in the file */
	char* store_prop;             /* flags: property wanted by user? */
	int other_offset;             /* offset to un-asked-for props, or -1 if none*/
	int other_size;               /* size of other_props structure */
} PlyElement_t;

typedef struct PlyOtherProp {   /* describes other properties in an element */
	char* name;                   /* element name */
	int size;                     /* size of other_props */
	int nprops;                   /* number of properties in other_props */
	PlyProperty_t** props;          /* list of properties in other_props */
} PlyOtherProp_t;

typedef struct OtherData { /* for storing other_props for an other element */
	void* other_props;
} OtherData_t;

typedef struct OtherElem {     /* data for one "other" element */
	char* elem_name;             /* names of other elements */
	int elem_count;              /* count of instances of each element */
	OtherData** other_data;      /* actual property data for the elements */
	PlyOtherProp_t* other_props;   /* description of the property data */
} OtherElem_t;

typedef struct PlyOtherElems {  /* "other" elements, not interpreted by user */
	int num_elems;                /* number of other elements */
	OtherElem* other_list;        /* list of data for other elements */
} PlyOtherElems_t;

typedef struct PlyFile {        /* description of PLY file */
	FILE* fp;                     /* file pointer */
	int file_type;                /* ascii or binary */
	float version;                /* version number of file */
	int nelems;                   /* number of elements of object */
	PlyElement_t** elems;           /* list of elements */
	int num_comments;             /* number of comments */
	char** comments;              /* list of comments */
	int num_obj_info;             /* number of items of object information */
	char** obj_info;              /* list of object info items */
	PlyElement_t* which_elem;       /* which element we're currently writing */
	PlyOtherElems* other_elems;   /* "other" elements from a PLY file */
} PlyFile_t;

/* memory allocation */
extern char* my_alloc();
#define myalloc(mem_size) my_alloc((mem_size), __LINE__, __FILE__)


/*** delcaration of routines ***/

extern PlyFile_t* ply_write(FILE*, int, char**, int);
extern PlyFile_t* ply_open_for_writing(char*, int, char**, int, float*);
extern void ply_describe_element(PlyFile_t*, char*, int, int, PlyProperty_t*);
extern void ply_describe_property(PlyFile_t*, char*, PlyProperty_t*);
extern void ply_element_count(PlyFile_t*, char*, int);
extern void ply_header_complete(PlyFile_t*);
extern void ply_put_element_setup(PlyFile_t*, char*);
extern void ply_put_element(PlyFile_t*, void*);
extern void ply_put_comment(PlyFile_t*, char*);
extern void ply_put_obj_info(PlyFile_t*, char*);
extern PlyFile_t* ply_read(FILE*, int*, char***);
extern PlyFile_t* ply_open_for_reading(char*, int*, char***, int*, float*);
extern PlyProperty_t** ply_get_element_description(PlyFile_t*, char*, int*, int*);
extern void ply_get_element_setup(PlyFile_t*, char*, int, PlyProperty_t*);
extern void ply_get_property(PlyFile_t*, char*, PlyProperty_t*);
extern PlyOtherProp* ply_get_other_properties(PlyFile_t*, char*, int);
extern void ply_get_element(PlyFile_t*, void*);
extern char** ply_get_comments(PlyFile_t*, int*);
extern char** ply_get_obj_info(PlyFile_t*, int*);
extern void ply_close(PlyFile_t*);
extern void ply_get_info(PlyFile_t*, float*, int*);
extern PlyOtherElems* ply_get_other_element(PlyFile_t*, char*, int);
extern void ply_describe_other_elements(PlyFile_t*, PlyOtherElems*);
extern void ply_put_other_elements(PlyFile_t*);
extern void ply_free_other_elements(PlyOtherElems*);

extern int equal_strings(char*, char*);


#ifdef __cplusplus
}
#endif
#endif /* !__PLY_H__ */
