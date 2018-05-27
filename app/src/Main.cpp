/*
 Copyright (C) 2010-2017 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Main.h"
#include "Version.h"

#include "ccall.h"

IMPLEMENT_APP(TrenchBroom::View::TrenchBroomApp)

// testadd(a,b) = ccall( (:testadd, "G:\\TrenchBroom\\build3\\Debug\\TrenchBroom.dll"), Int32, (Int32,Int32), a, b )
CCALL int testadd(int a, int b) {
	return a + b;
}

#if 0
    extern "C" int WINAPI WinMainTest(HINSTANCE hInstance, HINSTANCE hPrevInstance, wxCmdLineArgType WXUNUSED(lpCmdLine), int nCmdShow)                             
    {                                                                       
        wxDISABLE_DEBUG_SUPPORT();                                          
                                                                            
        /* NB: We pass NULL in place of lpCmdLine to behave the same as  */ 
        /*     Borland-specific wWinMain() above. If it becomes needed   */ 
        /*     to pass lpCmdLine to wxEntry() here, you'll have to fix   */ 
        /*     wWinMain() above too.                                     */ 
        return wxEntry(hInstance, hPrevInstance, NULL, nCmdShow);           
    }  



	TrenchBroom::View::TrenchBroomApp& wxGetApp() { return *static_cast<TrenchBroom::View::TrenchBroomApp*>(wxApp::GetInstance()); }
    wxAppConsole *wxCreateApp()
    {                                                                      
        wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "your program");
        return new TrenchBroom::View::TrenchBroomApp;
    }                                                                      
    wxAppInitializer wxTheAppInitializer((wxAppInitializerFunction) wxCreateApp);
#endif


#if 0

#include "wx/wx.h"
//#include "wx/dialog.h"

	class MyApp: public wxApp
{
public:
    bool OnInit()
    {
        wxDialog* dlg = new wxDialog(NULL, -1,
            "Hello World From Library", wxDefaultPosition, wxDefaultSize,
            wxDEFAULT_DIALOG_STYLE, wxDialogNameStr);
        dlg->Show();
        return true;
    }
};


static wxAppConsole *wxCreateApp()
{
    wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "your program");
    return new TrenchBroom::View::TrenchBroomApp;
    return new MyApp;
}
 
// RunApp() = ccall( (:RunApp, "G:\\TrenchBroom\\build3\\Debug\\TrenchBroom.dll"), Void, ())
CCALL void RunApp()
{
    wxApp::SetInitializerFunction(wxCreateApp);
    wxEntry(0,NULL);
} 

#endif