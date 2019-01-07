//
//  ArUco-OpenGL.cpp
//
//  Created by Jean-Marie Normand on 28/02/13.
//  Copyright (c) 2013 Centrale Nantes. All rights reserved.
//

#include <iostream>
#include "ArUco-OpenGL.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

int ArUco::level = 0;
float ArUco::touchDistance = 0.3;
bool ArUco::transportingGold = true;

//Models
Mesh humanModel("3d/human.obj");
Mesh goldModel("3d/gold.obj");
Mesh bossModel("3d/boss.obj");
Mesh gunModel("3d/gun.obj");
Mesh bankModel("3d/bank.obj");
Mesh testModel("3d/test2.obj");

// Constructor
ArUco::ArUco(string intrinFileName, float markerSize) {
   // Initializing attributes
   m_IntrinsicFile= intrinFileName;
   m_MarkerSize   = markerSize;
   // read camera parameters if passed
   m_CameraParams.readFromXMLFile(intrinFileName);
}

// Destructor
ArUco::~ArUco() {}

// Detect marker and draw things
void ArUco::doWork(Mat inputImg) {
   m_InputImage   = inputImg;
   m_GlWindowSize = m_InputImage.size();
   resize(m_GlWindowSize.width, m_GlWindowSize.height);
}

// Draw axis function
void ArUco::drawAxis(float axisSize) {
   // X
   glColor3f (1,0,0);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(axisSize,0.0f, 0.0f); // ending point of the line
   glEnd( );
   
   // Y
   glColor3f (0,1,0);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(0.0f, axisSize, 0.0f); // ending point of the line
   glEnd( );
   
   // Z
   glColor3f (0,0,1);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(0.0f, 0.0f, axisSize); // ending point of the line
   glEnd( );
}

// GLUT functionnalities

// Drawing function
void ArUco::drawScene() {
   // If we do not have an image we don't do anyhting
   if (m_ResizedImage.rows==0)
      return;
   
   // Setting up  OpenGL matrices
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, m_GlWindowSize.width, 0, m_GlWindowSize.height, -1.0, 1.0);
   glViewport(0, 0, m_GlWindowSize.width , m_GlWindowSize.height);
   glDisable(GL_TEXTURE_2D);
   glPixelZoom( 1, -1);
   
   //////glRasterPos3f( 0, m_GlWindowSize.height  - 0.5f, -1.0f );
   glRasterPos3f(0, m_GlWindowSize.height, -1.0f);
   
   glDrawPixels (m_GlWindowSize.width, m_GlWindowSize.height, GL_RGB, GL_UNSIGNED_BYTE, m_ResizedImage.ptr(0));
   
   // Enabling depth test
   glEnable(GL_DEPTH_TEST);
   
   // Set the appropriate projection matrix so that rendering is done 
   // in an environment like the real camera (without distorsion)
   glMatrixMode(GL_PROJECTION);
   double proj_matrix[16];
   //m_CameraParams.glGetProjectionMatrix(m_InputImage.size(),m_GlWindowSize,proj_matrix,0.05,10);
   m_CameraParams.glGetProjectionMatrix(m_ResizedImage.size(),m_GlWindowSize,proj_matrix,0.05,10);
   glLoadIdentity();
   glLoadMatrixd(proj_matrix);
   
   // Debug : outputting projection matrix
   /*
    std::cout<<"Projection Matrix"<<std::endl;
   for(int i=0;i<4;i++) {
      std::cout<<"| ";
      for(int j=0;j<4;j++) {
         std::cout<<proj_matrix[j*4+i]<<"\t\t\t";
      }
      std::cout<<" |"<<std::endl;
   }
    */
   
   //now, for each marker,
   double modelview_matrix[16];
   std::cout << "Number of markers: " << m_Markers.size() << std::endl;
   
   
   bool goldDetected = false;
   Vector3D goldPosition;
   bool humanDetected= false;
   Vector3D humanPosition;
   bool bankDetected = false;
   Vector3D bankPosition;


   // For each detected marker
   for (unsigned int m=0;m<m_Markers.size();m++)
   {
      m_Markers[m].glGetModelViewMatrix(modelview_matrix);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glLoadMatrixd(modelview_matrix);
      
      // Disabling light if it is on
      GLboolean lightOn = false;
      glGetBooleanv(GL_LIGHTING, &lightOn);

      if(lightOn) {
         //glDisable(GL_LIGHTING);
      }
	  string text = "Level " + to_string(level);
	  switch(m_Markers[m].id){
	  case 7:
		  if(transportingGold){
			goldModel.Display();
		  }else{
			  gunModel.Display();
		  }
		  goldDetected = true;
		  goldPosition = Vector3D(m_Markers[m].Tvec.at<float>(0,0),m_Markers[m].Tvec.at<float>(1,0),m_Markers[m].Tvec.at<float>(2,0));
		  break;
	  case 10:
		  if(level < 10){
			  humanModel.Display();
		  }else{
			  bossModel.Display();
		  }
		  //Afficher le level de l'humain

		  drawText(m_ResizedImage,m_Markers[m]);
		  humanDetected = true;
		  humanPosition = Vector3D(m_Markers[m].Tvec.at<float>(0,0),m_Markers[m].Tvec.at<float>(1,0),m_Markers[m].Tvec.at<float>(2,0));
		  break;
	  case 8:
		  bankModel.Display();
		  bankDetected = true;
		  bankPosition = Vector3D(m_Markers[m].Tvec.at<float>(0,0),m_Markers[m].Tvec.at<float>(1,0),m_Markers[m].Tvec.at<float>(2,0));
		  break;
	  
	  default:
		  break;
	  }
	  if(transportingGold){
		  if(humanDetected && goldDetected){
			  cout << (humanPosition - goldPosition).length() << endl;
			  if((humanPosition - goldPosition).length() < touchDistance)
			  {
				  level++;
				  transportingGold = false;
			  }
		  }
	  }
	  else
	  {
		  if(bankDetected && goldDetected){
			  cout << (bankPosition - goldPosition).length() << endl;
			  if((bankPosition - goldPosition).length() < touchDistance)
			  {
				  transportingGold = true;
			  }
		  }
	  }
	  //DRAW HERE
      /*
      // Drawing axis
      drawAxis(m_MarkerSize);
      
      // Drawing a cube
      glColor3f(1,0.4f,0.4f);
      //glTranslatef(0, m_MarkerSize/2,0);
      glTranslatef(0, 0, m_MarkerSize/2);
      
      glPushMatrix();
      glutWireCube( m_MarkerSize );
      */

	  glPushMatrix();
		

      // Re-enabling light if it is on
      if(lightOn) {
         //glEnable(GL_LIGHTING);
      }
      
      glPopMatrix();
   }
   
   // Disabling depth test
   glDisable(GL_DEPTH_TEST);
   
   m_pixels.create(m_GlWindowSize.height , m_GlWindowSize.width, CV_8UC3);
   //use fast 4-byte alignment (default anyway) if possible
   glPixelStorei(GL_PACK_ALIGNMENT, (m_pixels.step & 3) ? 1 : 4);
   //set length of one complete row in destination data (doesn't need to equal img.cols)
   glPixelStorei(GL_PACK_ROW_LENGTH, m_pixels.step/m_pixels.elemSize());
   // Reading back the pixels
   glReadPixels(0, 0, m_GlWindowSize.width , m_GlWindowSize.height, GL_RGB, GL_UNSIGNED_BYTE, m_pixels.data);
   // Flip the pixels since OpenCV stores top to bottom and OpenGL from bottom to top
   cv::flip(m_pixels, m_pixels, 0);
}

// Idle function
void ArUco::idle(Mat newImage) {
   // Getting new image
   m_InputImage = newImage.clone();
   
   // Do that here ?
   resize(m_InputImage.size().width, m_InputImage.size().height);
   
   // Undistort image based on distorsion parameters
   m_UndInputImage.create(m_InputImage.size(),CV_8UC3);
   
   //transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
   cv::cvtColor(m_InputImage,m_InputImage,CV_BGR2RGB);
   
   //remove distorion in image
   // Jim commented next line and added the clone line
   //cv::undistort(m_InputImage,m_UndInputImage, m_CameraParams.CameraMatrix, m_CameraParams.Distorsion);
   m_UndInputImage = m_InputImage.clone();
   
   //detect markers
   m_PPDetector.detect(m_UndInputImage, m_Markers, m_CameraParams.CameraMatrix, Mat(), m_MarkerSize);
   //m_PPDetector.detect(m_UndInputImage, m_Markers, m_CameraParams, m_MarkerSize);
   
   //resize the image to the size of the GL window
   cv::resize(m_UndInputImage,m_ResizedImage,m_GlWindowSize);
}

// Resize function
void ArUco::resize(GLsizei iWidth, GLsizei iHeight) {
   m_GlWindowSize=Size(iWidth,iHeight);
   
   //not all sizes are allowed. OpenCv images have padding at the end of each line in these that are not aligned to 4 bytes
   if (iWidth*3%4!=0) {
      iWidth+=iWidth*3%4;//resize to avoid padding
      resize(iWidth, m_GlWindowSize.height);
   }
   else {
      //resize the image to the size of the GL window
      //if (m_UndInputImage.rows!=0)
         //cv::resize(m_UndInputImage, m_ResizedImage, m_GlWindowSize);
   }
}


// Jim
cv::Mat ArUco::getPixels() {
   return m_pixels.clone();
}

// Test using ArUco to display a 3D cube in OpenCV
void ArUco::draw3DCube(cv::Mat img, int markerInd) {
   if(m_Markers.size() > markerInd) {
      aruco::CvDrawingUtils::draw3dCube(img, m_Markers[markerInd], m_CameraParams); 
   }
}

void ArUco::draw3DAxis(cv::Mat img, int markerInd) {
   if(m_Markers.size() > markerInd) {
      aruco::CvDrawingUtils::draw3dAxis(img, m_Markers[markerInd], m_CameraParams); 
   }
}


void ArUco::drawText(cv::Mat &Image,Marker &m)
{
 
     Mat objectPoints (4,3,CV_32FC1);
     objectPoints.at<float>(0,0)=0;
     objectPoints.at<float>(0,1)=0;
     objectPoints.at<float>(0,2)=0;
     objectPoints.at<float>(1,0)=m.ssize;
     objectPoints.at<float>(1,1)=0;
     objectPoints.at<float>(1,2)=0;
     objectPoints.at<float>(2,0)=0;
     objectPoints.at<float>(2,1)=m.ssize;
     objectPoints.at<float>(2,2)=0;
	 objectPoints.at<float>(3,0)=0;
     objectPoints.at<float>(3,1)=0;
     objectPoints.at<float>(3,2)=m.ssize;
 
     vector<Point2f> imagePoints;
     cv::projectPoints( objectPoints, m.Rvec,m.Tvec, m_CameraParams.CameraMatrix,m_CameraParams.Distorsion,   imagePoints);
 //draw lines of different colours
	 /*
     cv::line(Image,imagePoints[0],imagePoints[1],Scalar(0,0,255),1,CV_AA);
     cv::line(Image,imagePoints[0],imagePoints[2],Scalar(0,255,0),1,CV_AA);
     cv::line(Image,imagePoints[0],imagePoints[3],Scalar(255,0,0),1,CV_AA);
     putText(Image,"x", imagePoints[1],FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0,0,255),2);
     putText(Image,"y", imagePoints[2],FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0,255,0),2);
	 */
     putText(Image,"Level", imagePoints[3],FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255,0,0),2);
}