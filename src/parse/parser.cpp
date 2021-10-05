/* Copyright 2021 Yutaro Yamanaka */
#include "parse/parser.hpp"

namespace parse {
  Parser::Parser(const std::string& s) {
    lex_ = std::make_unique<Lexer>(s);
  }

  std::string Parser::field() const {
    return lex_->eatId();
  }

  scan::Constant Parser::constant() const {
    if (lex_->matchStringConstant()) {
      scan::Constant c(lex_->eatStringConstant());
      return c;
    } else {
      scan::Constant c(lex_->eatIntConstant());
      return c;
    }
  }

  scan::Expression Parser::expression() const {
    if (lex_->matchId()) {
      scan::Expression e(field());
      return e;
    } else {
      scan::Expression e(constant());
      return e;
    }
  }

  scan::Term Parser::term() const {
    scan::Expression lhs = expression();
    lex_->eatDelim(Word::EQUAL_SIGN);
    scan::Expression rhs = expression();
    scan::Term t(lhs, rhs);
    return t;
  }

  scan::Predicate Parser::predicate() const {
    scan::Predicate pred(term());
    if (lex_->matchKeyword(Word::AMPERSAND)) {
      lex_->eatKeyword(Word::AMPERSAND);
      pred.conjoinWith(predicate());
    }
    return pred;
  }

  QueryData Parser::query() const {
    lex_->eatKeyword(Word::SELECT);
    std::vector<std::string> fields = selectList();

    lex_->eatKeyword(Word::FROM);
    TableData tableData = tableList();

    scan::Predicate pred;
    if (lex_->matchKeyword(Word::WHERE)) {
      lex_->eatKeyword(Word::WHERE);
      pred = predicate();
    }

    std::vector<std::string> groupFields = groupFieldList();
    std::vector<std::string> sortFields = sortFieldList();

    QueryData qrydata(fields, tableData.getTableNames(), pred, groupFields, sortFields);
    for (const auto& qd : tableData.getQueryDataList()) {
      qrydata.addQueryData(qd);
    }

    return qrydata;
  }

  std::vector<std::string> Parser::selectList() const {
    std::vector<std::string> L;
    L.emplace_back(field());
    if (lex_->matchDelm(Word::COMMA)) {
      lex_->eatDelim(Word::COMMA);
      for (const auto& sl : selectList()) {
        L.emplace_back(sl);
      }
    }
    return L;
  }

  TableData Parser::tableList() const {
    TableData td;
    if (lex_->matchDelm(Word::LEFT_PARENTHESIS)) {
      lex_->eatDelim(Word::LEFT_PARENTHESIS);
      QueryData qd = query();
      td.addQueryData(qd);
      lex_->eatDelim(Word::RIGHT_PARENTHESIS);
    } else {
      td.addTableName(lex_->eatId());
    }

    while (lex_->matchDelm(Word::COMMA)) {
      lex_->eatDelim(Word::COMMA);
      if (lex_->matchDelm(Word::LEFT_PARENTHESIS)) {
        lex_->eatDelim(Word::LEFT_PARENTHESIS);
        QueryData qd = query();
        td.addQueryData(qd);
        lex_->eatDelim(Word::RIGHT_PARENTHESIS);
      } else {
        td.addTableName(lex_->eatId());
      }
    }
    return td;
  }

  std::shared_ptr<Object> Parser::updateCmd() {
    if (lex_->matchKeyword(Word::INSERT)) {
      return std::static_pointer_cast<Object>(insert());
    } else if (lex_->matchKeyword(Word::DELETE)) {
      return std::static_pointer_cast<Object>(remove());
    } else if (lex_->matchKeyword(Word::UPDATE)) {
      return std::static_pointer_cast<Object>(modify());
    } else {
      return std::static_pointer_cast<Object>(create());
    }
  }

  std::shared_ptr<Object> Parser::create() {
    lex_->eatKeyword(Word::CREATE);
    if (lex_->matchKeyword(Word::TABLE)) {
      return std::static_pointer_cast<Object>(createTable());
    } else if (lex_->matchKeyword(Word::VIEW)) {
      return std::static_pointer_cast<Object>(createView());
    } else {
      return std::static_pointer_cast<Object>(createIndex());
    }
  }

  std::shared_ptr<DeleteData> Parser::remove() {
    lex_->eatKeyword(Word::DELETE);
    lex_->eatKeyword(Word::FROM);
    std::string tblname = lex_->eatId();
    scan::Predicate pred;
    if (lex_->matchKeyword(Word::WHERE)) {
      lex_->eatKeyword(Word::WHERE);
      pred = predicate();
    }
    return std::make_shared<DeleteData>(tblname, pred);
  }

  std::shared_ptr<InsertData> Parser::insert() {
    lex_->eatKeyword(Word::INSERT);
    lex_->eatKeyword(Word::INTO);
    std::string tblname = lex_->eatId();
    lex_->eatDelim(Word::LEFT_PARENTHESIS);
    std::vector<std::string> flds = fieldList();
    lex_->eatDelim(Word::RIGHT_PARENTHESIS);
    lex_->eatKeyword(Word::VALUES);
    lex_->eatDelim(Word::LEFT_PARENTHESIS);
    std::vector<scan::Constant> vals = constList();
    lex_->eatDelim(Word::RIGHT_PARENTHESIS);
    return std::make_shared<InsertData>(tblname, flds, vals);
  }

  std::vector<std::string> Parser::fieldList() const {
    std::vector<std::string> L;
    L.emplace_back(field());
    if (lex_->matchDelm(Word::COMMA)) {
      lex_->eatDelim(Word::COMMA);
      for (const auto& fld : fieldList()) {
        L.emplace_back(fld);
      }
    }
    return L;
  }

  std::vector<std::string> Parser::groupFieldList() const {
    std::vector<std::string> L;
    if (lex_->matchKeyword(Word::GROUP)) {
      lex_->eatKeyword(Word::GROUP);
      if (lex_->matchKeyword(Word::BY)) {
        lex_->eatKeyword(Word::BY);
        for (const auto& fld : fieldList()) {
          L.emplace_back(fld);
        }
      }
    }
    return L;
  }

  std::vector<std::string> Parser::sortFieldList() const {
    std::vector<std::string> L;
    if (lex_->matchKeyword(Word::ORDER)) {
      lex_->eatKeyword(Word::ORDER);
      if (lex_->matchKeyword(Word::BY)) {
        lex_->eatKeyword(Word::BY);
        for (const auto& fld : fieldList()) {
          L.emplace_back(fld);
        }
      }
    }
    return L;
  }

  std::vector<scan::Constant> Parser::constList() const {
    std::vector<scan::Constant> L;
    L.emplace_back(constant());
    if (lex_->matchDelm(Word::COMMA)) {
      lex_->eatDelim(Word::COMMA);
      for (const auto& cst : constList()) {
        L.emplace_back(cst);
      }
    }
    return L;
  }

  std::shared_ptr<ModifyData> Parser::modify() {
    lex_->eatKeyword(Word::UPDATE);
    std::string tblname = lex_->eatId();
    lex_->eatKeyword(Word::SET);
    std::string fldname = field();
    lex_->eatDelim(Word::EQUAL_SIGN);
    scan::Expression newval;
    scan::Predicate pred;
    if (lex_->matchKeyword(Word::WHERE)) {
      lex_->eatKeyword(Word::WHERE);
      pred = predicate();
    }
    return std::make_shared<ModifyData>(tblname, fldname, newval, pred);
  }

  std::shared_ptr<CreateTableData> Parser::createTable() {
    lex_->eatKeyword(Word::TABLE);
    std::string tblname = lex_->eatId();
    lex_->eatDelim(Word::LEFT_PARENTHESIS);
    record::Schema sch = fieldDefs();
    lex_->eatDelim(Word::RIGHT_PARENTHESIS);
    return std::make_shared<CreateTableData>(tblname, sch);
  }

  record::Schema Parser::fieldDefs() const {
    record::Schema schema = fieldDef();
    if (lex_->matchDelm(Word::COMMA)) {
      lex_->eatDelim(Word::COMMA);
      record::Schema schema2 = fieldDefs();
      schema.addAll(schema2);
    }
    return schema;
  }

  record::Schema Parser::fieldDef() const {
    std::string fldname = field();
    return fieldType(fldname);
  }

  record::Schema Parser::fieldType(const std::string& fldname) const {
    record::Schema schema;
    if (lex_->matchKeyword(Word::INT)) {
      lex_->eatKeyword(Word::INT);
      schema.addIntField(fldname);
    } else {
      lex_->eatKeyword(Word::VARCHAR);
      lex_->eatDelim(Word::LEFT_PARENTHESIS);
      int strlen = lex_->eatIntConstant();
      lex_->eatDelim(Word::RIGHT_PARENTHESIS);
      schema.addStringField(fldname, strlen);
    }
    return schema;
  }

  std::shared_ptr<CreateViewData> Parser::createView() {
    lex_->eatKeyword(Word::VIEW);
    std::string viewname = lex_->eatId();
    lex_->eatKeyword(Word::AS);
    QueryData qd = query();
    return std::make_shared<CreateViewData>(viewname, qd);
  }

  std::shared_ptr<CreateIndexData> Parser::createIndex() {
    lex_->eatKeyword(Word::INDEX);
    std::string idxname = lex_->eatId();
    lex_->eatKeyword(Word::ON);
    std::string tblname = lex_->eatId();
    lex_->eatDelim(Word::LEFT_PARENTHESIS);
    std::string fldname = field();
    lex_->eatDelim(Word::RIGHT_PARENTHESIS);
    return std::make_shared<CreateIndexData>(idxname, tblname, fldname);
  }
}  // namespace parse
