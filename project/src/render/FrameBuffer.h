#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "gl.h"
#include "glfw_window.h"

class FrameBuffer
{
public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void select_as_render_target();
    void bind();
    void unbind();
private:
    int width_;
    int height_;

    GLuint fbo = 0;
    GLuint color_tex = 0;
    GLuint depth_tex = 0;
};

#endif
