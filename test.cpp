#include <iostream>
using namespace std;

class Figure
{
public:
	virtual void area() = 0;
};

class Tri : public Figure
{
public:
    void area() {
    	cout << "tri" << endl ;
    }
	
};

class Squ : public Figure
{
public:
	void area() {
		cout << "squ" << endl;
	}
};

int main(int argc, char const *argv[])
{
	Tri t;
	t.area();

	Squ s;
	s.area();

	Figure *f;
	f = &t;
	f->area();

	f=&s;
	f->area();

	return 0;
}