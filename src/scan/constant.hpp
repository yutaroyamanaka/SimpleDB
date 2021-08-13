/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <string>
#include <memory>
#include <functional>

namespace scan {
class Constant {
    friend bool operator==(const Constant& lhs, const Constant& rhs);
    friend bool operator!=(const Constant& lhs, const Constant& rhs);
    friend bool operator<(const Constant& lhs, const Constant& rhs);
    friend bool operator>(const Constant& lhs, const Constant& rhs);
    friend bool operator<=(const Constant& lhs, const Constant& rhs);
    friend bool operator>=(const Constant& lhs, const Constant& rhs);

 public:
    Constant();
    Constant(const Constant& val);
    Constant(int ival);
    Constant(const std::string& sval);
    Constant &operator=(const Constant& val);
    int asInt() const;
    std::string asString() const;
    int hashCode() const;
    std::string toString() const;
    bool isNull() const;
 private:
    std::unique_ptr<int> ival_;
    std::unique_ptr<std::string> sval_;
};
}  // namespace scan
