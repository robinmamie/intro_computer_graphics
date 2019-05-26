#include "render/ShaderViewer.h"
#include <vector>
#include <string>
#include <cmath>


#include "render/Mesh.h"
#include "render/MeshViewer.h"

// Render your GLSL fBm into a 2D array of height values.
// \param size	resolution at which to sample the fBm.
Array2D<float> calculate_fbm(std::pair<size_t, size_t> const& size) {
	ShaderViewer shvi("fBm", size.first, size.second);
	std::vector<std::string>  vtxShaders = { SHADER_PATH "/display.vert" };
	std::vector<std::string> fragShaders = { SHADER_PATH "/noise.frag",
											 SHADER_PATH "/display_fbm_terrain.frag" };
	shvi.setShaderFiles(vtxShaders, fragShaders);

	Array2D<vec4> noise_value_rgbd({0, 0});
	shvi.run([&]() {
		shvi.resize(size.first, size.second); // Force the texture size--GLFW might have given us the wrong window size.
		noise_value_rgbd = shvi.extractFrame();
		return false;
	});

	return noise_value_rgbd.map<float>([](size_t x, size_t y, vec4 const& val_rgbd) {
		return val_rgbd[0];
	});
}

Array2D<float> compute_water_at_time(ShaderViewer& shvi, std::pair<size_t, size_t> const& size, float time){
	shvi.set_time(time);

	Array2D<vec4> noise_value_rgbd({0, 0});
	shvi.run([&]() {
		shvi.resize(size.first, size.second); // Force the texture size--GLFW might have given us the wrong window size.
		noise_value_rgbd = shvi.extractFrame();
		return false;
	});

	return noise_value_rgbd.map<float>([](size_t x, size_t y, vec4 const& val_rgbd) {
		return val_rgbd[0];
	});
}

// NB_WATER_ITERATIONS is defined in mesh.h
#define WATER_DELTA_T 0.001f//0.007f // corresponds to the steps in the z coordinate in the perlin3d function

// This function simply remaps the nested Array2D<vec4> into an Array2D<float>
std::vector<Array2D<float>> remap(std::vector<Array2D<vec4>> values){
	std::vector<Array2D<float>> value;
	for(int i=0; i< values.size(); i++){
		Array2D<float> v = values[i].map<float>([](size_t x, size_t y, vec4 const& val_rgbd) {return val_rgbd[0];});
		value.push_back(v);
	}
	return value;
}

std::vector<Array2D<float>> generate_water(std::pair<size_t, size_t> const& size) {
	ShaderViewer shvi("water", size.first, size.second);
	std::vector<std::string>  vtxShaders = { SHADER_PATH "/display.vert" };
	std::vector<std::string> fragShaders = { SHADER_PATH "/noise.frag",
											 SHADER_PATH "/display_water.frag" };
	shvi.setShaderFiles(vtxShaders, fragShaders);

	std::vector<Array2D<vec4>> noise_values_rgbd; //({0, 0});
	shvi.run([&]() {
		shvi.resize(size.first, size.second); // Force the texture size--GLFW might have given us the wrong window size.

		for(int i= 0; i < NB_WATER_ITERATIONS; i++){
			shvi.set_time(i*WATER_DELTA_T);
			noise_values_rgbd.push_back(shvi.extractFrame());
		}
		return false;
	});

	return remap(noise_values_rgbd);
}

constexpr float WATER_LEVEL = 0; //-0.03125;

// Procedurally generate a triangle mesh of the terrain from the 2D array of height values.
// This triangle mesh is a uniform grid in the (x, y) plane that is displaced in the z
// direction by the height values.
std::shared_ptr<Mesh> build_terrain_mesh(Array2D<float> const& height_map, bool isWater) {
	std::pair<size_t, size_t> const grid_size = height_map.size();

	std::vector<vec3> vertices(grid_size.first * grid_size.second);
	std::vector<Mesh::Face> faces;

	// Map a 2D grid index (x, y) into a 1D index into the output vertex array.
	auto const xy_to_v_index = [&](int x, int y) {
		return x + y*grid_size.first;
	};

	// First, generate the displaced vertices of the grid.
	// (iterate over y first then over x to use CPU cache better)
	for(int gy = 0; gy < grid_size.second; gy++) {
		for(int gx = 0; gx < grid_size.first; gx++) {
			int const idx = xy_to_v_index(gx, gy);

            float x = gx / (float) grid_size.first  - 0.5f;
            float y = gy / (float) grid_size.second - 0.5f;
            float z = height_map(gx, gy);
            z = isWater ? WATER_LEVEL - height_map(gx, gy) : z;
			vertices[idx] = vec3(x, y, z);
		}
	}

	// Second, connect the grid vertices with triangles to triangulate the terrain.
	for(int gy = 0; gy < grid_size.second-1; gy++) {
		for(int gx = 0; gx < grid_size.first-1; gx++) {
			long unsigned int const idx[4] = {
                xy_to_v_index(gx  , gy),
                xy_to_v_index(gx+1, gy),
                xy_to_v_index(gx  , gy+1),
                xy_to_v_index(gx+1, gy+1)
            };
            faces.push_back(Mesh::Triangle(idx[0], idx[1], idx[2]));
            faces.push_back(Mesh::Triangle(idx[1], idx[3], idx[2]));
		}
	}

	return std::make_shared<Mesh>(vertices, faces);
}



/// Aquarium effect
std::shared_ptr<Mesh> build_filler_mesh(Array2D<float> const& height_map) {
	std::pair<size_t, size_t> const grid_size = height_map.size();

	std::vector<vec3> vertices((4*grid_size.first) + (4*grid_size.second));
	std::vector<Mesh::Face> faces;

	long unsigned int index = 0;

	float z_low = -0.5;

	// First wall
	float x = -0.5f;
	float y = -0.5f;
	float z = height_map(0,0);
	vertices[index] = vec3(x,y,z_low);
	vertices[index + 1] = vec3(x,y,z);

	for(int gx = 1; gx < grid_size.first; gx++) {

        x = gx / (float) (grid_size.first)  - 0.5f;
        z = height_map(gx, 0);

		vertices[index + 2] = vec3(x,y,z_low);
		vertices[index + 3] = vec3(x,y,z);
		
		faces.push_back(Mesh::Triangle(index + 1,index,index + 2));
		faces.push_back(Mesh::Triangle(index + 1,index + 2,index + 3));
		index += 2;
	}
	index += 2;
	// Second wall
	x = -0.5f;
	y = 0.5f - 1.0f / grid_size.second;
	z = height_map(0,grid_size.second - 1);
	vertices[index] = vec3(x,y,z_low);
	vertices[index + 1] = vec3(x,y,z);

	for(int gx = 1; gx < grid_size.first; gx++) {

        x = gx / (float) (grid_size.first)  - 0.5f;
        z = height_map(gx, grid_size.second - 1);
		
		vertices[index + 2] = vec3(x,y,z_low);
		vertices[index + 3] = vec3(x,y,z);
		
		faces.push_back(Mesh::Triangle(index + 1,index,index + 2));
		faces.push_back(Mesh::Triangle(index + 1,index + 2,index + 3));
		index += 2;
	}

	index += 2;
	// Third wall
	x = -0.5f;
	y = -0.5f;
	z = height_map(0,0);
	vertices[index] = vec3(x,y,z_low);
	vertices[index + 1] = vec3(x,y,z);

	for(int gy = 1; gy < grid_size.second; gy++) {

        y = gy / (float) (grid_size.second)  - 0.5f;
        z = height_map(0, gy);

		vertices[index + 2] = vec3(x,y,z_low);
		vertices[index + 3] = vec3(x,y,z);
		
		faces.push_back(Mesh::Triangle(index + 1,index,index + 2));
		faces.push_back(Mesh::Triangle(index + 1,index + 2,index + 3));
		index += 2;
	}

	index += 2;
	// Fourth wall
	x = 0.5f - 1.0f / grid_size.first;
	y = -0.5f;
	z = height_map(grid_size.first-1,0);
	vertices[index] = vec3(x,y,z_low);
	vertices[index + 1] = vec3(x,y,z);

	for(int gy = 1; gy < grid_size.second; gy++) {

        y = gy / (float) (grid_size.second)  - 0.5f;
        z = height_map(grid_size.first-1, gy);

		vertices[index + 2] = vec3(x,y,z_low);
		vertices[index + 3] = vec3(x,y,z);
		
		faces.push_back(Mesh::Triangle(index + 1,index,index + 2));
		faces.push_back(Mesh::Triangle(index + 1,index + 2,index + 3));
		index += 2;
	}

	// Ground
	for (int i = 2; i < 2*grid_size.first; i += 2) {
		long unsigned int i00 = i - 2;
		long unsigned int i10 = i;
		long unsigned int i01 = 2*grid_size.first + i - 2;
		long unsigned int i11 = 2*grid_size.first + i;
		faces.push_back(Mesh::Triangle(i00,i10,i01));
		faces.push_back(Mesh::Triangle(i11,i01,i10));
	}

	return std::make_shared<Mesh>(vertices, faces);
}


int main(int arg_count, char *arg_values[]) {
	std::pair<size_t, size_t> grid_size(256, 256);

	// If we try to build meshes when no window is created (GLFW is not loaded)
	// calls to OpenGL will crash randomly

	std::vector<Array2D<float>> water_values = generate_water(grid_size);

	Array2D<float> fbm_values = calculate_fbm(grid_size);
	fbm_values *= 0.5;

	MeshViewer mvi;
	auto meshLand = build_terrain_mesh(fbm_values, false);
	auto meshFiller = build_filler_mesh(fbm_values);
	auto meshWater = build_terrain_mesh(water_values[0], true);
	meshWater->water_values = water_values; // save the values for animation


	meshLand->print_info();
	meshWater->print_info();
	meshFiller->print_info();
	mvi.setMesh(meshLand, meshWater, meshFiller);
	return mvi.run();
}
