# This is Boiler plate to use OCR

First you should install to development

```sh
g++ -o myprogram myprogram.cpp -llept -ltesseract
g++ -o myprogram myprogram.cpp -I/home/faoziaziz/local/include/tesseract -L/home/faoziaziz/local/lib -llept -ltesseract

```


# ocr-cross


## To Install  MySQLconn

This installation is used to acces mysql

```sh
git clone https://github.com/mysql/mysql-connector-cpp
git checkout 8.0
cd mysql-connector-cpp
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=${HOME}/.local
```

## Install Boost

The installation of boost is require to execute mysql_connection.h so u must install this.

```sh
wget https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.tar.gz
tar xvzf boost
cd boost
./bootstrap.sh
./b2 install --prefix=${HOME}/.local

```
# Notification

This work on 64 bit linux environtment because of mysqlclient.

change to cpp mode

## There maybe some of Troble in setInt

There many trouble in setInt so I contain in this directory to include some

```sh
export PKG_CONFIG_PATH=$HOME/.local/lib/pkgconfig
LIBLEPT_HEADERSDIR=$HOME/.local/include ./configure --prefix=$HOME/.local/ --with-extra-libraries=$HOME/.local/lib
```

# Refference
1. [Blob](https://stackoverflow.com/questions/15771880/mysql-c-connector-crashes-executing-insert-with-blob)

runtime and library
# silit-mambu
