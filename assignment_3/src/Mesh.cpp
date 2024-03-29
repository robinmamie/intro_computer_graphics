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

#include "Mesh.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <limits>


//== IMPLEMENTATION ===========================================================


Mesh::Mesh(std::istream &is, const std::string &scenePath)
{
    std::string meshFile, mode;
    is >> meshFile;

    // load mesh from file
    read(scenePath.substr(0, scenePath.find_last_of("/\\") + 1) + meshFile); // Use both Unix and Windows path separators

    is >> mode;
    if      (mode ==  "FLAT") draw_mode_ = FLAT;
    else if (mode == "PHONG") draw_mode_ = PHONG;
    else throw std::runtime_error("Invalid draw mode " + mode);

    is >> material;
}


//-----------------------------------------------------------------------------


bool Mesh::read(const std::string &_filename)
{
    // read a mesh in OFF format


    // open file
    std::ifstream ifs(_filename);
    if (!ifs)
    {
        std::cerr << "Can't open " << _filename << "\n";
        return false;
    }


    // read OFF header
    std::string s;
    unsigned int nV, nF, dummy, i;
    ifs >> s;
    if (s != "OFF")
    {
        std::cerr << "No OFF file\n";
        return false;
    }
    ifs >> nV >> nF >> dummy;
    std::cout << "\n  read " << _filename << ": " << nV << " vertices, " << nF << " triangles";


    // read vertices
    Vertex v;
    vertices_.clear();
    vertices_.reserve(nV);
    for (i=0; i<nV; ++i)
    {
        ifs >> v.position;
        vertices_.push_back(v);
    }


    // read triangles
    Triangle t;
    triangles_.clear();
    triangles_.reserve(nF);
    for (i=0; i<nF; ++i)
    {
        ifs >> dummy >> t.i0 >> t.i1 >> t.i2;
        triangles_.push_back(t);
    }


    // close file
    ifs.close();


    // compute face and vertex normals
    compute_normals();

    // compute bounding box
    compute_bounding_box();


    return true;
}


//-----------------------------------------------------------------------------

// Determine the weights by which to scale triangle (p0, p1, p2)'s normal when
// accumulating the vertex normals for vertices 0, 1, and 2.
// (Recall, vertex normals are a weighted average of their incident triangles'
// normals, and in our raytracer we'll use the incident angles as weights.)
// \param[in] p0, p1, p2    triangle vertex positions
// \param[out] w0, w1, w2    weights to be used for vertices 0, 1, and 2
void angleWeights(const vec3 &p0, const vec3 &p1, const vec3 &p2,
                  double &w0, double &w1, double &w2) {
    // compute angle weights
    const vec3 e01 = normalize(p1-p0);
    const vec3 e12 = normalize(p2-p1);
    const vec3 e20 = normalize(p0-p2);
    w0 = acos( std::max(-1.0, std::min(1.0, dot(e01, -e20) )));
    w1 = acos( std::max(-1.0, std::min(1.0, dot(e12, -e01) )));
    w2 = acos( std::max(-1.0, std::min(1.0, dot(e20, -e12) )));
}


//-----------------------------------------------------------------------------

void Mesh::compute_normals()
{
    // compute triangle normals
    for (Triangle& t: triangles_)
    {
        const vec3& p0 = vertices_[t.i0].position;
        const vec3& p1 = vertices_[t.i1].position;
        const vec3& p2 = vertices_[t.i2].position;
        t.normal = normalize(cross(p1-p0, p2-p0));
    }

    // initialize vertex normals to zero
    for (Vertex& v: vertices_)
    {
        v.normal = vec3(0,0,0);
    }

    for (const Triangle t: triangles_)
    {
        double w0, w1, w2;
        Vertex& v0 = vertices_[t.i0];
        Vertex& v1 = vertices_[t.i1];
        Vertex& v2 = vertices_[t.i2];
        angleWeights(v0.position, v1.position, v2.position, w0, w1, w2);

        const vec3& n = t.normal;
        v0.normal += w0 * n;
        v1.normal += w1 * n;
        v2.normal += w2 * n;
    }

    for (Vertex& v: vertices_)
    {
        v.normal = normalize(v.normal);
    }
}


//-----------------------------------------------------------------------------


void Mesh::compute_bounding_box()
{
    bb_min_ = vec3(std::numeric_limits<double>::max());
    bb_max_ = vec3(std::numeric_limits<double>::lowest());

    for (Vertex v: vertices_)
    {
        bb_min_ = min(bb_min_, v.position);
        bb_max_ = max(bb_max_, v.position);
    }
}


//-----------------------------------------------------------------------------


bool Mesh::intersect_bounding_box(const Ray& _ray) const
{

    /**
    * Source of code:
    * https://tavianator.com/fast-branchless-raybounding-box-intersections/
    * Archive link:
    * https://web.archive.org/web/20180218205615/https://tavianator.com/fast-branchless-raybounding-box-intersections/
    */

    const vec3& d_inv = vec3(1 / _ray.direction[0],
                             1 / _ray.direction[1],
                             1 / _ray.direction[2]);

    const vec3& t1 = (bb_min_ - _ray.origin) * d_inv;
    const vec3& t2 = (bb_max_ - _ray.origin) * d_inv;

    const vec3& tmin = min(t1, t2);
    const vec3& tmax = max(t1, t2);

    double vmin = std::max(tmin[0], std::max(tmin[1], tmin[2]));
    double vmax = std::min(tmax[0], std::min(tmax[1], tmax[2]));

    return vmin <= vmax;
}


//-----------------------------------------------------------------------------


bool Mesh::intersect(const Ray& _ray,
                     vec3&      _intersection_point,
                     vec3&      _intersection_normal,
                     double&    _intersection_t ) const
{
    // check bounding box intersection
    if (!intersect_bounding_box(_ray))
    {
        return false;
    }

    vec3   p, n;
    double t;

    _intersection_t = NO_INTERSECTION;

    // for each triangle
    for (const Triangle& triangle : triangles_)
    {
        // does ray intersect triangle?
        if (intersect_triangle(triangle, _ray, p, n, t))
        {
            // is intersection closer than previous intersections?
            if (t < _intersection_t)
            {
                // store data of this intersection
                _intersection_t      = t;
                _intersection_point  = p;
                _intersection_normal = n;
            }
        }
    }

    return (_intersection_t != NO_INTERSECTION);
}


//-----------------------------------------------------------------------------


bool
Mesh::
intersect_triangle(const Triangle&  _triangle,
                   const Ray&       _ray,
                   vec3&            _intersection_point,
                   vec3&            _intersection_normal,
                   double&          _intersection_t) const
{
    const Vertex& v0 = vertices_[_triangle.i0];
    const Vertex& v1 = vertices_[_triangle.i1];
    const Vertex& v2 = vertices_[_triangle.i2];

    const vec3& p0 = v0.position;
    const vec3& p1 = v1.position;
    const vec3& p2 = v2.position;

    _intersection_t = NO_INTERSECTION;

    const vec3& col_1 = p0 - p2;
    const vec3& col_2 = p1 - p2;
    const vec3& col_3 = - _ray.direction;
    const vec3& col_r = _ray.origin - p2;

    double det_M = dot(cross(col_1, col_2), col_3);

    if (det_M == 0.0) {
        return false;
    }

    double det_M1 = dot(cross(col_r, col_2), col_3);
    double det_M2 = dot(cross(col_1, col_r), col_3);
    double det_M3 = dot(cross(col_1, col_2), col_r);

    double a = det_M1 / det_M;
    double b = det_M2 / det_M;
    double c = 1 - a - b;
    double t = det_M3 / det_M;

    if (a < 0 || b < 0 || c < 0 || t <= 0) {
        return false;
    }

    _intersection_t = t;
    _intersection_point = _ray.origin + t * _ray.direction;

    if (draw_mode_ == FLAT) {
        _intersection_normal = _triangle.normal;
    } else {
        _intersection_normal =
            normalize(a * v0.normal + b * v1.normal + c * v2.normal);
    }

    return true;
}


//=============================================================================
