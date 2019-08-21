/*
  Authors : Aziz Amerul Faozi
*/
#include "main.hpp"

int main(void)
{

  /* Stack variable*/
  string status_license;
  string cpu_id="kalistaumari";
  string freelicense="freeprasimax";
  /* Some procedure calling **************************************/
  cout<<"tester execured "<<endl;

  /* get license file and try to decrypt */
  status_license = license();

  if(status_license==cpu_id){
    /*  Normal License executed */
    cout <<"executed"<<endl;
  }
  else if(status_license==freelicense)
  {
    /* Free license executed */
    cout<<"free license executed ()()====>"<<endl;
    writeTexttoTable();
  }
  else{
    cout<<"thereis no license in this device "<<endl;
    cout<<"(.)(.)======>"<<endl;

  }

  //writeTexttoTable(); -> for get execute update apk image translator

  /* Return to exit process **************************************/
  return EXIT_SUCCESS;
}
