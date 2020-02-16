#ifndef SPHEREMESH
#define SPHEREMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

namespace mare
{
class SphereMesh : public SimpleMesh
{
public:
    SphereMesh(unsigned int recursionLevel, float radius, bool force_flat = false)
    {
        render_state->set_draw_method(DrawMethod::TRIANGLES);

        const float X = 0.525731112119133606f;
        const float Z = 0.850650808352039932f;
        const float N = 0.0f;

        float verts[36] = {-X, N, Z, X, N, Z, -X, N, -Z, X, N, -Z,
                           N, Z, X, N, Z, -X, N, -Z, X, N, -Z, -X,
                           Z, X, N, -Z, X, N, Z, -X, N, -Z, -X, N};

        unsigned int indes[60] = {0, 4, 1, 0, 9, 4, 9, 5, 4, 4, 5, 8, 4, 8, 1,
                                  8, 10, 1, 8, 3, 10, 5, 3, 8, 5, 2, 3, 2, 7, 3,
                                  7, 10, 3, 7, 6, 10, 7, 11, 6, 11, 0, 6, 0, 1, 6,
                                  6, 1, 10, 9, 0, 11, 9, 11, 2, 9, 2, 5, 7, 2, 11};

        int count = 180 * (int)pow(4, recursionLevel);

        std::vector<glm::vec3> vertices;
        vertices.reserve(count / 3);
        for (int i = 0; i < count; ++i)
        {
            vertices.push_back(glm::vec3(0.0f));
        }

        unsigned int iter = 0;

        for (int i = 0; i < 20; i++)
        {
            glm::vec3 v1 = {verts[3 * indes[3 * i]], verts[3 * indes[3 * i] + 1],
                            verts[3 * indes[3 * i] + 2]};
            glm::vec3 v2 = {verts[3 * indes[3 * i + 1]], verts[3 * indes[3 * i + 1] + 1],
                            verts[3 * indes[3 * i + 1] + 2]};
            glm::vec3 v3 = {verts[3 * indes[3 * i + 2]], verts[3 * indes[3 * i + 2] + 1],
                            verts[3 * indes[3 * i + 2] + 2]};
            subdivide(v1, v2, v3, recursionLevel, vertices, iter);
        }

        std::vector<float> data;
        data.reserve(count * 2);
        for (int i = 0; i < count / 9; i++)
        {
            glm::vec3 v1 = radius * vertices[3 * i];
            glm::vec3 v2 = radius * vertices[3 * i + 1];
            glm::vec3 v3 = radius * vertices[3 * i + 2];
            if (force_flat)
            {
                glm::vec3 n = glm::normalize(glm::cross(v3 - v1, v2 - v1));
                data.push_back(v3[0]);
                data.push_back(v3[1]);
                data.push_back(v3[2]);
                data.push_back(n[0]);
                data.push_back(n[1]);
                data.push_back(n[2]);
                data.push_back(v2[0]);
                data.push_back(v2[1]);
                data.push_back(v2[2]);
                data.push_back(n[0]);
                data.push_back(n[1]);
                data.push_back(n[2]);
                data.push_back(v1[0]);
                data.push_back(v1[1]);
                data.push_back(v1[2]);
                data.push_back(n[0]);
                data.push_back(n[1]);
                data.push_back(n[2]);
                continue;
            }
            data.push_back(v3[0]);
            data.push_back(v3[1]);
            data.push_back(v3[2]);
            data.push_back(vertices[3 * i + 2][0]);
            data.push_back(vertices[3 * i + 2][1]);
            data.push_back(vertices[3 * i + 2][2]);
            data.push_back(v2[0]);
            data.push_back(v2[1]);
            data.push_back(v2[2]);
            data.push_back(vertices[3 * i + 1][0]);
            data.push_back(vertices[3 * i + 1][1]);
            data.push_back(vertices[3 * i + 1][2]);
            data.push_back(v1[0]);
            data.push_back(v1[1]);
            data.push_back(v1[2]);
            data.push_back(vertices[3 * i][0]);
            data.push_back(vertices[3 * i][1]);
            data.push_back(vertices[3 * i][2]);
        }

        Buffer<float> *vertex_buffer = Renderer::API->GenFloatBuffer(&data);
        vertex_buffer->set_format({{ShaderDataType::VEC3, "position"},
                                   {ShaderDataType::VEC3, "normal"}});

        render_state->set_vertex_buffer(vertex_buffer);
    }

private:
    void subdivide(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3, int depth,
                   std::vector<glm::vec3> &vertices, unsigned int &iter)
    {
        glm::vec3 v12, v23, v31;

        if (depth == 0)
        {
            vertices[iter] = v1;
            iter++;
            vertices[iter] = v2;
            iter++;
            vertices[iter] = v3;
            iter++;
            return;
        }

        // calculate midpoints of each side
        for (int i = 0; i < 3; i++)
        {
            v12[i] = (v1[i] + v2[i]) / 2.0f;
            v23[i] = (v2[i] + v3[i]) / 2.0f;
            v31[i] = (v3[i] + v1[i]) / 2.0f;
        }
        // extrude midpoints to lie on unit sphere
        v12 = glm::normalize(v12);
        v23 = glm::normalize(v23);
        v31 = glm::normalize(v31);

        // recursively subdivide new triangles
        subdivide(v1, v12, v31, depth - 1, vertices, iter);
        subdivide(v2, v23, v12, depth - 1, vertices, iter);
        subdivide(v3, v31, v23, depth - 1, vertices, iter);
        subdivide(v12, v23, v31, depth - 1, vertices, iter);
    }
};
} // namespace mare

#endif