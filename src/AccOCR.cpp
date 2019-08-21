#include "main.hpp"

char* AccOCR(){

    /*
      String out text variable tesseract.
    */
    string outText;
    string imPath = "outfile_temp.png";
    Mat om;


    // Create Tesseract object
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

    // Initialize tesseract to use English (eng) and the LSTM OCR engine.
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);

    // Set Page segmentation mode to PSM_AUTO (3)
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);

    // Open input image using OpenCV

    cout<<"before resize "<<endl;
    Mat im = imread(imPath, IMREAD_COLOR);

    cout <<"setelah interpolasi"<<endl;


    // Resize of image file
    resize(im, om, Size(im.cols*3, im.rows*3), 0, 0, INTER_AREA);

    // adaptive threshold
    //adaptiveThreshold(im, thress, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 5);

    // Set image data
    ocr->SetImage(om.data, om.cols, om.rows, 3, om.step);

    // Run Tesseract OCR on image
    outText = string(ocr->GetUTF8Text());

    // print recognized text
    cout<<"(.)(.)====>>"<<endl;
    cout << outText << endl;
    return outText;
    delete [] outText;

}
