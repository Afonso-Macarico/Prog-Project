#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    Circle::Circle(const Color &fill, const Point &center, const int &radius) : fill(fill), center(center), radius(radius) {}

    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points) : stroke(stroke), points(points) {}

    Line::Line(const Color &stroke, const Point &start, const Point &end) : stroke(stroke), start(start), end(end) {}

    Polygon::Polygon(const Color &fill, const std::vector<Point> &points) : fill(fill), points(points) {}

    Rect::Rect(const Color &fill, const int &width, const int &height, const Point &corner) : fill(fill), width(width), height(height), corner(corner) {}

    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    void Circle::draw(PNGImage &img) const
    {
        Point raio;
        raio.x=radius;
        raio.y=radius;
        img.draw_ellipse(center, raio, fill);
    }
    void Polyline::draw(PNGImage &img) const
    {
        size_t count=0;
        while (count+1<points.size()) {
            img.draw_line(points[count], points[count+1], stroke);
            count++;
        }
    }

    void Line::draw(PNGImage &img) const {
        img.draw_line(start, end, stroke);
    }

    void Polygon::draw(PNGImage &img) const {
        img.draw_polygon(points, fill);
    }

    void Rect::draw(PNGImage &img) const {
        Point up_r_corner, down_r_corner, down_l_corner;
        up_r_corner.x= corner.x + width;
        up_r_corner.y= corner.y;
        down_r_corner.x = corner.x +width;
        down_r_corner.y=corner.y + height;
        down_l_corner.x = corner.x;
        down_l_corner.y = corner.y + height;
        std::vector<Point> ps;
        ps.push_back(corner);
        ps.push_back(up_r_corner);
        ps.push_back(down_r_corner);
        ps.push_back(down_l_corner);
        img.draw_polygon(ps, fill);
    }

}
