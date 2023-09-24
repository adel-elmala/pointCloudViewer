#if !defined(CPLYPARSER_H)
#define CPLYPARSER_H

#include "plyParser.h"


/* user's vertex and face definitions for a polygonal object */

typedef struct Vertex {
  float x,y,z;             /* the usual 3-space position of a vertex */
} Vertex;

typedef struct Face {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
} Face;


class CplyParser
{
private:
    /* data */
    int i, j, k;
    PlyFile_t *ply;
    int nelems;
    char **elist;
    int file_type;
    float version;
    int nprops;
    int num_elems;
    PlyProperty_t **plist;
    Vertex **vlist;
    Face **flist;
    char *elem_name;
    int num_comments;
    char **comments;
    int num_obj_info;
    char **obj_info;
    

public:
    CplyParser();
    ~CplyParser();
    void read_file(char *file_path);
};

#endif // CPLYPARSER_H
