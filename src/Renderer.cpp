#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Vector.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Line.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Trade/MeshData.h>
#include <Corrade/Containers/Array.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/MeshVisualizerGL.h>

#include <q3.h>

using namespace Magnum;

struct TriangleVertex
{
    Vector3 position;
    Color3 color;
};

class Renderer : public q3Render
{
    GL::Mesh _mesh;
    Shaders::VertexColorGL2D _shader;
    Shaders::PhongGL _shaderPhong;

    GL::Mesh _lineMesh;
    Matrix4 _transformation;
    Matrix4 _projection;
    Color3 _color;

    Shaders::MeshVisualizerGL3D _shaderWire{Shaders::MeshVisualizerGL3D::Flag::Wireframe};

public:
    Renderer()
    {
        GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
        GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

        using namespace Math::Literals;

        //_mesh = MeshTools::compile(Primitives::cubeSolid());

        _transformation = Matrix4::rotationX(30.0_degf) * Matrix4::rotationY(40.0_degf);
        //_transformation = Matrix4::lookAt({20,20,10},{0, 0, 0},Vector3::yAxis(1.f));
        _projection =
            Matrix4::perspectiveProjection(
                55.0_degf, Vector2{640.f, 640.f}.aspectRatio(), 0.01f, 100.0f) *
            Matrix4::translation(Vector3::zAxis(-35.0f));
        _color = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});

        // Wireshader
        _shaderWire
            .setColor(0x2f83cc_rgbf)
            .setWireframeColor(0xdcdcdc_rgbf)
            .setViewportSize(Vector2{GL::defaultFramebuffer.viewport().size()})
            .setTransformationMatrix(_transformation)
            .setProjectionMatrix(_projection);

        _shaderPhong.setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
            .setDiffuseColor(_color)
            .setAmbientColor(Color3::fromHsv({_color.hue(), 1.0f, 0.3f}))
            .setTransformationMatrix(_transformation)
            .setNormalMatrix(_transformation.normalMatrix())
            .setProjectionMatrix(_projection);
    }

    void SetPenColor(f32 r, f32 g, f32 b, f32 a = 1.0f) override
    {
        Q3_UNUSED(a);

        // glColor3f( (float)r, (float)g, (float)b );
    }

    void SetPenPosition(f32 x, f32 y, f32 z) override
    {
        x_ = x, y_ = y, z_ = z;
    }

    void SetScale(f32 sx, f32 sy, f32 sz) override
    {
        // glPointSize( (float)sx );
        sx_ = sx, sy_ = sy, sz_ = sz;
    }

    void Line(f32 x, f32 y, f32 z) override
    {
        //auto _lineMesh = MeshTools::compile(Primitives::line3D({x_, y_, z_}, {x, y, z}));
        //_shader.draw(_lineMesh);
    }

    void Triangle(
        f32 x1, f32 y1, f32 z1,
        f32 x2, f32 y2, f32 z2,
        f32 x3, f32 y3, f32 z3) override
    {

        using namespace Math::Literals;
        const TriangleVertex vertices[]{
            {{x1, y1, z1}, 0xff0000_rgbf}, // red color
            {{x2, y2, z2}, 0x00ff00_rgbf}, // green color
            {{x3, y3, z3}, 0x0000ff_rgbf}  // blue color
        };

        _mesh.setPrimitive(GL::MeshPrimitive::Triangles)
            .setCount(Containers::arraySize(vertices))
            .addVertexBuffer(GL::Buffer{vertices}, 0,
                             Shaders::VertexColorGL3D::Position{},
                             Shaders::VertexColorGL3D::Color3{});

        _shaderWire.draw(_mesh);
        //_shaderPhong.draw(_mesh); // TODO: Fix lighting / normals?
    }

    void SetTriNormal(f32 x, f32 y, f32 z) override
    {
        nx_ = x;
        ny_ = y;
        nz_ = z;
    }

    void Point() override{

        // printf("%f %f %f\n", x_, y_, z_);
        /*
              glBegin( GL_POINTS );
              glVertex3f( (float)x_, (float)y_, (float)z_ );
              glEnd( );
              */
    };

private:
    f32 x_, y_, z_;
    f32 sx_, sy_, sz_;
    f32 nx_, ny_, nz_;
};