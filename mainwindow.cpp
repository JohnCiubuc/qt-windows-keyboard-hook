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
    //      By keeping it within the parenthesis, doMultimedia will fire on that specific key repeat
    //      Otherwise it'll spam on both win and key repeat, meaning it'll fire 30+ a second if outside brackets
    if(key == VK_LWIN || key == VK_RWIN)
    {
        bWinKey = true;
        doMultimedia(key);
    }
    if(key == VK_F9)
        doMultimedia(key);
    if(key == VK_F10)
        doMultimedia(key);
    if(key == VK_DOWN)
        doMultimedia(key);
    if(key == VK_UP)
        doMultimedia(key);
    if(key == VK_END)
        doMultimedia(key);
}

void MainWindow::keyUp(DWORD key)
{
    // Remove the bools
    if(key == VK_LWIN || key == VK_RWIN)
        bWinKey = false;
}

void MainWindow::pressKey(DWORD vkKeyCode)
{
    INPUT Input;
    // Set up a generic keyboard event.
    Input.type = INPUT_KEYBOARD;
    Input.ki.wScan = 0;
    Input.ki.time = 0;
    Input.ki.dwExtraInfo = 0;
    Input.ki.dwFlags = 0;

    Input.ki.wVk = vkKeyCode;
    SendInput(1, &Input, sizeof(INPUT));
}

void MainWindow::doMultimedia(DWORD vkKeyCode)
{
    // Check if any win key is being pressed first
    if(!bWinKey) return;

    // Play / Pause
    if (vkKeyCode == VK_F9)
        pressKey(VK_MEDIA_PLAY_PAUSE);
    // Next Track
    else if (vkKeyCode == VK_F10)
        pressKey(VK_MEDIA_NEXT_TRACK);
    // Lower Volume
    else if (vkKeyCode == VK_DOWN)
        pressKey(VK_VOLUME_DOWN);
    // Increase Volume
    else if (vkKeyCode == VK_UP)
        pressKey(VK_VOLUME_UP);
    // Mute
    else if (vkKeyCode == VK_END)
        pressKey(VK_VOLUME_MUTE);
}
