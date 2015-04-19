///Includes ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include"decode_barcode.cpp"
#include"file_io.cpp"
#include<iostream>
#include<stdio.h>
#include<vector>
#include<fstream>
#include<algorithm>
#include<string>
#include<iomanip>
#include "detect.cpp"
#include<windows.h>

///Global Variable////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Opens the COM port 6 for data transfer between bot and computer.
HANDLE hPort = CreateFile("COM6", GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
DCB dcb;
bool flag=true;

///Namespaces////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

///XBEE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Function for receiving data from Xbee to bot.
int receive_data()
{

    int Val;
    BYTE byte;
    DWORD dwBytesTransferred;
    DWORD dwCommModemStatus;

    if(!GetCommState(hPort, &dcb))
        return 0;

    SetCommMask(hPort, EV_RXCHAR|EV_ERR);

    WaitCommEvent(hPort, &dwCommModemStatus, 0);

    if (dwCommModemStatus& EV_RXCHAR)
    {
        ReadFile(hPort, &byte, 1, &dwBytesTransferred, 0);
    }

    Val = byte;

    return Val;
}

//Function for sending data from bot to Xbee.
bool send_command (char* data)
{

    DWORD byteswritten;

    if (!GetCommState(hPort, &dcb))
    {
        cout<<"\n\t Serial port can't be opened\n\tPlease check the port.";
        flag=false;

        return false;
    }

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if (!SetCommState(hPort, &dcb))
    {
        return false;
    }

    bool retVal = WriteFile(hPort, data, 1, &byteswritten, NULL);

    return retVal;
}

///Main Program starts here/////////////////////////////////////////////////////////////////////////////////////////

int main()
{


    char Continue =' ';    // if user wish to continue at end

    loadStart :             //Goto statement's label
            int operation=0;         // operation mode

            cout<<"\n\tPLEASE REFER HELP MENU IF YOU ARE OPERATING PROGRAMME FIRST TIME"<<endl;
            cout<<endl;

            cout<<"\t\tEnter Operation Mode:"<<endl<<"\t\t1:Create new file"<<endl<<"\t\t2:Read from Old file"<<endl<<"\t\t3:Help"<<endl<<"\t\t4:Quit"<<endl;
            cin>>operation;


    switch(operation)    /// load corresponding mode page
    {
        case 1 :
                    break;
        case 2 :
                    break;
        case 3 :
                    break;
        case 4 :
                    CloseHandle(hPort);
                    cout<<endl<<"\t\tTHANK YOU FOR USING OUR PRODUCT";

                    return 0;
        default:
                    cout<<"Invalid option"<<endl;
                    operation=0;
                    goto loadStart;
    }

    ///Operation 1 Starts here///////////////////////////////////////////////////////////////////////////////////////////////
    if(operation==1)
    {
        //Declares a long long vector
        vector<long long>tmpCodeStore;
        long long int previousCode= -1;
        string newFileName;

        cout<<"Enter New File Name:"<<endl;
        cin>>newFileName;

        char start;

        cout<<"Enter any character to start and (n) to go to home page:"<<endl;
        cin>>start;

        if(start=='n')
        {
            goto loadStart;         //Goto used to redirect flow to the label above
        }

        int countScannedCodes =0;
        cout<<endl<<"\t\tNo.       Code";

        while(true)
        {

            //Sends command to start the bot.
            send_command("8");

            //If COM Port is not available, redirects user to the label of goto.
            if(!flag)
            {
                cout<<"\n\t\t REDIRECTING TO THE HOME PAGE";
                goto loadStart;
            }

            //Calls a function which detects if the current image contains a bar code.
            detect();
            //Flow goes to detect.cpp
            cout<<"\n\tBarcode is detected.";

            //Stops the bot
            send_command("8");
            //Beeps a buzzer
            send_command("7");

            short int barcode[12];

            //Calls a function to decode the bar code found
            //Flow goes to decode_barcode.cpp
            decodeBarcode(barcode,12);

            long long int code=0;

            //Converts the bar code  array to a number.
            for(int i=0;i<12;i++)
            {
                code=10*code+barcode[i];
            }

            //Checks for repetition.
            if(code!=previousCode)
            {

                previousCode=code;
                countScannedCodes++;

                //Stores the bar code number in a vector.
                tmpCodeStore.push_back(code);

                //Prints the number of scanned codes along with the bar code number.
                cout<<endl<<"\t\t"<<setw(2)<<countScannedCodes<<"  "<<setprecision(13)<<code;
            }

            //Starts the bot to receive data from the bot.
            send_command("8");

            //Receive data from the bot.
            int temp=receive_data();

            //Checks for stoppage of the bot.
            if(temp==52)
            {

                cout<<endl<<"Bot is stopped "<<endl;
                //Stops the bot.
                send_command("8");

                break;
            }

            //Stops the bot and the data inflow.
            send_command("8");
        }

        //Creates a new file of the name newFileName in an object of ofstream class.
        ofstream oFileNew(newFileName.c_str());

        //Sorts the bar codes.
        sort(tmpCodeStore.begin(),tmpCodeStore.end());

        //Uploads the bar code numbers into the file
        for(size_t i=0;i<tmpCodeStore.size();i++)
        {
            oFileNew<<tmpCodeStore[i]<<endl;
        }

        //Closes the object,,
        oFileNew.close();

        //Creates a new file of the name fileList where list of the object is saved.
        ifstream ifileList("fileList.txt");

        //Creates  a vector of string.
        vector<string> fileNames;

        string str;

        //Takes in the fileList and stores in the vector.
        while(ifileList)
        {

        //Closes the object.

            getline(ifileList,str);
            fileNames.push_back(str);

        }
		ifileList.close();

		//Checks if the file is empty or not.
		if(fileNames.size()!=0)
		{
			bool fileExists=false;

            //Loads the file if it exists
			for(size_t i=0;i<fileNames.size()-1;i++)
			{
				if(fileNames[i]==newFileName)
				{
					fileExists=true;
				}
			}

			//For cases when file does not exist
			if(!fileExists)
			{
			    ofstream ofileList("fileList.txt",ios::app);

				//Upload the new file name into the vector
				ofileList<<newFileName<<endl;
				ofileList.close();
			}
		}

		//For cases when the file name is empty.
		else
		{
			ofstream ofileList("fileList.txt",ios::app);

			//Adds the new file to the vector.
			ofileList<<newFileName<<endl;
			ofileList.close();
		}

        //Gives output.
        cout<<endl<<endl<<"************************************************";
        cout<<endl<<endl<<"\tFile"<<newFileName<<" is saved."<<endl;
        cout<<"\t Number of total scanned barcodes is : "<<countScannedCodes;
        cout<<endl<<endl<<"************************************************"<<endl;

    }
    ///Operation 1 Ends here////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///Operation 2 Starts here  ///////////////////////////////////////
    if(operation==2)
    {
        //Explained in operation 1.
        vector<long long>tmpCodeStore;
        long long int previousCode= -1;

        ifstream fileList("fileList.txt");
        vector<string> fileNames;

        string str;

        //Adds the file from which the bar codes are to be read.
        while(fileList)
        {
            getline(fileList,str);

            //Stores the file list in the vector.
            fileNames.push_back(str);
        }

        //Closes the file list.
        fileList.close();

        //The minimum size of the files is checked
        if(fileNames.size()==1)
        {
            cout<<endl<<"\tNO file exits in record\n\tUse Operation 1 to create database"<<endl<<endl;

            //Transfer control to the label
            goto loadStart;
        }

        //Chooses files from which bar codes are to be read.
        cout<<"\t\tFollowing are the available files:"<<endl;
        short int file_no;

        //Displays the available file names
        for(size_t i=0;i<fileNames.size()-1;i++)
        {
            cout<<"\t\t"<<i+1<<" ) "<<fileNames[i]<<endl;
        }

        //Choice is accepted.
        cout<<"\t\tEnter your choice:"<<endl;
        cin>>file_no;

        //Stores the selected file name.
        string fileName;
        fileName=fileNames[file_no-1];

        cout<<"Press any character to start and (n) to go to home page:"<<endl;
        char start;
        cin>>start;

        if(start=='n')
        {
           goto loadStart;

        }

        int countScannedCodes =0;
        cout<<endl<<"\t\tNo.       Code";

        while(true)
        {
            send_command("8");

            if(!flag)
            {
                cout<<"\n\t\t REDIRECTING TO THE HOME PAGE";
                goto loadStart;
            }

            detect();

            cout<<"The barocde is detected"<<endl;

            send_command("8");
            send_command("7");

            short int barcode[12];
            decodeBarcode(barcode,12);

            long long int code=0;

            for(int i=0;i<12;i++)
            {
               code=10*code+barcode[i];
            }

            if(code!=previousCode)
            {
                previousCode=code;
                countScannedCodes++;
                tmpCodeStore.push_back(code);
                cout<<endl<<"\t\t"<<setw(2)<<countScannedCodes<<"  "<<setprecision(13)<<code;

            }
            //Starts the bot to receive data from the bot.
            send_command("8");

            //Receive data from the bot.
            int temp=receive_data();

            //Checks for stoppage of the bot.
            if(temp==52)
            {

                cout<<endl<<"Bot is stopped "<<endl;
                //Stops the bot.
                send_command("8");

                break;
            }

            //Stops the bot and the data inflow.
            send_command("8");
        }


        vector<long long> sorted = getSortedBarcodesFromFile(fileName);/// take fromFile
        vector<char> status(sorted.size(),'a');                          /// status vector

        string oFileExctraName = createOutputFileExctraName(fileName);
        ofstream oFileExctra(oFileExctraName.c_str());                   /// create or overWrite exctra file
        string oFileAbsentName = createOutputFileAbsentName(fileName);
        ofstream oFileAbsent(oFileAbsentName.c_str());                   /// /// create or overWrite exctra file

        sort(tmpCodeStore.begin(),tmpCodeStore.end());

        int sortedIndex=0;
        int exctraCount=0;
        int absentCount=0;
        for(size_t i=0;i<tmpCodeStore.size();i++)
        {
            while(true)
            {
                if(tmpCodeStore[i]==sorted[sortedIndex])
                {
                    status[sortedIndex]='p';
                    sortedIndex++;
                    break;
                }
                else if(tmpCodeStore[i]>sorted[sortedIndex])
                {
                    sortedIndex++;
                    continue;
                }
                else if(tmpCodeStore[i]<sorted[sortedIndex])
                {
                    oFileExctra<<sorted[sortedIndex];
                    exctraCount++;
                    break;
                }
            }
        }

        for(size_t i=0;i<status.size();i++)
        {
            if(status[i]=='a')
            {
                oFileAbsent<<sorted[i]<<endl;
                absentCount++;
            }
        }

        oFileAbsent.close();
        oFileExctra.close();

        cout<<endl<<endl<<"**************************************************************";
        cout<<endl<<endl<<"\tFile "<<fileName<<" is checked";
        cout<<endl<<"\t Result of scan is stored in files "<<oFileAbsentName<<" & "<<oFileExctraName;
        cout<<endl<<"\t Number of Absent Barcodes is : "<<absentCount;
        cout<<endl<<"\t Number of Extra Barcodes is : "<<exctraCount;
        cout<<endl<<endl<<"**************************************************************";
    }  /// old File Mode  Ends here

    ///Operation 2 Ends here/////////////////////////////////////////////////////

    ///Operation 3 Starts here/////////////////////////////////////////////////////
    if(operation==3)
    {
        cout<<"\t1:Check if your com port of xbee is COM6 or not from Device Manager"<<endl;
        cout<<"\t2:Check if bot is placed correctly on line"<<endl;
        cout<<"\t3:Check whether only one camera is mounted on computer\n\t(in case of multiple camera's read manual)"<<endl;
        cout<<"\t\t\t For Detail Help Please Refer User Manual"<<endl;
        cout<<"\n\n Press Key to Start Page or press 'e' to end"<<endl;

        char escape;
        cin>>escape;

        if(escape!='e')
        {
            goto loadStart;
        }
        else
        {
            cout<<"\t\tTHANK YOU FOR USING OUR PRODUCT"<<endl;
            //Closes the COM port.
            CloseHandle(hPort);

            return 0;
        }
    }
    ///Operation 3 Ends here////////////////////////////////////////////////////

    cout<<endl<<"\t\tDo you wish to continue \n\t\t PRESS ANY KEY TO QUIT and y to CONTINUE: "<<endl;
    cin>>Continue;

    if(Continue=='y')
    {
        Continue = ' ';
        goto loadStart;
    }

    cout<<"\t\tTHANK YOU FOR USING OUR PRODUCT"<<endl;

    //Closes the COM port
    CloseHandle(hPort);

    return 0;
}

///Main Program ends here////////////////////////////////////////////////////////////////////////////////////////////
