#include "../include/vertex.hpp"


const tr::VertexFormat& tr::ClrVtx2::vertexFormat() noexcept
{
    const std::initializer_list<VertexAttribute> attrs = {
        VertexAttributeF { VertexAttributeF::Type::FP32, 2, false, offsetof(ClrVtx2, pos) },
        VertexAttributeF { VertexAttributeF::Type::UI8,  4, true,  offsetof(ClrVtx2, color) }
    };
    static VertexFormat format { attrs };
    #ifndef NDEBUG
    format.setLabel("(tr) 2D Color Vertex Format");
    #endif
    return format;
}

const tr::VertexFormat& tr::TexVtx2::vertexFormat() noexcept
{
    const std::initializer_list<VertexAttribute> attrs = {
        VertexAttributeF { VertexAttributeF::Type::FP32, 2, false, offsetof(TexVtx2, pos) },
        VertexAttributeF { VertexAttributeF::Type::FP32, 2, false, offsetof(TexVtx2, uv) }
    };
    static VertexFormat format { attrs };
    #ifndef NDEBUG
    format.setLabel("(tr) 2D Texture Vertex Format");
    #endif
    return format;
}

const tr::VertexFormat& tr::TintVtx2::vertexFormat() noexcept
{
    const std::initializer_list<VertexAttribute> attrs = {
        VertexAttributeF { VertexAttributeF::Type::FP32, 2, false, offsetof(TintVtx2, pos) },
        VertexAttributeF { VertexAttributeF::Type::FP32, 2, false, offsetof(TintVtx2, uv) },
        VertexAttributeF { VertexAttributeF::Type::UI8,  4, true,  offsetof(TintVtx2, color) }
    };
    static VertexFormat format { attrs };
    #ifndef NDEBUG
    format.setLabel("(tr) 2D Tinted Vertex Format");
    #endif
    return format;
}