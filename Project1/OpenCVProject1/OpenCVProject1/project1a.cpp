#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << argv[0] << ": "
			<< "got " << argc - 1 << " arguments. Expecting two: width height."
			<< endl;
		return(-1);
	}

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int** RED1 = new int*[height];
	int** GREEN1 = new int*[height];
	int** BLUE1 = new int*[height];
	int** RED2 = new int*[height];
	int** GREEN2 = new int*[height];
	int** BLUE2 = new int*[height];

	for (int i = 0; i < height; i++) {
		RED1[i] = new int[width];
		GREEN1[i] = new int[width];
		BLUE1[i] = new int[width];
		RED2[i] = new int[width];
		GREEN2[i] = new int[width];
		BLUE2[i] = new int[width];
	}

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int r1, g1, b1;
			int r2, g2, b2;

			double x = (double)j / (double)width;
			double y = (double)i / (double)height;
			double Y = 1.0;

			double L = 90;
			double u = x * 512 - 255;
			double v = y * 512 - 255;


			/* Your code should be placed here
			It should translate xyY to byte sRGB
			and Luv to byte sRGB
			*/
			//==========for xyY to RGB=======
			//----------xyY to XYZ
			double X1 = (x / y) *Y;
			double Y1 = Y;
			double Z1 = ((1 - x - y) / y)* Y;
			//----------XYZ to linear sRGB
			double linear_r1 = 3.240479 * X1 - 1.53715 * Y1 - 0.498535 * Z1;
			double linear_g1 = -0.969256 * X1 + 1.875991 * Y1 + 0.041556* Z1;
			double linear_b1 = 0.055648 * X1 - 0.204043 * Y1 + 1.057311 * Z1; 
			//clipping for r
			if (linear_r1 > 1){
				linear_r1 = 1;
			}
			if (linear_r1 <0){
				linear_r1 = 0;
			}
			//clipping for g
			if (linear_g1 > 1){
				linear_g1 = 1;
			}
			if (linear_g1 <0){
				linear_g1 = 0;
			}
			//clipping for b
			if (linear_b1 > 1){
				linear_b1 = 1;
			}
			if (linear_b1 <0){
				linear_b1 = 0;
			}

			//----------linear sRGB to nonlinear sRGB
			double nonlinear_r1;
			double nonlinear_g1;
			double nonlinear_b1;
			
			if (linear_r1 < 0.00304){
				nonlinear_r1 = 12.92 * linear_r1;
			}
			else{
				nonlinear_r1 = 1.055*(pow(linear_r1,((double)1 / (double)2.4)) ) - 0.055;
			}

			
			if (linear_g1 < 0.00304){
				nonlinear_g1 = 12.92 * linear_g1;
			}
			else{
				nonlinear_g1 = 1.055*(pow(linear_g1, ((double)1 / (double)2.4))) - 0.055;
			}

			
			if (linear_b1 < 0.00304){
				nonlinear_b1 = 12.92 * linear_b1;
			}
			else{
				nonlinear_b1 = 1.055*(pow(linear_b1, ((double)1 / (double)2.4))) - 0.055;
			}

			//clipping for r
			if (nonlinear_r1 > 1){
				nonlinear_r1 = 1;
			}
			if (nonlinear_r1 <0){
				nonlinear_r1 = 0;
			}
			//clipping for g
			if (nonlinear_g1 > 1){
				nonlinear_g1 = 1;
			}
			if (nonlinear_g1 <0){
				nonlinear_g1 = 0;
			}
			//clipping for b
			if (nonlinear_b1 > 1){
				nonlinear_b1 = 1;
			}
			if (nonlinear_b1 <0){
				nonlinear_b1 = 0;
			}
			//----------nonlinear sRGB to pixels
			r1 = nonlinear_r1 * 255;
			g1 = nonlinear_g1 * 255;
			b1 = nonlinear_b1 * 255;

			//==========for Luv to RGB=======
			//----------Luv to XYZ
			double Xw = 0.95;
			double Yw = 1.0;
			double Zw = 1.09;

			double uw = (double)4 * Xw / (Xw + (double)15 * Yw + (double)3 * Zw);
			double vw = (double)9 * Yw / (Xw + (double)15 * Yw + (double)3 * Zw);

			double u_p = (double)(u + (double)13 * uw*L) / (double)((double)13 * L);
			double v_p = (double)(v + (double)13 * vw*L) / (double)((double)13 * L);

			double Y2 = 0;
			if (L > 7.9996)
			{
				Y2 = pow((double)(L + 16) / (double)116, 3) * Yw;
			}
			else{
				Y2 = (double)L / 903.3* Yw;
			}
			double X2 = (double)Y2*2.25 * u_p / (double)v_p;
			double Z2 = (double)Y2*(3 - 0.75*u_p - 5 * v_p) / (double)v_p;
			//----------XYZ to linear sRGB
			double linear_r2 = 3.240479 * X2 - 1.53715 * Y2 - 0.498535 * Z2;
			double linear_g2 = -0.969256 * X2 + 1.875991 * Y2 + 0.041556* Z2;
			double linear_b2 = 0.055648 * X2 - 0.204043 * Y2 + 1.057311 * Z2;
			//clipping for r
			if (linear_r2 > 1){
				linear_r2 = 1;
			}
			if (linear_r2 <0){
				linear_r2 = 0;
			}
			//clipping for g
			if (linear_g2 > 1){
				linear_g2 = 1;
			}
			if (linear_g2 <0){
				linear_g2 = 0;
			}
			//clipping for b
			if (linear_b2 > 1){
				linear_b2 = 1;
			}
			if (linear_b2 <0){
				linear_b2 = 0;
			}

			//----------linear sRGB to nonlinear sRGB
			double nonlinear_r2;
			double nonlinear_g2;
			double nonlinear_b2;

			if (linear_r2 < 0.00304){
				nonlinear_r2 = 12.92 * linear_r2;
			}
			else{
				nonlinear_r2 = 1.055*(pow(linear_r2, ((double)1 / (double)2.4))) - 0.055;
			}


			if (linear_g2 < 0.00304){
				nonlinear_g2 = 12.92 * linear_g2;
			}
			else{
				nonlinear_g2 = 1.055*(pow(linear_g2, ((double)1 / (double)2.4))) - 0.055;
			}


			if (linear_b2 < 0.00304){
				nonlinear_b2 = 12.92 * linear_b2;
			}
			else{
				nonlinear_b2 = 1.055*(pow(linear_b2, ((double)1 / (double)2.4))) - 0.055;
			}

			//clipping for r
			if (nonlinear_r2 > 1){
				nonlinear_r2 = 1;
			}
			if (nonlinear_r2 <0){
				nonlinear_r2 = 0;
			}
			//clipping for g
			if (nonlinear_g2 > 1){
				nonlinear_g2 = 1;
			}
			if (nonlinear_g2 <0){
				nonlinear_g2 = 0;
			}
			//clipping for b
			if (nonlinear_b2 > 1){
				nonlinear_b2 = 1;
			}
			if (nonlinear_b2 <0){
				nonlinear_b2 = 0;
			}
			//----------nonlinear sRGB to pixels
			r2 = (int)(nonlinear_r2 * 255);
			g2 = (int)(nonlinear_g2 * 255);
			b2 = (int)(nonlinear_b2 * 255);



			/*r1 = (int) (x * 255);
	g1 = (int) (y * 255);
	b1 = (int) (1.0 * 255);

	r2 = (int) (1.0 * 255);
	g2 = (int) (x * 255);
	b2 = (int) (y * 255);*/
			
			

			// this is the end of your code

			RED1[i][j] = r1;
			GREEN1[i][j] = g1;
			BLUE1[i][j] = b1;
			RED2[i][j] = r2;
			GREEN2[i][j] = g2;
			BLUE2[i][j] = b2;
		}


	Mat R1(height, width, CV_8UC1);
	Mat G1(height, width, CV_8UC1);
	Mat B1(height, width, CV_8UC1);

	Mat R2(height, width, CV_8UC1);
	Mat G2(height, width, CV_8UC1);
	Mat B2(height, width, CV_8UC1);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {

			R1.at<uchar>(i, j) = RED1[i][j];
			G1.at<uchar>(i, j) = GREEN1[i][j];
			B1.at<uchar>(i, j) = BLUE1[i][j];

			R2.at<uchar>(i, j) = RED2[i][j];
			G2.at<uchar>(i, j) = GREEN2[i][j];
			B2.at<uchar>(i, j) = BLUE2[i][j];
		}

	Mat xyY;
	Mat xyY_planes[] = { B1, G1, R1 };
	merge(xyY_planes, 3, xyY);
	namedWindow("xyY", CV_WINDOW_AUTOSIZE);
	imshow("xyY", xyY);

	Mat Luv;
	Mat Luv_planes[] = { B2, G2, R2 };
	merge(Luv_planes, 3, Luv);
	namedWindow("Luv", CV_WINDOW_AUTOSIZE);
	imshow("Luv", Luv);
	waitKey(0); // Wait for a keystroke
	return(0);
}