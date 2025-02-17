#pragma once
using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;

namespace DAL {
    public ref class DBHelper {
    public:
        static SqlConnection^ GetConnection();
        static DataTable^ ExecuteQuery(String^ query);
        static int ExecuteNonQuery(String^ query);

        static DataTable^ ExecuteStoredProcedure(String^ procedureName);

        static DataTable^ ExecuteStoredProcedure(String^ procedureName, array<SqlParameter^>^ parameters);
    };
}
