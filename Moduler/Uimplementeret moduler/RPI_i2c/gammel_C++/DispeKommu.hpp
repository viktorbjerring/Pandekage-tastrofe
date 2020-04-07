#include "I2CData.hpp"
class DispeKommu
{

public:
    DispeKommu();
    ~DispeKommu();
    I2CData readI2C(int addr);
    char writeI2C(int address, char cmd);
    I2CData getData() const;
private:
    int I2CFile_;
    I2CData data_;
};
