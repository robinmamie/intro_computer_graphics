#pragma once

#include "gl.h"
#include "glfw_window.h"

#include "FrameBuffer.h"
#include "shader.h"
#include "StaticMeshActor.h"
#include <memory>

//=============================================================================


/// OpenGL viewer that handles all the rendering for us
class MeshViewer : public GLFW_window
{
public:

	/// default constructor
	/// \_title the window's title
	/// \_width the window's width
	/// \_height the window's height
	MeshViewer(std::string const& _title = "Mesh Viewer", int _width=1920, int _height=1080);
    ~MeshViewer();

	void setMesh(std::shared_ptr<Mesh> new_landMesh, std::shared_ptr<Mesh> new_waterMesh, std::shared_ptr<Mesh> new_fillerMesh);

	virtual void update_water(double dt) override;

protected:

	/// function that is called on the creation of the widget for the initialisation of OpenGL
	virtual void initialize() override;

	/// resize function - called when the window size is changed
	virtual void resize(int width, int height) override;

	/// paint function - called when the window should be refreshed
	virtual void paint() override;

	/// keyboard interaction
	virtual void keyboard(int key, int scancode, int action, int mods) override;

	virtual void scroll_wheel(double xoffset, double yoffset) override;

	/// function that draws the planet system
	/// \param _projection the projection matrix for the scene
	/// \param _view the view matrix for the scene
	void draw_scene(mat4& _projection, mat4& _view);


private:
	Shader phong_shader_;

	Shader reflection_shader_;

	Shader color_shader_;

    std::unique_ptr<FrameBuffer> fb;

	/// the field of view for the camera
	float fovy_;
	/// the near plane for the virtual camera
	float near_;
	/// the far plane for the virtual camera
	float far_;

	/// rotation in x direction around the planet/sun from the original point
	float x_angle_;
	/// rotation in y direction around the planet/sun from the original point
	float z_angle_;

	float camera_distance;

	/// current viewport dimension
	int  width_, height_;

	std::shared_ptr<Mesh> landMesh;
	std::shared_ptr<Mesh> waterMesh;
	std::shared_ptr<Mesh> fillerMesh1;
	std::shared_ptr<StaticMeshActor> landActor;
	std::shared_ptr<StaticMeshActor> waterActor;
	std::shared_ptr<StaticMeshActor> fillerActor1;


    GLuint frame_buffer_, color_, depth_;

};


//=============================================================================
