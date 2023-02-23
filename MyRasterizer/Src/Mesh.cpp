#include "Mesh.h"

My::Mesh::Mesh(const std::vector<Vertex>& p_vertices, const std::vector<int> p_indices)
{
    this->m_vertices = p_vertices;
    this->m_indices = p_indices;
}

My::Mesh* My::Mesh::CreateCube()
{
    // TODO : Create Cube
    return nullptr;
}

My::Mesh* My::Mesh::CreateSphere(int latitudeCount, int longitudeCount)
{
    // TODO : Create Sphere
    return nullptr;
}
