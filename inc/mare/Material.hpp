#ifndef MATERIAL
#define MATERIAL

namespace mare
{
class Material
{
public:
    virtual ~Material() {}
    virtual void render() = 0;
    virtual void bind() = 0;
};
} // namespace mare

#endif