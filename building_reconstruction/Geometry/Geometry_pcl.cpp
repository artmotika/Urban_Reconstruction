#include "Geometry_pcl.h"

using namespace pcl;

bool Geometry_pcl::point_in_radius(PointXYZ p, PointXYZ center, double radius) {
    return pow(p.x - center.x, 2) + pow(p.y - center.y, 2) + pow(p.z - center.z, 2) < pow(radius, 2);
}

bool Geometry_pcl::point_in_radius(PointNormal p, PointXYZ center, double radius) {
    return pow(p.x - center.x, 2) + pow(p.y - center.y, 2) + pow(p.z - center.z, 2) < pow(radius, 2);
}

double Geometry_pcl::euclidean_dist_between_two_points(PointXYZ a, PointXYZ b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

double Geometry_pcl::euclidean_dist_between_two_points(PointXYZRGB a, PointXYZRGB b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

double Geometry_pcl::max_euclidean_dist_side_in_polygon(std::vector <PointXYZ> ps) {
    if (ps.size() < 3) return 0.0;
    pcl::PointXYZ p1 = ps[0];
    pcl::PointXYZ p2 = ps[1];
    pcl::PointXYZ p3 = ps[2];
    double max_dist = euclidean_dist_between_two_points(p1, p2);
    max_dist = std::max(max_dist, euclidean_dist_between_two_points(p2, p3));
    max_dist = std::max(max_dist, euclidean_dist_between_two_points(p1, p3));
    return max_dist;
}

double Geometry_pcl::triangle_area_geron(PointXYZ p1, PointXYZ p2, PointXYZ p3) {
    double a = euclidean_dist_between_two_points(p1, p2);
    double b = euclidean_dist_between_two_points(p2, p3);
    double c = euclidean_dist_between_two_points(p1, p3);
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

double Geometry_pcl::min_euclidean_dist_between_point_and_polygon_points(PointXYZ p, std::vector <PointXYZ> ps) {
    pcl::PointXYZ start_p = *ps.begin();
    double min_dist = euclidean_dist_between_two_points(p, start_p);
    for (std::vector<pcl::PointXYZ>::iterator it = ps.begin() + 1; it < ps.end(); it++) {
        min_dist = std::min(min_dist, euclidean_dist_between_two_points(p, *it));
    }
    return min_dist;
}

double sign(Eigen::Vector2i p1, Eigen::Vector2i p2, Eigen::Vector2i p3) {
    return (p1(0) - p3(0)) * (p2(1) - p3(1)) - (p2(0) - p3(0)) * (p1(1) - p3(1));
}

bool
Geometry_pcl::check_point_in_triangle(Eigen::Vector2i pt, Eigen::Vector2i p1, Eigen::Vector2i p2, Eigen::Vector2i p3) {
    double d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, p1, p2);
    d2 = sign(pt, p2, p3);
    d3 = sign(pt, p3, p1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

bool Geometry_pcl::check_point_in_triangle_vector(Eigen::Vector2i pt, Eigen::Vector2i p1, Eigen::Vector2i p2,
                                                  Eigen::Vector2i p3) {
    // Compute vectors
    Eigen::Vector2d v0, v1, v2;
    v0(0) = p3(0) - p1(0);
    v0(1) = p3(1) - p1(1); // v0= C - A
    v1(0) = p2(0) - p1(0);
    v1(1) = p2(1) - p1(1); // v1= B - A
    v2(0) = pt(0) - p1(0);
    v2(1) = pt(1) - p1(1); // v2= P - A

    // Compute dot products
    double dot00 = v0.dot(v0); // dot00 = dot(v0, v0)
    double dot01 = v0.dot(v1); // dot01 = dot(v0, v1)
    double dot02 = v0.dot(v2); // dot02 = dot(v0, v2)
    double dot11 = v1.dot(v1); // dot11 = dot(v1, v1)
    double dot12 = v1.dot(v2); // dot12 = dot(v1, v2)

    // Compute barycentric coordinates
    double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return ((u >= 0) && (v >= 0) && (u + v < 1));
}

double Geometry_pcl::dist_between_two_points(Eigen::Vector2i p1, Eigen::Vector2i p2) {
    return sqrt(pow(p1(0) - p2(0), 2) + pow(p1(1) - p2(1), 2));
}

bool Geometry_pcl::arePointsCollinear(pcl::PointXYZ a, pcl::PointXYZ b, pcl::PointXYZ c) {
    float epsilon = std::numeric_limits<float>::epsilon();
    return triangle_area_geron(a, b, c) < epsilon;
}

bool Geometry_pcl::isPointInsideSegment(pcl::PointXYZ a, pcl::PointXYZ b, pcl::PointXYZ p) {
    float epsilon = std::numeric_limits<float>::epsilon();
    return p.x >= fmin(a.x, b.x) - epsilon && p.x <= fmax(a.x, b.x) + epsilon &&
           p.y >= fmin(a.y, b.y) - epsilon && p.y <= fmax(a.y, b.y) + epsilon &&
           p.z >= fmin(a.z, b.z) - epsilon && p.z <= fmax(a.z, b.z) + epsilon;
}

double Geometry_pcl::distanceToSegment(pcl::PointXYZ a, pcl::PointXYZ b, pcl::PointXYZ p) {
    float epsilon = std::numeric_limits<float>::epsilon();
    double l2 = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
    if (l2 < epsilon) {
        return sqrt((p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y) + (p.z - a.z) * (p.z - a.z));
    }

    double t = ((p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y) + (p.z - a.z) * (b.z - a.z)) / l2;
    t = std::max(0.0, std::min(1.0, t));

    double x = a.x + t * (b.x - a.x);
    double y = a.y + t * (b.y - a.y);
    double z = a.z + t * (b.z - a.z);

    return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z - z));
}


bool Geometry_pcl::isSegmentOnSegment(pcl::PointXYZ a, pcl::PointXYZ b, pcl::PointXYZ c, pcl::PointXYZ d) {
    if (!arePointsCollinear(a, b, c) || !arePointsCollinear(a, b, d)) {
        return false;
    }
    return isPointInsideSegment(a, b, c) && isPointInsideSegment(a, b, d) ||
           isPointInsideSegment(c, d, a) && isPointInsideSegment(c, d, b);
}

bool Geometry_pcl::arePointsEqual(pcl::PointXYZ a, pcl::PointXYZ b) {
    float epsilon = std::numeric_limits<float>::epsilon();
    return abs(a.x - b.x) < epsilon && abs(a.y - b.y) < epsilon && abs(a.z - b.z) < epsilon;
}

pcl::PointXYZ Geometry_pcl::getTriangleCenterOfMass(pcl::PointXYZ a, pcl::PointXYZ b, pcl::PointXYZ c) {
    float x = (a.x + b.x + c.x) / 3.0;
    float y = (a.y + b.y + c.y) / 3.0;
    float z = (a.z + b.z + c.z) / 3.0;
    pcl::PointXYZ center(x, y, z);
    return center;
}

bool Geometry_pcl::checkPointInsideTriangle(const pcl::PointXY &p1, const pcl::PointXY &p2, const pcl::PointXY &p3,
                                            const pcl::PointXY &pt) {
    // Compute vectors
    Eigen::Vector2d v0, v1, v2;
    v0(0) = p3.x - p1.x;
    v0(1) = p3.y - p1.y; // v0= C - A
    v1(0) = p2.x - p1.x;
    v1(1) = p2.y - p1.y; // v1= B - A
    v2(0) = pt.x - p1.x;
    v2(1) = pt.y - p1.y; // v2= P - A

    // Compute dot products
    double dot00 = v0.dot(v0); // dot00 = dot(v0, v0)
    double dot01 = v0.dot(v1); // dot01 = dot(v0, v1)
    double dot02 = v0.dot(v2); // dot02 = dot(v0, v2)
    double dot11 = v1.dot(v1); // dot11 = dot(v1, v1)
    double dot12 = v1.dot(v2); // dot12 = dot(v1, v2)

    // Compute barycentric coordinates
    double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return ((u >= 0) && (v >= 0) && (u + v < 1));
}

double Geometry_pcl::triangle_area(const pcl::PointXY &p1, const pcl::PointXY &p2, const pcl::PointXY &p3) {
    return 0.5 * abs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y));
}



