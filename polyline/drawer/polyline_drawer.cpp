#include "polyline_drawer.h"

#include "util.h"

#include <vector>
#include <cmath>

struct Data {
    int ref;
    int type; // -1, 0, 1
    int start;
    int end;
};
typedef std::vector<int> PatternArray;
typedef std::vector<Data> DataArray;

int GetCapSize(poly::CapStyle cap_style)
{
	switch (cap_style)
	{
	case poly::CapStyle::kRound:
	case poly::CapStyle::kSquare:
		return 1;
	case poly::CapStyle::kFlat:
	default:
		return 0;
	}
}
static void EncodeData(poly::CapStyle cap_style, const PatternArray& pattern, DataArray& data)
{
    const int pattern_size = static_cast<int>(pattern.size());

    // Calc texture (data) size
    int size = 0;
    for (int length : pattern)
        size += length;
    data.resize(static_cast<size_t>(size));
    
    const int cap_begin = ::GetCapSize(cap_style);
    int distance = 0;
    int pattern_index = 0;
    int dash_start = cap_begin;
    int dash_end = dash_start + pattern[pattern_index];
    int ref = dash_start;
    int type = (distance < ref) ? -1 : 0;
    int breakpoint = (type == 1) ? dash_end : dash_start;
    
    for (int i = 0; i < size; ++i)
    {
        if (type == -1 && distance >= breakpoint) // breakpoint = dash_start
        {
            // Change type from -1 to 0
            type = 0;
            // ref remains the same
            breakpoint = dash_end;
        }
        if (type == 0 && distance >= breakpoint) // breakpoint = dash_end
        {
            // Change type from 0 to 1
            type = 1;
            ref = dash_end;
            int space_length = pattern[(pattern_index+1) % pattern_size];
            breakpoint = (dash_end + dash_end + space_length) / 2; // middle of space
        }
        if (type == 1 && distance >= breakpoint) // breakpoint = middle of space
        {
            // Next dash
            type = -1;
            int space_length = pattern[(pattern_index+1) % pattern_size];
            dash_start = dash_end + space_length;
            dash_end = dash_start + pattern[(pattern_index+2) % pattern_size];
            pattern_index += 2;
            ref = dash_start;
            breakpoint = dash_start;
        }
     
        Data& d = data[i];
        d.ref = ref;
        d.type = type;
        d.start = dash_start;
        d.end = dash_end;
        
        ++distance;
    }
}

namespace poly {

PolylineDrawer::PolylineDrawer(const Viewport* viewport)
: viewport_(viewport)
, program_(0)
, texture_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, num_vertices_(0)
, texture_width_(0)
, vertices_array_(nullptr)
, texture_data_(nullptr)
, pixel_width_(20.0f)
, cap_style_(CapStyle::kRound)
{

}
PolylineDrawer::~PolylineDrawer()
{
	Destroy();
}
bool PolylineDrawer::Create(const PointArray& points)
{
	if (!LoadShaders(
		"data/shaders/polyline/polyline.vs", 
		"data/shaders/polyline/polyline.fs", 
		"data/shaders/polyline/polyline.gs", 
		program_))
		return false;

	if (!CreateData(points))
		return false;
	MakeRenderable();

	return true;
}
void PolylineDrawer::Destroy()
{
	FreeArrays();
	if (program_ != 0)
	{
		glDeleteProgram(program_);
		program_ = 0;
	}
	if (texture_ != 0)
	{
		glDeleteTextures(1, &texture_);
		texture_ = 0;
	}
	if (vertex_buffer_object_ != 0)
	{
		glDeleteBuffers(1, &vertex_buffer_object_);
		vertex_buffer_object_ = 0;
	}
	if (vertex_array_object_ != 0)
	{
		glDeleteVertexArrays(1, &vertex_array_object_);
		vertex_array_object_ = 0;
	}
}
void PolylineDrawer::Render()
{
	ActivateShader();
	ActivateTexture();

	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, num_vertices_ - 1);
	glBindVertexArray(0);

	DeactivateTexture();
	DeactivateShader();
}
bool PolylineDrawer::CreateData(const PointArray& points)
{
	uint32_t num_points = static_cast<uint32_t>(points.size());
	if (num_points < 2) return false;

	// We add one point before and one point after to have access to previous and next segments.
	num_vertices_ = num_points;
	vertices_array_ = new uint8_t[num_vertices_ * sizeof(Vertex)];
	Vertex* vertices = reinterpret_cast<Vertex*>(vertices_array_);

	// Position
	uint32_t n = 0;
	for (uint32_t i = 0; i < num_points; ++i)
	{
		const Point& point = points[i];
		vertices[n++].position = {point[0], point[1]};
	}

	// Texture data
	// ------------
	const std::vector<int> pattern = {2,3,0,3};//{2,4};
	DataArray data;
	::EncodeData(cap_style_, pattern, data);
	uint32_t element_size = sizeof(float) * 4;
	texture_width_ = static_cast<uint32_t>(data.size());
	texture_data_ = new uint8_t[texture_width_ * element_size];
	float* float_data = reinterpret_cast<float*>(texture_data_);
	for (uint32_t i = 0; i < data.size(); ++i)
	{
		float_data[4*i  ] = static_cast<float>(data[i].ref);
		float_data[4*i+1] = static_cast<float>(data[i].type);
		float_data[4*i+2] = static_cast<float>(data[i].start);
		float_data[4*i+3] = static_cast<float>(data[i].end);
	}

	return true;
}
void PolylineDrawer::FreeArrays()
{
	if (vertices_array_ != nullptr)
	{
		delete[] vertices_array_;
		vertices_array_ = nullptr;
	}
	if (texture_data_ != nullptr)
	{
		delete[] texture_data_;
		texture_data_ = nullptr;
	}
}
void PolylineDrawer::MakeRenderable()
{
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);

	glGenBuffers(1, &vertex_buffer_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, num_vertices_ * sizeof(Vertex), vertices_array_, GL_STATIC_DRAW);

	static_assert(sizeof(Point) == 2*sizeof(float), "Point size mismatch");

	// Attributes layout
	const GLsizei stride = sizeof(Vertex);
	const uint8_t* base = nullptr;
	const uint8_t* curr_offset = base;
	const uint8_t* next_offset = curr_offset + stride;
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, curr_offset); // vec2 a_position_curr
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, next_offset); // vec2 a_position_next
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Create texture
	// --------------
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_1D, texture_);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, (GLsizei)texture_width_, 0, GL_RGBA, GL_FLOAT, texture_data_);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Finally
	FreeArrays();
}
void PolylineDrawer::ActivateShader()
{
	int location;

	const float dash_unit = pixel_width_;
	const float dash_period = dash_unit * static_cast<float>(texture_width_);
	const float dash_phase = dash_unit * static_cast<float>(::GetCapSize(cap_style_));

	glUseProgram(program_);
	location = glGetUniformLocation(program_, "u_viewport");
	glUniform4f(location, 0.0f, 0.0f, (float)viewport_->width, (float)viewport_->height);
	location = glGetUniformLocation(program_, "u_pixel_width");
	glUniform1f(location, pixel_width_);
	location = glGetUniformLocation(program_, "u_cap_style");
	glUniform1i(location, static_cast<int>(cap_style_));
	location = glGetUniformLocation(program_, "u_dash_unit");
	glUniform1f(location, dash_unit);
	location = glGetUniformLocation(program_, "u_dash_period");
	glUniform1f(location, dash_period);
	location = glGetUniformLocation(program_, "u_dash_phase");
	glUniform1f(location, dash_phase);
	location = glGetUniformLocation(program_, "u_texture");
	glUniform1i(location, 0);
}
void PolylineDrawer::DeactivateShader()
{
	glUseProgram(0);
}
void PolylineDrawer::ActivateTexture()
{
	glBindTexture(GL_TEXTURE_1D, texture_);
}
void PolylineDrawer::DeactivateTexture()
{
	glBindTexture(GL_TEXTURE_1D, 0);
}
void PolylineDrawer::SetCapStyle(CapStyle cap_style)
{
	cap_style_ = cap_style;
}
CapStyle PolylineDrawer::cap_style() const
{
	return cap_style_;
}

} // namespace poly