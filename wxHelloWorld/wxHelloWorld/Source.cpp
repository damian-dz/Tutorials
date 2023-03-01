#include <wx/wx.h>

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title)
        : wxFrame(nullptr, wxID_ANY, title)
    {

    }
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        MyFrame* frame = new MyFrame("Hello, World!");
        return frame->Show();
    }
};

wxIMPLEMENT_APP(MyApp);
