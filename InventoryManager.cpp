#include "pch.h"
#include "InventoryManager.h"
#include "Item.h"
#include "Category.h"
#include "DBHelper.h"
#include "Login.h"


using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace Models;
using namespace System::Security::Cryptography;

namespace BLL {
    List<Item^>^ InventoryManager::GetAllItems() {
        List<Item^>^ items = gcnew List<Item^>();
        DataTable^ table = DAL::DBHelper::ExecuteStoredProcedure("sp_GetAllItems");

        for each (DataRow ^ row in table->Rows) {
            Item^ item = gcnew Item();
            item->ItemID = Convert::ToInt32(row["ItemID"]);
            item->ItemName = row["ItemName"]->ToString();
            item->CategoryID = Convert::ToInt32(row["CategoryID"]);
            item->CategoryName = row["CategoryName"]->ToString();
            item->Quantity = Convert::ToInt32(row["Quantity"]);
            item->UnitPrice = Convert::ToDouble(row["UnitPrice"]);
            items->Add(item);
        }
        return items;
    }


   //Add Item
    bool InventoryManager::AddItem(String^ name, int categoryId, int quantity, double unitPrice) {
        bool success = false;

        try {
            SqlConnection^ conn = DAL::DBHelper::GetConnection();
            conn->Open();

            SqlTransaction^ transaction = conn->BeginTransaction();

            SqlCommand^ cmd = gcnew SqlCommand("sp_AddItem", conn, transaction);
            cmd->CommandType = CommandType::StoredProcedure;

            cmd->Parameters->AddWithValue("@ItemName", name);
            cmd->Parameters->AddWithValue("@CategoryID", categoryId);
            cmd->Parameters->AddWithValue("@Quantity", quantity);
            cmd->Parameters->AddWithValue("@UnitPrice", unitPrice);

            int rowsAffected = cmd->ExecuteNonQuery();

            if (rowsAffected > 0) {
                transaction->Commit();
                success = true;
            }
            else {
                transaction->Rollback();
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show("An error occurred while adding the product: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }

        return success;
    }


    //Update Item
    bool InventoryManager::UpdateItem(int itemId, String^ name, int categoryId, int quantity, double unitPrice) {
        try {
            array<SqlParameter^>^ parameters = {
                gcnew SqlParameter("@ItemID", itemId),
                gcnew SqlParameter("@ItemName", name),
                gcnew SqlParameter("@CategoryID", categoryId),
                gcnew SqlParameter("@Quantity", quantity),
                gcnew SqlParameter("@UnitPrice", unitPrice)
            };

            DataTable^ result = DAL::DBHelper::ExecuteStoredProcedure("sp_UpdateItem", parameters);

            if (result->Rows->Count > 0) {
                return true;
            }
            else {
                MessageBox::Show(L"Update işlemi sırasında hata oluştu! Veritabanını kontrol edin.",
                    L"Hata", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return false;
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show(L"SQL Hatası: " + ex->Message, L"Hata", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return false;
        }
    }


    //Delete Item
    bool InventoryManager::DeleteItem(int itemId) {
        array<SqlParameter^>^ parameters = { gcnew SqlParameter("@ItemID", itemId) };
        return DAL::DBHelper::ExecuteStoredProcedure("sp_DeleteItem", parameters)->Rows->Count > 0;
    }
    Tuple<bool, String^>^ InventoryManager::AuthenticateUser(String^ username, String^ password) {
        bool isAuthenticated = false;
        try {
            SqlConnection^ conn = DAL::DBHelper::GetConnection();
            conn->Open();

            SqlCommand^ cmd = gcnew SqlCommand("sp_AuthenticateUser", conn);
            cmd->CommandType = CommandType::StoredProcedure;

            cmd->Parameters->AddWithValue("@Username", username);
            cmd->Parameters->AddWithValue("@Password", ComputeSHA256(password));

            Object^ result = cmd->ExecuteScalar();
            if (result != nullptr) {
                isAuthenticated=true;
                role = result->ToString();
            }

            conn->Close();

            isAuthenticated = (result != nullptr);
        }
        catch (Exception^ ex) {
            MessageBox::Show("Login Error: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }

        return gcnew Tuple<bool, String^>(isAuthenticated, role);
    }
   
    String^ InventoryManager::ComputeSHA256(String^ input) {
        array<Byte>^ inputBytes = System::Text::Encoding::UTF8->GetBytes(input);
        SHA256^ sha256 = gcnew SHA256Managed();
        array<Byte>^ hashBytes = sha256->ComputeHash(inputBytes);
        return BitConverter::ToString(hashBytes)->Replace("-", "");
    }

    // Düşük Stoklu Ürünler
    DataTable^ InventoryManager::GetLowStockItems() {
        return DAL::DBHelper::ExecuteStoredProcedure("sp_GetLowStockItems");
    }

    // Tüm Kategoriler
    List<Category^>^ InventoryManager::GetAllCategories() {
        List<Category^>^ categories = gcnew List<Category^>();
        DataTable^ table = DAL::DBHelper::ExecuteStoredProcedure("sp_GetAllCategories");

        Console::WriteLine("SQL Rows Retrieved: " + table->Rows->Count);

        for each (DataRow ^ row in table->Rows) {
            Category^ category = gcnew Category();
            category->CategoryID = Convert::ToInt32(row["CategoryID"]);
            category->CategoryName = row["CategoryName"]->ToString();
            categories->Add(category);
        }
        return categories;
    }


    bool InventoryManager::AddCategory(String^ categoryName) {
        try {
            SqlParameter^ outputParam = gcnew SqlParameter("@NewCategoryID", SqlDbType::Int);
            outputParam->Direction = ParameterDirection::Output;

            array<SqlParameter^>^ parameters = {
                gcnew SqlParameter("@CategoryName", categoryName),
                outputParam 
            };

            DataTable^ result = DAL::DBHelper::ExecuteStoredProcedure("sp_AddCategory", parameters);

            if (outputParam->Value != nullptr) {
                int newCategoryId = Convert::ToInt32(outputParam->Value);
                Console::WriteLine(L"New category has been added! ID: " + newCategoryId);
                return true;
            }

            return false;
        }
        catch (Exception^ ex) {
            MessageBox::Show(L"Hata: " + ex->Message, L"Exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
            Console::WriteLine(L"[ERROR] AddCategory: " + ex->Message);
            return false;
        }
    }

    bool InventoryManager::DeleteCategory(int categoryId) {
        try {
          
            array<SqlParameter^>^ parameters = { gcnew SqlParameter("@CategoryID", categoryId) };

            DataTable^ result = DAL::DBHelper::ExecuteStoredProcedure("sp_DeleteCategory", parameters);

          
            if (result->Rows->Count > 0) {
                return true;
            }
            else {
               
                MessageBox::Show(L"Failed to delete category. It may have linked items or does not exist.",
                    L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return false;
            }
        }
        catch (SqlException^ ex) {
          
            if (ex->Number == 547) {
                MessageBox::Show(L"Cannot delete category. It is linked to items.",
                    L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            else {
                MessageBox::Show(L"Database error: " + ex->Message,
                    L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            return false;
        }
        catch (Exception^ ex) {
            MessageBox::Show(L"An unexpected error occurred: " + ex->Message,
                L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return false;
        }
    }

    bool InventoryManager::UpdateCategory(int categoryId, String^ newName) {
        array<SqlParameter^>^ parameters = {
            gcnew SqlParameter("@CategoryID", categoryId),
            gcnew SqlParameter("@NewCategoryName", newName)
        };

        return DAL::DBHelper::ExecuteStoredProcedure("sp_UpdateCategory", parameters)->Rows->Count > 0;
    }


}
