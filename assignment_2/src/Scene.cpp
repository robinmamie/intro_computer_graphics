//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

//== INCLUDES =================================================================
#include "Scene.h"

#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Mesh.h"
#include "Object.h"

#include <limits>
#include <map>
#include <functional>
#include <stdexcept>

#include <cmath>

#if HAS_TBB
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
#endif

#define EPS 1e-10
//-----------------------------------------------------------------------------

Image Scene::render()
{
    // allocate new image.
    Image img(camera.width, camera.height);

    // Function rendering a full column of the image
    auto raytraceColumn = [&img, this](int x) {
        for (int y=0; y<int(camera.height); ++y)
        {
            Ray ray = camera.primary_ray(x,y);

            // compute color by tracing this ray
            vec3 color = trace(ray, 0);

            // avoid over-saturation
            color = min(color, vec3(1, 1, 1));

            // store pixel color
            img(x,y) = color;
        }
    };

    // If possible, raytrace image columns in parallel. We use TBB if available
    // and try OpenMP otherwise. Note that OpenMP only works on the latest
    // clang compilers, so macOS users will probably have the best luck with TBB.
    // You can install TBB with MacPorts/Homebrew, or from Intel:
    // https://github.com/01org/tbb/releases
#if HAS_TBB
    tbb::parallel_for(tbb::blocked_range<int>(0, camera.width), [&raytraceColumn](const tbb::blocked_range<int> &range) {
        for (size_t i = range.begin(); i < range.end(); ++i)
            raytraceColumn(i);
    });
#else
#if defined(_OPENMP)
#pragma omp parallel for
#endif
    for (int x=0; x<int(camera.width); ++x)
        raytraceColumn(x);
#endif

    // Note: compiler will elide copy.
    return img;
}

//-----------------------------------------------------------------------------

vec3 Scene::trace(const Ray& _ray, int _depth)
{
    // stop if recursion depth (=number of reflections) is too large
    if (_depth > max_depth) return vec3(0,0,0);

    // Find first intersection with an object. If an intersection is found,
    // it is stored in object, point, normal, and t.
    Object_ptr  object;
    vec3        point;
    vec3        normal;
    double      t;
    if (!intersect(_ray, object, point, normal, t))
    {
        return background;
    }

    // Compute local Phong lighting (ambient+diffuse+specular)
    vec3 color = lighting(point, normal, -_ray.direction, object->material);

    // Compute reflections
    double alpha = object->material.mirror;

    if (alpha > 0.0) {
        vec3 reflected_dir = reflect(_ray.direction, normal);
        Ray ray_out(point + reflected_dir * EPS, reflected_dir);
        vec3 reflected_color = trace(ray_out, _depth + 1);

        color = (1 - alpha) * color + alpha * reflected_color;
    }

    return color;
}

//-----------------------------------------------------------------------------

bool Scene::intersect(const Ray& _ray, Object_ptr& _object, vec3& _point, vec3& _normal, double& _t)
{
    double  t, tmin(Object::NO_INTERSECTION);
    vec3    p, n;

    for (const auto &o: objects) // for each object
    {
        if (o->intersect(_ray, p, n, t)) // does ray intersect object?
        {
            if (t < tmin) // is intersection point the currently closest one?
            {
                tmin = t;
                _object = o.get();
                _point  = p;
                _normal = n;
                _t      = t;
            }
        }
    }

    return (tmin != Object::NO_INTERSECTION);
}

vec3 Scene::lighting(const vec3& _point, const vec3& _normal, const vec3& _view, const Material& _material)
{

    const vec3& ambient = ambience * _material.ambient;
    vec3 diffuse(0, 0, 0);
    vec3 specular(0, 0, 0);

    // Adding diffusion + specular for each light source.
    for(const auto light: lights) {

		const vec3& light_dir = normalize(light.position - _point);
		const double dot_nl = dot(_normal, light_dir);

        // Compute shadows
        const vec3& offset_point = _point + light_dir * EPS;
        Ray ray(offset_point, light_dir);
        Object_ptr object;
        vec3 a;
        double t;
        bool shadow = intersect(ray, object, a, a, t);

		// Diffusion
        if (!shadow && dot_nl >= 0) {
            diffuse +=  light.color * _material.diffuse * dot_nl;

            const vec3& reflectedRay = 2 * _normal * dot_nl - light_dir;
            const double dot_rv = dot(reflectedRay, _view);

			// Specular
            if (dot_rv >= 0) {
				specular += light.color *  _material.specular * pow(dot_rv, _material.shininess);
			}
        }
    }

    // Visualize the normal as a RGB color for now.
    vec3 color = ambient + diffuse + specular;

    return color;
}

//-----------------------------------------------------------------------------

void Scene::read(const std::string &_filename)
{
    std::ifstream ifs(_filename);
    if (!ifs)
        throw std::runtime_error("Cannot open file " + _filename);

    const std::map<std::string, std::function<void(void)>> entityParser = {
        {"depth",      [&]() { ifs >> max_depth; }},
        {"camera",     [&]() { ifs >> camera; }},
        {"background", [&]() { ifs >> background; }},
        {"ambience",   [&]() { ifs >> ambience; }},
        {"light",      [&]() { lights .emplace_back(ifs); }},
        {"plane",      [&]() { objects.emplace_back(new    Plane(ifs)); }},
        {"sphere",     [&]() { objects.emplace_back(new   Sphere(ifs)); }},
        {"cylinder",   [&]() { objects.emplace_back(new Cylinder(ifs)); }},
        {"mesh",       [&]() { objects.emplace_back(new     Mesh(ifs, _filename)); }}
    };

    // parse file
    std::string token;
    while (ifs && (ifs >> token) && (!ifs.eof())) {
        if (token[0] == '#') {
            ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (entityParser.count(token) == 0)
            throw std::runtime_error("Invalid token encountered: " + token);
        entityParser.at(token)();
    }
}


//=============================================================================
