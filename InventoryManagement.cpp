#include "pch.h"  
#include "Form1.h"
#include "Login.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<System::String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Login^ loginForm = gcnew Login();
    Application::Run(loginForm);

 
    if (loginForm->DialogResult == System::Windows::Forms::DialogResult::OK) {
        WindowsFormsUI::Form1^ mainForm = gcnew WindowsFormsUI::Form1(loginForm->getUsername(),"User");
        Application::Run(mainForm);
    }

    return 0;
}
