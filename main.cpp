#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

// Mat src, src_gray,dst;
// Mat threshold_output;


/*
int main( )
{
  /// Load an image
  src = imread( "D:/Image_Project/123.jpg", CV_LOAD_IMAGE_COLOR );

  /// Convert the image to Gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Create a window to display results
  namedWindow( "Thresholding Image", CV_WINDOW_AUTOSIZE );

  threshold( src_gray, dst, 200, 255, THRESH_BINARY );
	
  imshow("Thresholding Image",dst);
  
  waitKey(0);

  return 0;


} */
 
	


Mat src; Mat src_gray;
int thresh = 200;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
 int main( int argc, char** argv )
{
  /// Load source image and convert it to gray

   src = imread( "D:/Image_Project/123.jpg", CV_LOAD_IMAGE_COLOR );
	
//	src = imread( "D:/Image_Project/portrait.jpg", CV_LOAD_IMAGE_COLOR );

 // src = imread( "D:/Image_Project/has.png", CV_LOAD_IMAGE_COLOR );
	
	//src = imread( "D:/Image_Project/cup.png", CV_LOAD_IMAGE_COLOR );
	
	//src = imread( "D:/Image_Project/rol.png", CV_LOAD_IMAGE_COLOR );
	
//	src = imread( "D:/Image_Project/por.jpg", CV_LOAD_IMAGE_COLOR );
  
//	src = imread( "D:/Image_Project/pattern.jpg", CV_LOAD_IMAGE_COLOR );

	/// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  imshow( source_window, src );

 createTrackbar( " Thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
} 

/** @function thresh_callback */
 void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  Mat src_copy = src.clone();
  Mat threshold_output;
	

  /// Detect edges using canny

  threshold( src_gray, threshold_output,thresh, 255, THRESH_BINARY );
  
 // Canny( src_gray, canny_output, thresh, thresh*2, 3 );

  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  vector<vector<Point> >hull( contours.size() );
	
	for( int i = 0; i < contours.size(); i++ )
	{  
		convexHull( Mat(contours[i]), hull[i], false ); 
	}

  /// Get the moments
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false );
     }

  ///  Get the mass centers:thresh
  vector<Point2f> mc( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
     } 

  /// Draw contours
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
	   drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       circle( drawing, mc[i], 2, color, -1, 8, 0 );


		//printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );

		 if(mu[i].m00 > 1000 )
       {
           Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
           drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
		   drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
           circle( drawing, mc[i], 4, color, -1, 8, 0 );
       }


    }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
   imshow( "Contours", drawing );
   imwrite("D:/Image_Project/convexHullCenter.png", drawing);

 } 
