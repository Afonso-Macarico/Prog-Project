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
        int count=0;
        while (count+1<points.size()) {
            img.draw_line(points[count], points[count+1], stroke);
            count++;
        }
    }
    void
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->

}
