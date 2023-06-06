#ifndef MULTITASK_H
#define MULTITASK_H

#include "tables.h"

using namespace std;

struct token_s{

    enum class type_e {TABLE, OPERATOR, LEFT_BKT, RIGHT_BKT, UNKNOWN};
    
    enum class associative_e {LEFT, RIGHT, UNKNOWN};

    token_s();

    token_s(const string& new_name, const type_e& new_type, const associative_e& new_associative, const size_t& new_priority);
    
    string name;    // natural token name from input
    type_e type;    // token type
    associative_e associative;  // associativity of the token - direction of execution of the token
    size_t priority;    // the precedence of executing this token compared to other tokens
};

class multitask_ra_c{
public:

    multitask_ra_c();

    bool solve(const string& table_dst_name, const string& request, tables_c& tables);

private:

    shared_ptr<table_c> create_tmp_table(const string& table_src_name, tables_c& tables);

    shared_ptr<table_c> cross_tmp_tables(shared_ptr<table_c> left_table, shared_ptr<table_c> right_table);

    shared_ptr<table_c> intersect_tmp_tables(shared_ptr<table_c> left_table, shared_ptr<table_c> right_table);

    shared_ptr<table_c> except_tmp_tables(shared_ptr<table_c> left_table, shared_ptr<table_c> right_table);

    shared_ptr<table_c> union_tmp_tables(shared_ptr<table_c> left_table, shared_ptr<table_c> right_table);

    bool find_tokens(const string& request, tables_c& tables);

    bool make_queue();

    deque<token_s> tokens;
    deque<token_s> tokens_queue;
    vector<shared_ptr<table_c>> tables_stack;
    size_t id_tables_stack;
};

class multitask_sql_c{
public:

    multitask_sql_c();

    bool solve(const string& table_dst_name, const string& request);

private:

    bool find_tokens(const string& request);

    bool make_queue();

    deque<token_s> tokens;
    deque<token_s> tokens_queue;
    vector<string> tables_stack;
};






#endif /* MULTITASK_H */