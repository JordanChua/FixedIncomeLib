#include <iostream>
#include <string>
#include <algorithm>  // std::transform
#include <cctype>     // std::toupper

class ModelType {
private:
    std::string valuestr_;

public:
    enum modeltypes {
        YIELD_CURVE = 1,
        IR_SABR = 2,
    };
    
    ModelType(const std::string& modeltype) : valuestr_(modeltype) {
        std::string upperModelType = modeltype;
        std::transform(upperModelType.begin(), upperModelType.end(),
                       upperModelType.begin(),
                       [](unsigned char c){ return static_cast<char>(std::toupper(c)); });

        if (upperModelType == "YIELD_CURVE") {
            value_ = YIELD_CURVE;
        } else if (upperModelType == "IR_SABR") {
            value_ = IR_SABR;
        } else {
            throw std::runtime_error("Model type " + modeltype + " is not supported.");
        }
    }

    const std::string& valueStr() const { return valuestr_; }
};

class 
