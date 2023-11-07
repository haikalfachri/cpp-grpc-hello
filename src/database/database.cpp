#include "database.h"
#include "../globals/globals.h"

using namespace std;

Database::Database()
    : db_connection("host=" + env_reader->get("DB_HOST") + " port=" + env_reader->get("DB_PORT") +
                    " dbname=" + env_reader->get("DB_NAME") + " user=" +
                    env_reader->get("DB_USER") + " password=" + env_reader->get("DB_PASSWORD")) {}

pqxx::connection &Database::get_connection() {
    return this->db_connection;
}

void Database::create_table() {
    try {
        pqxx::work txn{this->db_connection};
        txn.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id SERIAL PRIMARY KEY,
            name VARCHAR(50) NOT NULL,
            created_at TIMESTAMP NOT NULL DEFAULT NOW(),
            updated_at TIMESTAMP NOT NULL DEFAULT NOW()
        )
    )");
        cout << "Table created successfully or already created" << endl;
        txn.commit();
    } catch (const exception &e) {
        cerr << "Exception caught: " << e.what() << endl;
    }
}