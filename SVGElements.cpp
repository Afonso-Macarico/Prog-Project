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

    Rect::Rect(const Color &fill, const int &width, const int &height, const Point &corner) : Polygon(fill,{
        corner,
        {corner.x + width-1, corner.y},
        {corner.x + width-1, corner.y+height-1},
        {corner.x, corner.y+height-1}
    }) {}

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


    void Ellipse::translate(const Point& t){
        center = center.translate(t);
    }
    void Ellipse::rotate(const Point& origin, int degrees){
        center = center.rotate(origin, degrees);
    }
    void Ellipse::scale(const Point& origin, int v){
        center = center.scale(origin, v);
        radius = radius.scale({0,0}, v);

    }

    void Circle::translate(const Point& t){
        center = center.translate(t);
    }
    void Circle::rotate(const Point& origin, int degrees){
        center = center.rotate(origin, degrees);
    }
    void Circle::scale(const Point& origin, int v){
        center = center.scale(origin, v);
        radius = radius*v;
    }

    void Polyline::translate(const Point& t){
        for (Point& p : points){
            p = p.translate(t);
        }
    }
    void Polyline::rotate(const Point& origin, int degrees){
        for (Point& p : points){
            p = p.rotate(origin, degrees);
        }
    }
    void Polyline::scale(const Point& origin, int v){
        for (Point& p : points){
            p = p.scale(origin, v);
        }
    }

    void Line::translate(const Point& t){
        start = start.translate(t);
        end = end.translate(t);
    }
    void Line::rotate(const Point& origin, int degrees){
        start = start.rotate(origin, degrees);
        end = end.rotate(origin, degrees);
    }
    void Line::scale(const Point& origin, int v){
        start = start.scale(origin, v);
        end = end.scale(origin, v);
    }

    void Polygon::translate(const Point& t){
        for (Point& p : points){
            p = p.translate(t);
        }
    }
    void Polygon::rotate(const Point& origin, int degrees){
            for (Point& p : points){
            p = p.rotate(origin, degrees);
        }
    }
    void Polygon::scale(const Point& origin, int v){
        for (Point& p : points){
            p = p.scale(origin, v);
        }
    }

    SVGElement* Ellipse::clone() const {return new Ellipse(*this);}
    SVGElement* Circle::clone() const {return new Circle(*this);}
    SVGElement* Polyline::clone() const {return new Polyline(*this);}
    SVGElement* Line::clone() const {return new Line(*this);}
    SVGElement* Polygon::clone() const {return new Polygon(*this);}
    SVGElement* Rect::clone() const {return new Rect(*this);}

}
