//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

namespace svg
{
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual void translate(const Point& t) =0;
        virtual void rotate(const Point& origin, int degrees) =0;
        virtual void scale(const Point& origin, int v)=0;
        virtual SVGElement* clone() const = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);

    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;
        void translate(const Point& t) override;
        void rotate(const Point& origin, int degrees) override;
        void scale(const Point& origin, int v) override;
        SVGElement* clone() const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };
    class Circle : public SVGElement
    {
    public:
        Circle(const Color &fill, const Point &center, const int &radius);
        void draw(PNGImage &img) const override;
        void translate(const Point& t) override;
        void rotate(const Point& origin, int degrees) override;
        void scale(const Point& origin, int v) override;
        SVGElement* clone() const override;

    private:
        Color fill;
        Point center;
        int radius;
    };
    class Polyline : public SVGElement
    {
    public:
        Polyline(const Color &stroke, const std::vector<Point> &points);
        void draw(PNGImage &img) const override;
        void translate(const Point& t) override;
        void rotate(const Point& origin, int degrees) override;
        void scale(const Point& origin, int v) override;
        SVGElement* clone() const override;

    private:
        Color stroke;
        std::vector<Point> points;
    };
    class Line : public SVGElement
    {
    public:
        Line(const Color &stroke, const Point &start, const Point &end);
        void draw(PNGImage &img) const override;
        void translate(const Point& t) override;
        void rotate(const Point& origin, int degrees) override;
        void scale(const Point& origin, int v) override;
        SVGElement* clone() const override;

    private:
        Color stroke;
        Point start;
        Point end;
    };
    class Polygon : public SVGElement
    {
    public:
        Polygon(const Color &fill, const std::vector<Point> &points);
        void draw(PNGImage &img) const override;
        void translate(const Point& t) override;
        void rotate(const Point& origin, int degrees) override;
        void scale(const Point& origin, int v) override;
        SVGElement* clone() const override;


    private:
        Color fill;
        std::vector<Point> points;
    };
    class Rect : public Polygon
    {
    public:
        Rect(const Color &fill, const int &width, const int &height, const Point &corner);
        SVGElement* clone() const override;

    };
    class Group : public SVGElement
    {
    public:
            Group(const std::vector<SVGElement*> &elements);
            ~Group();
            void draw(PNGImage &img) const override;
            SVGElement *clone() const override;
            void translate(const Point &t) override;
            void rotate(const Point &origin, int degrees) override;
            void scale(const Point &origin, int v) override;
        private:
            std::vector<SVGElement*> elements;
    };



}
#endif
