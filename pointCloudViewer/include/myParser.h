#ifndef MYPARSER_H
#define MYPARSER_H

#include <string>
#include <vector>

#define    PLY_SIZE_CHAR   1
#define    PLY_SIZE_UCHAR   1
#define    PLY_SIZE_SHORT   2
#define    PLY_SIZE_USHORT   2
#define    PLY_SIZE_INT   4
#define    PLY_SIZE_UINT   4
#define    PLY_SIZE_FLOAT   4
#define    PLY_SIZE_DOUBLE   8

enum type_code_name
{
    PLY_CHAR,
    PLY_UCHAR,
    PLY_SHORT,
    PLY_USHORT,
    PLY_INT,
    PLY_UINT,
    PLY_FLOAT,
    PLY_DOUBLE
};
typedef struct element_vertex
{
    unsigned long num_verts;
    unsigned int pos_property_num;
    unsigned int pos_property_type_size;
    type_code_name pos_property_type_name;
    unsigned int color_property_num;
    unsigned int color_property_type_size;
    type_code_name color_property_type_name;

} element_vertex_t;

typedef struct ply_header
{
    bool is_ascii;
    int version;
    // std::vector<std::string> comments; // ignore comments
    element_vertex_t vertex_element;
    size_t header_size_in_bytes;

} ply_header_t;

struct vertex
{
    float x, y, z;
};
struct rgba
{
    float r, g, b, a;
};

class myParser
{
private:
    /* data */
    std::string m_file_path;
    bool finished_header;
    ply_header_t m_header;

    void parse_header_line(std::ifstream& fin,std::vector<std::string>& word_vec);
    std::vector<std::string> myParser::split_line(char* line);
    std::pair<type_code_name, unsigned int> match_type(std::string word);
    void parse_data_txt_mode(std::ifstream& fin);
    void parse_data_binary_mode(std::ifstream& fin);

    template<class V, class C>
    void extract_vetex_data(char* data, unsigned int nVerts, V vert_type, C color_type);
    void dispatch_type(char* data, unsigned int nVerts);

public:
    myParser(std::string file_path);
    ~myParser();

    void read_file();
    void parse_header(std::ifstream &fin);
    std::vector<vertex> m_points;
    std::vector<rgba> m_colors;
};

#endif // MYPARSER_H