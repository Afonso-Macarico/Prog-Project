#include <iostream>
#include <sstream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    // Helper: parse a "points" attribute string into a vector of Points
    // Format: "x1,y1 x2,y2 x3,y3 ..."
    static vector<Point> parse_points(const string &pts_str)
    {
        vector<Point> pts;
        istringstream ss(pts_str);
        string token;
        while (ss >> token)
        {
            // token is "x,y"
            size_t comma = token.find(',');
            int x = stoi(token.substr(0, comma));
            int y = stoi(token.substr(comma + 1));
            pts.push_back({x, y});
        }
        return pts;
    }

    static void parse_transform(XMLElement *elem,Point &origin,string &type,vector<int> &args) {
        const char *str = elem->Attribute("transform-origin");
        if (str) {
            istringstream ss(str);
            ss>>origin.x>>origin.y;
            const char *tra = elem->Attribute("transform");
            if (!tra) return;
            string ts(tra);
            size_t paren_open  = ts.find('(');
            size_t paren_close = ts.find(')');
            type = ts.substr(0, paren_open);
            string inner = ts.substr(paren_open + 1, paren_close - paren_open - 1);
            istringstream is(inner);
            int v;
            while (is >> v) args.push_back(v);
        }
    }

    static Point apply_transform(const Point &p,const Point &origin,const string &type,const vector<int> &args) {
        if (type == "translate")
            return p.translate({args[0], args[1]});
        if (type == "rotate")
            return p.rotate(origin, args[0]);
        if (type == "scale")
            return p.scale(origin, args[0]);
        return p;
    }

    static vector<Point> apply_transform_vec(const vector<Point> &pts,const Point &origin,const string &type,const vector<int> &args) {
        vector<Point> result;
        for (const Point &p : pts)
            result.push_back(apply_transform(p, origin, type, args));
        return result;
    }

    // Helper: parse one SVG child element and return a new SVGElement* (or nullptr if unknown)
    static SVGElement *parse_element(XMLElement *child)
    {
        Point origin;
        string transform_type;
        vector<int> transform_args;
        parse_transform(child, origin, transform_type, transform_args);
        string name = child->Name();

        if (name == "g")
        {
            vector<SVGElement *> children;
            for (XMLElement *c = child->FirstChildElement();
                 c != nullptr;
                 c = c->NextSiblingElement())
            {
                SVGElement *e = parse_element(c);
                if (e != nullptr)
                    children.push_back(e);
            }
            return new Group(children);
        }
        else if (name == "ellipse")
        {
            Color fill = parse_color(child->Attribute("fill"));
            Point center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
            Point radius = {child->IntAttribute("rx"), child->IntAttribute("ry")};
            center=apply_transform(center,origin,transform_type,transform_args);
            if (transform_type == "scale") radius = radius.scale({0, 0}, transform_args[0]);
            return new Ellipse(fill, center, radius);
        }
        else if (name == "circle")
        {
            Color fill = parse_color(child->Attribute("fill"));
            Point center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
            int r = child->IntAttribute("r");
            center = apply_transform(center, origin, transform_type, transform_args);
            if (transform_type == "scale") r*= transform_args[0];
            return new Circle(fill, center, r);
        }
        else if (name == "polygon")
        {
            Color fill = parse_color(child->Attribute("fill"));
            vector<Point> pts = parse_points(child->Attribute("points"));
            pts = apply_transform_vec(pts, origin, transform_type, transform_args);
            return new Polygon(fill, pts);
        }
        else if (name == "rect")
        {
            Color fill = parse_color(child->Attribute("fill"));
            Point corner = {child->IntAttribute("x"), child->IntAttribute("y")};
            int w = child->IntAttribute("width");
            int h = child->IntAttribute("height");
            if (!transform_type.empty())
            {
                vector<Point> pts = {
                    corner,
                    {corner.x + w,corner.y},
                    {corner.x + w,corner.y + h},
                    {corner.x,corner.y + h}
                };
                pts = apply_transform_vec(pts, origin, transform_type, transform_args);
                return new Polygon(fill, pts);
            }
            return new Rect(fill, w, h, corner);
        }
        else if (name == "polyline")
        {
            Color stroke = parse_color(child->Attribute("stroke"));
            vector<Point> pts = parse_points(child->Attribute("points"));
            pts = apply_transform_vec(pts, origin, transform_type, transform_args);
            return new Polyline(stroke, pts);
        }
        else if (name == "line")
        {
            Color stroke = parse_color(child->Attribute("stroke"));
            Point start = {child->IntAttribute("x1"), child->IntAttribute("y1")};
            Point end   = {child->IntAttribute("x2"), child->IntAttribute("y2")};
            start = apply_transform(start, origin, transform_type, transform_args);
            end   = apply_transform(end,   origin, transform_type, transform_args);
            return new Line(stroke, start, end);
        }

        return nullptr; // unknown element
    }

    void readSVG(const string &svg_file, Point &dimensions, vector<SVGElement *> &svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");

        for (XMLElement *child = xml_elem->FirstChildElement();
             child != nullptr;
             child = child->NextSiblingElement())
        {
            SVGElement *elem = parse_element(child);
            if (elem != nullptr)
            {
                svg_elements.push_back(elem);
            }
        }
    }
}
