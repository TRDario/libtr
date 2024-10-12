/**
 * @file vertex.cppm
 * @brief Provides generic vertex types and special views for manipulating ranges of them.
 */

module;
#include <cstddef>

export module tr:vertex;

import std;
import glm;
import :color;
import :ranges;
import :vertex_format;

export namespace tr {
    /******************************************************************************************************************
    * Concept denoting a range trasnformable with tr::positions.
    *
    * To satisfy this requirement, @em T must satisfy @em std::ranges::range and its value type must have
    * a member field "pos".
    *******************************************************************************************************************/
    template <class T> concept PositionsRange = std::ranges::range<T> &&
                                                requires (std::ranges::range_value_t<T> val) { val.pos; };

    /******************************************************************************************************************
    * Concept denoting a range trasnformable with tr::uvs.
    *
    * To satisfy this requirement, @em T must satisfy @em std::ranges::range and its value type must have
    * a member field "uv".
    *******************************************************************************************************************/
    template <class T> concept UVsRange = std::ranges::range<T> &&
                                          requires (std::ranges::range_value_t<T> val) { val.uv; };

    /******************************************************************************************************************
    * Concept denoting a range trasnformable with tr::colors.
    *
    * To satisfy this requirement, @em T must satisfy @em std::ranges::range and its value type must have
    * a member field "color".
    *******************************************************************************************************************/
    template <class T> concept ColorsRange = std::ranges::range<T> &&
                                             requires (std::ranges::range_value_t<T> val) { val.color; };


    /******************************************************************************************************************
    * Untextured, colored 2D vertex.
    *******************************************************************************************************************/
    struct ClrVtx2 {
        /**************************************************************************************************************
        * Vertex position field.
        ***************************************************************************************************************/
        glm::vec2 pos;

        /**************************************************************************************************************
        * Vertex color field.
        ***************************************************************************************************************/
        RGBA8 color;

        
        /**************************************************************************************************************
        * Gets a predefined vertex format for this type.
        *
        * @return A reference to a predefined vertex format.
        ***************************************************************************************************************/
        static const VertexFormat& vertexFormat() noexcept;
    };

    /******************************************************************************************************************
    * Textured 2D vertex.
    *******************************************************************************************************************/
    struct TexVtx2 {
        /**************************************************************************************************************
        * Vertex position field.
        ***************************************************************************************************************/
        glm::vec2 pos;

        /**************************************************************************************************************
        * Vertex texture UV field.
        ***************************************************************************************************************/
        glm::vec2 uv;


        /**************************************************************************************************************
        * Gets a predefined vertex format for this type.
        *
        * @return A reference to a predefined vertex format.
        ***************************************************************************************************************/
        static const VertexFormat& vertexFormat() noexcept;
    };

    /******************************************************************************************************************
    * Textured, tinted 2D vertex.
    *******************************************************************************************************************/
    struct TintVtx2 {
        /**************************************************************************************************************
        * Vertex position field.
        ***************************************************************************************************************/
        glm::vec2 pos;

        /**************************************************************************************************************
        * Vertex texture UV field.
        ***************************************************************************************************************/
        glm::vec2 uv;

        /**************************************************************************************************************
        * Vertex tint color field.
        ***************************************************************************************************************/
        RGBA8 color;


        /**************************************************************************************************************
        * Gets a predefined vertex format for this type.
        *
        * @return A reference to a predefined vertex format.
        ***************************************************************************************************************/
        static const VertexFormat& vertexFormat() noexcept;
    };


    /******************************************************************************************************************
    * Special view for manipulating positions of a range of vertices.
    *******************************************************************************************************************/
    inline constexpr struct Positions {
        /**************************************************************************************************************
        * Creates a transformed view over a range of vertices' positions.
        *
        * @tparam R A range type that fulfills <em>PositionsRange</em>.
        *
        * @param range The range to transform.
        *
        * @return The transformed view.
        **************************************************************************************************************/
        template <PositionsRange R>
        friend constexpr auto operator|(R&& range, const Positions&) noexcept;
        
        /**************************************************************************************************************
        * Creates a transformed view over a range of vertices' positions.
        *
        * @tparam R A range type that fulfills <em>PositionsRange</em>.
        *
        * @param range The range to transform.
        *
        * @return The transformed view.
        **************************************************************************************************************/
        template <PositionsRange R>
        constexpr auto operator()(R&& range) const noexcept;
    } positions;

    /******************************************************************************************************************
    * Special view for manipulating UVs of a range of vertices.
    ******************************************************************************************************************/
    inline constexpr struct UVs {
        /**************************************************************************************************************
        * Creates a transformed view over a range of vertices' UVs.
        *
        * @tparam R A range type that fulfills <em>UVsRange</em>.
        *
        * @param range The range to transform.
        *
        * @return The transformed view.
        **************************************************************************************************************/
        template <UVsRange R>
        friend constexpr auto operator|(R&& range, const UVs&) noexcept;
        
        /**************************************************************************************************************
        * Creates a transformed view over a range of vertices' UVs.
        *
        * @tparam R A range type that fulfills <em>UVsRange</em>.
        *
        * @param range The range to transform.
        *
        * @return The transformed view.
        **************************************************************************************************************/
        template <UVsRange R>
        constexpr auto operator()(R&& range) const noexcept;
    } uvs;

    /******************************************************************************************************************
    * Special view for manipulating colors of a range of vertices.
    ******************************************************************************************************************/
    inline constexpr struct Colors {
        /**************************************************************************************************************
        * Creates a transformed view over a range of vertices' colors.
        *
        * @tparam R A range type that fulfills <em>ColorsRange</em>.
        *
        * @param range The range to transform.
        *
        * @return The transformed view.
        **************************************************************************************************************/
        template <ColorsRange R>
        friend constexpr auto operator|(R&& range, const Colors&) noexcept;
        
        /**************************************************************************************************************
        * Creates a transformed view over a range of vertices' colors.
        *
        * @tparam R A range type that fulfills <em>ColorsRange</em>.
        *
        * @param range The range to transform.
        *
        * @return The transformed view.
        **************************************************************************************************************/
        template <ColorsRange R>
        constexpr auto operator()(R&& range) const noexcept;
    } colors;
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

template <tr::PositionsRange R>
constexpr auto tr::operator|(R&& range, const Positions&) noexcept
{
    return positions(range);
}

template <tr::PositionsRange R>
constexpr auto tr::Positions::operator()(R&& range) const noexcept
{
    return range | project(&std::ranges::range_value_t<R>::pos);
}

template <tr::UVsRange R>
constexpr auto tr::operator|(R&& range, const UVs&) noexcept
{
    return uvs(range);
}

template <tr::UVsRange R>
constexpr auto tr::UVs::operator()(R&& range) const noexcept
{
    return range | project(&std::ranges::range_value_t<R>::uv);
}

template <tr::ColorsRange R>
constexpr auto tr::operator|(R&& range, const Colors&) noexcept
{
    return colors(range);
}

template <tr::ColorsRange R>
constexpr auto tr::Colors::operator()(R&& range) const noexcept
{
    return range | project(&std::ranges::range_value_t<R>::color);
}