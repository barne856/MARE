#ifndef SHADER
#define SHADER

// MARE
#include "mare/Buffers.hpp"
#include "mare/Mare.hpp"
#include "mare/Components/Transform.hpp"

// External Libraries
#include "glm.hpp"

// Standard Library
#include <unordered_map>

namespace mare {
// forward declarations
class Camera;
class Mesh;

/**
 * @brief The type of Barrier to issue from the
 * ComputeProgram::barrier(BarrierType) function.
 * @details After the barrier is placed by calling
 * ComputeProgram::barrier(BarrierType), the data in all Buffers will reflect
 * any previous operations of type BarrierType in all future Buffer operations.
 */
enum class BarrierType {
  ATTRIBUTE,      /**< Specifies vertex data supplied as input attributes to
                     shaders.*/
  INDEX,          /**< Specifies index data from Index Buffers.*/
  UNIFORM,        /**< Specifies uniforms sourced from Buffers.*/
  TEXTURE_FETCH,  /**< Specifies textures sourced from Buffers.*/
  IMAGE,          /**< Specifies images sourced from Buffers.*/
  COMMAND,        /**< Specifies commands sourced from Buffers.*/
  BUFFER_UPDATE,  /**< Specifies reads or writes to any Buffer object.*/
  FRAMEBUFFER,    /**< Specifies framebuffers sourced from Buffers.*/
  ATOMIC_COUNTER, /**< Specifies accesses to atomic counters.*/
  STORAGE,        /**< Specifies Shader Storage Buffers objects.*/
  ALL             /**< Specifies any Buffer operation or modification.*/
};

/**
 * @brief An abstract Shader class implemented by the API.
 * @details Used as a Base for ShaderPrograms and Materials. Provides functions
 * to upload data to shaders. The implementation will compile glsl files into a
 * shader program.
 */
class Shader {
public:
  /**
   * @brief Abstract Constructor of a Shader object
   */
  Shader() {}
  /**
   * @brief Virtual destructor of the Shader object
   */
  virtual ~Shader() {}
  /**
   * @brief Get the unique ID assigned to the Shader by the implemented
   * Rendering API.
   *
   * @return The unique ID of the Shader.
   */
  inline uint32_t name() { return shader_ID_; }
  /**
   * @brief Abstract function implemented by the Rendering API that will enable
   * the Shader for rendering to and diable any previously enabled Shader.
   */
  virtual void use() const = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a 32-bit floating point number as a uniform to a glsl Shader.
   *
   * @param name The name of the uniform in the glsl shader.
   * @param value The floating point number to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  virtual void upload_float(const char *name, float value,
                            bool suppress_warnings = false) = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a value to a glsl Shader uniform.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  virtual void upload_vec3(const char *name, glm::vec3 value,
                           bool suppress_warnings = false) = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a value to a glsl Shader uniform.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  virtual void upload_vec4(const char *name, glm::vec4 value,
                           bool suppress_warnings = false) = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a value to a glsl Shader uniform.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  virtual void upload_mat3(const char *name, glm::mat3 value,
                           bool suppress_warnings = false) = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a value to a glsl Shader uniform.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  virtual void upload_mat4(const char *name, glm::mat4 value,
                           bool suppress_warnings = false) = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a uniform Buffer to a glsl Shader uniform.
   * @details The uniform buffer block should use the std140 layout.
   *
   * @param name The name of the uniform buffer block in the glsl Shader.
   * @param uniform A pointer to the Buffer to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform buffer block.
   */
  virtual void upload_uniform(const char *name, IBuffer *uniform,
                              bool suppress_warnings = false) = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a storage Buffer to a glsl Shader uniform.
   * @details The storage buffer block should use the std430 layout.
   *
   * @param name The name of the storage buffer block in the glsl Shader.
   * @param storage A pointer to the Buffer to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the storage buffer block.
   */
  virtual void upload_storage(const char *name, IBuffer *storage,
                              bool suppress_warnings = false) = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a texture2D to a glsl Shader uniform sampler2D.
   *
   * @param name The name of the uniform sampler2D in the glsl Shader.
   * @param texture2D A pointer to a Texture2D to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform sampler2D.
   */
  virtual void upload_texture2D(const char *name, Texture2D *texture2D,
                                bool suppress_warnings = false) = 0;
  /**
   * @brief Abstract function implemented by the Rendering API that will upload
   * a texture2D to a glsl Shader uniform image2D.
   *
   * @param name The name of the uniform image2D in the glsl Shader.
   * @param texture2D A pointer to a Texture2D to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform image2D.
   */
  virtual void upload_image2D(const char *name, Texture2D *texture2D,
                              bool suppress_warnings = false) = 0;

  /**
   * @brief Abstract function implemented by the Rendering API that will place
   * barriers in code where buffer operation must be completed before
   * continuing.
   *
   * @param type The BarrierType to use.
   * @see BarrierType
   */
  virtual void barrier(BarrierType type) = 0;

protected:
  uint32_t shader_ID_; /**< The unique ID for the Shader assigned by the
                          implemented Rendering API.*/
};

/**
 * @brief A compiled glsl shader program.
 * @details The class manages the creation of Shaders so that the same Shader is
 * not compiled more than once.
 * @see Shader
 */
class ShaderProgram {
public:
  /**
   * @brief Construct a new ShaderProgram object
   * @param directory The directory of the glsl Shader files.
   * @see Shader
   */
  ShaderProgram(const char *directory);
  /**
   * @brief Virtual destructor of the ShaderProgram object
   */
  virtual ~ShaderProgram(){};
  /**
   * @brief Bind the ShaderProgram so that is will be used for rendering. Any
   * previously bound ShaderProgram will be unbound.
   */
  inline void bind() const { shader_->use(); }
  /**
   * @brief Get the unique ID generated by the Rendering API for the Shader.
   *
   * @return The unique ID generated by the Rendering API for the Shader.
   */
  inline unsigned int name() { return shader_->name(); }
  /**
   * @brief Upload a float to the ShaderProgram. The ShaderProgram must be bound
   * first.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_float(const char *name, float value,
                    bool suppress_warnings = false) {
    shader_->upload_float(name, value, suppress_warnings);
  }
  /**
   * @brief Upload a vec3 to the ShaderProgram. The ShaderProgram must be bound
   * first.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_vec3(const char *name, glm::vec3 value,
                   bool suppress_warnings = false) {
    shader_->upload_vec3(name, value, suppress_warnings);
  }
  /**
   * @brief Upload a vec4 to the ShaderProgram. The ShaderProgram must be bound
   * first.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_vec4(const char *name, glm::vec4 value,
                   bool suppress_warnings = false) {
    shader_->upload_vec4(name, value, suppress_warnings);
  }
  /**
   * @brief Upload a mat3 to the ShaderProgram. The ShaderProgram must be bound
   * first.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_mat3(const char *name, glm::mat3 value,
                   bool suppress_warnings = false) {
    shader_->upload_mat3(name, value, suppress_warnings);
  }
  /**
   * @brief Upload a mat4 to the ShaderProgram. The ShaderProgram must be bound
   * first.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_mat4(const char *name, glm::mat4 value,
                   bool suppress_warnings = false) {
    shader_->upload_mat4(name, value, suppress_warnings);
  }
  /**
   * @brief Upload a Uniform Buffer to the ShaderProgram. The ShaderProgram must
   * be bound first.
   *
   * @param name The name of the uniform buffer block in the glsl Shader.
   * @param uniform A pointer to a Buffer to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform buffer block.
   */
  void upload_uniform(const char *name, IBuffer *uniform,
                      bool suppress_warnings = false) {
    shader_->upload_uniform(name, uniform, suppress_warnings);
  }
  /**
   * @brief Upload a Storage Buffer to the ShaderProgram. The ShaderProgram must
   * be bound first.
   *
   * @param name The name of the storage buffer block in the glsl Shader.
   * @param storage A pointer to a Buffer to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the storage buffer block.
   */
  void upload_storage(const char *name, IBuffer *storage,
                      bool suppress_warnings = false) {
    shader_->upload_storage(name, storage, suppress_warnings);
  }
  /**
   * @brief Upload a Texture2D to the ShaderProgram. The ShaderProgram must
   * be bound first.
   *
   * @param name The name of the uniform sampler2D in the glsl Shader.
   * @param texture2D A pointer to a Texture2D to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform sampler2D.
   */
  void upload_texture2D(const char *name, Texture2D *texture2D,
                        bool suppress_warnings = false) {
    shader_->upload_texture2D(name, texture2D, suppress_warnings);
  }
  /**
   * @brief Upload a Texture2D to the ShaderProgram. The ShaderProgram must
   * be bound first.
   *
   * @param name The name of the uniform image2D in the glsl Shader.
   * @param texture2D A pointer to a Texture2D to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform image2D.
   */
  void upload_image2D(const char *name, Texture2D *texture2D,
                      bool suppress_warnings = false) {
    shader_->upload_image2D(name, texture2D, suppress_warnings);
  }

protected:
  Referenced<Shader> shader_; /**< The Referenced Shader.*/

private:
  static std::unordered_map<std::string, Referenced<Shader>>
      shader_cache_; /**< The cache of compiled Shaders.*/
};

/**
 * @brief An interface to a GraphicsProgram that provides a render() function.
 */
class GraphicsProgram : public ShaderProgram {
public:
  /**
   * @brief Construct a new GraphicsProgram object
   *
   * @param directory The directory of the glsl Shader files.
   */
  GraphicsProgram(const char *directory) : ShaderProgram(directory) {}
  /**
   * @brief Virtual destructor of the GraphicsProgram object
   */
  virtual ~GraphicsProgram() {}
  /**
   * @brief A virtual render method that can be implemented in any derived
   * class.
   */
  virtual void render() {}
};

/**
 * @brief An interface to a ComputeProgram that provides a compute() function.
 */
class ComputeProgram : public ShaderProgram {
public:
  /**
   * @brief Construct a new ComputeProgram object
   *
   * @param directory The directory of the glsl Shader files.
   */
  ComputeProgram(const char *directory) : ShaderProgram(directory) {}
  /**
   * @brief Virtual destructor of the ComputeProgram object
   *
   */
  virtual ~ComputeProgram() {}
  /**
   * @brief Calls the implemented Rendering API's dispatch_compute() function to
   * run a ComputeShader's compiled shader program.
   * @details This function also calls the ComputeShader::compute() function.
   *
   * @param x The amount of global workgroups to dispatch in the x dimension.
   * @param y The amount of global workgroups to dispatch in the y dimension.
   * @param z The amount of global workgroups to dispatch in the z dimension.
   */
  void dispatch_compute(uint32_t x = 1, uint32_t y = 1, uint32_t z = 1);
  /**
   * @brief A virtual compute method that can be implemented in any derived
   * class.
   */
  virtual void compute() {}
  /**
   * @brief Place a barrier of type BarrierType.
   *
   * @param type The BarrierType
   * @see BarrierType
   */
  void barrier(BarrierType type);
};

/**
 * @brief A class providing an interface to a Material object.
 * @details A Material is a GraphicsProgram that provides some more functions
 * for uploading Meshes anc Cameras. Materials can be paired with a Mesh to form
 * a RenderPacket.
 * @see RenderPack
 * @see Mesh
 * @see GraphicsProgram
 */
class Material : public GraphicsProgram {
public:
  /**
   * @brief Construct a new Material object.
   *
   * @param directory The directory of the glsl Shader files.
   */
  Material(const char *directory) : GraphicsProgram(directory) {}
  /**
   * @brief Virtual destructor of the Material object
   */
  virtual ~Material() {}
  /**
   * @brief Upload a Camera to the Material. The Material must be bound first.
   * @details The Material's glsl shader must have two uniform mat4 objects
   * named "projection" and "view".
   *
   * @param camera The Camera to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the Camera uniforms.
   * @see Camera
   */
  void upload_camera(Camera *camera, bool suppress_warnings = false);
  /**
   * @brief Upload a Mesh's Transform or model matrix to the Material. The
   * Material must be bound first.
   * @details The Material's glsl shader must have a uniform mat4 named "model".
   *
   * @param mesh The Mesh whose model matrix will be uploaded.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the model matrix uniform.
   * @see Mesh
   */
  void upload_mesh_model_matrix(Mesh *mesh, bool suppress_warnings = false);
  /**
   * @brief Upload a Mesh's Transform or model matrix to the Material after
   * being trnasformed by a parent model matrix. The Material must be bound
   * first.
   * @details The Material's glsl shader must have a uniform mat4 named "model".
   *
   * @param mesh The Mesh whose model matrix will be uploaded after being
   * transformed by the parent model matrix.
   * @param parent_transform The parent Transform Component.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the model matrix uniform.
   * @see Mesh
   */
  void upload_mesh_model_matrix(Mesh *mesh, Transform* parent_transform,
                                bool suppress_warnings = false);
  /**
   * @brief Upload a Mesh's normal matrix to the Material after. The Material
   * must be bound first.
   *
   * @param mesh The Mesh whose normal matrix will be uploaded.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the normal matrix uniform.
   * @see Mesh
   */
  void upload_mesh_normal_matrix(Mesh *mesh, bool suppress_warnings = false);
  /**
   * @brief Upload a Mesh's normal matrix to the Material after being
   * transformed by the parent model matrix. The Material must be bound
   * first.
   * @details The Material's glsl shader must have a uniform mat4 named
   * "normal_matrix".
   *
   * @param mesh The Mesh whose normal matrix will be uploaded after being
   * transformed by the parent model matrix.
   * @param parent_transform The parent Transform Component.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the normal matrix uniform.
   * @see Mesh
   */
  void upload_mesh_normal_matrix(Mesh *mesh, Transform* parent_transform,
                                 bool suppress_warnings = false);
  /**
   * @brief Upload a Transform Buffer to the Material.
   * @details The Material's glsl shader must have a shader storage block using
   * the std430 layout named "model_instances".
   *
   * @param models A pointer to the Transform Buffer to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the shader storage block.
   */
  void upload_mesh_instance_matrices(Buffer<Transform> *models,
                                     bool suppress_warnings = false);
  /**
   * @brief A convience function to call all the neccesary functions to upload a
   * Mesh to the Material.
   * @details Calls upload_mesh_model_matrix(Mesh*, bool) and
   * upload_mesh_normal_matrix(Mesh*, bool)
   *
   * @param mesh The Mesh to upload
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the Shader resources.
   */
  void upload_mesh(Mesh *mesh, bool suppress_warnings = false);
  /**
   * @brief A convience function to call all the neccesary functions to upload a
   * Mesh to the Material.
   * @details Calls upload_mesh_model_matrix(Mesh*, glm::mat4, bool) and
   * upload_mesh_normal_matrix(Mesh*, glm::mat4, bool)
   *
   * @param mesh The Mesh to upload.
   * @param parent_transform The parent Transform Component.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the Shader resources.
   */
  void upload_mesh(Mesh *mesh, Transform* parent_transform,
                   bool suppress_warnings = false);
  /**
   * @brief A convience function to call all the neccesary functions to upload a
   * Mesh to the Material.
   * @details Calls upload_mesh_model_matrix(Mesh*, glm::mat4, bool),
   * upload_mesh_normal_matrix(Mesh*, glm::mat4, bool), and
   * upload_mesh_instance_matrices(Buffer<glm::mat4>*,bool)
   *
   * @param mesh The Mesh to upload.
   * @param parent_transform The parent Transform Component.
   * @param models A pointer to a Transform Buffer to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the Shader resources.
   */
  void upload_mesh(Mesh *mesh, Transform* parent_transform,
                   Buffer<Transform> *models, bool suppress_warnings = false);
};
} // namespace mare

#endif