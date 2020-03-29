#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "DispeKommu.hpp"

using namespace std;

DispeKommu::DispeKommu()
{
    I2CFile_ = open("/dev/i2c-1", O_RDWR);
    if(I2CFile_ == -1) {
        cout << "Failed to open the I2C file. Exiting program" << endl; 
        exit(1);
    }
}

DispeKommu::~DispeKommu() {
    I2CFile_ = close(I2CFile_);
    if(I2CFile_ == -1) {
        cout << "Failed to close the I2C file. Exiting program" << endl; 
        exit(1);
    }
}

I2CData DispeKommu::readI2C(int length){
    char buf[1024];
    int bytes_read = read(I2CFile_,buf,length);
    string s(buf);
    data_.setData(s);
    
    if(bytes_read > 0) {
        data_.setErr(0);
        data_.setLength(bytes_read);
    } else {
        data_.setErr(bytes_read);
        data_.setLength(0);
    }

    return data_;
}

I2CData DispeKommu::getData() const{
    return data_;
}

char DispeKommu::writeI2C(int address, char cmd){
    int err = ioctl(I2CFile_, address, cmd); // Send command to address 
    if(err < 0) {
        cout << "Could not find i2c device" << endl;
        exit(1);
    }
    return err;
}