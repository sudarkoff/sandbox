#include <iostream>

class Shape {
public:
	Shape();
    virtual void Draw() = 0;
};

class Circle : public Shape {
public:
    virtual void Draw() {
    	// This function never called
    	std::cout << "Circle::Draw()\n";
    }
};

class DrawOnCreationHelper {
public:
    DrawOnCreationHelper(Shape* shape) {
    	shape->Draw();
    }
};

Shape::Shape()
{
//	DrawOnCreationHelper h(this);
	Draw();
}

int main()
{
    Circle c;

    return 1;
}
