#include <iostream>
#include <vector>
#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;

using namespace std;

// ´´½¨H Matrix
MatrixXd CreateHMat(vector<vector<double>> &vec)
{
	int len = vec.size();
	MatrixXd mat(len, 6);
	for (int i = 0; i < vec.size(); i++)
	{
		mat(i, 0) = vec[i][0] * vec[i][0];
		mat(i, 1) = vec[i][1] * vec[i][1];
		mat(i, 2) = vec[i][2] * vec[i][2];
		mat(i, 3) = vec[i][0] * vec[i][1] * 2;
		mat(i, 4) = vec[i][1] * vec[i][2] * 2;
		mat(i, 5) = vec[i][2] * vec[i][0] * 2;
	}
	return mat;
}

// HS = (H^T*H)^-1*(H^T)
MatrixXd ComputeHS(MatrixXd& H)
{
	auto HT = H.transpose();
	return (HT*H).inverse()*HT;
}

// B vector
VectorXd CreateBVec(vector<double> &vec)
{
	int len = vec.size();
	VectorXd vex(len, 1);
	for (int i = 0; i < vec.size(); i++)
	{
		vex(i) = vec[i];
	}
	return vex;
}

MatrixXd ComputeDMat(MatrixXd& hs, VectorXd& vb)
{
	auto vd = hs * vb;
	cout << vd << endl;


	MatrixXd mat(3, 3);
	mat(0, 0) = vd(0);
	mat(0, 1) = vd(3);
	mat(0, 2) = vd(5);

	mat(1, 0) = vd(3);
	mat(1, 1) = vd(1);
	mat(1, 2) = vd(4);

	mat(2, 0) = vd(5);
	mat(2, 1) = vd(4);
	mat(2, 2) = vd(2);
	return mat;
}

//ComputeDomVec()


void main()
{
	// ÊäÈë10*3
	vector<vector<double>> vec{
		vector<double>{1,2,3},
		vector<double>{4,5,6} ,
		vector<double>{7,8,9},
		vector<double>{10,8,9},
		vector<double>{7,2,9},
		vector<double>{2,1,4},
		vector<double>{12,1,4},
		vector<double>{2,12,4},
		vector<double>{2,1,24},
		vector<double>{22,11,2},
	};

	// H => (10,6)
	auto H = CreateHMat(vec);
	cout << "H  =" << endl << H << endl;


	// HS => (6,10)
	auto HS = ComputeHS(H);
	cout << "HS =" << endl << HS << endl;

	// B => (10,)
	auto B = CreateBVec(vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

	// D = HS * B
	auto D = ComputeDMat(HS, B);
	cout << "D  =" << endl << D << endl;

	system("pause");
}

