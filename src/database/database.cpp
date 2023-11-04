#include "database.h"

Database::Database() : db_connection("host=localhost port=5433 dbname=cpp-grpc-crud user=postgres password=postgres") {
    std::string url = "host=localhost port=5433 dbname=cpp-grpc-crud user=postgres password=postgres";
    this->db_connection = pqxx::connection(url);
}

pqxx::connection& Database::get_connection() {
    return this->db_connection;
}

void Database::create_table() {
    pqxx::work txn{this->db_connection};
    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id SERIAL PRIMARY KEY,
            name VARCHAR(50) NOT NULL,
            created_at TIMESTAMP NOT NULL DEFAULT NOW(),
            updated_at TIMESTAMP NOT NULL DEFAULT NOW()
        )
    )");
    cout << "Table created successfully" << endl;
    txn.commit();
}