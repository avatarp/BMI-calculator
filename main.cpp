#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <string>
#include <tchar.h>
#include <windows.h>
#include <sstream>
#include <commctrl.h>
#include <shlwapi.h>

#define MAINICON 505
#define ID_WEIGHTTXTBX 100
#define ID_HEIGHTTXTBX 101
#define ID_WEIGHTVIEW 110
#define ID_HEIGHTVIEW 111
#define ID_RESULTVIEW 112
#define ID_CALCULATEBUTTON 120
#define ID_FEMALERADIO 130
#define ID_MALERADIO 131

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("BMI App");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               //handle for our window
    MSG messages;            //here messages to the application are saved
    WNDCLASSEX wincl;        //structure for windowclass


    //window structure
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;                 // Catch double-clicks
    wincl.cbSize = sizeof (WNDCLASSEX);
    // window style
    wincl.hIcon = LoadIcon (NULL, MAKEINTRESOURCE(MAINICON));
    wincl.hIconSm = LoadIcon (NULL, IDI_INFORMATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.cbClsExtra = 0;
    // windows default colour as the background
    wincl.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,                   // Extended possibilites for variation
           szClassName,         // Classname
           _T("BMI"),           // Title
           WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX| WS_SYSMENU|WS_MINIMIZE, //  window without maximize button and resizing
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           240,                 /* The programs width */
           240,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );


    HFONT hNormalFont =( HFONT ) GetStockObject( DEFAULT_GUI_FONT );
    HWND weightTextbox = CreateWindowEx( 0, "EDIT", NULL, ES_NUMBER|WS_CHILD | WS_VISIBLE | WS_BORDER,5, 5, 50, 20,hwnd, (HMENU)ID_WEIGHTTXTBX, hThisInstance, NULL );
    SendMessage( weightTextbox, WM_SETFONT,( WPARAM ) hNormalFont, 0 );
    HWND heightTextbox = CreateWindowEx( 0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,5, 35, 50, 20,hwnd, (HMENU)ID_HEIGHTTXTBX, hThisInstance, NULL );
    SendMessage( heightTextbox, WM_SETFONT,( WPARAM ) hNormalFont, 0 );

    HWND weightTextview = CreateWindowEx( 0, "STATIC", "Weight (kg)", WS_CHILD | WS_VISIBLE |SS_CENTER, 60, 8, 60, 30, hwnd, (HMENU)ID_WEIGHTVIEW, hThisInstance, NULL );
    SendMessage( weightTextview, WM_SETFONT,( WPARAM ) hNormalFont, 0 );
    HWND heightTextview = CreateWindowEx( 0, "STATIC", "Height (cm)", WS_CHILD | WS_VISIBLE |SS_CENTER, 60, 38, 60, 30, hwnd, (HMENU)ID_HEIGHTVIEW, hThisInstance, NULL );
    SendMessage( heightTextview, WM_SETFONT,( WPARAM ) hNormalFont, 0 );

    HWND femaleRadio = CreateWindowEx( 0, "BUTTON", "Female", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,5, 55, 60, 30, hwnd, (HMENU)ID_FEMALERADIO, hThisInstance, NULL );
    SendMessage( femaleRadio, WM_SETFONT,( WPARAM ) hNormalFont, 0 );
    HWND maleRadio = CreateWindowEx( 0, "BUTTON", "Male", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,65, 55, 75, 30, hwnd, (HMENU)ID_MALERADIO, hThisInstance, NULL );
    SendMessage( maleRadio, WM_SETFONT,( WPARAM ) hNormalFont, 0 );

    HWND calculateButton= CreateWindowEx(0,"BUTTON","Calculate",WS_CHILD|WS_VISIBLE,15,85,110,30,hwnd,(HMENU) ID_CALCULATEBUTTON,hThisInstance,NULL);
    SendMessage( calculateButton, WM_SETFONT,( WPARAM ) hNormalFont, 0 );

    HWND resultTextview = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |SS_LEFT, 5, 120, 220, 185, hwnd, (HMENU)ID_RESULTVIEW, hThisInstance, NULL );
    SendMessage( resultTextview, WM_SETFONT,( WPARAM ) hNormalFont, 0 );

    ShowWindow (hwnd, nCmdShow);


    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}


//  This function is called by the Windows function DispatchMessage()
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

        case WM_COMMAND:
        switch( wParam )
        {
            case ID_CALCULATEBUTTON:
            {
            HWND weightTxt = GetDlgItem( hwnd, ID_WEIGHTTXTBX );
            HWND heightTxt = GetDlgItem( hwnd, ID_HEIGHTTXTBX );
            HWND resultText;
            DWORD weightTxtLen = GetWindowTextLength( weightTxt );
            DWORD heighTxtLen=GetWindowTextLength( heightTxt );
            LPSTR weightTxtBuff =( LPSTR ) GlobalAlloc( GPTR, weightTxtLen + 1 );
            LPSTR heightTxtBuff =( LPSTR ) GlobalAlloc( GPTR, heighTxtLen + 1 );
            resultText=GetDlgItem(hwnd,ID_RESULTVIEW);
            bool female=0;

            GetWindowText( weightTxt, weightTxtBuff, weightTxtLen+1 );

            if(IsDlgButtonChecked( hwnd, ID_FEMALERADIO ) == BST_CHECKED)
            {female=true;}
            else
            {female=false;}

            GetWindowText( heightTxt, heightTxtBuff, heighTxtLen+1 );
            std::string temp=static_cast<std::string>(weightTxtBuff);
            unsigned weight=atoi(temp.c_str());
            temp=static_cast<std::string>(heightTxtBuff);
            unsigned height=atoi(temp.c_str());
            double heightSq=(height*0.01)*(height*0.01);//in meters

            GlobalFree( heightTxtBuff );
            GlobalFree( weightTxtBuff );

            if(weight==0||height==0)
            {
                MessageBox(hwnd,TEXT("Wrong weight and/or height"),"Spooky error!",MB_ICONERROR);
                SetWindowText(resultText,NULL);
            }
            else
            {
                double bmi=weight/heightSq;
                double bmimin=18.5*heightSq;
                double bmimax=25*heightSq;

                std::ostringstream resultStream;
                std::string weightStatus;
                if(bmi<18.5){weightStatus=" - Underweight";}else
                if(bmi<25&&bmi>18.5){weightStatus=" - Normal";}else
                if(bmi<30&&bmi>25){weightStatus=" - Overweight";}else
                if(bmi>30){weightStatus=" - Obese";}

                resultStream <<"Your BMI is: "<<bmi<<weightStatus<<".\n"
                <<"For your height ("<<height<<" cm)\na normal weight range would be from:\n"
                <<bmimin  <<" kg to "<<bmimax <<" kg.";

                temp = resultStream.str();
                weightTxtBuff=const_cast<char *>(temp.c_str());
                SetWindowText(resultText,weightTxtBuff);
            }


        }
        break;
        case ID_FEMALERADIO:
        CheckRadioButton( hwnd, ID_FEMALERADIO, ID_MALERADIO, ID_FEMALERADIO );
        break;
        case ID_MALERADIO:
        CheckRadioButton( hwnd, ID_FEMALERADIO, ID_MALERADIO, ID_MALERADIO );
        break;
        default:
            break;
        }

        case WM_CREATE:
            break;
        case WM_DESTROY:

            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
}


    return 0;
}
