#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

class parser
{
private:
    /* data */
    Assimp::Importer m_importer;

public:
    parser(const std::string &file_path);
    ~parser();
    aiScene *m_scene = nullptr;
    aiVector3t<float> *m_verticies = nullptr; // array holding the mesh verticies
    int m_nVerts = 0;
};

#endif