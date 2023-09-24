#include "myParser.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <utility>      // std::pair, std::make_pair

#include <chrono>

class manual_timer
{
    std::chrono::high_resolution_clock::time_point t0;
    double timestamp{ 0.0 };

public:
    void start() { t0 = std::chrono::high_resolution_clock::now(); }
    void stop() { timestamp = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count() * 1000.0; }
    const double& get() { return timestamp; }
};


myParser::myParser(std::string file_path) : m_file_path(file_path), m_header{0}
{
}

myParser::~myParser()
{
}

void myParser::read_file()
{
    manual_timer read_timer;

    read_timer.start();


    std::ifstream fin(m_file_path);

    if (fin.is_open())
    {

        parse_header(fin);
        
        if (m_header.is_ascii)
        {
            // continue to read the file in text mode 
            parse_data_txt_mode(fin);


        }
        else // binary
        {
            fin.close(); // close the txt file
            std::ifstream fin(m_file_path, std::ifstream::binary); // reopen the same file in binary mode
            
            fin.seekg(m_header.header_size_in_bytes); // seek to where the data begins

            parse_data_binary_mode(fin);

        }
        

    }
    read_timer.stop();
    const float parsing_time = static_cast<float>(read_timer.get()) / 1000.f;
    std::cout << "\tparsing " << parsing_time << " seconds " << std::endl;


}

void myParser::parse_header(std::ifstream& fin)
{
    char line[256];
    std::string header_end("end_header");
    std::vector<std::string> words;

    
    // get a line at a time 
    while(fin.getline(line,256))
    {
        m_header.header_size_in_bytes += fin.gcount();
        // if reachered the header end , finish
        if (header_end == line)
        {
            std::cout << line << std::endl;
            //m_header.header_size_in_bytes = fin.tellg();
            break;
        }

        words = split_line(line);
        
        parse_header_line(fin,words);

    }
}
void myParser::parse_header_line(std::ifstream& fin,std::vector<std::string>& word_vec)
{
    // skip comments
    if (word_vec[0] == "comment")
        return;
    // format line 
    if (word_vec[0] == "format")
    {
        assert(word_vec.size() == 3);

        m_header.is_ascii = (word_vec[1] == "ascii");
        m_header.version = atoi(word_vec[2].c_str());
        return;
    }
    // element description line
    if (word_vec[0] == "element")
    {
        assert(word_vec.size() == 3);
        if (word_vec[1] == "vertex")
        {
            m_header.vertex_element.num_verts = atoi(word_vec[2].c_str());

            char line[256];
            std::vector<std::string> words;
            // pull in the property lines following this line.
            while (fin.getline(line, 256))
            {
                m_header.header_size_in_bytes += fin.gcount();

                words = split_line(line);
                if (words[0] != "property")
                    break;
                std::string property_type = words[1];
                std::string property_name = words[2];
                // position property
                if (property_name == "x" || property_name == "y" || property_name == "z")
                {
                    m_header.vertex_element.pos_property_num++;
                    auto vert_type = match_type(property_type);
                    m_header.vertex_element.pos_property_type_name = vert_type.first;
                    m_header.vertex_element.pos_property_type_size = vert_type.second;

                }
                // color property
                if (property_name == "red" ||
                    property_name == "diffuse_red" || 
                    property_name == "green" ||
                    property_name == "diffuse_green" ||
                    property_name == "blue"||
                    property_name == "diffuse_blue" ||
                    property_name == "alpha" ||
                    property_name == "diffuse_alpha")
                {
                    m_header.vertex_element.color_property_num++;

                    auto color_type = match_type(property_type);

                    m_header.vertex_element.color_property_type_name = color_type.first;
                    m_header.vertex_element.color_property_type_size = color_type.second;

                }
            }
        }
        return;
    }
}
std::vector<std::string>
myParser::split_line(char* line)
{
    std::string word("");
    std::vector<std::string> words;

    // flush the previous line content
    //words.clear();

    std::cout << line << std::endl;
    std::stringstream word_stream(line);

    while (word_stream >> word)
        words.push_back(word);
    
    return words;

}


std::pair<type_code_name,unsigned int> myParser::match_type(std::string word)
{
    std::pair<type_code_name, unsigned int> result;
    if (word == "char")
        return std::make_pair(PLY_CHAR, PLY_SIZE_CHAR);

    if (word == "uchar")
        return std::make_pair(PLY_UCHAR, PLY_SIZE_UCHAR);

    
    if (word == "short")
        return std::make_pair(PLY_SHORT, PLY_SIZE_SHORT);

    if (word == "ushort")
        return std::make_pair(PLY_USHORT, PLY_SIZE_USHORT);

    if (word == "int")
        return std::make_pair(PLY_INT, PLY_SIZE_INT);

    if (word == "uint")
        return std::make_pair(PLY_UINT, PLY_SIZE_UINT);

    if (word == "float")
        return std::make_pair(PLY_FLOAT, PLY_SIZE_FLOAT);

    if (word == "double")
        return std::make_pair(PLY_DOUBLE, PLY_SIZE_DOUBLE);


}

void myParser::parse_data_txt_mode(std::ifstream& fin)
{
    // TODO Later
}

void myParser::parse_data_binary_mode(std::ifstream& fin)
{
    // calculate the size of the data that need t be read 
    unsigned int size_of_each_attribute = 
        ((m_header.vertex_element.pos_property_num * m_header.vertex_element.pos_property_type_size) +
        (m_header.vertex_element.color_property_num * m_header.vertex_element.color_property_type_size));

    double total_bytes_to_read = m_header.vertex_element.num_verts * size_of_each_attribute;

    // Buffer size 1 Megabyte (or any number you like)
    size_t buffer_size = 1 << 20;

    // how many vertex attribs per 1 MBs
    unsigned int nVerts_per_mb = buffer_size / size_of_each_attribute;
    buffer_size = nVerts_per_mb * size_of_each_attribute; // to avoid spliting the last vert attrib in the buffer between 2 buffer reads

    unsigned int nBuffers_needed = m_header.vertex_element.num_verts / nVerts_per_mb;
    unsigned int remaining = m_header.vertex_element.num_verts - (nBuffers_needed * nVerts_per_mb);

    char* buffer = new char[buffer_size];


    // read the file in binary mode starting from where the header ended
    while (fin && (total_bytes_to_read > 0))
    {

        if(nBuffers_needed == 0)
            fin.read(buffer, remaining * size_of_each_attribute);
        else
            // Try to read next chunk of data
            fin.read(buffer, buffer_size);

        // Get the number of bytes actually read
        size_t count = fin.gcount();
        // If nothing has been read, break
        if (!count)
            break;
        // Do whatever you need with first count bytes in the buffer
      
        //extract_vetex_data(data, count / size_of_each_attribute);
        dispatch_type(buffer, count / size_of_each_attribute);
        total_bytes_to_read -= count;
        nBuffers_needed--;

    }
    delete[] buffer;

}
template<class V,class C>
void myParser::extract_vetex_data(char* data,unsigned int nVerts,V vert_type,C color_type)
{
    /*std::vector<V> points;
    std::vector<C> color;*/
   
    vertex vert;
    rgba col;
    unsigned int nPos = m_header.vertex_element.pos_property_num;
    unsigned int nColor = m_header.vertex_element.color_property_num;
    size_t vs = sizeof(V);
    size_t cs = sizeof(C);
    while (nVerts--)
    {
        // assuming the order is vertex coords then color values.
        for (int i = 0; i < nPos; ++i)
        {
            //points.push_back(( (V*)data )[i]);
            ((float*)&vert)[i] = (((float*)data)[i]);
        }

        data = data + sizeof(V) * nPos;
        for (int i = 0; i < nColor; ++i)
        {
            //color.push_back(( (C*)data )[i]);
            ((float*)&col)[i] = (float)(((C*)data)[i]);

        }
        data = data + sizeof(C) * nColor;

        m_points.push_back(vert);
        
        if(nColor)
            m_colors.push_back(col);

    }

}

void myParser::dispatch_type(char* data, unsigned int nVerts)
{
    switch (m_header.vertex_element.pos_property_type_name)
    {
    case PLY_CHAR:
    {
        switch (m_header.vertex_element.color_property_type_name)
        {
        case PLY_CHAR:
            extract_vetex_data(data, nVerts, (char)1, (char)1);
            break;
        case PLY_UCHAR:
            extract_vetex_data(data, nVerts, (char)1, (unsigned char)1);
            break;
        case PLY_SHORT:
            extract_vetex_data(data, nVerts, (char)1, (short)1);
            break;
        case PLY_USHORT:
            extract_vetex_data(data, nVerts, (char)1, (unsigned short)1);
            break;
        case PLY_INT:
            extract_vetex_data(data, nVerts, (char)1, (int)1);
            break;
        case PLY_UINT:
            extract_vetex_data(data, nVerts, (char)1, (unsigned int)1);
            break;
        case PLY_FLOAT:
            extract_vetex_data(data, nVerts, (char)1, (float)1.0f);
            break;
        case PLY_DOUBLE:
            extract_vetex_data(data, nVerts, (char)1, (double)1.0);
            break;

        }
        break;
    }
    case PLY_UCHAR:
    {
        switch (m_header.vertex_element.color_property_type_name)
        {
        case PLY_CHAR:
            extract_vetex_data(data, nVerts, (unsigned char)1, (char)1);
            break;
        case PLY_UCHAR:
            extract_vetex_data(data, nVerts, (unsigned char)1, (unsigned char)1);
            break;
        case PLY_SHORT:
            extract_vetex_data(data, nVerts, (unsigned char)1, (short)1);
            break;
        case PLY_USHORT:
            extract_vetex_data(data, nVerts, (unsigned char)1, (unsigned short)1);
            break;
        case PLY_INT:
            extract_vetex_data(data, nVerts, (unsigned char)1, (int)1);
            break;
        case PLY_UINT:
            extract_vetex_data(data, nVerts, (unsigned char)1, (unsigned int)1);
            break;
        case PLY_FLOAT:
            extract_vetex_data(data, nVerts, (unsigned char)1, (float)1.0f);
            break;
        case PLY_DOUBLE:
            extract_vetex_data(data, nVerts, (unsigned char)1, (double)1.0);
            break;

        }
        break;
    }
    case PLY_SHORT:
    {
        switch (m_header.vertex_element.color_property_type_name)
        {
        case PLY_CHAR:
            extract_vetex_data(data, nVerts, (short)1, (char)1);
            break;
        case PLY_UCHAR:
            extract_vetex_data(data, nVerts, (short)1, (unsigned char)1);
            break;
        case PLY_SHORT:
            extract_vetex_data(data, nVerts, (short)1, (short)1);
            break;
        case PLY_USHORT:
            extract_vetex_data(data, nVerts, (short)1, (unsigned short)1);
            break;
        case PLY_INT:
            extract_vetex_data(data, nVerts, (short)1, (int)1);
            break;
        case PLY_UINT:
            extract_vetex_data(data, nVerts, (short)1, (unsigned int)1);
            break;
        case PLY_FLOAT:
            extract_vetex_data(data, nVerts, (short)1, (float)1.0f);
            break;
        case PLY_DOUBLE:
            extract_vetex_data(data, nVerts, (short)1, (double)1.0);
            break;
        }
        break;
    }
    case PLY_USHORT:
    {
        switch (m_header.vertex_element.color_property_type_name)
        {
        case PLY_CHAR:
            extract_vetex_data(data, nVerts, (unsigned short)1, (char)1);
            break;
        case PLY_UCHAR:
            extract_vetex_data(data, nVerts, (unsigned short)1, (unsigned char)1);
            break;
        case PLY_SHORT:
            extract_vetex_data(data, nVerts, (unsigned short)1, (short)1);
            break;
        case PLY_USHORT:
            extract_vetex_data(data, nVerts, (unsigned short)1, (unsigned short)1);
            break;
        case PLY_INT:
            extract_vetex_data(data, nVerts, (unsigned short)1, (int)1);
            break;
        case PLY_UINT:
            extract_vetex_data(data, nVerts, (unsigned short)1, (unsigned int)1);
            break;
        case PLY_FLOAT:
            extract_vetex_data(data, nVerts, (unsigned short)1, (float)1.0f);
            break;
        case PLY_DOUBLE:
            extract_vetex_data(data, nVerts, (unsigned short)1, (double)1.0);
            break;
        }
        break;
    }
    case PLY_INT:
    {
        switch (m_header.vertex_element.color_property_type_name)
        {
        case PLY_CHAR:
            extract_vetex_data(data, nVerts, (int)1, (char)1);
            break;
        case PLY_UCHAR:
            extract_vetex_data(data, nVerts, (int)1, (unsigned char)1);
            break;
        case PLY_SHORT:
            extract_vetex_data(data, nVerts, (int)1, (short)1);
            break;
        case PLY_USHORT:
            extract_vetex_data(data, nVerts, (int)1, (unsigned short)1);
            break;
        case PLY_INT:
            extract_vetex_data(data, nVerts, (int)1, (int)1);
            break;
        case PLY_UINT:
            extract_vetex_data(data, nVerts, (int)1, (unsigned int)1);
            break;
        case PLY_FLOAT:
            extract_vetex_data(data, nVerts, (int)1, (float)1.0f);
            break;
        case PLY_DOUBLE:
            extract_vetex_data(data, nVerts, (int)1, (double)1.0);
            break;

        }
        break;
    }
    case PLY_UINT:
    {
        switch (m_header.vertex_element.color_property_type_name)
        {
        case PLY_CHAR:
            extract_vetex_data(data, nVerts, (unsigned int)1, (char)1);
            break;
        case PLY_UCHAR:
            extract_vetex_data(data, nVerts, (unsigned int)1, (unsigned char)1);
            break;
        case PLY_SHORT:
            extract_vetex_data(data, nVerts, (unsigned int)1, (short)1);
            break;
        case PLY_USHORT:
            extract_vetex_data(data, nVerts, (unsigned int)1, (unsigned short)1);
            break;
        case PLY_INT:
            extract_vetex_data(data, nVerts, (unsigned int)1, (int)1);
            break;
        case PLY_UINT:
            extract_vetex_data(data, nVerts, (unsigned int)1, (unsigned int)1);
            break;
        case PLY_FLOAT:
            extract_vetex_data(data, nVerts, (unsigned int)1, (float)1.0f);
            break;
        case PLY_DOUBLE:
            extract_vetex_data(data, nVerts, (unsigned int)1, (double)1.0);
            break;


        }
        break;
    }
    case PLY_FLOAT:
    {
        switch (m_header.vertex_element.color_property_type_name)
        {
        case PLY_CHAR:
            extract_vetex_data(data, nVerts, (float)1, (char)1);
            break;
        case PLY_UCHAR:
            extract_vetex_data(data, nVerts, (float)1, (unsigned char)1);
            break;
        case PLY_SHORT:
            extract_vetex_data(data, nVerts, (float)1, (short)1);
            break;
        case PLY_USHORT:
            extract_vetex_data(data, nVerts, (float)1, (unsigned short)1);
            break;
        case PLY_INT:
            extract_vetex_data(data, nVerts, (float)1, (int)1);
            break;
        case PLY_UINT:
            extract_vetex_data(data, nVerts, (float)1, (unsigned int)1);
            break;
        case PLY_FLOAT:
            extract_vetex_data(data, nVerts, (float)1, (float)1.0f);
            break;
        case PLY_DOUBLE:
            extract_vetex_data(data, nVerts, (float)1, (double)1.0);
            break;

        }
        break;
    }
    case PLY_DOUBLE:
    {
        switch (m_header.vertex_element.color_property_type_name)
        {
        case PLY_CHAR:
            extract_vetex_data(data, nVerts, (double)1, (char)1);
            break;
        case PLY_UCHAR:
            extract_vetex_data(data, nVerts, (double)1, (unsigned char)1);
            break;
        case PLY_SHORT:
            extract_vetex_data(data, nVerts, (double)1, (short)1);
            break;
        case PLY_USHORT:
            extract_vetex_data(data, nVerts, (double)1, (unsigned short)1);
            break;
        case PLY_INT:
            extract_vetex_data(data, nVerts, (double)1, (int)1);
            break;
        case PLY_UINT:
            extract_vetex_data(data, nVerts, (double)1, (unsigned int)1);
            break;
        case PLY_FLOAT:
            extract_vetex_data(data, nVerts, (double)1, (float)1.0f);
            break;
        case PLY_DOUBLE:
            extract_vetex_data(data, nVerts, (double)1, (double)1.0);
            break;


        }
        break;
    }
    }
}