#include <string>
#include "Vec3.h"
class Render
{
private:
    unsigned int max_depth,width,height;
    float field_of_view,aspect_ratio;
    Vec3<unsigned int> background_color;
    std::string filename;
    // 3 values per pixel
public:
    Render(int max_depth, );
    ~Render();
};

Render::Render(/* args */)
{
}

Render::~Render()
{
}
