#include "App.h"

#include "resource.h"

int WINAPI WinMain(                   // WinMain
    _In_ HINSTANCE     Instance,     // handle to current instance
    _In_opt_ HINSTANCE hPrevInstance, // handle to previous instance
    _In_ LPSTR         lpCmdLine,     // command line
    _In_ int           nCmdShow       // show state
)
{
    try
    {
        return App().Run();
    }
    catch (const FException& Ex)
    {
        MessageBox(nullptr, Ex.What(), Ex.GetType(), MB_OK);
    }
    catch (const std::exception& Ex)
    {
        MessageBoxA(nullptr, Ex.what(), nullptr, MB_OK);
    }
    catch (...)
    {
        MessageBox(nullptr, TEXT("Something wrong!"), nullptr, MB_OKCANCEL);
    }
    return -1;
}
