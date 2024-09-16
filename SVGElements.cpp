#include "SVGElements.hpp"
#include <sstream>
#include <iostream>

namespace svg
{
    Point parse_tstring(std::istringstream& transform_string) {
        int t_x = 0,t_y = 0,temp;
        bool first_check = true;
        while (!transform_string.eof()) {
            if (transform_string >> temp)
                {
                    first_check ? t_x = temp : t_y = temp;
                    first_check = false;
                }
                else{
                    transform_string.clear();
                    transform_string.ignore();
                } 
        }
        return {t_x, t_y};
    }

    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}
    void SVGElement::transform(const char* transform, const char* origin){}

    void SVGElement::set_point(Point& p,Point NewPoint){
        p.x = NewPoint.x; p.y = NewPoint.y;
    }

    // Implementation of the member functions of the Ellipse object.
    Ellipse::Ellipse(const Point &center,
                     const Point &radius,
                     const Color &fill
                    )
        : fill(fill), center(center), radius(radius)
    {
    }

    //Acessors.
    Point Ellipse::get_center() const{return center;}
    Point Ellipse::get_radius() const{return radius;}
    Color Ellipse::get_fill()const{return fill;}

    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    SVGElement* Ellipse::clone() const {
        return new Ellipse(*this);
    }

    void Ellipse::transform(const char* transform, const char* origin){
        if (transform != nullptr)
        {
            // Ignoring the '(' and ')'.
            std::string t_str = transform;
            std::string t_type = t_str.substr(0,t_str.find("("));
            std::string t_args = t_str.substr(t_str.find("(")+1,t_str.find(")"));
            // Variable setup.
            std::istringstream iss(t_args);
            Point t_point;
            // Translate operation.
            if (t_type == "translate")
            {
                t_point = parse_tstring(iss);
                set_point(center,center.translate(t_point));               
            }
            // Rotate operation.
            if (t_type == "rotate")
            {
                if (origin != nullptr)
                {
                    std::istringstream origin_iss(origin);
                    t_point = parse_tstring(origin_iss);   
                }
                int angle = stoi(t_args);
                set_point(center,center.rotate(t_point,angle));
            }
            // Scale operation.
            if (t_type == "scale")
            {
                if (origin != nullptr)
                {
                    std::istringstream origin_iss(origin);
                    t_point = parse_tstring(origin_iss);
                }
                int scalar = stoi(t_args);
                set_point(center,center.scale(t_point,scalar));
                radius.x *= scalar; radius.y*=scalar;
            }
        }
    }

    // Implementation of the member functions of the circle object.
    Circle::Circle(const Point &center,const int& radius ,const Color& fill)
                : Ellipse(center,{radius,radius},fill){}
    
    void Circle::draw(PNGImage &img){
        img.draw_ellipse(get_center(), get_radius(), get_fill());
    }

    
    // Implementation of the member functions of the Polygon object.
    Polygon::Polygon(const std::vector<Point>& points, const Color &fill) : points(points), fill(fill) {}

    // Acessors
    std::vector<Point> Polygon::get_points() const{ return points;}
    Color Polygon::get_fill() const{ return fill;}

    void Polygon::draw(PNGImage &img) const {
        img.draw_polygon(points, fill);
    }

    SVGElement* Polygon::clone() const {
        return new Polygon(*this);
    }

    void Polygon::transform(const char* transform, const char* origin){
        if (transform != nullptr)
        {
            // Ignoring the '(' and ')'
            std::string t_str = transform;
            std::string t_type = t_str.substr(0,t_str.find("("));
            std::string t_args = t_str.substr(t_str.find("(")+1,t_str.find(")"));
            // Variable setup
            std::istringstream iss(t_args);
            Point t_point = {0,0};
            // Translate operation
            if (t_type == "translate")
            {
                t_point = parse_tstring(iss);
                for (Point& p : points)
                {
                    set_point(p,p.translate({t_point}));
                }
            }
            // Rotate operation
            if (t_type == "rotate")
            {
                if (origin != nullptr)
                {
                    std::istringstream origin_iss(origin);
                    t_point = parse_tstring(origin_iss);  
                }
                int angle = stoi(t_args);
                for (Point& p : points)
                {
                    set_point(p,p.rotate(t_point,angle));
                }
            }
            // Scale operation
            if (t_type == "scale")
            {
                if (origin != nullptr)
                {
                    std::istringstream origin_iss(origin);
                    t_point = parse_tstring(origin_iss);
                }
                int scalar = stoi(t_args);
                for (Point& p : points)
                {
                    set_point(p,p.scale(t_point,scalar));
                }
            }
        }
    }

    // Implementation of the member functions of the Rectangle object
    Rectangle::Rectangle(const Point &topLeft, int width, int height, const Color &fill)
    : Polygon({topLeft,{topLeft.x + width -1, topLeft.y},{topLeft.x + width -1, topLeft.y + height -1},{topLeft.x, topLeft.y + height -1}},fill) {}

    // Acessors
    int Rectangle::get_width() const{ return width;}
    int Rectangle::get_height() const{ return height;}

    void Rectangle::draw(PNGImage &img) const
    {
        img.draw_polygon(get_points(),get_fill());
    }

    //implementation of the member functions of the Polyline object.
    Polyline::Polyline(const std::vector<Point>& points, const Color &c) : points(points), stroke(c) {}
    void Polyline::draw(PNGImage &img) const 
    {
        for (size_t i = 0; i < points.size() - 1; i++) 
        {
            img.draw_line(points[i], points[i + 1], stroke);
        }
    }

    std::vector<Point>Polyline::get_points() const { return points; }
    Color Polyline::get_color() const { return stroke; }

    void Polyline::transform(const char* transform, const char* origin){
        if (transform != nullptr)
        {   
            // Ignoring the '(' and ')'.
            std::string t_str = transform;
            std::string t_type = t_str.substr(0,t_str.find("("));
            std::string t_args = t_str.substr(t_str.find("(")+1,t_str.find(")"));

            // Variable setup.
            std::istringstream iss(t_args);
            Point t_point = {0,0};
            // Translate operation.
            if (t_type == "translate")
            {
                t_point = parse_tstring(iss);
                for (Point& p : points)
                {
                    set_point(p,p.translate(t_point));
                }
            }

            // Rotate operation.
            if (t_type == "rotate")
            {
                if (origin != nullptr)
                    {
                        std::istringstream origin_iss(origin);
                        t_point = parse_tstring(origin_iss); 
                    }
                int angle;
                iss >> angle;
                for (Point& p : points)
                {
                    set_point(p,p.rotate(t_point,angle));
                }
            
            // Scale operation.
            }
            if (t_type == "scale")
            {
                if (origin != nullptr)
                {
                    std::istringstream origin_iss(origin);
                    t_point = parse_tstring(origin_iss); 
                }
                int scalar = stoi(t_args);
                for (Point& p : points)
                {
                    set_point(p,p.scale(t_point,scalar));
                }
            }
        }
    }

    SVGElement* Polyline::clone() const {
        return new Polyline(*this);        // (*this) refers to the current object.
    }

    //implementation of the member functions of the Line object.
    Line::Line(const std::vector<Point>& points, const Color &c) : Polyline(points, c) {}
    void Line::draw(PNGImage &img) const {
        img.draw_line(get_points()[0], get_points()[1], get_color());
    }

    
    //implementation of the member functions for a group of objects.
    Group::Group(const std::vector<SVGElement*>& elements) {
        for (SVGElement* element : elements)
        {
            group_elements.push_back(element);
        }   
    }

    Group::~Group(){
        for (SVGElement* element : group_elements)
        {
            delete element;
        }
        
    }

    void Group::draw(PNGImage &img) const{
        for ( SVGElement* element : group_elements)
        {
            element->draw(img);
        }
    }

    void Group::transform(const char* transform, const char* origin){
        for ( SVGElement* element : group_elements)
        {
            element->transform(transform,origin);
        }
    }
    
    SVGElement* Group::clone() const {
        std::vector<SVGElement*> clone_elements;
        for (const SVGElement* element : group_elements) {
            clone_elements.push_back(element->clone());
        }
        return new Group(clone_elements);
    }
    
}