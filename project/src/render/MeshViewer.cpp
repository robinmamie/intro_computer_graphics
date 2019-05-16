//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "MeshViewer.h"
#include "../utils/vec.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <array>

//=============================================================================


MeshViewer::MeshViewer(std::string const& _title, int _width, int _height)
	: GLFW_window(_title.c_str(), _width, _height)
	, fovy_(90)
	, near_(0.01)
	, far_(1024)
	, x_angle_(50)
	, z_angle_(180)
	, camera_distance(0.85)
    , fb(new FrameBuffer(_width, _height))
	, landMesh(new Mesh)
	, landActor(new StaticMeshActor(landMesh))
	, waterMesh(new Mesh)
	, waterActor(new StaticMeshActor(waterMesh))
{
    ;
}

void MeshViewer::setMesh(std::shared_ptr<Mesh> new_landMesh, std::shared_ptr<Mesh> new_waterMesh)
{
	landMesh = new_landMesh;
	landActor->mesh = new_landMesh;
	waterMesh = new_waterMesh;
	waterActor->mesh = new_waterMesh;
}

void MeshViewer::update_water(double dt){
	waterActor->mesh->move(dt);
}

//-----------------------------------------------------------------------------

void MeshViewer::scroll_wheel(double xoffset, double yoffset)
{
    float const scroll_amount = -yoffset * 0.1;
    camera_distance = std::min(std::max(camera_distance + scroll_amount, 0.1f), 30.0f);
}

void MeshViewer::keyboard(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // Change view between the various bodies with keys 1..6
        switch (key) {
        case GLFW_KEY_LEFT:
        case GLFW_KEY_A: {
            z_angle_ -= 10.0;
            break;
        }

        case GLFW_KEY_RIGHT:
        case GLFW_KEY_D: {
            z_angle_ += 10.0;
            break;
        }

        case GLFW_KEY_DOWN:
        case GLFW_KEY_S: {
            x_angle_ += 10.0;
            break;
        }

        case GLFW_KEY_UP:
        case GLFW_KEY_W: {
            x_angle_ -= 10.0;
            break;
        }

        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q: {
            glfwSetWindowShouldClose(window_, GL_TRUE);
            break;
        }
        }
    }
}


//-----------------------------------------------------------------------------


void MeshViewer::resize(int _width, int _height)
{
    width_  = _width;
    height_ = _height;
    glViewport(0, 0, _width, _height);
    fb.reset(new FrameBuffer(_width, _height));
}


//-----------------------------------------------------------------------------


void MeshViewer::initialize()
{
    // set initial state
    glClearColor(1,1,1,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // setup shaders
    phong_shader_.load(SHADER_PATH "/terrain.vert", SHADER_PATH "/terrain.frag");
    reflection_shader_.load(SHADER_PATH "/reflection.vert", SHADER_PATH "/reflection.frag");
    color_shader_.load(SHADER_PATH "/paint_color.vert", SHADER_PATH "/paint_color.frag");
}
//-----------------------------------------------------------------------------


void MeshViewer::paint()
{
    // clear framebuffer and depth buffer first
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4  center(0, 0, 0, 1);
    mat4     rot = mat4::rotate_z(z_angle_) * mat4::rotate_x(x_angle_);
    vec4     eye = center + rot * vec4(0, 0, camera_distance, 0);
    vec4      up = rot * vec4(0, 1, 0, 0);
    mat4    view = mat4::look_at(vec3(eye), vec3(center), vec3(up));

    mat4 projection = mat4::perspective(fovy_, (float)width_/(float)height_, near_, far_);
    draw_scene(projection, view);
}

//-----------------------------------------------------------------------------


void MeshViewer::draw_scene(mat4& _projection, mat4& _view)
{
    if (!phong_shader_.is_valid() || !reflection_shader_.is_valid()) {
        return;
    }

	vec4 light = vec4(0.6, 0.4, 2., 1.0); //in world coordinates
	light = _view * light;

	/// Draw land
	mat4 m_matrix = landActor->model_matrix;
	mat4 mv_matrix  = _view * m_matrix;
	mat4 mvp_matrix = _projection * mv_matrix;

	mat3 n_matrix    = transpose(inverse(mv_matrix));

    fb->select_as_render_target();

    glClearColor(100.0f, 100.0f, 100.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

	phong_shader_.use();
	phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
	phong_shader_.set_uniform("modelview_matrix", mv_matrix);
	phong_shader_.set_uniform("normal_matrix", n_matrix);
	phong_shader_.set_uniform("light_position", vec3(light));

	landActor->draw();
    phong_shader_.disable();

    // Render on the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width_, height_);

    glClearColor(100.0f, 100.0f, 100.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

	/// Draw water
    fb->bind();

    color_shader_.use();
    color_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    color_shader_.set_uniform("resolution", vec2(width_, height_));
    color_shader_.set_uniform("color_map", 0);
	landActor->draw();
    color_shader_.disable();

	m_matrix = waterActor->model_matrix;
	mv_matrix  = _view * m_matrix;
	mvp_matrix = _projection * mv_matrix;
	n_matrix    = transpose(inverse(mv_matrix));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    reflection_shader_.use();
    reflection_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    reflection_shader_.set_uniform("modelview_matrix", mv_matrix);
    reflection_shader_.set_uniform("normal_matrix", n_matrix);
    reflection_shader_.set_uniform("projection_matrix", _projection);
    //reflection_shader_.set_uniform("resolution", vec2(width_, height_));
    reflection_shader_.set_uniform("light_position", vec3(light));


    reflection_shader_.set_uniform("color_map", 0);
    reflection_shader_.set_uniform("depth_map", 1);
    waterActor->draw();
    reflection_shader_.disable();
    glDisable(GL_BLEND);

    fb->unbind();

    // check for OpenGL errors
    glCheckError("MeshViewer::draw_scene");
}

MeshViewer::~MeshViewer()
{
    glDeleteTextures(1, &color_);
    glDeleteTextures(1, &depth_);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &frame_buffer_);
}
