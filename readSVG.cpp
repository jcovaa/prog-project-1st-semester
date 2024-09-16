#include <iostream>
#include <sstream>
#include <unordered_map>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void readXMLElement(XMLElement* xml_elem,vector<SVGElement *>& svg_elements,std::unordered_map<std::string, SVGElement*> id_map);

    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
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
        
        /*Per each child node, an object should be dynamically allocated 
        using new for the corresponding type of SVGElement, and be stored 
        in the elements vector
        To read colors use Color parse(std::string) function (see Color.hpp
        /Color.cpp)
        */

       //unordered map container used to correlate objects with their specific ids. 
       std::unordered_map<std::string, SVGElement*> id_map;
       readXMLElement(xml_elem,svg_elements,id_map);
    }
    
    void readXMLElement(XMLElement* xml_elem,vector<SVGElement *>& svg_elements,std::unordered_map<std::string, SVGElement*> id_map){

        // Iterates over the Child nodes.
        for (XMLElement* element = xml_elem->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
            // Name of the svg element.
            std::string elementType = element->Name();
            // If there is none, "transform" will be nullpointer.
            const char* transform = element->Attribute("transform");
            // If there is none, "origin" will be nullpointer.
            const char* origin = element->Attribute("transform-origin"); 
            // If there is none, "id" will be nullpointer. 
            const char* id = element->Attribute("id");

            //strcmp compares the strings and returns a int type ( 0 if equal, -1 if 1st < 2nd and 1 if 1st > 2nd ).

            if (elementType == "line")   // If element type is line.
            { 
                // Attributes needed for the line constructor.
                Point start,end;
                Color stroke = parse_color(element->Attribute("stroke"));
                start = {element->IntAttribute("x1"),element->IntAttribute("y1")};
                end = {element->IntAttribute("x2"),element->IntAttribute("y2")};

                // Create dynamically allocated line object.
                Line* line_elem = new Line({start, end}, stroke);

                line_elem->transform(transform,origin);
                svg_elements.push_back(line_elem);

                if (id) id_map[id] = line_elem;
            }

            else if (elementType == "polyline")  // If element is of type polyline.
            {    
                // Attribute needed for the polyline constructor
                std::vector<Point> points;                
                Color stroke = parse_color(element->Attribute("stroke"));            
                const char* points_cstr = element->Attribute("points");

                // Processing the points string and parsing to a points vector.
                std::istringstream iss(points_cstr);
                int x, y;
                char comma;
                // Assuming points can be separated by any simbol and x and y values inside points are always separated with a comma.
                while (!iss.eof()) {
                    if (iss >> x)
                    {
                        iss >> comma >> y;
                        points.push_back({x, y});
                    }
                    else{
                        iss.clear();
                        iss.ignore();
                    }
                }

                // Dynamcally allocated polyline object
                Polyline* polyline_elem = new Polyline(points, stroke);

                polyline_elem->transform(transform,origin);
                svg_elements.push_back(polyline_elem);

                if (id) id_map[id] = polyline_elem;
            }

            
            else if (elementType == "ellipse")    // If element is of type ellipse.
            {
                // Attributes needed for the polyline constructor.
                Point center,radius;
                Color fill = parse_color(element->Attribute("fill"));
                center = {element->IntAttribute("cx"),element->IntAttribute("cy")};
                radius = {element->IntAttribute("rx"),element->IntAttribute("ry")};

                // Dynamcally allocated ellipse object.
                Ellipse* ellipse_elem = new Ellipse(center,radius,fill);

                ellipse_elem->transform(transform,origin);
                svg_elements.push_back(ellipse_elem);
                
                if (id) id_map[id] = ellipse_elem;
            }

            else if (elementType == "circle")     // If element is of type circle.
            {
                // Attributes needed for the circle constructor.
                Point center; int radius;
                Color fill = parse_color(element->Attribute("fill"));
                center = {element->IntAttribute("cx"),element->IntAttribute("cy")};
                radius = element->IntAttribute("r");

                // Dynamcally allocated circle object.
                Circle* circle_elem = new Circle(center,radius,fill);

                circle_elem->transform(transform,origin);
                svg_elements.push_back(circle_elem);

                if (id)id_map[id] = circle_elem;
            }

            
            else if (elementType == "polygon")    // If element is of type polygon.
            {
                std::vector<Point> points;
                Color fill = parse_color(element->Attribute("fill"));
                const char* pointsStr = element->Attribute("points");

                // Processing the points string and parsing to a points vector.
                std::istringstream iss(pointsStr);
                int x, y;
                char comma;
                while (!iss.eof()) {
                    if (iss >> x)
                    {
                        iss >> comma >> y;
                        points.push_back({x, y});
                    }
                    else{
                        iss.clear();
                        iss.ignore();
                    }
                }
                // Dynamcally allocated Polygon object.
                Polygon* polygon_elem = new Polygon(points, fill);

                polygon_elem->transform(transform,origin);
                svg_elements.push_back(polygon_elem);

                if (id) id_map[id] = polygon_elem;
            }

            else if (elementType == "rect")   // If element is of type rectangle.
            {
                // Atributes needed for the rect constructor
                Point top_left; int width, height;
               
                Color fill = parse_color(element->Attribute("fill"));
            
                top_left = {element->IntAttribute("x"),element->IntAttribute("y")};
                width = element->IntAttribute("width");
                height = element->IntAttribute("height");
                
                // Dynamically allocated rectangle object.
                Rectangle* rect_elem = new Rectangle(top_left,width,height,fill);
                
                rect_elem->transform(transform,origin);
                svg_elements.push_back(rect_elem);

                if (id) id_map[id] = rect_elem;
            }
            
            else if (elementType == "g")  // If element type is of type group.
            {
                // Atributes needed for the rect constructor.
                std::vector<SVGElement *> group_elements;
                // Call recursively readXMLElement for elements inside the group.
                readXMLElement(element,group_elements,id_map);

                // Dynamcally allocated Group object.
                Group* group_elem = new Group(group_elements);
                
                group_elem->transform(transform,origin);
                svg_elements.push_back(group_elem);

                if (id) id_map[id] = group_elem;
            }
            
            else if (elementType == "use")   // If element type is of type use.
            {
                // reference to id attribute.
                const char* href = element->Attribute("href");

                // Element id, ignoring the '#'.
                std::string reference = string(href + 1);

                // Getting the element needed for copying from id_map.
                SVGElement* reference_element = id_map[reference];

                SVGElement* clone_element = reference_element->clone();
                clone_element->transform(transform, origin);
                svg_elements.push_back(clone_element);

                if (id) id_map[id] = clone_element;
            }
        }
    }
}