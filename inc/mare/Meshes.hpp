#ifndef MESHES
#define MESHES

// MARE
#include "mare/Buffers.hpp"
#include "mare/Components/Transform.hpp"
#include "mare/Entities/Camera.hpp"
#include "mare/Mare.hpp"
#include "mare/Shader.hpp"

// Standard Library
#include <unordered_map>
#include <vector>

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare {
/**
 * @brief Specifies the method of generating points, lines, and triangles from a
 * Geometry Buffer in the Mesh.
 * @details DrawMethod specifies how the data in a Geometry Buffer will be
 * interpreted as a mesh when rendered.
 * @see Mesh
 */
enum class DrawMethod {
  NONE,   /**< Signals no drawing will be done with this mesh.*/
  POINTS, /**< The Mesh's Geometry Buffer will be interpreted as points and each
             vertex will be rendered individually.*/
  LINES,  /**< The Mesh's Geometry Buffer will be interpreted as lines and each
             pair of vertices will be rendered as a separate line.*/
  LINE_STRIP, /**< The Mesh's Geometry Buffer will be interpreted as a strip of
              lines where the first two vertices make a line and each vertex
              following the first two extend the line.*/
  LINE_LOOP,  /**< Same as DrawMethod::LINE_STRIP but the final vertex also
                 creates a line connecting to the first vertex in the Vertex
                 Buffer.*/
  TRIANGLES, /**< Every three consecutive vertices in the Mesh's Geometry Buffer
                define a separate triangle to be rendered.*/
  TRIANGLE_STRIP, /**< The first three vertices in the Mesh's Geometry Buffer
                     define a triangle and every additional vertex in the Vertex
                     Buffer defines another triangle consisting of that vertex
                     and the last two vertices in the Geometry Buffer. */
  TRIANGLE_FAN    /**< The first three vertices in the Mesh's Geometry Buffer
                     define a triangle and every additional vertex in the Vertex
                     Buffer defines another triangle consisting of that vertex
                     the previous vertex, and the first vertex in the Vertex
                     Buffer. */
};

/**
 * @brief An abstract Mesh object containing all of the information required to
 * fully describe the geometry of a renderable object.
 * @details Meshes are combined in pairs with Materials to form *render packets*
 * that can be rendered with the Rendering API. There are three kinds of Meshes:
 *  -SimpleMesh
 *  -CompositeMesh
 *  -InstancedMesh
 *
 * Each kind of Mesh is implemented by the Rendering API.
 * @see Material
 * @see RenderPack
 * @see SimpleMesh
 * @see CompositeMesh
 * @see InstancedMesh
 */
class Mesh : public Transform {
public:
  /**
   * @brief Construct a new Mesh object. Mesh is abstract and the constructor
   * does nothing.
   */
  Mesh() {}
  /**
   * @brief Virtual destructor of the Mesh object.
   */
  virtual ~Mesh() {}

  /**
   * @brief Abstract interface to render a Mesh using a Material from the
   * perspective of a Camera.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   */
  virtual void render(Camera *camera, Material *material) = 0;
  /**
   * @brief Abstract interface to render a Mesh using a Material from the
   * perspective of a Camera and transformed by a parent transformation matrix.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @param parent_model The parent transformation matrix to use.
   */
  virtual void render(Camera *camera, Material *material,
                      glm::mat4 parent_model) = 0;
  /**
   * @brief Abstract interface to render multiple instances of a Mesh using a
   * Material from the perspective of a Camera and transformed by a parent
   * transformation matrix.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @param parent_model The parent transformation matrix to use.
   * @param models The Buffer of transformation matricies to use for instanced.
   * rendering.
   */
  virtual void render(Camera *camera, Material *material,
                      glm::mat4 parent_model, unsigned int instance_count,
                      Buffer<glm::mat4> *models) = 0;
};

/**
 * @brief A basic type of mesh that consists of a single DrawMethod.
 * @details A SimpleMesh is the most basic type of Mesh and contains the
 * information to describe a single piece of geometry that can be rendered with
 * a single Material.
 * @see Mesh
 * @see Material
 */
class SimpleMesh : public Mesh {
public:
  /**
   * @brief Construct a new Simple Mesh object
   *
   */
  SimpleMesh();
  /**
   * @brief Destroy the Simple Mesh object
   *
   */
  ~SimpleMesh();

  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @see Mesh
   */
  void render(Camera *camera, Material *material) override;
  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @param parent_model The parent model matrix to use.
   * @see Mesh
   */
  void render(Camera *camera, Material *material,
              glm::mat4 parent_model) override;
  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @param parent_model The parent transformation matrix to use.
   * @param models The Buffer of transformation matricies to use for instanced.
   * @see Mesh
   */
  void render(Camera *camera, Material *material, glm::mat4 parent_model,
              unsigned int instance_count, Buffer<glm::mat4> *models) override;

  /**
   * @brief Binds the Mesh's Geometry Buffer and render state to the Material.
   * @details This runs the necessary proccesses to link the Geometry Buffer,
   * Index Buffer, and DrawMethod with a Material so that the inputs in the
   * Shader match the data supplied in the Mesh.
   *
   * @param material The material to bind to.
   */
  virtual void bind(Material *material);
  /**
   * @brief Adds a Geometry Buffer to the Mesh.
   * @details Multiple Geometry Buffers can be combined to describe various
   * inputs to a Vertex Shader. All Geometry Buffers must be of type
   * Buffer<float> and have a format set with the Attributes required by the
   * Material that will be used to render the Mesh.
   *
   * @param geometry_buffer The Geometry Buffer to add to the Mesh.
   * @see AttributeType
   * @see BufferAttribute
   * @see BufferFormat
   * @see Material
   */
  virtual void add_geometry_buffer(Referenced<Buffer<float>> geometry_buffer);
  /**
   * @brief Get the geometry buffers fo the Mesh.
   *
   * @return The geometry buffers.
   */
  virtual std::vector<Referenced<Buffer<float>>> get_geometry_buffers();
  /**
   * @brief Set the Index Buffer of the Mesh
   * @details A SimpleMesh can only have a single Index Buffer. The Index Buffer
   * is optional. If an Index Buffer is set, the verticies in the Geometry
   * Buffer will be drawn in the order of the indices given in the Index Buffer.
   * An Index Buffer must be a Buffer of type Buffer<uint32_t>.
   *
   * @param index_buffer The Index Buffer to set.
   */
  virtual void set_index_buffer(Referenced<Buffer<uint32_t>> index_buffer);
  /**
   * @brief If Indexed, returns the number of indices in the Index Buffer. Else,
   * returns the number of vertices in the Geometry Buffer.
   *
   * @return The count of indices to be rendered.
   */
  size_t render_count() const;
  /**
   * @brief Invalidates the cache created from binding the Mesh to a Material.
   * @details Used to invalidate the cache when the render state is modified
   * such as when adding a Geometry Buffer. Render State Cache is created from
   * binding the Mesh to a Material.
   */
  void invalidate_render_state_cache();
  /**
   * @brief Returns true is there is an Index Buffer attached to the Mesh and
   * false otherwise.
   *
   * @return true, there is an Index Buffer attached to the Mesh.
   * @return false, there is not an Index Buffer attached to the Mesh.
   */
  bool is_indexed() const;
  /**
   * @brief Set the DrawMethod of the Mesh.
   *
   * @param method The DrawMethod to use.
   * @see DrawMethod
   */
  void set_draw_method(DrawMethod method);
  /**
   * @brief Get the DrawMethod of the Mesh.
   *
   * @return The DrawMethod
   * @see DrawMethod
   */
  DrawMethod get_draw_method();
  /**
   * @brief Swap the Geometry Buffer with the next Buffer in the Buffer
   * swapchain if the Buffer is multibuffered.
   */
  void swap_buffers();
  /**
   * @brief Get the index of the currently active Buffer in the Geometry Buffer
   * swap chain if the Geometry Buffer is multibuffered.
   *
   * @return The index of the Buffer in the swap chain. 0 if not multibuffered.
   */
  unsigned int get_render_index();
  /**
   * @brief Wait for all Buffer operations submitted before the call to
   * lock_buffers() to complete before proceeding.
   */
  void wait_buffers();
  /**
   * @brief Set a barrier for Buffer operations on the Mesh so that any Buffer
   * operations submitted before the barrier must be completed before proceeding
   * past the next call to wait_buffers().
   */
  void lock_buffers();
  std::unordered_map<uint32_t, uint32_t>
      render_states; //*< An unordered map that maps a shader ID key value to a
                     // render state ID value. The render state ID contains the
                     // information to link data in the Geomerty Buffer to the
                     // shader inputs/*/
  std::vector<Referenced<Buffer<float>>>
      geometry_buffers; /**< A vector of the Geometry Buffer attached to the
                           Mesh.*/
  Referenced<Buffer<uint32_t>>
      index_buffer; /**< The Index Buffer attached to the Mesh if any.*/
  size_t geometry_buffer_count; /**< The number of Geometry Buffers attached to
                                   the Mesh.*/
  size_t
      vertex_render_count; /**< The number of vertices in the Geometry Buffer.*/
  size_t index_render_count; /**< The number of indices in the Index Buffer.*/

protected:
  DrawMethod draw_method_; /**< The DrawMethod of the Mesh.*/
};

/**
 * @brief A CompositeMesh is a collection of other Meshes that can be rendered
 * using a single material.
 * @details Eventually, this will provide some optimizations to batch render
 * Meshes.
 * @see Mesh
 * @see SimpleMesh
 * @see InstancedMesh
 */
class CompositeMesh : public Mesh {
public:
  /**
   * @brief Construct a new CompositeMesh object/
   */
  CompositeMesh() {}
  /**
   * @brief Destroy the CompositeMesh object
   */
  virtual ~CompositeMesh() {}
  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @see Mesh
   */
  void render(Camera *camera, Material *material) override;
  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @param parent_model The parent model matrix to use.
   * @see Mesh
   */
  void render(Camera *camera, Material *material,
              glm::mat4 parent_model) override;
  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @param parent_model The parent transformation matrix to use.
   * @param models The Buffer of transformation matricies to use for instanced.
   * @see Mesh
   */
  void render(Camera *camera, Material *material, glm::mat4 parent_model,
              unsigned int instance_count, Buffer<glm::mat4> *models) override;
  /**
   * @brief Push a Mesh onto the Mesh stack.
   *
   * @param mesh The Mesh to push onto the stack.
   */
  void push_mesh(Referenced<Mesh> mesh);
  /**
   * @brief Remove the last Mesh on the Mesh stack.
   */
  void pop_mesh();
  /**
   * @brief Delete all Meshes on the Mesh stack.
   */
  void clear();
  /**
   * @brief Get the meshes of type <T> in the CompositeMesh.
   *
   * @return The Meshes.
   */
  template <typename T> std::vector<T*> get_meshes() {
    std::vector<T *> meshes{};
    for (auto mesh : meshes_) {
      if (auto m = std::dynamic_pointer_cast<T>(mesh)) {
        meshes.push_back(m.get());
      }
    }
    return meshes;
  }

protected:
  std::vector<Referenced<Mesh>> meshes_{}; /**< The Mesh stack.*/
};

/**
 * @brief An InstancedMesh is used to render multiple instanced of the same
 * Mesh.
 * @details An InstancedMesh can effectivley render instances of SimpleMeshes
 * or CompositeMeshes consisting of SimpleMeshes and CompositeMeshes with
 * only Simple Meshes in their Mesh trees. InstancedMeshes of InstancedMeshes is
 * allowed, but does not benefit from any optimizations.
 * @see Mesh
 * @see SimpleMesh
 * @see CompositeMesh
 */
class InstancedMesh : public Mesh {
public:
  /**
   * @brief Construct a new InstancedMesh object
   *
   * @param max_instances The maximum number of instances allowed.
   */
  InstancedMesh(unsigned int max_instances);
  /**
   * @brief Destroy the InstancedMesh object
   */
  virtual ~InstancedMesh() {}
  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @see Mesh
   */
  void render(Camera *camera, Material *material) override;
  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @param parent_model The parent model matrix to use.
   * @see Mesh
   */
  void render(Camera *camera, Material *material,
              glm::mat4 parent_model) override;
  /**
   * @brief The implementation of the abstract render method supplied by Mesh.
   * @details Rendering an InstancedMesh of InstancedMeshes is supported, but
   * will not benefit from optimizations such as reduced draw calls. Usually
   * this can be avoided anyways.
   *
   * @param camera The Camera to render from.
   * @param material The Material to render with.
   * @param parent_model The parent transformation matrix to use.
   * @param models The Buffer of transformation matricies to use for instanced.
   * @see Mesh
   */
  void render(Camera *camera, Material *material, glm::mat4 parent_model,
              unsigned int instance_count, Buffer<glm::mat4> *models) override;
  /**
   * @brief Set the Mesh to be instanced.
   *
   * @param mesh The Mesh to be instanced.
   */
  void set_mesh(Referenced<Mesh> mesh);
  /**
   * @brief Push an instance transform onto the Transform Buffer.
   *
   * @param model The transform to push onto the Transform Buffer.
   */
  void push_instance(glm::mat4 model);
  /**
   * @brief Remove the last instance transform from the Transform Buffer.
   */
  void pop_instance();
  /**
   * @brief Delete all of the instance transforms on the Transform Buffer.
   */
  void clear_instances();
  /**
   * @brief Insert one or more instance transforms at a time at some place in
   * the Transform Buffer.
   *
   * @param models A vector of Transforms to insert into the Transform Buffer.
   * @param offset The index into the Buffer to start the insertion. In units of
   * number of Transforms.
   */
  void flush_instances(std::vector<glm::mat4> *models, uint32_t offset);
  /**
   * @brief Read an write a Transform to the Transform Buffer using the
   * subscript operator.
   *
   * @param i The index into the Buffer to read or write to.
   * @return A reference to the Transform in the Buffer at the index provided.
   */
  glm::mat4 &operator[](unsigned int i);
  /**
   * @brief Read a Transform from the Transform Buffer using the subscript
   * operator.
   *
   * @param i The index into the Buffer to read from.
   * @return A copy of the Transform in the Transform Buffer at the index
   * provided.
   */
  glm::mat4 operator[](unsigned int i) const;
  /**
   * @brief Get a pointer to the Transform Buffer.
   * @details Used to send the Buffer to a shader when rendering.
   *
   * @return A pointer to the Transform Buffer.
   */
  Buffer<glm::mat4> *get_instance_models();
  /**
   * @brief Swap the instanced Transforms with the Transforms of another
   * Transform Buffer.
   *
   * @param models The Transform Buffer the swap with.
   * @return The Transform Buffer of the InstancedMesh before the swap occured.
   */
  Referenced<Buffer<glm::mat4>>
  swap_instance_models(Referenced<Buffer<glm::mat4>> models);
  /**
   * @brief Replace the Transform Buffer with another Transform Buffer.
   *
   * @param models The Transform Buffer used to replace the current Transform
   * Buffer.
   */
  void set_instance_models(Referenced<Buffer<glm::mat4>> models);

protected:
  unsigned int instance_count_; /**< The current number of instances.*/
  Referenced<Buffer<glm::mat4>>
      instance_transforms_;    /**< The Transform Buffer.*/
  Referenced<Mesh> mesh_;          /**< The Mesh that is instanced.*/
  unsigned int max_instances_; /**< The maximum number of instances allowed.*/
};

} // namespace mare

#endif