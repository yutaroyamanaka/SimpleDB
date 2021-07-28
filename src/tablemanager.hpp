#pragma once
#include "schema.hpp"
#include "layout.hpp"
#include "transaction.hpp"
#include <vector>
#include <map>
#include <string>
#include <memory>

namespace meta {
  class TableManager {
    public:
      TableManager(bool isNew, tx::Transaction* transaction);
      void createTable(const std::string& tblname, const record::Schema& schema, tx::Transaction* transaction);
      record::Layout getLayout(const std::string& tblname, tx::Transaction* transaction);
      const static int MAX_NAME = 16;
    private:
      std::unique_ptr<record::Layout> tcat_layout_, fcat_layout_;
  };
}
