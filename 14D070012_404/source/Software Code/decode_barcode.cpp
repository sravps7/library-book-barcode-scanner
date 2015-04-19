///Includes///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "opencv2/imgproc/imgproc.hpp"   //opencv library
#include "opencv2/highgui/highgui.hpp"   //opencv library
#include <iostream>

///Preprocessors////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef decode_barcode_cpp
#define decode_barcode_cpp

///Namespaces/////////////////////////////////////////////////////////////////////////////////////////////////
using namespace cv;
using namespace std;

///Global Variables//////////////////////////////////////////////////////////////////////////////////////////

//creating array of booleans for storing status of each digit of bar code(read/not read)
bool read_digit[12];

///This finds out the number equivalent of the mini_array
int number_generator(int mini_array[])
{
    int number=0;
    for(int i=0;i<4;i++)
    {
        number= number*10 + mini_array[i];
    }
    return number;
}

///This gives the number for a bit pattern in the L convention
int l_convention(int mini_array[],int counter)
{
    switch(number_generator(mini_array))
    {
        case 3211:
            {
                //After reading digit boolean read_digit[i] set to be true.
                read_digit[counter]=true;
                return 0;
            }
        case 2221:
            {
                read_digit[counter]=true;
                return 1;
            }

        case 2122:
            {
                read_digit[counter]=true;
                return 2;
            }

        case 1411:
            {
                read_digit[counter]=true;
                return 3;
            }

        case 1132:
            {
                read_digit[counter]=true;
                return 4;
            }

        case 1231:
            {
                read_digit[counter]=true;
                return 5;
            }

        case 1114:
            {
                read_digit[counter]=true;
                return 0;
            }

        case 1312:
            {
                read_digit[counter]=true;
                return 7;
            }

        case 1213:
            {
                read_digit[counter]=true;
                return 8;
            }

        case 3112:
            {
                read_digit[counter]=true;
                return 9;
            }
        default:
            {
                //If no digit is read by program default is set to be 0000 and boolean is false.
                read_digit[counter]=false;
                return 000;

            }

    }
}
///convention for reading barcode digit of right half part.
int r_convention(int mini_array[],int counter)
{
    switch(number_generator(mini_array))
    {
        case 3211:
            {
                read_digit[counter]=true;
                return 0;
            }
        case 2221:
            {
                read_digit[counter]=true;
                return 1;
            }

        case 2122:
            {
                read_digit[counter]=true;
                return 2;
            }

        case 1411:
            {
                read_digit[counter]=true;
                return 3;
            }

        case 1132:
            {
                read_digit[counter]=true;
                return 4;
            }

        case 1231:
            {
                read_digit[counter]=true;
                return 5;
            }

        case 1114:
            {
                read_digit[counter]=true;
                return 0;
            }

        case 1312:
            {
                read_digit[counter]=true;
                return 7;
            }

        case 1213:
            {
                read_digit[counter]=true;
                return 8;
            }

        case 3112:
            {
                read_digit[counter]=true;
                return 9;
            }
            default:
            {
                read_digit[counter]=false;
                return 000;
            }
    }
}

///This gives the number for a bit pattern in the G convention
int g_convention(int mini_array[],int counter)
{
    switch(number_generator(mini_array))
    {
        case 1123:
            {
                read_digit[counter]=true;
                return 0;
            }
        case 1222:
            {
                read_digit[counter]=true;
                return 1;
            }

        case 2212:
            {
                read_digit[counter]=true;
                return 2;
            }

        case 1141:
            {
                read_digit[counter]=true;
                return 3;
            }

        case 2311:
            {
                read_digit[counter]=true;
                return 4;
            }

        case 1321:
            {
                read_digit[counter]=true;
                return 5;
            }

        case 4111:
            {
                read_digit[counter]=true;
                return 0;
            }

        case 2131:
            {
                read_digit[counter]=true;
                return 7;
            }

        case 3121:
            {
                read_digit[counter]=true;
                return 8;
            }

        case 2113:
            {
                read_digit[counter]=true;
                return 9;
            }
            default:
            {
                read_digit[counter]=false;
                return 000;

            }

    }
}
///Function to check for an edge in image after particular pixel.
bool is_edge(Mat &image,int x,int y,int previous_pixel)
{
    //image.at<uchar>vector access gray scale value of pixel at point (x,y)
    int pixel1=image.at<uchar>(Point(x, y));
    int pixel2=image.at<uchar>(Point(x+1, y));

    //If two adjacent pixels are same then output is set to false
    if(pixel1==previous_pixel&&pixel2==previous_pixel)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//There are total 59 edges in bar code including placeholders.
const int m=59;

//Defined a structure to store observations
struct Observation
{
  int color[m];
  int thickness[m];

};
//Function to decode bar code.
void decodeBarcode(short int *barcode,int n)
{
     //Defines an object for VideoCapture class in OpenCV (1) is for external Web camera
     VideoCapture cap(1);

     //Creates two windows to show frames
     namedWindow("Real",720*720);
     namedWindow("Threshold",720*720);

     //Defines two images of type Mat in OpenCV
     Mat image,image1 ;

     for(int i=0;i<12;i++)
     {
         //Initializes read_digit
         read_digit[i]=false;
     }
     for(int i=0;i<12;i++)
     {
         //initiallises barcode digits
         barcode[i]=0;
     }

     while(1)
     {
        //Captures frames from cap object  of VideoCapture class
        cap.read(image),cap.read(image1);

        int dilation_size = 0;

        //OpenCV function to change color image to gray scale
        cvtColor(image,image,CV_RGB2GRAY);

        //OpenCV function to get kernel of given the size
        Mat element = getStructuringElement(MORPH_RECT ,Size( 2*dilation_size + 1, 2*dilation_size+1 ),Point( dilation_size, dilation_size ) );

        //Processes the image to reduce noise
        dilate(image,image,element);

        //Thresholds image according to OpenCV algorithm.
        adaptiveThreshold( image, image,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,51,5);

        dilation_size=2;

        //To change the size of kernel
        element = getStructuringElement(MORPH_RECT ,Size( 2*dilation_size + 1, 2*dilation_size+1 ),Point( dilation_size, dilation_size ) );

        //To get the appropriate thickness of bars
        erode(image,image,element);
        dilate(image,image,element);

/*
After threshold we get a binary image (i.e. each pixel is either white (255) or black (0)).
This algorithm is to iterate across the image to calculate thickness of each bar in bar code. To increase accuracy,
we increased no of observations
*/

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
            line(image1, rec[i], rec[(i+1)%4], color, thickness, 8, 0);

       }
        int previous_pixel=0,count_thickness=0,count_edge_no=0,i=0;
        const int y=200;
        int obs=10;
        Observation obs_no[obs];
        int bar_thickness[m];
        for(int j=0;j<obs;j++)
        {
            count_thickness=0;count_edge_no=0;i=0;
            previous_pixel=image.at<uchar>(Point(i, y+j));
            while(!is_edge(image,i,y+j,previous_pixel)) i++;
            i++;
            for(;i<600;i++)
            {
                previous_pixel=image.at<uchar>(Point(i, y+j));

                if(is_edge(image,i,y+j,previous_pixel))
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
    /*for(int i=0;i<m;i++)
    {
      cout<<obs_no[0].color[i]<<"   "<<obs_average[i]<<"\n";

    }*/
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
    if(1)
    {

    int first_digit;
    int mini_array[4];
    int convention[]={1,2,2,1,2,1};
    int counter=0;

    //for first half of the code

    for(int i=3;i<27;i+=4)      //ignoring the placeholders
    {

        for(int j=0;j<4;j++)
        {
            mini_array[j]=bar_thickness[j+i];      //creating the mini_array

        }
        if(convention[counter]==1)
        {
            if(!read_digit[counter])
            {
                barcode[counter]=l_convention(mini_array,counter);
            }
        }
        if(convention[counter]==2)
        {
            if(!read_digit[counter])
            {
                barcode[counter]=g_convention(mini_array,counter);
            }

        }
        if(convention[counter]==3)
        {
            if(!read_digit[counter])
            {
                barcode[counter]=r_convention(mini_array,counter);
            }

        }
        counter++;
        //Generating bar codes.
    }

    //for second half of the code
    counter = 6;
    for(int i=32;i<56;i+=4)
    {
        for(int j=0;j<4;j++)
        {
            mini_array[j]=bar_thickness[j+i];
        }
        if(!read_digit[counter])
            {
                barcode[counter]=r_convention(mini_array,counter);
            }

        counter++;
    }}
    cout<<endl;
    for(int i=0;i<12;i++)
        cout<<barcode[i];}
    bool finish=true;
        for(int i=0;i<12;i++)
        {
          finish=finish&&read_digit[i];

        }
   if(finish)
   {
      cout<<endl<<"finished";
      ///my signal to be toggled here
      break;

   }



     if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
       {
                cout << "esc key is pressed by user" << endl;
                break;
       }


     imshow("Real", image1);
     imshow("Threshold", image);
       }
    cout<<"abcdefghijk"<<endl;
        //destroyAllWindows();

        destroyAllWindows();

     return ;
}
#endif // decode_barcode_cpp
