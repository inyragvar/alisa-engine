#ifndef ALISA_GRAPHICS_GL_FUNCTION_H_
#define ALISA_GRAPHICS_GL_FUNCTION_H_

namespace alisa {
namespace graphics {
namespace gl {

void CheckGLError();

unsigned int GetTextureFormat(int channels);

void SetTextureFilters(bool is_nearest, bool is_power_of_two = false);

void SetTextureWrapMode(bool is_repeat = false, bool is_power_of_two = false);

void GenVertexArrays(unsigned int n, unsigned int* arrays);
void BindVertexArray(unsigned int array);

void ClearColor(float r, float g, float b, float a);

void PolygonMode(unsigned int type);

void GenTextures(unsigned int n, unsigned int* ids);
void DeleteTextures(unsigned int n, const unsigned int* ids);

void BindTexture2D(unsigned int id);

void TexImage2D(int level, int internalformat, int width, int height, int border, unsigned int format, unsigned int type, const void* pixels);

}
}
}

#endif
