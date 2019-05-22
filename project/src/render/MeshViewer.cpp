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
	, fillerMesh(new Mesh)
	, fillerActor(new StaticMeshActor(fillerMesh))
	, sky_ (2.5f) // radius
	, unit_sphere_(50) //level of tesselation
{
    ;
}

void MeshViewer::setMesh(std::shared_ptr<Mesh> new_landMesh, std::shared_ptr<Mesh> new_waterMesh,  std::shared_ptr<Mesh> new_fillerMesh)
{
	landMesh = new_landMesh;
	landActor->mesh = new_landMesh;
	waterMesh = new_waterMesh;
	waterActor->mesh = new_waterMesh;
	fillerMesh = new_fillerMesh;
	fillerActor->mesh = new_fillerMesh;
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
    const float debug_v = 10.0;
    const float cinematic_v = 0.1;
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // Change view between the various bodies with keys 1..6
        switch (key) {
        case GLFW_KEY_LEFT: {
            z_angle_ -= debug_v;
            break;
        }

        case GLFW_KEY_RIGHT: {
            z_angle_ += debug_v;
            break;
        }

        case GLFW_KEY_DOWN: {
            x_angle_ += debug_v;
            break;
        }

        case GLFW_KEY_UP: {
            x_angle_ -= debug_v;
            break;
        }

        case GLFW_KEY_A: {
            z_angle_ -= cinematic_v;
            break;
        }

        case GLFW_KEY_D: {
            z_angle_ += cinematic_v;
            break;
        }

        case GLFW_KEY_S: {
            x_angle_ += cinematic_v;
            break;
        }

        case GLFW_KEY_W: {
            x_angle_ -= cinematic_v;
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

    // setup shaders
    std::vector<std::string> sky_vert = {SHADER_PATH "/sky.vert"};
    std::vector<std::string> sky_frag = {SHADER_PATH "/noise.frag", SHADER_PATH "/sky.frag"};

    phong_shader_.load(SHADER_PATH "/terrain.vert", SHADER_PATH "/terrain.frag");
    reflection_shader_.load(SHADER_PATH "/reflection.vert", SHADER_PATH "/reflection.frag");
    color_shader_.load(SHADER_PATH "/paint_color.vert", SHADER_PATH "/paint_color.frag");
    sky_shader_.load(sky_vert, sky_frag);
}
//-----------------------------------------------------------------------------

void MeshViewer::render_sky(Sky& sky, mat4 &_projection, mat4 &_view, Sphere unit_sphere) {
	mat4 m_matrix = mat4::translate(sky.pos_)
	              * mat4::scale(sky.radius_);
    
	mat4 mv_matrix  = _view * m_matrix;
    mat4 mvp_matrix = _projection * mv_matrix;
    sky_shader_.use();
    sky_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
	unit_sphere.draw();
    sky_shader_.disable();
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

	vec4 light = vec4(0., 0., 3., 1.0); //in world coordinates
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
    phong_shader_.set_uniform("mat", false);

	landActor->draw();
    phong_shader_.disable();

    // Render on the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width_, height_);

    glClearColor(100.0f, 100.0f, 100.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    fb->bind();

    color_shader_.use();
    color_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    color_shader_.set_uniform("resolution", vec2(width_, height_));
    color_shader_.set_uniform("color_map", 0);
    
	landActor->draw();
    color_shader_.disable();
    
    /// Draw filler
	m_matrix = fillerActor->model_matrix;
	mv_matrix  = _view * m_matrix;
	mvp_matrix = _projection * mv_matrix;

	n_matrix    = transpose(inverse(mv_matrix));

	phong_shader_.use();
	phong_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
	phong_shader_.set_uniform("modelview_matrix", mv_matrix);
	phong_shader_.set_uniform("normal_matrix", n_matrix);
	phong_shader_.set_uniform("light_position", -vec3(light));
    phong_shader_.set_uniform("mat", true);

	fillerActor->draw();
    phong_shader_.disable();

    color_shader_.use();
    color_shader_.set_uniform("modelview_projection_matrix", mvp_matrix);
    color_shader_.set_uniform("resolution", vec2(width_, height_));
    color_shader_.set_uniform("color_map", 0);
    
	fillerActor->draw();
    color_shader_.disable();


	/// Draw water
	m_matrix = waterActor->model_matrix;
	mv_matrix  = _view * m_matrix;
	mvp_matrix = _projection * mv_matrix;
	n_matrix    = transpose(inverse(mv_matrix));
	
	/// Draw sky 
	render_sky(sky_, _projection, _view, unit_sphere_);

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
