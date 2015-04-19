
#include "opencv2/imgproc/imgproc.hpp"   //opencv library
#include "opencv2/highgui/highgui.hpp"   //opencv library
#include <iostream>
#include "decode_barcode.cpp"


using namespace cv;
using namespace std;

///function to check whether there edge in image after particular pixel.
bool is_barcode(int *bar_thickness)
{
    bool b=(bar_thickness[0]==1)&&(bar_thickness[1]==1)&&(bar_thickness[2]==1)&&(bar_thickness[56]==1)&&(bar_thickness[57]==1)&&(bar_thickness[58]==1);
    return b;

}


/*////////////*////function to decode barcode///////////////
void detect()
{
     VideoCapture cap(1);        //defines object for VideoCapture class in opencv (1) is for external webcam
     namedWindow("Real",720*720);       //creates 2 windows to show frames
     namedWindow("Threshold",720*720);
     Mat img,img1 ;
     int barcode[12];                //defines 2 images of type Mat in opencv
     for(int i=0;i<12;i++) barcode[i]=0;          //initiallises barcode digits


     while(1)
     {

        cap.read(img),cap.read(img1);   //captures frames from cap object  of VideoCapture class
        int dilation_size = 0;
        cvtColor(img,img,CV_RGB2GRAY);   //opencv fun to change color img to grayscale

        Mat element = getStructuringElement(MORPH_RECT ,Size( 2*dilation_size + 1, 2*dilation_size+1 ),Point( dilation_size, dilation_size ) ); //opencv fun to get kernel of given size


        dilate(img,img,element);         //pre process for image to reduce noise


        adaptiveThreshold( img, img,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,51,5);   //thresholds image according to opencv algorithem

        dilation_size=2;
        element = getStructuringElement(MORPH_RECT ,Size( 2*dilation_size + 1, 2*dilation_size+1 ),Point( dilation_size, dilation_size ) ); //to change size of kernel
        erode(img,img,element);
        dilate(img,img,element);   //to get apropriate thickness of bars

/*after threshold we get a binary image(ie eah pixel is either white(255) or black(0)
algo is to iterate across the image to calculate thickness of each bar in barcode to increase accuracy
we increased no of observations*/
        Point rec[4];
        rec[0]=Point2f(10,215);
        rec[1]=Point2f(600,215);
        rec[2]=Point2f(600,265);
        rec[3]=Point2f(10,265);
        rec[3]=Point2f(10,265);
       int thickness=2;
       for(int i=0;i<4;i++)
       {
            Scalar color=(0,255,0);
            line(img1, rec[i], rec[(i+1)%4], color, thickness, 8, 0);

       }
        int previous_pixel=0,count_thickness=0,count_edge_no=0,i=0;
        const int y=200;
        int obs=10;
        Observation obs_no[obs];
        int bar_thickness[m];
        for(int j=0;j<obs;j++)
        {
            count_thickness=0;count_edge_no=0;i=0;
            previous_pixel=img.at<uchar>(Point(i, y+j));
            while(!is_edge(img,i,y+j,previous_pixel)) i++;
            i++;
            for(;i<600;i++)
            {
                previous_pixel=img.at<uchar>(Point(i, y+j));

                if(is_edge(img,i,y+j,previous_pixel))
                    {
                            obs_no[j].color[count_edge_no]=previous_pixel;
                            obs_no[j].thickness[count_edge_no]=count_thickness;
                            count_edge_no++;
                            count_thickness=0;

                    }


                count_thickness++;
                }
        }

        long int obs_average[59];
        for(int i=0;i<59;i++)
        {
            long int sum=0;

            for(int j=0;j<obs;j++)
                {
                    sum+=obs_no[j].thickness[i];

                }
            obs_average[i]=sum/obs;

    }

     if(obs_average[0]!=0&&obs_average[1]!=0)
    {

    for(int i=0;i<m;i++)
    {

    if(i%2==0)
         {
              bar_thickness[i]=obs_average[i]/obs_average[0];
              if(obs_average[0]-obs_average[i]%obs_average[0]<3) bar_thickness[i]++;

         }
         else
         {
           bar_thickness[i]=obs_average[i]/obs_average[1];
              if(obs_average[0]-obs_average[i]%obs_average[1]<3) bar_thickness[i]++;
         }
         if(bar_thickness[i]==0) bar_thickness[i]=1;

    }

   if(is_barcode(bar_thickness))
   {
      cout<<"sdvasdvsav";

       break;

   }



     if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
       {
                cout << "esc key is pressed by user" << endl;
                break;
       }


     imshow("Real", img1);
     imshow("Threshold", img);
       }
       }

        //destroyAllWindows();

        //destroyAllWindows();
        //waitKey(0);

     return ;
}

