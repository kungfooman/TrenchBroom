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

#include "Console.h"

#include "FileLogger.h"
#include "View/ViewConstants.h"

#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/wupdlock.h>

#include <iostream>

namespace TrenchBroom {
    namespace View {
        Console::Console(wxWindow* parent) :
        TabBookPage(parent),
        m_textView(new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP | wxTE_RICH2)) {
			m_textView->SetFont(Fonts::fixedWidthFont());

            wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(m_textView, 1, wxEXPAND);
            SetSizer(sizer);

			m_textView->SetEditable(true);
        }

        void Console::doLog(const LogLevel level, const String& message) {
            doLog(level, wxString(message));
        }

        void Console::doLog(const LogLevel level, const wxString& message) {
            if (!message.empty()) {
                logToDebugOut(level, message);
                logToConsole(level, message);
                FileLogger::instance().log(level, message);
            }
        }

        void Console::logToDebugOut(const LogLevel level, const wxString& message) {
            wxLogDebug(message);
        }

        void Console::logToConsole(const LogLevel level, const wxString& message) {
            if (m_textView->IsBeingDeleted()) return;
                
            wxWindowUpdateLocker locker(m_textView);

            const long start = m_textView->GetLastPosition();
            m_textView->AppendText(message);
            m_textView->AppendText("\n");
#ifndef __APPLE__
			m_textView->ScrollLines(5);
#endif
            const long end = m_textView->GetLastPosition();

            switch (level) {
                case LogLevel_Debug:
                    m_textView->SetStyle(start, end, wxTextAttr(Colors::disabledText(), m_textView->GetBackgroundColour()));
                    break;
                case LogLevel_Info:
                    // m_textView->SetStyle(start, end, wxTextAttr(*wxBLACK, m_textView->GetBackgroundColour()));
                    break;
                case LogLevel_Warn:
                    m_textView->SetStyle(start, end, wxTextAttr(Colors::defaultText(), m_textView->GetBackgroundColour()));
                    break;
                case LogLevel_Error:
                    m_textView->SetStyle(start, end, wxTextAttr(wxColor(250, 30, 60), m_textView->GetBackgroundColour()));
                    break;
            }
        }
    }
}



namespace TrenchBroom {
    namespace View {
        ConsoleInput::ConsoleInput(wxWindow* parent) :
        TabBookPage(parent),
        m_textView(new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP | wxTE_RICH2)) {
			m_textView->SetFont(Fonts::fixedWidthFont());

            wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(m_textView, 1, wxEXPAND);
            SetSizer(sizer);

			m_textView->SetEditable(true);
        }

        void ConsoleInput::doLog(const LogLevel level, const String& message) {
            doLog(level, wxString(message));
        }

        void ConsoleInput::doLog(const LogLevel level, const wxString& message) {
            if (!message.empty()) {
                logToDebugOut(level, message);
                logToConsole(level, message);
                FileLogger::instance().log(level, message);
            }
        }

        void ConsoleInput::logToDebugOut(const LogLevel level, const wxString& message) {
            wxLogDebug(message);
        }

        void ConsoleInput::logToConsole(const LogLevel level, const wxString& message) {
            if (m_textView->IsBeingDeleted()) return;
                
            wxWindowUpdateLocker locker(m_textView);

            const long start = m_textView->GetLastPosition();
            m_textView->AppendText(message);
            m_textView->AppendText("\n");
#ifndef __APPLE__
			m_textView->ScrollLines(5);
#endif
            const long end = m_textView->GetLastPosition();

            switch (level) {
                case LogLevel_Debug:
                    m_textView->SetStyle(start, end, wxTextAttr(Colors::disabledText(), m_textView->GetBackgroundColour()));
                    break;
                case LogLevel_Info:
                    // m_textView->SetStyle(start, end, wxTextAttr(*wxBLACK, m_textView->GetBackgroundColour()));
                    break;
                case LogLevel_Warn:
                    m_textView->SetStyle(start, end, wxTextAttr(Colors::defaultText(), m_textView->GetBackgroundColour()));
                    break;
                case LogLevel_Error:
                    m_textView->SetStyle(start, end, wxTextAttr(wxColor(250, 30, 60), m_textView->GetBackgroundColour()));
                    break;
            }
        }
    }
}


#include <Windows.h>
#include <stdio.h>
#include <direct.h>
#include <ccall.h>

wxTextCtrl *juliaTextWidgetOutput = NULL;
// ccall( :juliaPrint, Void, (Cstring,), "hai")
CCALL void juliaPrint(char *message) {
	if (juliaTextWidgetOutput == NULL)
		return;
	if (juliaTextWidgetOutput->IsBeingDeleted()) return;
	wxWindowUpdateLocker locker(juliaTextWidgetOutput);
	const long start = juliaTextWidgetOutput->GetLastPosition();
	juliaTextWidgetOutput->AppendText(message);
	#ifndef __APPLE__
	juliaTextWidgetOutput->ScrollLines(5);
	#endif
}

#include "Selection.h"
#include "FrameManager.h"
#include "TrenchBroomApp.h"
#include "MapFrame.h"
#include "MapDocument.h"
#include "Model/Brush.h"

// ripped from Brush.cpp
#include "CollectionUtils.h"
#include "Model/BrushContentTypeBuilder.h"
#include "Model/BrushFace.h"
#include "Model/BrushGeometry.h"
#include "Model/BrushSnapshot.h"
#include "Model/Entity.h"
#include "Model/FindContainerVisitor.h"
#include "Model/FindGroupVisitor.h"
#include "Model/FindLayerVisitor.h"
#include "Model/Group.h"
#include "Model/IssueGenerator.h"
#include "Model/NodeVisitor.h"
#include "Model/PickResult.h"
#include "Model/World.h"

using namespace TrenchBroom::View;

int print(char *format, ...) {
	char *buf;
	va_list args;
	va_start (args, format);
	
	// the output can be enourmous, so instead of a static buffer, we first figure out the length needed and then just alloc that size
	size_t needed = vsnprintf(NULL, 0, format, args);
	needed += 1;
	buf = (char *)malloc(needed);

	if (buf == NULL) {
		juliaPrint("failed to allocate memory for imgui_log()\n");
		return -1;
	}

	vsnprintf(buf, needed, format, args);
	va_end (args);

	juliaPrint(buf);

	free(buf);
}

// ccall( :testbrush, Void, ())
CCALL void testbrush() {
	
	TrenchBroom::View::FrameManager *fm = TrenchBroom::View::TrenchBroomApp::instance().frameManager();
	auto frames = fm->frames();

	for (MapFrame *frame : frames) {
		juliaPrint("frame\n");
		auto brushes = frame->document()->selectedNodes().brushes();
		for (auto brush : brushes) {
			print("brush %d has faceCount=%d\n", brush->id, brush->faceCount());
		}

		
	}

	

	//TrenchBroom_Selection::


	//selectedNodes
}

class LibJulia {
public:
	static bool loaded;
	typedef void  (*jl_init_t       )(               );
	typedef int  *(*jl_eval_string_t)(const char *str);
	static jl_init_t        jl_init_addr;
	static jl_eval_string_t jl_eval_string_addr;

	static bool InitIfNot() {
		if (loaded == true)
			return true;
		char currentDir[512];
		_getcwd(currentDir, sizeof(currentDir));
		//printf("currentDir=%s\n", currentDir);
		_chdir("C:\\julia_32bit\\bin");
		HMODULE libjulia = LoadLibraryA("C:\\julia_32bit\\bin\\libjulia.dll");
		if (libjulia == NULL) {
			//juliaPrint("libjulia == NULL\n");
			return false;
		}
		jl_init_addr        = (jl_init_t       )GetProcAddress(libjulia, "jl_init");
		jl_eval_string_addr = (jl_eval_string_t)GetProcAddress(libjulia, "jl_eval_string");
		if (!jl_init_addr)
			return false;
		if (!jl_eval_string_addr)
			return false;
		jl_init_addr();
		jl_eval_string_addr("include(\"g:/trenchbroom/julia/init.jl\")");
		loaded = true;
		return true;
	}

	static bool Eval(const char *code) {	
		if ( ! InitIfNot())
			return false;
		jl_eval_string_addr(code);
		return true;
	}

	static bool Include(const char *filename) {
		if ( ! InitIfNot())
			return false;
		char tmp[512];
		snprintf(tmp, sizeof(tmp), "include(\"%s\")", filename);
		jl_eval_string_addr(tmp);
		return true;
	}
};

bool LibJulia::loaded = false;
LibJulia::jl_init_t LibJulia::jl_init_addr = NULL;
LibJulia::jl_eval_string_t LibJulia::jl_eval_string_addr = NULL;

class cSpecialTextCtrl : public wxTextCtrl {
public:
	using wxTextCtrl::wxTextCtrl; // inherit constructors
	wxTextCtrl *outputTextControl;

	void OnChar( wxKeyEvent& ev ) {
		if (ev.ControlDown() && ev.m_keyCode == 'R') {
			LibJulia::Include("g:/trenchbroom/julia/init.jl");
			//juliaPrint("ctrl+r\n");
		} else if (ev.ControlDown() && ev.m_keyCode == 13/*enter*/) {
			wxString text;
			if (HasSelection()) {
				text = GetStringSelection().Clone();
			} else {
				text = GetValue().Clone();
			}
			text.Replace("\"", "\\\"");
			text = "eval_and_prettyprint(\"" + text + "\")";
			LibJulia::Eval(text.c_str());
		} else {
			wxTextCtrl::OnChar(ev);
		}
	}

	private:
		DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE( cSpecialTextCtrl , wxTextCtrl)
    EVT_KEY_DOWN( cSpecialTextCtrl ::OnChar)
END_EVENT_TABLE()

namespace TrenchBroom {
    namespace View {
        SplitterTab::SplitterTab(wxWindow* parent) :
        TabBookPage(parent)//,
        //m_textView(new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP | wxTE_RICH2))
		{

			//m_textView->SetFont(Fonts::fixedWidthFont());
			
			//m_splitterWindow = new wxSplitterWindow(parent, -1, wxPoint(0, 0), wxSize(400, 400), wxSP_3D);
			//m_splitterWindow = new wxSplitterWindow(this, wxID_ANY);
			//m_splitterWindow->SetMinimumPaneSize(50);

            //wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            //sizer->Add(m_splitterWindow, 1, wxEXPAND);
            //SetSizer(sizer);

			//m_textView->SetEditable(true);


    // Create the wxSplitterWindow window
    // and set a minimum pane size to prevent unsplitting
    wxSplitterWindow* splitterWindow = new wxSplitterWindow(this, wxID_ANY);
    splitterWindow->SetMinimumPaneSize(50);

    // Create the left panel
    wxPanel* panel1 = new wxPanel(splitterWindow, wxID_ANY);
    juliaTextWidgetOutput = new wxTextCtrl(panel1, wxID_ANY, L"Enter commands on the right\nCtrl+enter = execute all\nCtrl+enter with selection = execute selection\nAlt+enter = execute line of start cursor\n", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_PROCESS_TAB);
	juliaTextWidgetOutput->SetFont( wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL) );
    wxBoxSizer* panel1Sizer = new wxBoxSizer(wxHORIZONTAL);
    panel1Sizer->Add(juliaTextWidgetOutput, 1, wxEXPAND);
    panel1->SetSizer(panel1Sizer);

    // Create the right panel
    wxPanel* panel2 = new wxPanel(splitterWindow, wxID_ANY);
    cSpecialTextCtrl* textCtrl2 = new cSpecialTextCtrl(panel2, wxID_ANY, L"for i in 10:20\n\tlog(console, i)\nend", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTE_MULTILINE |  wxTE_DONTWRAP | wxTE_PROCESS_TAB);
	textCtrl2->SetFont( wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL) );
	textCtrl2->outputTextControl = juliaTextWidgetOutput;
	textCtrl2->SetEditable(true);
	//textCtrl2->SetEvtHandlerEnabled(false);

    wxBoxSizer* panel2Sizer = new wxBoxSizer(wxHORIZONTAL);
    panel2Sizer->Add(textCtrl2, 1, wxEXPAND);
    panel2->SetSizer(panel2Sizer);
	
	
    // Set up the sizer for the frame and resize the frame
    // according to its contents
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(splitterWindow, 1, wxEXPAND);
    SetSizerAndFit(topSizer);
	
	// Split the window vertically and set the left and right panes
    splitterWindow->SplitVertically(panel1, panel2, 1000 /* i dont know how to set 50/50, 1000 pixels is a nice guess */);

        }
    }
}
