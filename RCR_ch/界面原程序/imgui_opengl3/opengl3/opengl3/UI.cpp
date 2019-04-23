#include "UI.h"

// OpenCVתΪOpenGLͼƬ��ʽ
ImTextureID GetMatTextureID(const cv::Mat &mat, GLuint& tex) {
    glDeleteTextures(1, &tex);  // ɾ��ǰһ�����õ�ͼ����ֹ�ڴ�й©
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, // Type of texture
        0,                      // Pyramid level (for mip-mapping) - 0 is the top level
        GL_RGB,                 // Internal colour format to convert to
        mat.cols,               // Image width  i.e. 640 for Kinect in standard mode
        mat.rows,               // Image height i.e. 480 for Kinect in standard mode
        0,                      // Border width in pixels (can either be 1 or 0)
        GL_BGR,                 // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
        GL_UNSIGNED_BYTE,       // Image data type
        mat.ptr());             // The actual image data itself
    glGenerateMipmap(GL_TEXTURE_2D);

#pragma warning(disable:4312)
    // ������C4312���棬���ǲ������ʲô����Ӱ�죬��������
    return reinterpret_cast<ImTextureID>(tex);
}
