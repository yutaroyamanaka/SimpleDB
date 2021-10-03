/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "parse/createindexdata.hpp"
#include "parse/createtabledata.hpp"
#include "parse/createviewdata.hpp"
#include "parse/deletedata.hpp"
#include "parse/insertdata.hpp"
#include "parse/lexer.hpp"
#include "parse/modifydata.hpp"
#include "parse/object.hpp"
#include "parse/querydata.hpp"
#include "parse/tabledata.hpp"
#include "parse/word.hpp"
#include "scan/constant.hpp"
#include "scan/expression.hpp"
#include "scan/predicate.hpp"
#include "scan/term.hpp"
#include "record/schema.hpp"

namespace parse {
class Parser {
 public:
    Parser(const std::string& s);
    std::string field() const;
    scan::Constant constant() const;
    scan::Expression expression() const;
    scan::Term term() const;
    scan::Predicate predicate() const;
    QueryData query() const;
    std::shared_ptr<Object> updateCmd();
    std::shared_ptr<Object> create();
    // Methods for parsing delete commands
    std::shared_ptr<DeleteData> remove();
    // Methods for parsing insert commands;
    std::shared_ptr<InsertData> insert();
    // Methods for parsing modify commands;
    std::shared_ptr<ModifyData> modify();
    // Methods for parsing create table commands;
    std::shared_ptr<CreateTableData> createTable();
    // Methods for parsing create view commands;
    std::shared_ptr<CreateViewData> createView();
    // Methods for parsing create index commands;
    std::shared_ptr<CreateIndexData> createIndex();

 private:
    std::unique_ptr<Lexer> lex_;
    // Methods for parsing queries
    std::vector<std::string> selectList() const;
    //std::set<std::string> tableList() const;
    TableData tableList() const;

    std::vector<std::string> fieldList() const;
    std::vector<scan::Constant> constList() const;
    std::vector<std::string> sortFieldList() const;

    record::Schema fieldDefs() const;
    record::Schema fieldDef() const;
    record::Schema fieldType(const std::string& fldname) const;
};
}  // namespace parse
