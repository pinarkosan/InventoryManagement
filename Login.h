#pragma once

#include "pch.h"
#include "Form1.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System::Windows::Forms;
using namespace System::Security::Cryptography;

public ref class Login : public System::Windows::Forms::Form {
private:
    TextBox^ txtUsername;
    TextBox^ txtPassword;
    Button^ btnLogin;
    Label^ lblUsername;
    Label^ lblPassword;
    Label^ lblError;
    Label^ lblTitle;

public:
    String^ getUsername() {
        return txtUsername->Text;
    }

    Login();
    void btnLogin_Click(System::Object^ sender, System::EventArgs^ e);
   
};
