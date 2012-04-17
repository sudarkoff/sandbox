#include <iostream>
using namespace std;

class matrix
{
private:
	double *MATRIX;
	int width, height;
	
public:
	matrix(): width(0), height(0) {}
	matrix(int _w, int _h): width(_w), height(_h) { MATRIX = new double[width*height]; }
	matrix(matrix& _m) { memcpy(MATRIX, _m.MATRIX, sizeof(double)*width*height); }
	~matrix() { delete MATRIX; }
	
	double& operator[] (int _i) { return *(MATRIX + _i); }
	void print() { for (int i=0; i< width*height; i++) cout << i << ": " << MATRIX[i] << endl; }
};

int main(void)
{
	matrix M(4, 4);
	
	//  M[1][3] = 2.;
	
	M.print();
	
	int i;
	cin >> i;
	return (0);
}
