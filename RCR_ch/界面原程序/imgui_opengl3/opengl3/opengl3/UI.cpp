#include "UI.h"

// OpenCV转为OpenGL图片格式
ImTextureID GetMatTextureID(const cv::Mat &mat, GLuint& tex) {
    glDeleteTextures(1, &tex);  // 删除前一张无用的图，防止内存泄漏
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
    // 会引发C4312警告，但是不会造成什么坏的影响，请无视它
    return reinterpret_cast<ImTextureID>(tex);
}
