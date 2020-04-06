#include <string>

class I2CData
{
public:
    I2CData(char err = 0, int length = 0, std::string data = "");
    void setErr(char err);
    void setLength(int length);
    void setData(std::string data);
    char getErr() const;
    int getLength() const;
    std::string getData() const;
private:
    char err_;
    int length_;
    std::string data_;
};



