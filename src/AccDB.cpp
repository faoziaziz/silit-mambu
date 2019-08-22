#include "main.hpp"

struct membuf : streambuf {
    membuf(char* base, std::size_t n) {
        this->setg(base, base, base + n);
    }
};
void restore_flag_OCR(){
    /*
      0 : not yet executed with ocr
      1 : have executed ocr
      4 : written to text with debug mode
      99 : something error
    */

    int RefSNImage;
    try{
      /* Sql driver to check driver statements *****************************/
      sql::Driver *driver;
      sql::Connection *con;
      sql::Statement *stmt;
      sql::ResultSet *res;
      sql::PreparedStatement *prep_stmt, *update_stmt;
      /**********************************************************************/

      cout<<"Write to text table "<<endl;
      driver = get_driver_instance();
      con = driver->connect("tcp://127.0.0.1:3306", "root", "trumon1234!@#$");
      con->setSchema("Trumon");

      /***********************************************************************/
      /* Select in ascending order ********************************************/
      prep_stmt = con->prepareStatement("SELECT * FROM Image;");
      res = prep_stmt->executeQuery();

      while(res->next()){
        RefSNImage=res->getInt("RefSN");
        cout<<"REFSN Image : "<<RefSNImage<<endl;
        update_stmt = con->prepareStatement("UPDATE `Image` SET `Flag` = ? WHERE RefSN=?;");
        update_stmt->setInt(1, 0);
        update_stmt->setInt(2, RefSNImage);
        update_stmt->executeUpdate();

      }


      /* Clean all memory ***************************************************/
      //delete driver
      delete con;
      delete stmt;
      delete res;
      delete prep_stmt;
      delete update_stmt;

    }
    catch(sql::SQLException &e){
      /* SQL Exceptions *****************************************************/
      cout << "# ERR: SQLException in " << __FILE__;
      cout << "Something error"<< endl;
      cout << "# ERR: " << e.what();
      cout << " (MySQL error code: " << e.getErrorCode();
      cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }


}

void flag_ocr(){
  /*  Setting flag to OCR update flag ocr*/
  int flag;



}

void writeTexttoTable(){
  /* Procedure to send text to the table. */
  int Get_Flag;
  int RefSNImage;
  string DevieId;

  cout<<"write Text To table"<<endl;
  try{
    /* Sql driver to check driver statements *****************************/
    sql::Driver *driver;
    sql::Connection *con;
    //sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *prep_stmt, *update_stmt, *insert_stmt;
    std::vector<char> blob_buffer(1000000, '\0');
    char *OCR_get;
    string str_ocr_get;
    /**********************************************************************/

    cout<<"Write to text table "<<endl;
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "trumon1234!@#$");
    con->setSchema("trumon");

    /***********************************************************************/
    /* Select in ascending order ********************************************/
    /***********************************************************************/
    /* Select in ascending order ********************************************/
    prep_stmt = con->prepareStatement("SELECT * FROM Image;");
    res = prep_stmt->executeQuery();

    while(res->next()){
      string DeviceId = res->getString("DeviceId");
      Get_Flag=res->getInt("Flag");
      RefSNImage=res->getInt("RefSN");
      if((Get_Flag==0)){
        /* RefSN is some getting on line. ************************************/
        cout<<"REFSN Image : "<<RefSNImage<<endl;
        /* Get blob image *****************************************************/
        cout<<"entering getting blob ______"<<endl;
        streampos size_get;
        /* get image data ***************************************************/
        istream *blob_get = res->getBlob("Data");
        //size_t size = res->getInt("octet_length(blobField)");

        /* Write data to file ************************************************/
        ofstream outFile ("outfile_temp.png", ios::out|ios::binary);
        blob_get -> read(&blob_buffer[0], blob_buffer.size());
        //int length = sizeof((char *)&blob_get[0])/sizeof(char);
        outFile.write(&blob_buffer[0], MAX_BUFFER);
        //cout<<"length : "<<length<<endl;
        outFile.close();

        /* SeqNum should be RefSN ********************************************/
    //    int RefSN = res->getInt("RefSN");
        //string DeviceId = res-> getString("DeviceId");
        int ProfileSN = res->getInt("ProfileSN");
        int Flag = res->getInt("Flag");

        cout<<"DeviceId : "<<DeviceId<<endl;
        cout<<"RefSN : "<<RefSNImage<<endl;
        cout<<"ending of getting blob"<<endl;
        /* Translated using OCR************************************************/
        //OCR_get=AccOCR();
        str_ocr_get=AccOCR();
        cout<<"STR ocr"<<str_ocr_get<<endl;
       
        /* Delete File ********************************************************/
        if( remove( "outfile_temp.png" ) != 0 )
          perror( "Error deleting file" );
        else
          puts( "File successfully deleted" );

        /* Insert to text table ***********************************************/


        insert_stmt = con->prepareStatement("INSERT INTO Teks(DeviceId, RefSN, Data, Flag) VALUES (?, ?, ?, ?)");
        insert_stmt->setString(1, DeviceId);
        insert_stmt->setInt(2, RefSNImage);
        insert_stmt->setString(3, str_ocr_get);
        /* 4 in flag statement indicate this data still protototyping **********/
        insert_stmt->setInt(4, 4);
        insert_stmt->executeUpdate();
        /*Update flag coloumn on RefSN Image *************************************/
        update_stmt = con->prepareStatement("UPDATE `Image` SET `Flag` = ? WHERE RefSN=?;");
        update_stmt->setInt(1, 1);
        update_stmt->setInt(2, RefSNImage);
        update_stmt->executeUpdate();

        delete insert_stmt; //ada
        delete update_stmt; //ada
      }
      else {
        cout<<"RefSNImage : "<<RefSNImage<<endl;
        cout<<"Has been translated or something error."<<endl;
      }


    }



    /* Clean all memory ***************************************************/
    //delete driver
    delete con; //ada
    //delete stmt;
    delete res; //ada
    delete prep_stmt; //ada

    cout<<"Kalista ()()==>"<<endl;
  }
  catch(sql::SQLException &e){
    /* SQL Exceptions *****************************************************/
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "Something error"<< endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }
}
