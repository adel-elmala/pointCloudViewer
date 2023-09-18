#include "../include/parser.h"
#include <iostream>

parser::parser(const std::string &file_path)
{

  // Create an instance of the Importer class

  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // probably to request more postprocessing than we do in this example.
  m_scene = const_cast<aiScene *>(m_importer.ReadFile(file_path, 0));

  // If the import failed, report it
  if (nullptr == m_scene)
  {
    std::cerr << "Failed To Open File: " << file_path << std::endl;
    // DoTheErrorLogging( importer.GetErrorString());
  }
  // Now we can access the file's contents.
  m_verticies = (*(m_scene->mMeshes))->mVertices;
  m_nVerts = (*(m_scene->mMeshes))->mNumVertices;
  // We're done. Everything will be cleaned up by the importer destructor
}
parser::~parser()
{
}