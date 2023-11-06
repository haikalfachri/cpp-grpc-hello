#include "database.h"

#include "../globals/globals.h"

Database::Database()
    : db_connection("host=" + env_reader->get("DB_HOST") + " port=" + env_reader->get("DB_PORT") +
                    " dbname=" + env_reader->get("DB_NAME") + " user=" +
                    env_reader->get("DB_USER") + " password=" + env_reader->get("DB_PASSWORD")) {
}

pqxx::connection &Database::get_connection() {
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