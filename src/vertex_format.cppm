module;
#include <GL/glew.h>

export module tr:vertex_format;

import std;
import :handle;
import :integer;

export namespace tr {
	// Class representing a single-precision floating point vertex attribute.
	struct VertexAttributeF {
		// The type of the attribute.
		enum class Type {
			SI8 = 0x1400,
			UI8,
			SI16,
			UI16,
			SI32,
			UI32,
			FP16 = 0x140B,
			FP32 = 0x1406,
			FP64 = 0x140A,
			FIXED32 = 0x140C,
			SI2_10_10_10 = 0x8D9F, // 4 packed signed integers (2, 10, 10, 10 bits). Size must be 4 to match.
			UI2_10_10_10 = 0x8368, // 4 packed signed integers (2, 10, 10, 10 bits). Size must be 4 to match.
			FP10_11_11 = 0x8C3B
		};

		Type type;
		// The number of elements in the attribute (1 - 4).
		Ui8 elements;
		// Whether integer data is normalized when converted to floating point.
		bool normalized;
		// The relative position of the attribute within an element in bytes.
		Ui32 offset;
	};
	// Class representing a double-precision floating point vertex attribute.
	struct VertexAttributeD {
		// The number of elements in the attribute (1 - 4).
		Ui8 elements;
		// The relative position of the attribute within an element in bytes.
		Ui32 offset;
	};
	// Class representing an integral vertex attribute.
	struct VertexAttributeI {
		// The type of the attribute.
		enum class Type {
			SI8 = 0x1400,
			UI8,
			SI16,
			UI16,
			SI32,
			UI32,
			SI2_10_10_10 = 0x8D9F, // 4 packed signed integers (2, 10, 10, 10 bits). Size must be 4 to match.
			UI2_10_10_10 = 0x8368, // 4 packed unsigned integers (2, 10, 10, 10 bits). Size must be 4 to match.
		};

		Type type;
		// The number of elements in the attribute (1 - 4).
		Ui8 elements;
		// The relative position of the attribute within an element in bytes.
		Ui32 offset;
	};
	// A typedef representing a generic vertex attribute.
	using VertexAttribute = std::variant<VertexAttributeF, VertexAttributeD, VertexAttributeI>;

	// Vertex format class.
	class VertexFormat {
	public:
        // Creates a new vertex format.
        VertexFormat(std::span<const VertexAttribute> attrs) noexcept;

		friend bool operator==(const VertexFormat&, const VertexFormat&) = default;

        // Sets the debug label for the format.
        void setLabel(std::string_view label) noexcept;
	private:
		struct Deleter { void operator()(unsigned int id) noexcept; };
		Handle<unsigned int, 0, Deleter> _id;

		// Binds the vertex format.
		void bind() const noexcept;

		friend class GLContext;
	};
}

// IMPLEMENTATION

tr::VertexFormat::VertexFormat(std::span<const VertexAttribute> attrs) noexcept
{
    struct AttribFormatter {
        unsigned int vao;
        int          i;

        void operator()(const VertexAttributeF& attr) {
            glVertexArrayAttribFormat(vao, i, attr.elements, GLenum(attr.type), attr.normalized, attr.offset);
        }
        void operator()(const VertexAttributeD& attr) {
            glVertexArrayAttribLFormat(vao, i, attr.elements, GL_DOUBLE, attr.offset);
        }
        void operator()(const VertexAttributeI& attr) {
            glVertexArrayAttribIFormat(vao, i, attr.elements, GLenum(attr.type), attr.offset);
        }
    };

    unsigned int vao;
    glCreateVertexArrays(1, &vao);
    for (int i = 0; i < attrs.size(); ++i) {
        std::visit(AttribFormatter { vao, i }, attrs[i]);
        glVertexArrayAttribBinding(vao, i, 0);
        glEnableVertexArrayAttrib(vao, i);
    }
    _id.reset(vao);
}

void tr::VertexFormat::Deleter::operator()(unsigned int id) noexcept
{
    glDeleteVertexArrays(1, &id);
}

void tr::VertexFormat::setLabel(std::string_view label) noexcept
{
    glObjectLabel(GL_VERTEX_ARRAY, _id.get(), label.size(), label.data());
}

void tr::VertexFormat::bind() const noexcept
{
    glBindVertexArray(_id.get());
}