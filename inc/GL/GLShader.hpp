#ifndef GLSHADER
#define GLSHADER

// OpenGL
#include <GL/glew.h>

// Standard Library
#include <string>
#include <unordered_map>
#include <vector>

// MARE
#include "Buffers.hpp"
#include "Shader.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief The OpenGL 4.5 implementation of the Shader class.
 * @see Shader
 */
class GLShader : public Shader {
public:
  /**
   * @brief Construct a new GLShader object
   *
   * @param directory The directory of the glsl shader files that contain the
   * glsl shader program.
   */
  GLShader(const char *directory);
  /**
   * @brief Destroy the GLShader object
   */
  ~GLShader();
  /**
   * @brief Bind the GLShader so that it can be used for rendering. This will
   * unbind and previously bound Shader.
   * @see Shader
   */
  inline void use() const override { glUseProgram(shader_ID_); }
  /**
   * @brief utility function to decode a OpenGL shader type to a string.
   *
   * @param type The OpenGL shader type GLenum.
   * @return The decoded string.
   */
  const static std::string type_to_name(GLenum type);
  /**
   * @brief An unordered map with the glsl file extension used as a key to a
   * GLenum describing the type of glsl shader.
   * @details The following glsl extensions are used to determine the shader
   * type:
   *    - .vert -> Vertex Shader
   *    - .tese -> Tesselation Evaluation Shader
   *    - .tesc -> Tesselation Control Shader
   *    - .geom -> Geometry Shader
   *    - .frag -> Fragment Shader
   *    - .comp -> Compute Shader
   *
   */
  const static std::unordered_map<std::string, GLenum> shader_extension;

  /**
   * @brief Upload a 32-bit floating point number as a uniform to a glsl Shader.
   *
   * @param name The name of the uniform in the glsl shader.
   * @param value The floating point number to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_float(const char *name, float value,
                    bool suppress_warnings = false) override;
  /**
   * @brief Upload a value to a glsl Shader uniform.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_vec3(const char *name, glm::vec3 value,
                   bool suppress_warnings = false) override;
  /**
   * @brief Upload a value to a glsl Shader uniform.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_vec4(const char *name, glm::vec4 value,
                   bool suppress_warnings = false) override;
  /**
   * @brief Upload a value to a glsl Shader uniform.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_mat3(const char *name, glm::mat3 value,
                   bool suppress_warnings = false) override;
  /**
   * @brief Upload a value to a glsl Shader uniform.
   *
   * @param name The name of the uniform in the glsl Shader.
   * @param value The value to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform.
   */
  void upload_mat4(const char *name, glm::mat4 value,
                   bool suppress_warnings = false) override;
  /**
   * @brief Upload a uniform Buffer to a glsl Shader uniform.
   * @details The uniform buffer block should use the std140 layout.
   *
   * @param name The name of the uniform buffer block in the glsl Shader.
   * @param uniform A pointer to the Buffer to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform buffer block.
   */
  virtual void upload_uniform(const char *name, IBuffer *uniform,
                              bool suppress_warnings = false) override;
  /**
   * @brief Upload a storage Buffer to a glsl Shader uniform.
   * @details The storage buffer block should use the std430 layout.
   *
   * @param name The name of the storage buffer block in the glsl Shader.
   * @param storage A pointer to the Buffer to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the storage buffer block.
   */
  virtual void upload_storage(const char *name, IBuffer *storage,
                              bool suppress_warnings = false) override;
  /**
   * @brief Upload a texture2D to a glsl Shader uniform sampler2D.
   *
   * @param name The name of the uniform sampler2D in the glsl Shader.
   * @param texture2D A pointer to a Texture2D to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform sampler2D.
   */
  virtual void upload_texture2D(const char *name, Texture2D *texture2D,
                                bool suppress_warnings = false) override;
  /**
   * @brief Upload a texture2D to a glsl Shader uniform image2D.
   *
   * @param name The name of the uniform image2D in the glsl Shader.
   * @param texture2D A pointer to a Texture2D to upload.
   * @param suppress_warnings true enables reporting of warnings from the Engine
   * regarding the existence of the uniform image2D.
   */
  virtual void upload_image2D(const char *name, Texture2D *texture2D,
                              bool suppress_warnings = false) override;

  /**
   * @brief Place barriers in code where buffer operation must be completed
   * before continuing.
   *
   * @param type The BarrierType to use.
   * @see BarrierType
   */
  virtual void barrier(BarrierType type) override;

private:
  std::vector<GLuint> shaders_; /**< The container of each shader making up the
                                   entire program.*/
  /**
   * @brief Function to read a glsl shader file into a string.
   *
   * @param shader_path The filepath of the shader file.
   * @param source A reference to a string where the contents of the read file
   * will be stored.
   */
  void read_shader(std::string shader_path, std::string &source);
  /**
   * @brief Function to compile a glsl shader using OpenGL.
   *
   * @param shader_source The shader source code.
   * @param SHADER_TYPE The type of shader to compile.
   * @return GLuint The OpenGL generated ID of the compiled shader.
   */
  GLuint compile_shader(std::string &shader_source, GLenum SHADER_TYPE);
  /**
   * @brief Create a program object from the compiled shaders.
   *
   * @return GLuint The OpenGL generated ID of the linked shader program.
   */
  GLuint create_program();
  /**
   * @brief Function used on creation of the GLShader to read the glsl files
   * from a directory, compile the code, and link and generate a handle to a new
   * shader program. The files in the directory or interpreted accroding to
   * their extensions as specified in GLShader::type_to_name(GLenum).
   *
   * @param directory The directory where the glsl source code files are stored.
   * @see GLShader::type_to_name(GLenum)
   */
  void init_shader(const char *directory);
  std::unordered_map<std::string, GLint>
      resource_location_cache_; /**< The cahced location of the resources in the
                          shader program.*/
  std::unordered_map<std::string, GLuint>
      resource_index_cache_; /**< The cahced index of the resources in the
                          shader program.*/
  std::unordered_map<std::string, GLuint>
      uniform_binding_cache_; /**< The cahced location of the uniform block
                                 binding locations for the shader program.*/
  std::unordered_map<std::string, GLuint>
      storage_binding_cache_; /**< The cahced location of the storage block
binding locations for the shader program.*/
  std::unordered_map<std::string, GLuint>
      texture_binding_cache_; /**< The cahced location of the texture
binding locations for the shader program.*/
  std::unordered_map<std::string, GLuint>
      image_binding_cache_; /**< The cahced location of the image
binding locations for the shader program.*/
};
} // namespace mare

#endif