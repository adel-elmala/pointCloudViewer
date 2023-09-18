#ifndef PARSER2_H
#define PARSER2_H

#include <string>
#include "tinyply.h"

using namespace tinyply;
struct float2
{
    float x, y;
};
struct float3
{
    float x, y, z;
};
struct double3
{
    double x, y, z;
};
struct uint3
{
    uint32_t x, y, z;
};
struct uint4
{
    uint32_t x, y, z, w;
};

struct geometry
{
    std::vector<float3> vertices;
    std::vector<float3> normals;
    std::vector<float2> texcoords;
    std::vector<uint3> triangles;
};
class parser2
{
public:
    parser2();
    ~parser2();
    void read_file(const std::string &filepath, const bool preload_into_memory = true);

    /* data */
    std::vector<float3> verts;
};

#endif