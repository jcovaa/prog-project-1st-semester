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
        virtual void transform(const char* transform, const char* origin);

        void set_point(Point& p,Point NewPoint);
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

    // ELLIPSE SHAPE
    class Ellipse : public SVGElement
    {
    public:
        //! @brief constructor for Ellipse object.
        //! @param fill Filling Color.
        //! @param center Center point of the ellipse.
        //! @param radius Radius in x and y axis.  
        Ellipse(const Point &center, const Point &radius, const Color &fill);

        //! Acessor for ellipse's center Point.
        //! @return Center point.
        Point get_center() const;
        //! Acessor for ellipse's radius on xy axis.
        //! @return radius point per axis (x,y).
        Point get_radius() const;
        //! Acessor for ellipse's fill color.
        //! @return Fill color.
        Color get_fill() const;
        //! @brief Transform SVGElement. If transform = nullpointer -> Leads to no changes made. If origin = nullpointer -> origin = (0,0).
        //! @param transform pointer from the transform attribute.
        //! @param origin pointer from the transform-origin attribute.
        void transform(const char* transform, const char* origin) override;
        //! Creates a clone of an element
        //! @return a dynamically allocated SVGElement
        SVGElement* clone() const override;
        //! @brief Draw the ellipse.
        //! @param img Output PNGImage.
        void draw(PNGImage &img) const override;

    private:
        // Fill color.
        Color fill;
        // Center Point
        Point center;
        // Point with the value of the radius in the x and y axis.
        Point radius;
    };

    //CIRCLE SHAPE.
    class Circle : public Ellipse
    {
    public:
        //! @brief constructor for Circle object.
        //! @param fill Filling Color.
        //! @param center Center point of the circle.
        //! @param radius Radius (Same in x and y axis). 
        Circle(const Point &center,const int& radius ,const Color& fill);
        //! @brief Draw the circle.
        //! @param img Output PNGImage.
        void draw(PNGImage &img);
    };

    

    //POLYGON SHAPE.
    class Polygon : public SVGElement {
    public:
        //! Constructor that takes a vector of points.
        //! @param points Vector of points defining the polygon.
        Polygon(const std::vector<Point>& points, const Color &fill);

        //! Get all the points of the polygon.
        //! @return Vector of points.
        std::vector<Point> get_points() const;

        //! Get the color of the polygon.
        //! @return Fill Color.
        Color get_fill() const;

        //! @brief Transform SVGElement. If transform = nullpointer -> Leads to no changes made. If origin = nullpointer -> origin = (0,0).
        //! @param transform pointer from the transform attribute.
        //! @param origin pointer from the transform-origin attribute.
        void transform(const char* transform, const char* origin) override;
        //! Creates a clone of an element
        //! @return a dynamically allocated SVGElement
        SVGElement* clone() const override;

        //! Draw the polygon.
        //! @param img Output PNGImage.
        void draw(PNGImage &img) const override;

    private:
        // Vector with Polygon's edges.
        std::vector<Point> points;
        // Fill color.
        Color fill;
    };

    //RECTANGLE SHAPE.
    class Rectangle : public Polygon
    {
    public:
        //! Constructor for the rectangle object.
        //! @param topLeft point of the upper left corner
        //! @param width width of the rectangle.
        //! @param height height of the rectangle.
        //! @param fill rectangle color
        Rectangle(const Point &topLeft, int width, int height, const Color &fill);

        //! Acessor for rectangle's width.
        //! @return the width.
        int get_width() const;
        //! Acessor for rectangle's height.
        //! @return The height.
        int get_height() const;

        //! Draw the rectangle.
        //! @param img Output PNGImage.
        void draw(PNGImage &img) const override;

    private:
        // Width.
        int width;
        // Height.
        int height;
    };

    class Polyline : public SVGElement {
        public:
            //! Constructor for the polyline object.
            //! @param points points that define the lines.
            //! @param c line color
            Polyline(const std::vector<Point>& points, const Color &stroke);
            
            //! Acessor for the polyline's points.
            //! @return the points
            std::vector<Point> get_points() const;
            //! Acessor for the Polyline color.
            //! @return the color.
            Color get_color() const;

            //! @brief Transform SVGElement. If transform = nullpointer -> Leads to no changes made. If origin = nullpointer -> origin = (0,0).
            //! @param transform pointer from the transform attribute.
            //! @param origin pointer from the transform-origin attribute.
            void transform(const char* transform, const char* origin) override;

            //! Creates a clone of an element.
            //! @return a dynamically allocated SVGElement.
            SVGElement* clone() const override;

            //! Draw the polyline.
            //! @param img Output PNGImage.
            void draw(PNGImage &img) const override;

        private:
            // Vector with Polyline's defining points.
            std::vector<Point> points;
            // Color.
            Color stroke;
    };

    class Line : public Polyline {
        public:
            //! Constructor for the Line object.
            //! @param points vector with the two points.
            //! @param c color of the line.
            Line(const std::vector<Point>& points, const Color &c);

            //! Draw the line.
            //! @param img Output PNGImage.
            void draw(PNGImage &img) const override;
    };

    
    class Group : public SVGElement {

    public:
        //! Constructor for the group object.
        //! @param elements elements for the group.
        Group(const std::vector<SVGElement*>& elements);
        //! Destructor for the group object (Prevents memory leaks).
        ~Group();

        //! @brief Transform SVGElement. If transform = nullpointer -> Leads to no changes made. If origin = nullpointer -> origin = (0,0).
        //! @param transform pointer from the transform attribute.
        //! @param origin pointer from the transform-origin attribute.
        void transform(const char* transform, const char* origin) override;

        //! Creates a clone of an element.
        //! @return a dynamically allocated SVGElement.
        SVGElement* clone() const override;

        //! Draw the group elements.
        //! @param img Output PNGImage.
        void draw(PNGImage &img) const override;

    private:
        //! Vector with all the pointers to the elements inside the group.
        std::vector<SVGElement*> group_elements;
    };
    
    
}
#endif