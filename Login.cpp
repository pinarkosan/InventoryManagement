#include "pch.h"
#include "Login.h"
#include "InventoryManager.h"

Login::Login() {
    this->Text = L"User Login";
    this->Size = System::Drawing::Size(500, 400);
    this->MinimumSize = System::Drawing::Size(500, 400);
    this->FormBorderStyle = WindowsFormsUI::FormBorderStyle::FixedDialog;
    this->MaximizeBox = false;
    this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
    this->AutoSize = true;
    this->BackColor = System::Drawing::Color::FromArgb(96, 151, 64);

    lblTitle = gcnew Label();
    lblTitle->Text = L"Welcome Back!";
    lblTitle->Font = gcnew System::Drawing::Font("Arial", 24, System::Drawing::FontStyle::Bold);
    lblTitle->ForeColor = System::Drawing::Color::FromArgb(255, 208, 38, 67);
    lblTitle->Location = System::Drawing::Point(120, 30);
    lblTitle->AutoSize = true;
    this->Controls->Add(lblTitle);

    lblUsername = gcnew Label();
    lblUsername->Text = L"Username:";
    lblUsername->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    lblUsername->Location = System::Drawing::Point(50, 100);
    lblUsername->Size = System::Drawing::Size(80, 30);
    lblUsername->ForeColor = System::Drawing::Color::White;
    this->Controls->Add(lblUsername);

    txtUsername = gcnew TextBox();
    txtUsername->Location = System::Drawing::Point(150, 100);
    txtUsername->Size = System::Drawing::Size(200, 30); 
    txtUsername->Font = gcnew System::Drawing::Font("Arial", 12);
    txtUsername->BackColor = System::Drawing::Color::White;
    txtUsername->BorderStyle = BorderStyle::FixedSingle;
    txtUsername->ForeColor = System::Drawing::Color::Gray;
    this->Controls->Add(txtUsername);

    lblPassword = gcnew Label();
    lblPassword->Text = L"Password:";
    lblPassword->Font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);
    lblPassword->Location = System::Drawing::Point(50, 150);
    lblPassword->Size = System::Drawing::Size(80, 30);
    lblPassword->ForeColor = System::Drawing::Color::White;
    this->Controls->Add(lblPassword);

    txtPassword = gcnew TextBox();
    txtPassword->Location = System::Drawing::Point(150, 150);
    txtPassword->Size = System::Drawing::Size(200, 30);
    txtPassword->PasswordChar = '*';
    txtPassword->Font = gcnew System::Drawing::Font("Arial", 12);
    txtPassword->BackColor = System::Drawing::Color::White;
    txtPassword->BorderStyle = BorderStyle::FixedSingle;
    txtPassword->ForeColor = System::Drawing::Color::Gray;
    this->Controls->Add(txtPassword);

    btnLogin = gcnew Button();
    btnLogin->Text = L"Login";
    btnLogin->Location = System::Drawing::Point(150, 200);
    btnLogin->Size = System::Drawing::Size(200, 40);
    btnLogin->Font = gcnew System::Drawing::Font("Arial", 14, System::Drawing::FontStyle::Bold);
    btnLogin->BackColor = System::Drawing::Color::FromArgb(34, 89, 134);
    btnLogin->ForeColor = System::Drawing::Color::White;
    btnLogin->FlatStyle = FlatStyle::Flat;
    btnLogin->Click += gcnew EventHandler(this, &Login::btnLogin_Click);
    this->Controls->Add(btnLogin);

    lblError = gcnew Label();
    lblError->Text = L"Invalid username or password!";
    lblError->Font = gcnew System::Drawing::Font("Arial", 10);
    lblError->ForeColor = System::Drawing::Color::Red;
    lblError->Location = System::Drawing::Point(50, 240);
    lblError->AutoSize = true;
    lblError->Visible = false;
    this->Controls->Add(lblError);
}


void Login::btnLogin_Click(System::Object^ sender, System::EventArgs^ e) {
    String^ username = txtUsername->Text;
    String^ password = txtPassword->Text;
    Tuple<bool, String^>^ authResult = BLL::InventoryManager::AuthenticateUser(username, password);
    if (authResult->Item1) {
       /* MessageBox::Show(L"Login Successful! You will be redirected to the Inventory page shortly.!", L"Successful", MessageBoxButtons::OK, MessageBoxIcon::Information);*/
        this->Hide();

        WindowsFormsUI::Form1^ mainForm = gcnew WindowsFormsUI::Form1(username, authResult->Item2);
        mainForm->ShowDialog();
        Application::Exit();
    }
    else {
        MessageBox::Show(L"Invalid username or password!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}





