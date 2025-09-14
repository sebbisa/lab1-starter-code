#include <iostream>
#include <cstring>

struct Point {
    int x, y;

    Point () : x(), y() {}
    Point (int _x, int _y) : x(_x), y(_y) {}
};

class Shape {
    private: // ERROR:  a class must explicitly declare its member functions as public
             //         if they are to be called outside the class scope
        int vertices;
        Point** points;

    public:
        Shape (int _vertices) {
            vertices = _vertices;
            points = new Point*[vertices+1];
        }

        ~Shape () {
            // ERROR:   this destructor should deallocate memory of the points variable
            //          which is dynamically allocated in the Shape constructor
            for(int i = 0; i <= vertices; i++) delete points[i];
            delete[] points;
        }

        void addPoints (Point pts[] /* formal parameter for unsized array called pts */) {
            for (int i = 0; i <= vertices; i++) {
                points[i] = new Point; // ERROR: Point memory not allocated which is necessary for memcpy to work
                memcpy(points[i], &(pts[i%vertices]), sizeof(Point*)); // ERROR: cstring header file not included
            }
        }

        double area () {
            // ERROR: reference to local variable returned which will deallocate
            // after the function returns. Fixed by turning it into a normal double
            // instead of a pointer to a double
            int temp = 0;
            for (int i = 0; i < vertices; i++) { // ERROR: index goes out of bounds at i == vertices
                // FIXME: there are two methods to access members of pointers
                //        use one to fix lhs and the other to fix rhs
                int lhs = (*points[i]).x * (*points[i+1]).y;
                int rhs = points[i+1]->x * points[i]->y;
                temp += (lhs - rhs);
            }
            double area = abs(temp)/2.0;
            return area;
        }
};

int main () {
    // FIXME: create the following points using the three different methods
    //        of defining structs:
    //          tri1 = (0, 0)
    //          tri2 = (1, 2)
    //          tri3 = (2, 0)

    // adding points to tri

    struct Point tri1 = {0, 0}; // method 1

    Point tri2(1, 2); // method 2

    Point tri3; // method 3
    tri3.x = 2; tri3.y = 0;

    Point triPts[3] = {tri1, tri2, tri3};
    Shape* tri = new Shape(3);
    tri->addPoints(triPts); // ERROR: wrong way to get member of class reference instance

    // FIXME: create the following points using your preferred struct
    //        definition:
    //          quad1 = (0, 0)
    //          quad2 = (0, 2)
    //          quad3 = (2, 2)
    //          quad4 = (2, 0)

    Point quad1(0, 0);
    Point quad2(0, 2);
    Point quad3(2, 2);
    Point quad4(2, 0);

    // adding points to quad
    Point quadPts[4] = {quad1, quad2, quad3, quad4};
    Shape* quad = new Shape(4);
    quad->addPoints(quadPts); // ERROR: wrong way to get member of class reference instance

    // FIXME: print out area of tri and area of quad
    std::cout << "Tri area: " << tri->area() << std::endl;
    std::cout << "Quad area: " << quad->area() << std::endl;

    // ERROR: dynamically allocated shapes need to be freed from memory
    delete tri;
    delete quad;
}
