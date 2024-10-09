module;
#include <cstddef>

export module tr:vertex;

import std;
import glm;
import :color;
import :ranges;
import :vertex_format;

export namespace tr {
    // 2D color vertex.
    struct ClrVtx2 {
        glm::vec2 pos;
        RGBA8 color;

        // Returns a reference to a vertex format associated with this type.
        static const VertexFormat& vertexFormat() noexcept;
    };
    // 2D texture vertex.
    struct TexVtx2 {
        glm::vec2 pos;
        glm::vec2 uv;

        // Returns a reference to a vertex format associated with this type.
        static const VertexFormat& vertexFormat() noexcept;
    };
    // 2D tinted texture vertex.
    struct TintVtx2 {
        glm::vec2 pos;
        glm::vec2 uv;
        RGBA8 color;

        // Returns a reference to a vertex format associated with this type.
        static const VertexFormat& vertexFormat() noexcept;
    };

    // Gets a view of element positions.
    template <std::ranges::range Range> requires requires (std::ranges::range_value_t<Range> val) { val.pos; } constexpr auto positions(Range&& range) noexcept;
    // Gets a view of element UV coordinates.
    template <std::ranges::range Range> requires requires (std::ranges::range_value_t<Range> val) { val.uv; } constexpr auto uvs(Range&& range) noexcept;
    // Gets a view of element colors.
    template <std::ranges::range Range> requires requires (std::ranges::range_value_t<Range> val) { val.color; } constexpr auto colors(Range&& range) noexcept;
}

// IMPLEMENTATION

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

template <std::ranges::range Range> requires requires (std::ranges::range_value_t<Range> val) { val.pos; }
constexpr auto tr::positions(Range&& range) noexcept
{
    return member(std::forward<Range&&>(range), &std::ranges::range_value_t<Range>::pos);
}

template <std::ranges::range Range> requires requires (std::ranges::range_value_t<Range> val) { val.uv; }
constexpr auto tr::uvs(Range&& range) noexcept
{
    return member(std::forward<Range&&>(range), &std::ranges::range_value_t<Range>::uv);
}

template <std::ranges::range Range> requires requires (std::ranges::range_value_t<Range> val) { val.color; }
constexpr auto tr::colors(Range&& range) noexcept
{
    return member(std::forward<Range&&>(range), &std::ranges::range_value_t<Range>::color);
}