#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;


int* RGB2Luv(int r, int g, int b){

	//cout << "r= " << r<<"\n";
	//from pixel to nonlinear RGB
	double nonlinear_r = ((double)r) / (double)255;
	double nonlinear_g = ((double)g) / (double)255;
	double nonlinear_b = ((double)b) / (double)255;
	
	//cout << "nonlinear_r= " << nonlinear_r<<"\n";
	
	//from nonlinear RGB to linear RGB
	double linear_r;
	double linear_g;
	double linear_b;
	if (nonlinear_r < 0.03928){
		linear_r = nonlinear_r / 12.92;
	}
	else{
		linear_r = pow(((nonlinear_r + 0.055) / 1.055), 2.4);
	}

	if (nonlinear_g < 0.03928){
		linear_g = nonlinear_g / 12.92;
	}
	else{
		linear_g = pow(((nonlinear_g + 0.055) / 1.055), 2.4);
	}

	if (nonlinear_b < 0.03928){
		linear_b = nonlinear_b / 12.92;
	}
	else{
		linear_b = pow(((nonlinear_b + 0.055) / 1.055), 2.4);
	}
	//clipping for r
	if (linear_r > 1){
		linear_r = 1;
	}
	if (linear_r <0){
		linear_r = 0;
	}
	//clipping for g
	if (linear_g > 1){
		linear_g = 1;
	}
	if (linear_g <0){
		linear_g = 0;
	}
	//clipping for b
	if (linear_b > 1){
		linear_b = 1;
	}
	if (linear_b <0){
		linear_b = 0;
	}
	//cout << "linear_r= " << linear_r<<"\n";

	//linear sRGB to XYZ
	double X = 0.412453 * linear_r + 0.35758 * linear_g + 0.180423 * linear_b;
	double Y = 0.212671 * linear_r + 0.71516 * linear_g + 0.072169 * linear_b;
	double Z = 0.019334 * linear_r + 0.119193 * linear_g + 0.950227 * linear_b;
	
	if (X < 0){
		X = 0;
	}
	if (Y < 0){
		Y = 0;
	}
	if (Z < 0){
		Z = 0;
	}
	//cout << "X= " << X << "\n";
	//cout << "Y= " << Y << "\n";
	//cout << "Z= " << Z << "\n";
	//XYZ to Luv
	double Xw = 0.95;
	double Yw = 1;
	double Zw = 1.09;

	double uw = 4 * Xw / (Xw + 15 * Yw + 3 * Zw);
	double vw = 9 * Yw / (Xw + 15 * Yw + 3 * Zw);

	double t = Y / Yw;
	if (t > 1){
		t = 1;
	}
	if (t < 0){
		t = 0;
	}
	double L = 0;
	//cout << "t= " << t << "\n";
	
	if (t > 0.008856){
		//cout << "pow(t, (double)1 / (double)3)= " << pow(t, (double)1 / (double)3) << "\n";
		L = 116 * pow(t, (double)1 / (double)3) - 16;
	}
	else{
		L = 903.3*t;
	}
	//cout << "L= " << L << "\n";
	
	if (L > 100){
		L = 100;
	}
	if (L < 0){
		L = 0;
	}

	double d = X + 15 * Y + 3 * Z;
	double u_p = 4 * X / d;
	double v_p = 9 * Y / d;
	double u = 13 * L*(u_p - uw);
	double v = 13 * L*(v_p - vw);
	int Luv[3];
	Luv[0] = (int)L;
	Luv[1] = (int)u;
	Luv[2] = (int)v;
	
	return  Luv;


	//linear RGB
	/*int gray = (int)(0.3*r + 0.6*g + 0.1*b + 0.5);

	R[i][j] = G[i][j] = B[i][j] = gray;*/


}

int* Luv2RGB(int L, int u, int v){
	double Xw = 0.95;
	double Yw = 1;
	double Zw = 1.09;

	double uw = (double)4 * Xw / (Xw + (double)15 * Yw + (double)3 * Zw);
	double vw = (double)9 * Yw / (Xw + (double)15 * Yw + (double)3 * Zw);

	double u_p = (u + (double)13 * uw * (double)L) / ((double)13 * (double)L);
	double v_p = (v + (double)13 * vw*(double)L) / ((double)13 * (double)L);

	double Y = 0;
	if ((double)L > 7.9996){
		Y = pow((((double)L + (double)16) / (double)116), 3)*Yw;
	}
	else{
		Y = (double)L*Yw / (double)903.3;
	}
	//cout << "Y = " << Y << "\n";
	double X = Y * (double)2.25 * u_p / v_p;
	double Z = Y*((double)3 - 0.75 * u_p - (double)5 * v_p) / v_p;

	if (X < 0){
		X = 0;
	}
	if (Y < 0){
		Y = 0;
	}
	if (Z < 0){
		Z = 0;
	}
	//XYZ to linear sRGB
	double linear_r = 3.240479 * X - 1.53715 * Y - 0.498535 * Z;
	double linear_g = -0.969256 * X + 1.875991 * Y + 0.041556* Z;
	double linear_b = 0.055648 * X - 0.204043 * Y + 1.057311 * Z;
	//cout << "linear_r = " << linear_r << "\n";
	//clipping for r
	if (linear_r > 1){
		linear_r = 1;
	}
	if (linear_r <0){
		linear_r = 0;
	}
	//clipping for g
	if (linear_g > 1){
		linear_g = 1;
	}
	if (linear_g <0){
		linear_g = 0;
	}
	//clipping for b
	if (linear_b > 1){
		linear_b = 1;
	}
	if (linear_b <0){
		linear_b = 0;
	}

	//----------linear sRGB to nonlinear sRGB
	double nonlinear_r;
	double nonlinear_g;
	double nonlinear_b;

	if (linear_r < 0.00304){
		nonlinear_r = 12.92 * linear_r;
	}
	else{
		nonlinear_r = 1.055*(pow(linear_r, ((double)1 / 2.4))) - 0.055;
	}


	if (linear_g < 0.00304){
		nonlinear_g = 12.92 * linear_g;
	}
	else{
		nonlinear_g = 1.055*(pow(linear_g, ((double)1 / 2.4))) - 0.055;
	}
	

	if (linear_b < 0.00304){
		nonlinear_b = 12.92 * linear_b;
	}
	else{
		nonlinear_b = 1.055*(pow(linear_b, ((double)1 / 2.4))) - 0.055;
	}

	//clipping for r
	if (nonlinear_r > 1){
		nonlinear_r = 1;
	}
	if (nonlinear_r <0){
		nonlinear_r = 0;
	}
	//clipping for g
	if (nonlinear_g > 1){
		nonlinear_g = 1;
	}
	if (nonlinear_g <0){
		nonlinear_g = 0;
	}
	//clipping for b
	if (nonlinear_b > 1){
		nonlinear_b = 1;
	}
	if (nonlinear_b <0){
		nonlinear_b = 0;
	}
	//cout << "nonlinear_r = " << nonlinear_r << "\n";
	//----------nonlinear sRGB to pixels
	int r = (int)(nonlinear_r * 255);
	int g = (int)(nonlinear_g * 255);
	int b = (int)(nonlinear_b * 255);
	//cout << "r = " << r << "\n";
	int RGB[3];
	RGB[0] = r;
	RGB[1] = g;
	RGB[2] = b;
	return RGB;
}



void runOnWindow(int W1, int H1, int W2, int H2, Mat inputImage, char *outName) {
	
	int rows = inputImage.rows;
	int cols = inputImage.cols;

	vector<Mat> i_planes;
	split(inputImage, i_planes);
	Mat iB = i_planes[0];
	Mat iG = i_planes[1];
	Mat iR = i_planes[2];

	// dynamically allocate RGB arrays of size rows x cols
	int** R = new int*[rows];
	int** G = new int*[rows];
	int** B = new int*[rows];
	for (int i = 0; i < rows; i++) {
		R[i] = new int[cols];
		G[i] = new int[cols];
		B[i] = new int[cols];
	}

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			R[i][j] = iR.at<uchar>(i, j);
			G[i][j] = iG.at<uchar>(i, j);
			B[i][j] = iB.at<uchar>(i, j);
		}




	//	   The transformation should be based on the
	//	   historgram of the pixels in the W1,W2,H1,H2 range.
	//	   The following code goes over these pixels
	double max = 0;
	double min = 100;
	
	for (int i = H1; i <= H2; i++)
		for (int j = W1; j <= W2; j++) {
	//int i = 239; int j = 345;
			double r = R[i][j];
			double g = G[i][j];
			double b = B[i][j];
			int* Luv = RGB2Luv(r, g, b);//

			int L = Luv[0];
			//cout << "L= " << L << ", i= " << i << ", j=" << j << "\n\n";
			/*i = 239; j = 346;
			r = R[i][j];
			g = G[i][j];
			b = B[i][j];
			Luv = RGB2Luv(r, g, b);
			L = Luv[0];
			cout << "L= " << L << ", i= " << i << ", j=" << j << "\n\n";*/
			
			
			if (L > max){
				max = L;
			}
			if (L < min){
				min = L;
			}
			//linear RGB
			/*int gray = (int)(0.3*r + 0.6*g + 0.1*b + 0.5);

			R[i][j] = G[i][j] = B[i][j] = gray;*/
		}
	cout << "min= " << min<<"\n";
	cout << "max= " << max <<"\n";
for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++){
	//int i = 300; int j = 0;
			double r = R[i][j];
			double g = G[i][j];
			double b = B[i][j];
			int* Luv = RGB2Luv(r, g, b);
			int L = Luv[0];
			int u = Luv[1];
			int v = Luv[2];
			/*cout << "L= " << L << "\n";
			cout << "u= " << u << "\n";
			cout << "v= " << v << "\n";*/


			L = 100 * ((L - min) / (max - min));
			if (L > 100){
				L = 100;
			}
			if (L < 0){
				L = 0;
			}
			//cout << "L= " << L << "\n";
			int* RGB = Luv2RGB(L, u, v); 
			R[i][j] = RGB[0];
			G[i][j] = RGB[1];
			B[i][j] = RGB[2];
			//cout << "i= "<<i<<", j="<<j<<", R= " << R[i][j] << ", G= "<< G[i][j]<<", B= "<<B[i][j]<<"\n\n";


			//i = 200; j = 0;
			//r = R[i][j];
			//g = G[i][j];
			//b = B[i][j];
			//Luv = RGB2Luv(r, g, b);
			//L = Luv[0];
			//u = Luv[1];
			//v = Luv[2];
			//cout << "L= " << L << "\n";
			//cout << "u= " << u << "\n";
			//cout << "v= " << v << "\n";


			//L = 100 * ((L - min) / (max - min));
			//if (L > 100){
			//	L = 100;
			//}
			//if (L < 0){
			//	L = 0;
			//}
			////cout << "L= " << L << "\n";
			//RGB = Luv2RGB(L, u, v);
			//R[i][j] = RGB[0];
			//G[i][j] = RGB[1];
			//B[i][j] = RGB[2];
			//cout << "i= " << i << ", j=" << j << ", R= " << R[i][j] << ", G= " << G[i][j] << ", B= " << B[i][j] << "\n";
		}



		

	Mat oR(rows, cols, CV_8UC1);
	Mat oG(rows, cols, CV_8UC1);
	Mat oB(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {


			oR.at<uchar>(i, j) = R[i][j];;
			oG.at<uchar>(i, j) = G[i][j];;
			oB.at<uchar>(i, j) = B[i][j];;
		}

	Mat o_planes[] = { oB, oG, oR };
	Mat outImage;
	merge(o_planes, 3, outImage);

	namedWindow("output", CV_WINDOW_AUTOSIZE);
	imshow("output", outImage);
	imwrite(outName, outImage);
}

int main(int argc, char** argv) {
	if (argc != 7) {
		cerr << argv[0] << ": "
			<< "got " << argc - 1
			<< " arguments. Expecting six: w1 h1 w2 h2 ImageIn ImageOut."
			<< endl;
		cerr << "Example: proj1b 0.2 0.1 0.8 0.5 fruits.jpg out.bmp" << endl;
		return(-1);
	}
	double w1 = atof(argv[1]);
	double h1 = atof(argv[2]);
	double w2 = atof(argv[3]);
	double h2 = atof(argv[4]);
	char *inputName = argv[5];
	char *outputName = argv[6];

	if (w1<0 || h1<0 || w2 <= w1 || h2 <= h1 || w2>1 || h2>1) {
		cerr << " arguments must satisfy 0 <= w1 < w2 <= 1"
			<< " ,  0 <= h1 < h2 <= 1" << endl;
		return(-1);
	}

	Mat inputImage = imread(inputName, CV_LOAD_IMAGE_UNCHANGED);
	if (inputImage.empty()) {
		cout << "Could not open or find the image " << inputName << endl;
		return(-1);
	}

	string windowInput("input: ");
	windowInput += inputName;

	namedWindow(windowInput, CV_WINDOW_AUTOSIZE);
	imshow(windowInput, inputImage);

	if (inputImage.type() != CV_8UC3) {
		cout << inputName << " is not a standard color image  " << endl;
		return(-1);
	}

	int rows = inputImage.rows;
	int cols = inputImage.cols;
	int W1 = (int)(w1*(cols - 1));
	int H1 = (int)(h1*(rows - 1));
	int W2 = (int)(w2*(cols - 1));
	int H2 = (int)(h2*(rows - 1));

	runOnWindow(W1, H1, W2, H2, inputImage, outputName);
	
	
	

	waitKey(0); // Wait for a keystroke
	return(0);  
}
