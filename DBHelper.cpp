#include "pch.h"
#include "DBHelper.h"
#using <System.Configuration.dll>
using namespace System::Configuration;

namespace DAL {

    // Veri Tabanı Baglantisi
    SqlConnection^ DBHelper::GetConnection() {
        String^ connectionString = ConfigurationManager::ConnectionStrings["InventoryDBConnection"]->ConnectionString;
        return gcnew SqlConnection(connectionString);
    }

    // SQL Select
    DataTable^ DBHelper::ExecuteQuery(String^ query) {
        SqlConnection^ connection = GetConnection();
        SqlDataAdapter^ adapter = gcnew SqlDataAdapter(query, connection);
        DataTable^ table = gcnew DataTable();
        adapter->Fill(table);
        return table;
    }

    // SQL komut (INSERT, UPDATE, DELETE)
    int DBHelper::ExecuteNonQuery(String^ query) {
        SqlConnection^ connection = GetConnection();
        SqlCommand^ command = gcnew SqlCommand(query, connection);
        connection->Open();
        int result = command->ExecuteNonQuery();
        connection->Close();
        return result;
    }


    DataTable^ DBHelper::ExecuteStoredProcedure(String^ procedureName) {
        SqlConnection^ connection = GetConnection();
        SqlCommand^ command = gcnew SqlCommand(procedureName, connection);
        command->CommandType = CommandType::StoredProcedure;

        SqlDataAdapter^ adapter = gcnew SqlDataAdapter(command);
        DataTable^ table = gcnew DataTable();
        adapter->Fill(table);
        return table;
    }


    DataTable^ DBHelper::ExecuteStoredProcedure(String^ procedureName, array<SqlParameter^>^ parameters) {
        SqlConnection^ connection = GetConnection();
        SqlCommand^ command = gcnew SqlCommand(procedureName, connection);
        command->CommandType = CommandType::StoredProcedure;

        for each (SqlParameter ^ param in parameters) {
            command->Parameters->Add(param);
        }

        SqlDataAdapter^ adapter = gcnew SqlDataAdapter(command);
        DataTable^ table = gcnew DataTable();
        adapter->Fill(table);
        return table;
    }
}
