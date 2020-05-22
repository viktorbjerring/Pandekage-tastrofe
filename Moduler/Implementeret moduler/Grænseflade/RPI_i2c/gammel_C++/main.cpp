#include "DispeKommu.hpp"
#include <iostream>
#include <unistd.h>

int main(){
    DispeKommu test;
    test.writeI2C(0x0703, 0x48); //  Send command 0x0703 to address 0x48 (LM75)
    sleep(1);
    I2CData temp = test.readI2C(2);
    std::cout << temp.getData() << std::endl;
}