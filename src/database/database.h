#pragma once

#include <iostream>
#include <pqxx/pqxx>



class Database {
  public:
    pqxx::connection db_connection;

    Database();
    pqxx::connection& get_connection();
    void create_table();
};

