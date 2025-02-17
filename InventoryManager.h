#pragma once
#include "DBHelper.h"
#include "Item.h"
#include "Category.h"

namespace BLL {
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Data;
    using namespace Models;

    public ref class InventoryManager {
    public:
        static String^ role;
        static List<Item^>^ GetAllItems();
        static bool AddItem(String^ name, int categoryId, int quantity, double unitPrice);
        static bool UpdateItem(int itemId, String^ name, int categoryId, int quantity, double unitPrice);
        static bool DeleteItem(int itemId);

        static DataTable^ GetLowStockItems();

        static List<Category^>^ GetAllCategories(); 
        static bool AddCategory(String^ categoryName);
        static bool DeleteCategory(int categoryId);
        static bool UpdateCategory(int categoryId, String^ newName);
        static Tuple<bool, String^>^ AuthenticateUser(String^ username, String^ password);
        static String^ ComputeSHA256(String^ input);


    };
}
