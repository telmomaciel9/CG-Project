#include "headers/Bezier.h"
#include "../common/headers/MatrixOpp.h"

using namespace std;

int numPatches, numControlPoints;
vector<vector<int>> indices;
vector<Point> controlPoints;

int divs = 16;

void readPatchFile(char *filePath)
{
	string path = "../files/";
	path.append(filePath);
	ifstream patchFile(path);

	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
	{
		cerr << "Error opening file \"" << filePath << "\".\nMake sure the file exists...";
		exit(1);
	}

	string line;

	// read number of patches
	getline(patchFile, line);
	if (sscanf(line.c_str(), "%d", &numPatches) != 1)
	{
		cerr << "ERROR: Couldn't read number of patches!! Parsing Stopped" << endl;
		exit(2);
	}

	// read indices
	indices.reserve(numPatches);

	for (int i=0 ; i<numPatches ; i++)
	{
		indices.push_back(vector<int>());
		indices.at(i).reserve(16);
		getline(patchFile, line);
		char* str = strdup(line.c_str());
		char *temp;
		while ((temp = strsep(&str, ", ")) != NULL)
		{
			if (strlen(temp) == 0)
				continue;
			int val;
			if (sscanf(temp, "%d", &val) != 1)
			{
				cerr << "ERROR: Couldn't read index!!\nParsing Stopped" << endl;
				exit(2);
			}
			indices.at(i).push_back(val);
		}
		// if (indices.at(i).size() != 16)
		// {
			// cerr << "ERROR: Indices for path number " << i+1 << " should have 16 values!!\nParsing Stopped" << endl;
			// exit(2);
		// }
		free(str);
	}

	// read number of control points
	getline(patchFile, line);
	if (sscanf(line.c_str(), "%d", &numControlPoints) != 1)
	{
		cerr << "ERROR: Couldn't read number of control points!!\nParsing Stopped" << endl;
		exit(2);
	}
	
	// read control points
	controlPoints.reserve(numControlPoints);
	for (int i=0 ; i<numControlPoints ; i++)
	{
		getline(patchFile, line);
		float x,y,z;
		if (sscanf(line.c_str(), "%f, %f, %f", &x, &y, &z) != 3)
		{
			cerr << "ERROR: Error reading Control Point number " << i << "!!\nParsing Stopped" << endl;
			exit(2);
		}
		controlPoints.emplace_back(Point(x,y,z));
	}
}


Point MxPxMT[16];

void calcMxPxMT (Point P[16], bool debug)
{
	float M[16] ={ -1,  3, -3, 1, 
								 3, -6,  3, 0,
								-3,  3,  0, 0,
								 1,  0,  0, 0 };
	Point MxP[16];
	FloatMatrix_x_PointMatrix(M, P, MxP);
	PointMatrix_x_FloatMatrix(MxP, M, MxPxMT);;
	if (debug)
	{
		for(int k=0 ; k<16 ; k++)
			printf("%f%c", M[k], ((k+1)%4 == 0) ?'\n' :' ');
		puts("");
		for(int k=0 ; k<16 ; k++)
			printf("%f%c", P[k].getX(), ((k+1)%4 == 0) ?'\n' :' ');
		puts("");
		for(int k=0 ; k<16 ; k++)
			printf("%f%c", MxP[k].getX(), ((k+1)%4 == 0) ?'\n' :' ');
		puts("");
		for(int k=0 ; k<16 ; k++)
			printf("%f%c", M[k], ((k+1)%4 == 0) ?'\n' :' ');
		puts("");
		for(int k=0 ; k<16 ; k++)
			printf("%f%c", MxPxMT[k].getX(), ((k+1)%4 == 0) ?'\n' :' ');
	}
}

Point getBezierPatchPoint (float u, float v)
{
	float u2 = u*u, u3 = u2*u, v2 = v*v, v3 = v2*v;
	float U[4] = {u3, u2, u, 1},
				V[4] = {v3, v2, v, 1};
	Point UxMxPxMT[4];
	multVectorPointMatrix(MxPxMT, U, UxMxPxMT);

	return UxMxPxMT[0]*V[0] + UxMxPxMT[1]*V[1] + UxMxPxMT[2]*V[2] + UxMxPxMT[3]*V[3];
}



void createModel(int tesselation, char *dot3DFile)
{
	Shape s;
	float inc = 1.0f / tesselation;

	for (int i=0 ; i<numPatches ; i++)
	{
		vector<int> patchIndices = indices.at(i);
		Point P[16];
		int index;
		for (int j=0 ; j<patchIndices.size() ; index = patchIndices.at(j), P[j++] = controlPoints.at(index));
		calcMxPxMT(P, i==0);

		vector<Point> patch;
		float u=0.0f, v;
		for (int ui=0 ; ui<tesselation ; ui++, u+=inc)
		{
			v = 0.0f;
			for (int vi=0 ; vi<tesselation ; vi++, v+=inc)
				patch.push_back(getBezierPatchPoint(u, v));
		}

		// points per line
		int ppl = tesselation + 1;
		for (int ui=0 ; ui<tesselation ; ui++, u+=inc)
		{
			for (int vi=0 ; vi<tesselation ; vi++, v+=inc)
			{
				Point p0 = patch[ui     + vi       * ppl],
							p1 = patch[ui + 1 + vi       * ppl],
							p2 = patch[ui     + (vi + 1) * ppl],
							p3 = patch[ui + 1 + (vi + 1) * ppl];
		
				s.addPoint(p0); s.addPoint(p2); s.addPoint(p1);
				s.addPoint(p1); s.addPoint(p2); s.addPoint(p3);
			}
		}
	}

	s.writeToFile(dot3DFile);
}

void geraBezier(char *filePath, int tesselation, char *dot3DFile)
{
	readPatchFile(filePath);
	createModel(tesselation, dot3DFile);
}

