
#include <aruco.h>

#include <iostream>
#include <iomanip>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#define ESC_KEY 27

using namespace cv;
using namespace aruco;
using namespace std;
Mat im1;

// Main Method
int main(int argc, char * argv[]) {



   int cameraID;
   VideoCapture cap;
   // Creating the OpenCV capture
   cout << "Entrez l'identifiant de la camera" << endl;
   cin >> cameraID;
   cap.open(cameraID);

   if(!cap.isOpened()) {
      cerr << "Erreur lors de l'initialisation de la capture de la camera !"<< endl;
      cerr << "Fermeture..." << endl;
      exit(EXIT_FAILURE);
   }
   else{
      // retrieving a first frame so that the display does not crash
      cap >> im1;
   }


   

	MarkerDetector myDetector ;

	vector <Marker> markers ;

	myDetector.detect (im1, markers) ;

	for (unsigned int i =0; i <markers.size( ) ; i ++) {
		cout << markers [i] ;
		markers[i].draw (im1, Scalar( 0 , 0 , 255 ), 2);
	}
   
   // Showing image 1
   namedWindow("Image 1");
   imshow("Image 1", im1);
   

   // Launching the infinite loop
   char retK = 0;
   
   while(retK != ESC_KEY) {

	   cap >> im1;

	   myDetector.detect (im1, markers) ;

		for (unsigned int i =0; i <markers.size( ) ; i ++) {
			cout << markers [i] ;
			markers[i].draw (im1, Scalar( 0 , 0 , 255 ), 2);
		}


      imshow("Image 1", im1);
      // waiting for a key press
      retK = cv::waitKey(1);
   }
   
   // Done...
   return 0;
}