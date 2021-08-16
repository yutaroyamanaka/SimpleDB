/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <set>
#include <string>
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
    Object updateCmd() const;
    Object create() const;
    // Methods for parsing delete commands
    DeleteData remove() const;
    // Methods for parsing insert commands;
    InsertData insert() const;
    // Methods for parsing modify commands;
    ModifyData modify() const;
    // Methods for parsing create table commands;
    CreateTableData createTable() const;
    // Methods for parsing create view commands;
    CreateViewData createView() const;
    // Methods for parsing create index commands;
    CreateIndexData createIndex() const;

 private:
    std::unique_ptr<Lexer> lex_;
    // Methods for parsing queries
    std::vector<std::string> selectList() const;
    std::set<std::string> tableList() const;

    std::vector<std::string> fieldList() const;
    std::vector<scan::Constant> constList() const;

    record::Schema fieldDefs() const;
    record::Schema fieldDef() const;
    record::Schema fieldType(const std::string& fldname) const;

    /*
    const char EQUAL_SIGN = '=';
    const char COMMA = ',';
    const char LEFT_PARENTHESIS = '(';
    const char RIGHT_PARENTHESIS = ')';
    const std::string AMPERSAND = "and";
    const std::string SELECT = "select";
    const std::string FROM = "from";
    const std::string WHERE = "where";
    const std::string INSERT = "insert";
    const std::string DELETE = "delete";
    const std::string UPDATE = "update";
    const std::string TABLE = "table";
    const std::string VIEW = "view";
    const std::string INDEX = "index";
    const std::string CREATE = "create";
    const std::string SET = "set";
    const std::string AS = "as";
    const std::string ON = "on";
    const std::string INTO = "into";
    const std::string INT = "int";
    const std::string VARCHAR = "varchar";
    const std::string VALUES = "values";*/
};
}  // namespace parse
