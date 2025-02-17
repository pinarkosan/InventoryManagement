#pragma once
#include "InventoryManager.h"

namespace WindowsFormsUI {
    using namespace System;
    using namespace System::Windows::Forms;

    public ref class Form1 : public Form {
    public:
        Form1(String^ username, String^ userRole) {
            this->currentUsername = username;
            this->userRole = userRole;
            InitializeComponent();
            LoadInventory(nullptr, nullptr);
            ApplyRoleBasedAccess();
        }
    protected:
        void OnFormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
            Application::Exit();
        }
    public:
       DataGridView^ dgvInventory;

    private:
        DataGridView^ dgvCategories;
        WindowsFormsUI::Button^ btnLoad;
        WindowsFormsUI::Button^ btnAddItem;
       
        WindowsFormsUI::Button^ btnAddCategory;
        WindowsFormsUI::Button^ btnExportToExcel;
        
        WindowsFormsUI::TextBox^ txtItemName;
        WindowsFormsUI::TextBox^ txtQuantity;
        WindowsFormsUI::TextBox^ txtUnitPrice;
        WindowsFormsUI::TextBox^ txtSearch;
        WindowsFormsUI::TextBox^ txtCategoryName;
        WindowsFormsUI::Label^ lblCategory;

        ComboBox^ cmbCategory;

        bool isInitializing = true;

        String^ currentUsername;
        String^ userRole;

        System::Windows::Forms::ContextMenuStrip^ contextMenu;
        System::Windows::Forms::ContextMenuStrip^ contextMenuInventory;
        Label^ lblUsername;
        
        void ShowContextMenu(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e);
        void ShowInventoryContextMenu(System::Object^ sender, DataGridViewCellMouseEventArgs^ e);
        void DeleteCategoryFromMenu(System::Object^ sender, System::EventArgs^ e);
        void UpdateCategoryFromMenu(System::Object^ sender, System::EventArgs^ e);

        void InitializeComponent(void);
        void LoadInventory(System::Object^ sender, System::EventArgs^ e);
        void SearchItems(System::Object^ sender, System::EventArgs^ e);
        void LoadCategories(System::Object^ sender, System::EventArgs^ e);
        void AddCategory(System::Object^ sender, System::EventArgs^ e);
        void SaveNewRow(System::Object^ sender, DataGridViewCellEventArgs^ e);
        void dgvInventory_DefaultValuesNeeded(System::Object^ sender, DataGridViewRowEventArgs^ e);
        void dgvInventory_DataError(System::Object^ sender, DataGridViewDataErrorEventArgs^ e);
        void DeleteItem(System::Object^ sender, System::EventArgs^ e);
        void UpdateItem(System::Object^ sender, System::EventArgs^ e);
        void AddNewItem(System::Object^ sender, System::EventArgs^ e);
        void btnExportToExcel_Click(System::Object^ sender, System::EventArgs^ e);

      
      
  

        void CategoryChanged(System::Object^ sender, DataGridViewCellEventArgs^ e);

        void ApplyRoleBasedAccess() {
            if (userRole == "Admin") {
              /*  MessageBox::Show(L"Welcome Admin!", L"Info", MessageBoxButtons::OK, MessageBoxIcon::Information);*/
            }
            else {
               /* MessageBox::Show(L"Welcome User!", L"Info", MessageBoxButtons::OK, MessageBoxIcon::Information);*/
                btnAddItem->Enabled = false;
                btnAddCategory->Enabled = false;
            }
        }

    };
}
