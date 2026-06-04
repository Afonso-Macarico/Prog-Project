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

    // Helper: parse one SVG child element and return a new SVGElement* (or nullptr if unknown)
    static SVGElement *parse_element(XMLElement *child)
    {
        string name = child->Name();
        SVGElement* elem = nullptr;

        if (name == "ellipse")
        {
            Color fill = parse_color(child->Attribute("fill"));
            Point center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
            Point radius = {child->IntAttribute("rx"), child->IntAttribute("ry")};
            elem = new Ellipse(fill, center, radius);
        }
        else if (name == "circle")
        {
            Color fill = parse_color(child->Attribute("fill"));
            Point center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
            int r = child->IntAttribute("r");
            elem= new Circle(fill, center, r);
        }
        else if (name == "polygon")
        {
            Color fill = parse_color(child->Attribute("fill"));
            vector<Point> pts = parse_points(child->Attribute("points"));
            elem = new Polygon(fill, pts);
        }
        else if (name == "rect")
        {
            Color fill = parse_color(child->Attribute("fill"));
            Point corner = {child->IntAttribute("x"), child->IntAttribute("y")};
            int w = child->IntAttribute("width");
            int h = child->IntAttribute("height");
            elem = new Rect(fill, w, h, corner);
        }
        else if (name == "polyline")
        {
            Color stroke = parse_color(child->Attribute("stroke"));
            vector<Point> pts = parse_points(child->Attribute("points"));
            elem = new Polyline(stroke, pts);
        }
        else if (name == "line")
        {
            Color stroke = parse_color(child->Attribute("stroke"));
            Point start = {child->IntAttribute("x1"), child->IntAttribute("y1")};
            Point end   = {child->IntAttribute("x2"), child->IntAttribute("y2")};
            elem = new Line(stroke, start, end);
        }

        if (elem != nullptr){

            Point origin = {0, 0};
            const char* origin_at = child->Attribute("transform-origin");
            if (origin_at) {
                sscanf(origin_at, "%d %d", &origin.x, &origin.y);
            }

            const char* transform_at = child->Attribute("transform");
            if (transform_at) {
                if (strncmp(transform_at,"translate",9)==0){
                    Point t;
                    sscanf(transform_at, "translate(%d %d)", &t.x, &t.y);
                    elem ->translate(t);
                }

                if (strncmp(transform_at,"rotate",6)==0){
                    int deg;
                    sscanf(transform_at, "rotate(%d)", &deg);
                    elem ->rotate(origin, deg);
                }
            
                if (strncmp(transform_at,"scale",5)==0){
                    int scale;
                    sscanf(transform_at, "scale(%d)", &scale);
                    elem ->scale(origin, scale);
                }
            }
        }

        return elem;
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