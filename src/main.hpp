#ifndef main_H
#define main_H

/* Include Header **************************/

/* Standard Library*/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <typeinfo>

/* Binary manipulating headers */
#include <istream>
#include <streambuf>

/* Mysql Library*/
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

/* for suppporting OCR*/
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

/* for suppeorting license*/
#include<math.h>
#include <string.h>
//#include <fstream>

/* For opencv */
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

/* Just space for namespace. ***************/


using namespace std;
using namespace cv;
/* Some procedure will be executed. ********/


void AccDB();
void writeText();
void getPicture();
void writeData();
void insertBLOB();
void gettingBlob();
void restore_flag_OCR();
void writeTexttoTable();

/* Some function to support *****************/
int getStatus(int refsn_image, int refsn_text);
char* AccOCR();
//for license function
string license();


/* Some constant to support ****************/
//const char* file_name = "temporaly.png";
#define MAX_BUFFER  1000000


#endif
