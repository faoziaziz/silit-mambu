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
      con = driver->connect("tcp://127.0.0.1:3306", "tappingapk", "trumon123!");
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
    con = driver->connect("tcp://127.0.0.1:3306", "root", "trumon123");
    con->setSchema("Trumon");

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
      if((Get_Flag==0)&&((DeviceId=="SMT09160030")||(DeviceId=="SMT09160030"))){
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
        OCR_get=AccOCR();
        str_ocr_get=OCR_get;
        cout<<"STR ocr"<<str_ocr_get<<endl;
        delete[] OCR_get;
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

void gettingBlob(){

  /* Description of table Image and teks Description

  CREATE TABLE `Image` (
                `SeqNum` INT(11) NOT NULL AUTO_INCREMENT,
                `DeviceId` VARCHAR(45) NULL DEFAULT NULL,
                `RefSN` INT(11) NOT NULL,
                `Data` MEDIUMBLOB NULL,
                `Flag` TINYINT(4) NULL DEFAULT '0',
                `ProfileSN` INT(11) NULL DEFAULT NULL,
                PRIMARY KEY (`SeqNum`)
              )
  COMMENT='Menyimpan hasil image recon'
  COLLATE='latin1_swedish_ci'
  ENGINE=InnoDB
  AUTO_INCREMENT=19582;



  CREATE TABLE `testTeks` (
                  `SeqNum` INT(11) NOT NULL AUTO_INCREMENT,
                  `DeviceId` VARCHAR(45) NULL DEFAULT NULL,
                  `RefSN` INT(11) NOT NULL,
                  `Data` TEXT NULL,
                  `FileTimeStamp` DATETIME NOT NULL DEFAULT '0000-00-00 00:00:00',
                  `Flag` TINYINT(4) NULL DEFAULT '0',
                  `ProfileSN` INT(11) NULL DEFAULT '0',
                  `FlagParser` INT(11) NULL DEFAULT '0',
                  `FlagClean` INT(11) NULL DEFAULT NULL,
                  PRIMARY KEY (`SeqNum`)
              )
    COMMENT='Menyimpan hasil OCR'
    COLLATE='latin1_swedish_ci'
    ENGINE=InnoDB
    AUTO_INCREMENT=25437;

  */
  //string ukuran;

  /* next procedure will get blob from database ****************************/

  cout <<"getting blob executed "<<endl;
  try {
    /* SQL dependency needed to executeQuery ******************************/
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *prep_stmt;
    std::vector<char> blob_buffer(1000000, '\0');
    char *OCR_get;
    string str_ocr_get;
    /* Create a connection *************************************************/
    cout<<"executed var"<<endl;
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "tappingapk", "trumon123!");

    /* Connect to the MySQL Trumon database ********************************/
    con->setSchema("Trumon");
    cout<<"executed con"<<endl;
    prep_stmt = con->prepareStatement("SELECT * FROM Image;");
    //prep_stmt = con->prepareStatement("SELECT * FROM Image where `DeviceId`=? and `SeqNum`=?;");
    //prep_stmt->setString(1, "APK_CGK_0003");
    //prep_stmt->setInt(2, 27);

    /* Select from testBlob table ******************************************/
    cout<<"Road to execute query"<<endl;
    //prep_stmt->executeQuery();
    res = prep_stmt->executeQuery();

    while (res->next()) {

      if(res->getInt("RefSN")==27){
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
       int RefSN = res->getInt("RefSN");
       string DeviceId = res-> getString("DeviceId");
       int ProfileSN = res->getInt("ProfileSN");
       int Flag = res->getInt("Flag");

       cout<<"DeviceId : "<<DeviceId<<endl;
       cout<<"RefSN : "<<RefSN<<endl;
       cout<<"ending of getting blob"<<endl;
       //call ocr
       OCR_get=AccOCR();
       str_ocr_get=OCR_get;
       cout<<"STR ocr"<<str_ocr_get<<endl;
       delete[] OCR_get;

     }
       else{
         cout <<"nanti lah"<<endl;
       }
     }



    /*  releasing memory ****************************************************/
    delete prep_stmt;
    delete con;
    //delete stmt;
    delete res;

  }
  catch(sql::SQLException &e){
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "Something error"<< endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }
}


void insertBLOB(){
  /*
    The definition of this function is if you wanna to test insert this blob
    you can execute this function.

  */


  /* Variable declaration ***********************************************/
  streampos size;
  char * memblock;
  ifstream file("aziz.jpg", ios::in|ios::binary|ios::ate);
  ofstream myFile ("kalista.jpg", ios::out | ios::binary);
  size_t length_buff;


  try{

    /* SQL dependency needed to executeQuery ******************************/
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *prep_stmt;


    /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "tappingapk", "trumon123!");



    /* Connect to the MySQL Trumon database */
    con->setSchema("Trumon");

    stmt = con->createStatement();

   /* cek read and write file **********************************************/
   if(file.is_open()){



     /* This function will insert the Blob *********************************/
     size= file.tellg();
     memblock = new char[size];
     file.seekg(0, ios::beg);
     file.read(memblock, size);
     cout<<"Size"<<size<<endl;
     //instance lenth_buff to get lenght buffer
     length_buff = size;
     cout<<"length_buff keeksekusi"<<endl;

     membuf mbuf(memblock, length_buff);
     istream blob_file(&mbuf);

     file.close();
     cout<<"The entire file content is in memory"<<endl;

     prep_stmt = con->prepareStatement("INSERT INTO testBlob(id, blobField) VALUES (?, ?)");
     prep_stmt->setInt(1, 1);
     prep_stmt->setBlob(2, &blob_file);
     prep_stmt->executeUpdate();

     //release memory
     delete prep_stmt;
     delete con;
     delete stmt;
     delete[] memblock;

   }

   else {
     cout <<"Unable ot open file"<<endl;
   }

   cout << "ending of the day"<<endl;


    /* close some varieble to release connection and memory */


  }
  catch(sql::SQLException &e){
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "Something error"<< endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }



}

void testBuffer(){

  /*  input stream get from image table. */
  istream *is;
  unsigned char* ptr;
  size_t blobSize;

  /* output variable stream to write a file. */

  try{
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *prep_stmt;


    /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "tappingapk", "trumon123!");

    /* Connect to the MySQL Trumon database */
    con->setSchema("Trumon");



    /* Select in ascending order */
    prep_stmt = con->prepareStatement("SELECT id FROM test ORDER BY id ASC");
    res = prep_stmt->executeQuery();
    while (res->next()) {
     cout << "\t... MySQL replies: ";
     /* Access column data by alias or column name */
     printf("Reading from test table=(%d)\n", res->getInt(1));
   }

    /* close some varieble to release connection and memory */
    delete prep_stmt;
    delete res;
    delete con;
    delete stmt;

  }
  catch(sql::SQLException &e){
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "Something error"<< endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }

}

void getPicture(){
  /* This procedure will used to get file blob and write into file ******* */
  cout << endl;
  cout << "testing runing" << endl;

   try {

     /* Driver for declaration sql. ***************************************/
     sql::Driver *driver;
     sql::Connection *con;
     sql::Statement *stmt;
     sql::ResultSet *res;
     sql::PreparedStatement *prep_stmt;


     /* Create a connection */
     driver = get_driver_instance();
     con = driver->connect("tcp://127.0.0.1:3306", "tappingapk", "trumon123!");

     /* Connect to the MySQL test database */
     con->setSchema("Trumon");

     prep_stmt = con->prepareStatement("SELECT * FROM test;");
     res=prep_stmt->executeQuery();
     while (res->next()) {
      cout << "\t... MySQL replies: ";
      /* Access column data by alias or column name */
      printf("Reading from test table=(%d)\n", res->getInt(1));
    }

    /* c*/
    delete prep_stmt;
    delete res;
    delete con;

  } catch (sql::SQLException &e) {
    /*
      SQL error exceptions
    */
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "Something error"<< endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
   }

   cout << endl;

}

void writeText(){
  /* This procedure will usedToWrite to database.  **************************/
  cout << endl;
  cout << "from writeText() " << endl;

  try {
    /*
      SQL driver connection variable
    */
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    /* Create a connection **************************************************/
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "tappingapk", "trumon123!");

    /* Connect to the MySQL test database ***********************************/
    con->setSchema("Trumon");


    /* '?' is the supported placeholder syntax ******************************/
    pstmt = con->prepareStatement("INSERT INTO test(id) VALUES (?)");
    for (int i = 1; i <= 10; i++) {
      pstmt->setInt(1, i);
      pstmt->executeUpdate();
    }
    delete pstmt;

    /* Select in ascending order */
    pstmt = con->prepareStatement("SELECT id FROM test ORDER BY id ASC");
    res = pstmt->executeQuery();

    /* Fetch in reverse = descending order! */
    res->afterLast();
    while (res->previous())
        cout << "\t... MySQL counts: " << res->getInt("id") << endl;
      delete res;

      delete pstmt;
      delete con;

    } catch (sql::SQLException &e) {
      /*
        Error exception
      */
      cout << "# ERR: SQLException in " << __FILE__;
      //cout << "(" << __FUNCTION__ << ") on line " »
      // << __LINE__ << endl;
      cout << "# ERR: " << e.what();
      cout << " (MySQL error code: " << e.getErrorCode();
     // cout << ", SQLState: " << e.getSQLState() << »
       // " )" << endl;
     }

    cout << endl;

}
