#include "mainwindow.h"
#include "ui_mainwindow.h"

// Crosslink between Qt class and win callback
MainWindow * mwReference;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
            // Pass KeyDown/KeyUp messages for Qt class to logicize
            case WM_KEYDOWN:
                mwReference->keyDown(PKBDLLHOOKSTRUCT(lParam)->vkCode);
            break;
            case WM_KEYUP:
                mwReference->keyUp(PKBDLLHOOKSTRUCT(lParam)->vkCode);
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup variables
    mwReference = this;
    bWinKey     = false;
    bF9         = false;
    bF10        = false;

    // Install the low-level keyboard & mouse hooks
    hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
}

MainWindow::~MainWindow()
{
    // Be friendly! Remove hooks!
    UnhookWindowsHookEx(hhkLowLevelKybd);
    delete ui;
}

void MainWindow::keyDown(DWORD key)
{
    // This section gets launched for every key pressed ever
    // Filter out the keys we are interested
    // Launch doMultimedia everytime key of interest is pressed
    //      Otherwise it'll still launch even if non-interested key is pressed
    //      Also will result in massive trigger spam when right keys are pressed
    if(key == VK_LWIN)
    {
        bWinKey = true;
        doMultimedia();
    }
    if(key == VK_F9)
    {
        bF9 = true;
        doMultimedia();
    }
    if(key == VK_F10)
    {
        bF10 = true;
        doMultimedia();
    }
}

void MainWindow::keyUp(DWORD key)
{
    // Remove the bools
    if(key == VK_LWIN)
        bWinKey = false;
    if(key == VK_F9)
        bF9 = false;
    if(key == VK_F10)
        bF10 = false;
}

void MainWindow::doMultimedia()
{
    // Play / Pause
    if (bF9 && bWinKey)
    {
        INPUT Input;
        // Set up a generic keyboard event.
        Input.type = INPUT_KEYBOARD;
        Input.ki.wScan = 0;
        Input.ki.time = 0;
        Input.ki.dwExtraInfo = 0;
        Input.ki.dwFlags = 0;

        Input.ki.wVk = VK_MEDIA_PLAY_PAUSE;
        SendInput(1, &Input, sizeof(INPUT));
    }
    // Next Track
    else if (bF10 && bWinKey)
    {
        INPUT Input;
        // Set up a generic keyboard event.
        Input.type = INPUT_KEYBOARD;
        Input.ki.wScan = 0;
        Input.ki.time = 0;
        Input.ki.dwExtraInfo = 0;
        Input.ki.dwFlags = 0;

        Input.ki.wVk = VK_MEDIA_NEXT_TRACK;
        SendInput(1, &Input, sizeof(INPUT));
    }
}
