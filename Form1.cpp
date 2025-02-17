#include "pch.h"
#include "Form1.h"


using namespace System;
using namespace System::IO;
using namespace System::Data;
using namespace System::Data::SqlClient;


using namespace WindowsFormsUI;
using namespace Microsoft::VisualBasic;
using namespace BLL;

void Form1::InitializeComponent(void) {
    this->Text = L"Inventory Management";
    this->Size = System::Drawing::Size(1000, 600);

    dgvInventory = gcnew DataGridView();
    dgvCategories = gcnew DataGridView();

    btnAddCategory = gcnew WindowsFormsUI::Button();
    txtSearch = gcnew WindowsFormsUI::TextBox();
    txtCategoryName = gcnew WindowsFormsUI::TextBox();
    lblCategory = gcnew WindowsFormsUI::Label();
    contextMenu = gcnew System::Windows::Forms::ContextMenuStrip();
    contextMenuInventory = gcnew System::Windows::Forms::ContextMenuStrip();
    btnExportToExcel = gcnew System::Windows::Forms::Button();
    lblUsername = gcnew Label();

    ToolStripMenuItem^ updateItemMenuItem = gcnew ToolStripMenuItem("Update");
    ToolStripMenuItem^ deleteItemMenuItem = gcnew ToolStripMenuItem("Delete");

    updateItemMenuItem->Click += gcnew EventHandler(this, &Form1::UpdateItem);
    deleteItemMenuItem->Click += gcnew EventHandler(this, &Form1::DeleteItem);

    contextMenuInventory->Items->Add(updateItemMenuItem);
    contextMenuInventory->Items->Add(deleteItemMenuItem);
  
    lblUsername->Location = System::Drawing::Point(640, 30);
    lblUsername->Text = "Welcome,  " + currentUsername; 
    lblUsername->Size = System::Drawing::Size(200, 40);
    lblUsername->AutoSize = true;
    lblUsername->ForeColor = System::Drawing::Color::Black;
    lblUsername->Font = gcnew System::Drawing::Font(L"Arial", 14, System::Drawing::FontStyle::Bold);
    this->Controls->Add(lblUsername);

    txtSearch->Location = System::Drawing::Point(20, 20);
    txtSearch->Size = System::Drawing::Size(200, 50);
    txtSearch->TextChanged += gcnew EventHandler(this, &Form1::SearchItems);

    dgvInventory->Location = System::Drawing::Point(20, 70);
    dgvInventory->Size = System::Drawing::Size(500, 200);
    dgvInventory->BackgroundColor = System::Drawing::Color::White;
    dgvInventory->DefaultCellStyle->ForeColor = System::Drawing::Color::Black;
    dgvInventory->DefaultCellStyle->SelectionBackColor = System::Drawing::Color::LightSkyBlue;
    dgvInventory->DefaultCellStyle->SelectionForeColor = System::Drawing::Color::Black;
    dgvInventory->ColumnHeadersDefaultCellStyle->BackColor = System::Drawing::Color::DodgerBlue;
    dgvInventory->ColumnHeadersDefaultCellStyle->ForeColor = System::Drawing::Color::White;
    dgvInventory->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
    dgvInventory->ColumnHeadersDefaultCellStyle->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    dgvInventory->RowHeadersVisible = false;
 
    dgvInventory->ContextMenuStrip = contextMenuInventory;
    dgvInventory->CellMouseClick += gcnew DataGridViewCellMouseEventHandler(this, &Form1::ShowInventoryContextMenu);

    dgvInventory->CellEndEdit += gcnew DataGridViewCellEventHandler(this, &Form1::CategoryChanged);
    dgvInventory->RowValidated += gcnew DataGridViewCellEventHandler(this, &Form1::SaveNewRow);


    dgvInventory->DataError += gcnew DataGridViewDataErrorEventHandler(this, &Form1::dgvInventory_DataError);

    dgvInventory->DefaultValuesNeeded += gcnew DataGridViewRowEventHandler(this, &Form1::dgvInventory_DefaultValuesNeeded);
    dgvInventory->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
    dgvInventory->MultiSelect = false;
    dgvInventory->AllowUserToAddRows = false;

    btnExportToExcel->Text = "Excel'e Aktar";
    btnExportToExcel->Location = System::Drawing::Point(300, 300);
    btnExportToExcel->Size = System::Drawing::Size(150, 40);

    btnExportToExcel->Click += gcnew System::EventHandler(this, &Form1::btnExportToExcel_Click);
    btnExportToExcel->BackColor = System::Drawing::Color::Green;
    btnExportToExcel->ForeColor = System::Drawing::Color::White;
    // 📌 Butonu Forma Ekle
    Controls->Add(this->btnExportToExcel);

    DataGridViewComboBoxColumn^ cmbCategoryColumn = gcnew DataGridViewComboBoxColumn();
    cmbCategoryColumn->HeaderText = L"Category";
    cmbCategoryColumn->Name = L"CategoryNameComboBox";
    cmbCategoryColumn->DataPropertyName = "CategoryID";
    cmbCategoryColumn->DisplayMember = "CategoryName";
    cmbCategoryColumn->ValueMember = "CategoryID";

    dgvCategories->Location = System::Drawing::Point(640, 70);
    dgvCategories->Size = System::Drawing::Size(250, 200);
    dgvCategories->BackgroundColor = System::Drawing::Color::White;
    dgvCategories->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
    dgvCategories->RowHeadersVisible = false;
    dgvCategories->CellMouseClick += gcnew DataGridViewCellMouseEventHandler(this, &Form1::ShowContextMenu);
   
    lblCategory->Text = L"Category Name:";
    lblCategory->Location = System::Drawing::Point(640, 280);
    lblCategory->Size = System::Drawing::Size(100, 20);

    txtCategoryName->Location = System::Drawing::Point(640, 300);
    txtCategoryName->Size = System::Drawing::Size(180, 30);

    btnAddItem = gcnew WindowsFormsUI::Button();
    btnAddItem->Text = L"Ekle";
    btnAddItem->Location = System::Drawing::Point(20, 300);
    btnAddItem->Size = System::Drawing::Size(150, 40);
    btnAddItem->BackColor = System::Drawing::Color::Blue;
    btnAddItem->ForeColor = System::Drawing::Color::White;
    btnAddItem->Click += gcnew EventHandler(this, &Form1::AddNewItem);
    this->Controls->Add(btnAddItem);

    btnAddCategory->Text = L"Add Category";
    btnAddCategory->Location = System::Drawing::Point(640, 340);
    btnAddCategory->Size = System::Drawing::Size(180, 30);
    btnAddCategory->BackColor = System::Drawing::Color::Green;
    btnAddCategory->ForeColor = System::Drawing::Color::White;
    btnAddCategory->Click += gcnew EventHandler(this, &Form1::AddCategory);

    ToolStripMenuItem^ deleteMenuItem = gcnew ToolStripMenuItem("Sil");
    contextMenu->Items->Add(deleteMenuItem);
    deleteMenuItem->Click += gcnew EventHandler(this, &Form1::DeleteCategoryFromMenu);

    ToolStripMenuItem^ updateMenuItem = gcnew ToolStripMenuItem("Güncelle");
    contextMenu->Items->Add(updateMenuItem);
    updateMenuItem->Click += gcnew EventHandler(this, &Form1::UpdateCategoryFromMenu);

    List<Category^>^ categories = InventoryManager::GetAllCategories();
    cmbCategoryColumn->DataSource = categories;
    dgvInventory->Columns->Add(cmbCategoryColumn);
   
    if (dgvInventory->Columns->Contains("CategoryID")) {
        dgvInventory->Columns->Remove("CategoryID");
    }

    this->Controls->Add(txtSearch);
    this->Controls->Add(dgvInventory);
    this->Controls->Add(dgvCategories);
    this->Controls->Add(lblCategory);
    this->Controls->Add(txtCategoryName);
    this->Controls->Add(btnAddCategory);
  

    LoadCategories(nullptr, nullptr);
}



void Form1::btnExportToExcel_Click(System::Object^ sender, System::EventArgs^ e) {
    try {
        DataTable^ dt = InventoryManager::GetLowStockItems();

        if (dt->Rows->Count == 0) {
            MessageBox::Show("No low stock items found!", "Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }

        String^ filePath = "C:\\Users\\Public\\LowStockItems.csv";
        StreamWriter^ writer = gcnew StreamWriter(filePath);

        for (int i = 0; i < dt->Columns->Count; i++) {
            writer->Write(dt->Columns[i]->ColumnName);
            if (i < dt->Columns->Count - 1) writer->Write(",");
        }
        writer->WriteLine();

        for (int i = 0; i < dt->Rows->Count; i++) {
            for (int j = 0; j < dt->Columns->Count; j++) {
                writer->Write(dt->Rows[i][j]->ToString());
                if (j < dt->Columns->Count - 1) writer->Write(",");
            }
            writer->WriteLine();
        }

        writer->Close();

        System::Diagnostics::Process::Start("excel.exe", filePath);

        MessageBox::Show("Low stock items have been successfully saved in Excel-compatible CSV format!\File: " + filePath,
            "Başarı", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
    catch (Exception^ ex) {
        MessageBox::Show("Error: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}




void Form1::LoadInventory(System::Object^ sender, System::EventArgs^ e) {
    dgvInventory->DataSource = nullptr;
    List<Item^>^ items = InventoryManager::GetAllItems();

    if (items->Count == 0) {
        MessageBox::Show(L"There are no items in the inventory yet!", L"Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    List<Category^>^ categories = InventoryManager::GetAllCategories();
    if (categories->Count > 0) {
        for each (Item ^ item in items) {
            if (item->CategoryID <= 0) {
                item->CategoryID = categories[0]->CategoryID;
            }
        }
    }

    System::ComponentModel::BindingList<Item^>^ bindingList = gcnew System::ComponentModel::BindingList<Item^>(items);
    dgvInventory->DataSource = bindingList;

    if (dgvInventory->Columns->Contains("ItemID")) {
        dgvInventory->Columns["ItemID"]->Visible = false;
    }
    if (dgvInventory->Columns->Contains("CategoryID")) {
        dgvInventory->Columns["CategoryID"]->Visible = false;
    }

    if (!dgvInventory->Columns->Contains("CategoryNameComboBox")) {
        DataGridViewComboBoxColumn^ cmbCategoryColumn = gcnew DataGridViewComboBoxColumn();
        cmbCategoryColumn->HeaderText = L"Category";
        cmbCategoryColumn->Name = L"CategoryNameComboBox";
        cmbCategoryColumn->DataPropertyName = "CategoryID";
        cmbCategoryColumn->DisplayMember = "CategoryName";
        cmbCategoryColumn->ValueMember = "CategoryID";
        cmbCategoryColumn->DataSource = categories;
        dgvInventory->Columns->Add(cmbCategoryColumn);
    }

    // 📌 **Sütun Sırasını Manuel Olarak Ayarla**
    dgvInventory->Columns["ItemName"]->DisplayIndex = 0;
    dgvInventory->Columns["UnitPrice"]->DisplayIndex = 1;
    dgvInventory->Columns["Quantity"]->DisplayIndex = 2;
    dgvInventory->Columns["CategoryNameComboBox"]->DisplayIndex = 3;

    dgvInventory->AutoResizeColumns();
}

void Form1::AddNewItem(System::Object^ sender, System::EventArgs^ e) {

    List<Category^>^ categories = InventoryManager::GetAllCategories();
    
    try {
        if (categories->Count == 0) {
            MessageBox::Show(L"Select Category!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        String^ itemName = Microsoft::VisualBasic::Interaction::InputBox(
            L"Item Name:", L"Add Item", L"", -1, -1);
        if (String::IsNullOrEmpty(itemName)) return;

        String^ priceStr = Microsoft::VisualBasic::Interaction::InputBox(
            L"Birim Fiyat:", L"Add Item", L"0", -1, -1);
        double unitPrice = Convert::ToDouble(priceStr);

        String^ quantityStr = Microsoft::VisualBasic::Interaction::InputBox(
            L"Quantity:", L"Add Item", L"1", -1, -1);
        int quantity = Convert::ToInt32(quantityStr);

        System::Windows::Forms::Form^ categoryForm = gcnew System::Windows::Forms::Form();
        categoryForm->Text = L"Select Category";
        categoryForm->Size = System::Drawing::Size(300, 150);

        System::Windows::Forms::ComboBox^ cmbCategory = gcnew System::Windows::Forms::ComboBox();
        cmbCategory->Location = System::Drawing::Point(20, 20);
        cmbCategory->Size = System::Drawing::Size(200, 30);
        cmbCategory->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList; // 📌 Elle giriş engellendi
        cmbCategory->DataSource = categories;
        cmbCategory->DisplayMember = "CategoryName";
        cmbCategory->ValueMember = "CategoryID";

        System::Windows::Forms::Button^ btnOk = gcnew System::Windows::Forms::Button();
        btnOk->Text = L"Ok";
        btnOk->Location = System::Drawing::Point(100, 60);
        btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;

        categoryForm->Controls->Add(cmbCategory);
        categoryForm->Controls->Add(btnOk);
        categoryForm->AcceptButton = btnOk;

        if (categoryForm->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

        int categoryId = Convert::ToInt32(cmbCategory->SelectedValue);

        bool success = InventoryManager::AddItem(itemName, categoryId, quantity, unitPrice);

        if (success) {
            MessageBox::Show(L"The product has been successfully added!", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            LoadInventory(nullptr, nullptr);
        }
        else {
            MessageBox::Show(L"An error occurred while adding the item!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
    catch (Exception^ ex) {
        MessageBox::Show(L"Error: " + ex->Message, L"Exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}



void Form1::DeleteItem(System::Object^ sender, System::EventArgs^ e) {
    if (dgvInventory->CurrentRow == nullptr) {
        MessageBox::Show(L"Please select a product to delete!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

    DataGridViewRow^ row = dgvInventory->CurrentRow;
    int itemId = Convert::ToInt32(row->Cells["ItemID"]->Value);

    System::Windows::Forms::DialogResult result = MessageBox::Show(
        L"Are you sure you want to delete this product?",
        L"Delete Item", MessageBoxButtons::YesNo, MessageBoxIcon::Warning);

    if (result == System::Windows::Forms::DialogResult::Yes) {
        bool success = InventoryManager::DeleteItem(itemId);
        if (success) {
            MessageBox::Show(L"The item has been successfully deleted!", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            LoadInventory(nullptr, nullptr);
        }
        else {
            MessageBox::Show(L"The item could not be deleted!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}


void Form1::UpdateItem(System::Object^ sender, System::EventArgs^ e) {
    if (dgvInventory->SelectedRows->Count == 0) return;

    DataGridViewRow^ row = dgvInventory->SelectedRows[0];

    if (row->Cells["ItemID"]->Value == nullptr) {
        MessageBox::Show(L"Please select a product to update!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

    int itemId = Convert::ToInt32(row->Cells["ItemID"]->Value);
    String^ itemName = row->Cells["ItemName"]->Value->ToString();
    double unitPrice = Convert::ToDouble(row->Cells["UnitPrice"]->Value);
    int quantity = Convert::ToInt32(row->Cells["Quantity"]->Value);
    int categoryId = Convert::ToInt32(row->Cells["CategoryNameComboBox"]->Value);

    // 📌 Veritabanını güncelle
    bool success = InventoryManager::UpdateItem(itemId, itemName, categoryId, quantity, unitPrice);

    if (success) {
        MessageBox::Show(L"The product has been successfully updated!", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
        LoadInventory(nullptr, nullptr);
    }
    else {
        MessageBox::Show(L"An error occurred while updating the product!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}



void Form1::SaveNewRow(System::Object^ sender, DataGridViewCellEventArgs^ e) {
    try {
        if (e->RowIndex < 0 || e->RowIndex >= dgvInventory->Rows->Count) return;

        DataGridViewRow^ row = dgvInventory->Rows[e->RowIndex];

        if (row->Cells["ItemName"]->Value == nullptr ||
            row->Cells["CategoryNameComboBox"]->Value == nullptr ||
            row->Cells["Quantity"]->Value == nullptr ||
            row->Cells["UnitPrice"]->Value == nullptr) {
            return; 
        }

if (row->Cells["ItemID"]->Value != nullptr && row->Cells["ItemID"]->Value->ToString() != "") {
    return;
}

        String^ itemName = row->Cells["ItemName"]->Value->ToString();
        int categoryId = Convert::ToInt32(row->Cells["CategoryNameComboBox"]->Value);
        int quantity = Convert::ToInt32(row->Cells["Quantity"]->Value);
        double unitPrice = Convert::ToDouble(row->Cells["UnitPrice"]->Value);

        // 📌 Yeni ürünü ekle
        bool addSuccess = InventoryManager::AddItem(itemName, categoryId, quantity, unitPrice);
        if (addSuccess) {
            MessageBox::Show(L"Ürün başarıyla eklendi!", L"Başarılı", MessageBoxButtons::OK, MessageBoxIcon::Information);
            LoadInventory(nullptr, nullptr);
        }
        else {
            MessageBox::Show(L"An error occurred while adding the product! Please check the database.", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }

    }
    catch (Exception^ ex) {
        MessageBox::Show(L"Error: " + ex->Message, L"Exception", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}


void Form1::dgvInventory_DefaultValuesNeeded(System::Object^ sender, DataGridViewRowEventArgs^ e) {
    List<Category^>^ categories = InventoryManager::GetAllCategories();
    if (categories->Count > 0) {
        e->Row->Cells["CategoryNameComboBox"]->Value = categories[0]->CategoryID;
    }
    e->Row->Cells["UnitPrice"]->Value = 0.00;
    e->Row->Cells["Quantity"]->Value = 1;
}

void Form1::LoadCategories(System::Object^ sender, System::EventArgs^ e) {
    dgvCategories->DataSource = nullptr; 
    List<Category^>^ categories = InventoryManager::GetAllCategories();
    dgvCategories->DataSource = categories;
    dgvCategories->AutoResizeColumns();

    DataGridViewComboBoxColumn^ cmbCategoryColumn = dynamic_cast<DataGridViewComboBoxColumn^>(dgvInventory->Columns["CategoryNameComboBox"]);
    if (cmbCategoryColumn != nullptr) {
        cmbCategoryColumn->DataSource = categories;
        cmbCategoryColumn->DisplayMember = "CategoryName";
        cmbCategoryColumn->ValueMember = "CategoryID";
    }
}
void Form1::dgvInventory_DataError(System::Object^ sender, DataGridViewDataErrorEventArgs^ e) {
    MessageBox::Show(L"Category matching error: " + e->Exception->Message, L"Error",
        MessageBoxButtons::OK, MessageBoxIcon::Error);
    e->ThrowException = false;
}

void Form1::AddCategory(System::Object^ sender, System::EventArgs^ e) {
    String^ newCategory = txtCategoryName->Text;

    if (!String::IsNullOrEmpty(newCategory)) {
        bool success = InventoryManager::AddCategory(newCategory);

        if (success) {
            LoadCategories(sender, e);
            LoadInventory(sender, e);

            DataGridViewComboBoxColumn^ cmbCategoryColumn = dynamic_cast<DataGridViewComboBoxColumn^>(dgvInventory->Columns["CategoryNameComboBox"]);
            if (cmbCategoryColumn != nullptr) {
                cmbCategoryColumn->DataSource = InventoryManager::GetAllCategories();
            }

            dgvInventory->Columns["ItemName"]->DisplayIndex = 0;
            dgvInventory->Columns["UnitPrice"]->DisplayIndex = 1;
            dgvInventory->Columns["Quantity"]->DisplayIndex = 2;
            dgvInventory->Columns["CategoryNameComboBox"]->DisplayIndex = 3;

            MessageBox::Show(L"Category added successfully!", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        else {
            MessageBox::Show(L"Failed to add category!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }

        txtCategoryName->Text = ""; 
    }
}



void Form1::DeleteCategoryFromMenu(System::Object^ sender, System::EventArgs^ e) {
    if (dgvCategories->SelectedRows->Count > 0) {
        int categoryId = Convert::ToInt32(dgvCategories->SelectedRows[0]->Cells["CategoryID"]->Value);
        String^ categoryName = dgvCategories->SelectedRows[0]->Cells["CategoryName"]->Value->ToString();

        System::Windows::Forms::DialogResult result = MessageBox::Show(
            L"Are you sure you want to delete the category '" + categoryName + L"'?",
            L"Delete Confirmation",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Warning
        );

        if (result == System::Windows::Forms::DialogResult::Yes) {
            bool success = InventoryManager::DeleteCategory(categoryId);

            if (success) {
                MessageBox::Show(L"Category deleted successfully.", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
                LoadCategories(sender, e);
            }
            else {
                MessageBox::Show(L"Failed to delete category. It may have linked items.", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    }
    else {
        MessageBox::Show(L"Please select a category to delete.", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void Form1::UpdateCategoryFromMenu(System::Object^ sender, System::EventArgs^ e) {
    if (dgvCategories->SelectedRows->Count > 0) {
        int categoryId = Convert::ToInt32(dgvCategories->SelectedRows[0]->Cells["CategoryID"]->Value);
        String^ currentName = dgvCategories->SelectedRows[0]->Cells["CategoryName"]->Value->ToString();
        String^ newName = Microsoft::VisualBasic::Interaction::InputBox(
            L"Enter new category name:", L"Update Category", currentName, -1, -1);

        if (String::IsNullOrEmpty(newName) || newName == currentName) return;

        bool success = InventoryManager::UpdateCategory(categoryId, newName);

        if (success) {
            MessageBox::Show(L"Category updated successfully.", L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            LoadCategories(sender, e);
        }
        else {
            MessageBox::Show(L"Failed to update category.", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}

void Form1::ShowContextMenu(System::Object^ sender, DataGridViewCellMouseEventArgs^ e) {
    if (e->Button == System::Windows::Forms::MouseButtons::Right && e->RowIndex >= 0) {
        dgvCategories->ClearSelection();
        dgvCategories->Rows[e->RowIndex]->Selected = true;

        dgvCategories->CurrentCell = dgvCategories->Rows[e->RowIndex]->Cells[0];

        contextMenu->Show(dgvCategories, dgvCategories->PointToClient(System::Windows::Forms::Cursor::Position));
    }
}

void Form1::ShowInventoryContextMenu(System::Object^ sender, DataGridViewCellMouseEventArgs^ e) {
    if (e->Button == System::Windows::Forms::MouseButtons::Right && e->RowIndex >= 0) {
        dgvInventory->ClearSelection();
        dgvInventory->Rows[e->RowIndex]->Selected = true;
        dgvInventory->CurrentCell = dgvInventory->Rows[e->RowIndex]->Cells[0];

        contextMenu->Show(dgvInventory, dgvInventory->PointToClient(System::Windows::Forms::Cursor::Position));
    }
}




void Form1::CategoryChanged(System::Object^ sender, DataGridViewCellEventArgs^ e) {
    if (e->RowIndex < 0 || e->ColumnIndex < 0) return;

    if (dgvInventory->Columns[e->ColumnIndex]->Name == "CategoryNameComboBox") {
        String^ selectedCategory = dgvInventory->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value->ToString();
        List<Category^>^ categories = InventoryManager::GetAllCategories();

        if (categories->Count == 0) {
            MessageBox::Show(L"Please add the category first!", L"Hata", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        if (String::IsNullOrEmpty(selectedCategory)) {
            MessageBox::Show(L"Please select a category first!", L"Hata", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        for each (Category ^ cat in categories) {
            if (cat->CategoryName == selectedCategory) {
                if (dgvInventory->Columns->Contains("CategoryID")) {
                    dgvInventory->Rows[e->RowIndex]->Cells["CategoryID"]->Value = cat->CategoryID;
                }
                else {
                    MessageBox::Show(L"Error: CategoryID column not found!", L"Error",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
                break;
            }
        }
    }
}


void Form1::SearchItems(System::Object^ sender, System::EventArgs^ e) {
    if (dgvInventory->RowCount == 0) return;
    System::String^ searchText = txtSearch->Text->ToLower();

    for (int i = 0; i < dgvInventory->Rows->Count; i++) {
        DataGridViewRow^ row = dgvInventory->Rows[i];
        if (row == nullptr || row->Cells == nullptr) continue;

        System::String^ itemName = "";
        System::String^ unitPrice = "";
        System::String^ quantity = "";
        System::String^ categoryName = "";


        if (row->Cells["ItemName"]->Value != nullptr)
            itemName = row->Cells["ItemName"]->Value->ToString()->ToLower();

        if (row->Cells["UnitPrice"]->Value != nullptr) {
            try {
                unitPrice = row->Cells["UnitPrice"]->Value->ToString();
            }
            catch (...) {
                unitPrice = "";
            }
        }

        if (row->Cells["Quantity"]->Value != nullptr) {
            try {
                quantity = row->Cells["Quantity"]->Value->ToString();
            }
            catch (...) {
                quantity = "";
            }
        }

        if (row->Cells["CategoryNameComboBox"]->Value != nullptr)
            categoryName = row->Cells["CategoryNameComboBox"]->FormattedValue->ToString()->ToLower();

        bool isVisible = (itemName->IndexOf(searchText, StringComparison::OrdinalIgnoreCase) >= 0 ||
            unitPrice->IndexOf(searchText, StringComparison::OrdinalIgnoreCase) >= 0 ||
            quantity->IndexOf(searchText, StringComparison::OrdinalIgnoreCase) >= 0 ||
            categoryName->IndexOf(searchText, StringComparison::OrdinalIgnoreCase) >= 0);
      
        if (isVisible) {
            row->Height = 25; 
        }
        else {
            row->Height = 0; 
        }
    }
}


