/* Copyright 2021 Yutaro Yamanaka */
#include <string>
#include <vector>
#include "parse/object.hpp"
#include "scan/constant.hpp"

namespace parse {
class InsertData : public Object {
 public:
    InsertData(const std::string& tblname, const std::vector<std::string>& flds,
        const std::vector<scan::Constant>& vals);
    std::string tableName() const;
    std::vector<std::string> fields() const;
    std::vector<scan::Constant> vals() const;
 private:
    std::string tblname_;
    std::vector<std::string> flds_;
    std::vector<scan::Constant> vals_;
};
}  // namespace parse
