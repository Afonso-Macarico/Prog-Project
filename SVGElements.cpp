#include "SVGElements.hpp"
#include <vector>

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
    {}


    Circle::Circle(const Color &fill, const Point &center, const int &radius) : Ellipse(fill,center, {radius, radius}){}

    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points) : stroke(stroke), points(points) {}

    Line::Line(const Color &stroke, const Point &start, const Point &end) : Polyline(stroke,{start, end}){}

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

    void Polyline::draw(PNGImage &img) const
    {
        size_t count=0;
        while (count+1<points.size()) {
            img.draw_line(points[count], points[count+1], stroke);
            count++;
        }
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
    Group::Group(const std::vector<SVGElement*> &elements) : elements(elements) {}

    Group::~Group() { for (SVGElement *e : elements) delete e; }

    void Group::draw(PNGImage &img) const { for (SVGElement *e : elements) e->draw(img); }

    SVGElement *Group::clone() const {
        std::vector<SVGElement*> copies;
        for (SVGElement *e : elements) copies.push_back(e->clone());
        return new Group(copies);
    }

    void Group::translate(const Point &t) { for (SVGElement *e : elements) e->translate(t); }

    void Group::rotate(const Point &origin, int deg) { for (SVGElement *e : elements) e->rotate(origin, deg); }

    void Group::scale(const Point &origin, int v) { for (SVGElement *e : elements) e->scale(origin, v); }

    SVGElement* Ellipse::clone() const {return new Ellipse(*this);}
    SVGElement* Circle::clone() const {return new Circle(*this);}
    SVGElement* Polyline::clone() const {return new Polyline(*this);}
    SVGElement* Line::clone() const {return new Line(*this);}
    SVGElement* Polygon::clone() const {return new Polygon(*this);}
    SVGElement* Rect::clone() const {return new Rect(*this);}

}
