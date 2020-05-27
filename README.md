# MARE 1.0.0
MARE stands for *Minimal Abstraction Rendering Engine*. MARE aims to provide an easy to use and extremely flexible 3D rendering framework using the minimum possible amount of abstractions.

 MARE provides the user with access to lower level rendering concepts while removing much of the boilerplate code typically needed when using a low level 3D rendering API. This allows the user to easily combine these low level concepts with higher level concepts when needed to implement a specific or novel rendering technique. Ease of use, simplicity, and flexibility of the API are favored over performance but only when neccessary and MARE remains quite fast.
 
 MARE can be used for quickly prototyping real time 3D rendering techniques in C++ or creating complex full-fledged 3D applications that are not performance critical. MARE provides sets of standard Meshes, Materials, Components, and Systems that are useful for getting started, but also allows the user to create their own implementations of these using header files. Currently, MARE implementes the modern OpenGL 4.5 Rendering API using C++17 features.

![Alt text](/res/Screenshots/ShadowMapping.png)

## Setup
### Clone the Library with its Dependencies
All of the external libraries used by MARE are either included in the project as git submodules or directly included in the project source code. The following is a list of the dependencies and requirements for using MARE:
* GLFW (git submodule, zlib license) - Used for windowing and input.
* GLM (git submodule, MIT license) - Used for linear algebra.
* GLEW (included in the project, MIT license and Modified BSD license) - Used to load OpenGL extensions.
* STB Image Loader (included in the project, MIT license) - Used to load image files.
* OpenGL 4.5 capable harware.
* C++17 compiler (tested on GCC 9.3.0 and MSVC 2019)
* git
* CMake

If you are running GNU/Linux make sure you have the development files for the following libraries that are used by GLFW for windowing and input:
* X11
* pthread
* dl

MARE itself is licensed under the MIT license. Assuming your system meets the above requirements you should only need to clone the project with its submodules to get everything you need to use MARE. This can be accomplished with the following git command:

    git clone --recurse-submodules -j8 https://github.com/barne856/MARE.git

### Build with CMake
MARE uses CMake as its build system. To build MARE and a simple example Scene, run the following commands from the root directory:

    mkdir build
    cd build
    cmake ..
    cd ..
    cmake --build ./build

This will generate a binary executable in the build folder. The executable needs to be run from the root directory to work properly. This is because all the filepaths to textures and shaders used in the examples are relative.

### Documentation
The MARE API is fully documented with Doxygen. To build the documentation make sure you have doxygen installed and run the following command from the root directory:

    doxygen Doxyfile.dox

This will generate the HTML documentation in the /doc/html folder and the latex output will be generated in /doc/latex. If a PDF is prefered to HTML, a latex compiler is needed to build a PDF from the generated latex files.

## Engine Overview
MARE is structured as a tree with each level of the tree containing an ordered stack (an `std::vector`) of a specific class. The `Renderer` is the root of the tree. The `Renderer` contains `Scene` objects, the `Scene` objects contain `Layer` objects, the `Layer` objects contain `Entity` objects, and the `Entity` objects contain `System` objects.

### Entity Component System
MARE does not use a typical data-oriented Entity-Component-System (ECS), but Entities, Components, and Systems are used extensively.

To somewhat complicate things, but to great benefit, the `Layer` class inherits from the `Entity` class through the `Camera` class and the `Scene` class inherits from the `Layer` class. This allows Scenes and Layers to also contain Systems. These Systems typically operate on the Scene itself or on a subset of the Entities in the Scene.

![Alt text](/res/Screenshots/layer.png)

All useful Entities inherit from `Component` objects and the Systems attached to these Entities operate on those Components the Entity inherits from. Comonents provide data and methods to operate on the data.

Systems provide callbacks that operate on the Components of the Entity that they are attached to. There are three types of Systems that can be implemented by the user in addition to the standard Systems provided by MARE.

* ControlsSystems
* RenderSystems
* PhysicsSystems

ControlsSystems provide callbacks to input events that are called whenever an input event is recorded by the Engine. The parameters to the callbacks are a reference to the input data, and a variable number of pointers to all of the Components that the System operates on. This is accomplished through templates.

    on_key(RendererInput& input, Component* comps...)
    on_mouse_button(RendererInput& input, Component* comps...)
    on_mouse_move(RendererInput& input, Component* comps...)
    on_mouse_wheel(RendererInput& input, Component* comps...)
    on_resize(RendererInput& input, Component* comps...)

Render Systems provide a render callback function that is called on each frame, it takes as input the time in seconds since the previous frame, a pointer to a `Camera` to render with, and a variable number of pointers to all of the Components that the System operates on. This is accomplished through templates.

    render(float dt, Camera* camera, Component* comps...)

Physics Systems provide an update callback function that is called on each frame, it takes as input the time in seconds since the previous frame and a variable number of pointers to all of the Components that the System operates on. This is accomplished though templates.

    update(float dt, Component* comps...)

MARE gives the user the flexibilty and responsibility to organize their code how they want for the most part and therefore there are many ways a user can accomplish the same task. Sharing Components and Systems between similar Entites or creating specific Components and Systems for a particular Entity are both allowed. This provides much flexibilty when needed and allows code reuse when desired.

RenderSystems and PhysicsSystems are executed in forwards order from the begining of the stacks while ControlsSystems are executed in reverse order from the end of the stacks. This allows ControlsSystems to *handle* events and stop input propogation down through multiple Layers.

## Examples
### Renderer Specification
The first step to creating an application using MARE is to create a specificaiton of a Renderer. At the moment, MARE only supports OpenGL 4.5 which is implemented in the `GLRenderer` class. Your specification of the GLRenderer class will set the global properties of the application, create the Scenes to render, and load the first Scene. See the `main.cpp` file for the full code.
```C++
class Sandbox : public mare::GLRenderer
{
public:
    // set the Renderer properties
    void init_info() override
    {
        info.window_title = "Sandbox";     // Window title
        info.window_width = 1280;          // window width in pixels
        info.window_height = 720;          // window height in pixels
        info.window_aspect = 16.0f / 9.0f; // window aspect ratio
        info.samples = 8;                  // antialiasing samples
        info.fullscreen = false;           // render in fullscreen mode?
        info.vsync = true;                 // render vsync mode?
        info.debug_mode = 1;               // report all OpenGL errors
    }
    // perform startup operations and launch the first Scene.
    void startup() override
    {
        using namespace mare;
        // Generate a new Scene and push it onto the Scene Stack.
        auto layer = gen_scene<SampleShadowScene>();
        // Load the first Scene on the Scene Stack.
        load_scene(0);
    }
    void shutdown() override
    {
        // no cleanup necessary
    }
};
```
The application can then be launched from the main() function:
```C++
int main(int argc, char ** argv)
{
    return mare::launch<Sandbox>();
}
```

### Scenes
In order to create a Scene in the Renderer, a Scene header file must exists. Below is an example implementation of a Scene with Shadow Mapping of a Sphere and a Torus. See `SampleShadowScene.hpp` for the full implementation file.
```C++
class SampleShadowScene : public Scene, public Rigidbody {
public:
  SampleShadowScene() : Scene(ProjectionType::PERSPECTIVE) {
    ...
    // Generate Scene controls and push them onto the System Stack.
    // (See implementation below)
    gen_system<SampleShadowSceneControls>(); 
    ...
  }

  void on_enter() override {
    // Start with the cursor disabled for Fly Contorls
    Renderer::set_cursor(CursorType::DISABLED);
  }

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_blending(true);
    Renderer::enable_depth_testing(true);
    Renderer::enable_face_culling(true);
    // Clear color and depth buffer
    Renderer::clear_color_buffer(bg_color);
    Renderer::clear_depth_buffer();
    ...
  }

  void on_exit() override {
    // Show cursor on new scenes
    Renderer::set_cursor(CursorType::ENABLED);
  }
  ...
private:
  glm::vec4 bg_color{0.9f, 0.9f, 0.9f, 1.0f};
};
```
Specific controls for the Scene to handle inputs are implemented in the SampleShadowSceneControls class below:

```C++
class SampleShadowSceneControls : public ControlsSystem<SampleShadowScene> {
public:
  // When a key is pressed
  bool on_key(const RendererInput &input, SampleShadowScene *scene) override {
    // swap camera when "C" is pressed
    if (input.C_JUST_PRESSED) {
      scene->swap_camera(scene->light);
    }
    
    // toggle wireframe rendering when "T" is pressed
    if (input.T_JUST_PRESSED) {
      Renderer::get_info().wireframe = !(Renderer::get_info().wireframe);
    }
    Renderer::wireframe_mode(Renderer::get_info().wireframe);
    ...

    // pass though to next callback
    return false;
  }
};
```
### Layers
Layers can be added the the Layer Stack of any Scene to create UI Layers. Each element in the UI Layers will have their ControlsSystem executed in reverse order so that Layers can be stacked on top of each other and handle events. This will stop input from propogating through Layers if it is handled by a ControlsSystem. Below is an example implementation of a simple UI Layer that provides a slider bar UI element. See `SampleUI.hpp` for the full implementation.
```C++
class SampleUI : public Layer {
public:
  SampleUI() : Layer(ProjectionType::ORTHOGRAPHIC) {
    // Create The Slider and push it onto the Entity Stack.
    gen_entity<SliderUI>(this);
    // Set the slider properties
    get_entity<SliderUI>()->set_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    get_entity<SliderUI>()->set_value(0.5f);
  }

  void on_enter() override {}

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_depth_testing(false);
    Renderer::enable_face_culling(true);
  }

  void on_exit() override {}
};
```
The UI Layer is fairly simple since most of the input is handeled by the UI elements themselves.

### Entities
Entities are anything that exists in a Scene or Layer that inherit from Components and have Systems attached to them. Below is an example of an Entity used in the SampleShadowScene.
```C++
class SampleEntity : public Entity, public Shadow {
public:
  // Creates the assets used the render the Entity and attached 
  // RenderSystem to it.
  SampleEntity(unsigned int n_segments, unsigned int n_rings,
               float inner_radius, float outer_radius) {
    auto mesh1 =
        gen_ref<TorusMesh>(n_segments, n_rings, inner_radius, outer_radius);
    auto material = gen_ref<PhongMaterial>();
    auto mesh2 = gen_ref<SphereMesh>(3, 0.05f);
    mesh2->set_position(glm::vec3(0.0f, 0.0f, 0.4f));
    push_packet({mesh1, material});
    push_packet({mesh2, material});
    gen_system<ShadowRenderer>();
  }
};
```

#### UI Elements
Below is an implemented Slider UI element for use on a Layer. See `SliderUI.hpp` for the full implementation. UI Elements typically inherit from the Widget<T> Component which makes some common UI tasks simpler.
```C++
class SliderUI : public Entity, public Widget<float> {
public:
  SliderUI(Layer *layer)
      : Widget(layer), quad_mesh(nullptr), slider_mesh(nullptr),
        solid_material(nullptr), color(glm::vec4(1.0f)) {
    // create an instanced mesh from quads that will be the mesh for the 
    // slider bar
    ...

    // generare Systems and push them onto the System Stack
    // (See below for implementation)
    gen_system<SliderUIRenderer>();
    gen_system<SliderUIControls>();
  }
  void set_color(glm::vec4 color) { solid_material->set_color(color); }
  // Set widget value and update the slider mesh.
  void set_value(float value) override {
    this->value = value;
    (*slider_mesh)[0] =
        glm::translate(glm::mat4(1.0f), {value - 0.5f, 0.0f, 0.0f}) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
  }
  Scoped<QuadrangleMesh> quad_mesh;
  Referenced<InstancedMesh> slider_mesh;
  Referenced<BasicMaterial> solid_material; 
  glm::vec4 color;
};
```
The RenderSystem and ControlsSystem for the UI element are implemented using the overriden functions below:
```C++
class SliderUIRenderer : public RenderSystem<SliderUI> {
public:
  void render(float dt, Camera *camera, SliderUI *slider_ui) override {
    // render the Mesh with the Material
    ...
  }
};

class SliderUIControls : public ControlsSystem<SliderUI> {
public:
  // when the mouse is pressed focus the layer
  bool on_mouse_button(const RendererInput &input, SliderUI *slider_ui) {
    ...
  }
  // when the mouse moves update knob position
  bool on_mouse_move(const RendererInput &input, SliderUI *slider_ui) {
    ...
  }
  // when window resizes, update the slider position
  bool on_resize(const RendererInput &input, SliderUI *slider_ui) {
    ...
  }
};
```

#### Cameras
Cameras are a special type of Entity. All Layers and Scenes inherit from a Camera Entity. This is the active Camera and no additional Camera Entity is necessary to render a Layer or Scene. Multiple Camera Entities can be attached to a Scene and swaped with the active Camera using the `Layer::swap_camera(Referenced<Camera> camera)` function.

### Components & Systems
Components add functionality to Entities and are operated on by Systems. An Entity is created by inheriting from Components and pushing Systems that operate on those Comonents onto the Entity's System stack. The types of Systems must match the types of Components the Entity inherits from. Below is an example of a EulerMethod PhysicsSystem that is used to update a Rigidbody Component:
```C++
class EulerMethod : public PhysicsSystem<Rigidbody> {
public:
  void update(float dt, Rigidbody *rb) override {
    // euler method to calculate velocity and position
    rb->linear_velocity += rb->force * dt;
    rb->translate(rb->linear_velocity * dt);
  }
};
```
And this is an example of a Rigidbody Component that inherits from the Transform Component:
```C++
class Rigidbody : virtual public Transform {
public:
  glm::vec3 linear_velocity{};  /**< The linear velocity of the body.*/
  glm::vec3 angular_velocity{}; /**< The angular velocity of the body.*/
  glm::vec3 force{};            /**< The linear force acting on the body.*/
  glm::vec3 torque{};           /**< The angular force acting on the body.*/
};
```

#### Included Components
Standard Components included in MARE are the following:
* RenderPack
* Rigidbody
* Shadow
* Transform
* Widget

All Components are documented using doxygen.

### Included Assets
Standard Assest included in MARE are Primative Meshes and Primative Materials.

#### Primative Meshes
Mare includes the following Meshes:
* ArrayMesh
* ArrowMesh
* CharMesh
* CircleMesh
* ConeMesh
* CubeMesh
* CylinderMesh
* QuadrangleMesh
* SlopeMesh
* SphereMesh
* TorusMesh
* TriangleMesh
* TubeMesh

All Meshes are documented using doxygen. The CharMesh can be used to render simple text contructed out of lines without using a bitmap font.

#### Primative Materials
Mare includes the following Materials:
* BasicMaterial
* BasicTextureMaterial
* PhongMaterial

All Materials are documented using doxygen.

### Compute Programs
Mare allows the user to create general purpose compute programs from glsl Shaders. See the example `SampleParticleSystem.hpp` for an implementation of a particle system using a compute program.

## TODO List
### Version 2.0.0
* PhongMaterial to have arrays for spotlight, point light, and directional lights, as well as shadowmaping for directional, point, and spot lights.
* ShadowMap Component to have arrays for each type of light and depth buffers/light views for each
* ShadowMap Camera view creation to be automated.
* A key input enum for `JUST_RELEASED` to be added to RendererInput
* CharMesh to have a *thickness* parameter that uses quads and circles to render text with a thickness.
* A font loader to be added to enable the use of bitmap fonts.
* Text billboards for CharMesh and bitmap fonts.
* Raypicking of Entities in a Scene.
* Cylinder, Cone, and Tube Meshes to have an option for flat shading normals or smooth shading normals.
* More Widget to be added (Buttons, Switches, Checkboxes, Color Pickers, Text Entry Box)
* More Materials to be added (BasicColor, BasicTexture, Depth, Phong, Toon, Shadow.

### Version 3.0.0
* InstancedMeshes to have a Transform Buffer not a glm::mat4 Buffer and Transform Component to only contain a single glm::mat4 with functions to get and set parts of it.
* `api_render_mesh()` should use Transforms to render not glm::mat4
* Systems and Components for collisions to be added.
* Bezier curves and spline curves / surfaces to be added.
* Teapot Primative Mesh from bezier curves and surfaces to be added.
* Cloth Physics PhysicsSystem
* Metaballs Entity
* Loader for models to be added.
* Loader for sound to be added. Sound Components and Systems to be added.
* Loaders for animations to be added. Animation Components and Systems to be added.
* Batch rendering to be implemented in CompositeMesh. CompositeMeshes will be rendered with gl multidraw calls.
* Renderer blending to have all the opengl features such as custom blend functions.
* Buffer, Texture, and Framebuffer classes to have all of the OpenGL features.
