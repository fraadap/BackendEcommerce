#include "database.h"
#include "con2db/pgsql.h"

Database::Database(Con2DB *con) {

    this->conn = con;
}