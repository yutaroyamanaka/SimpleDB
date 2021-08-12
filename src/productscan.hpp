/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include "constant.hpp"
#include "updatescan.hpp"
#include "scan.hpp"
#include "predicate.hpp"
#include <string>
#include <memory>
#include <vector>

namespace scan {
  class ProductScan: public Scan {
    public:
      ProductScan(std::shared_ptr<Scan>& s1, std::shared_ptr<Scan>& s2);
      void beforeFirst() override;
      bool next() override;
      int getInt(const std::string& fldname) override;
      std::string getString(const std::string& fldname) override;
      Constant getVal(const std::string& fldname) override;
      bool hasField(const std::string& fldname) override;
      void close() override;
    private:
      std::shared_ptr<Scan> s1_;
      std::shared_ptr<Scan> s2_;
  };
}
