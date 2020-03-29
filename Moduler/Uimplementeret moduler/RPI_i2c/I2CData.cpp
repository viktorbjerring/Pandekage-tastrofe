#include "I2CData.hpp"
I2CData::I2CData(char err, int length, std::string data) : err_(err), length_(length), data_(data)
{
}

void I2CData::setErr(char err){
    err_ = err;
}
void I2CData::setLength(int length){
    length_ = length >= 0 ? length : -1;
}
void I2CData::setData(std::string data){
    data_ = data;
}
char I2CData::getErr() const {
    return err_;
}
int I2CData::getLength() const {
    return length_;
}

std::string I2CData::getData() const {
    return data_;
}