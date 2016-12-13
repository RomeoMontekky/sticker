#include "window.h"
#include "window_class.h"
#include "sticker.h"

#include <gdiplus.h>
#include <sstream>

class GdiplusInitializer
{
public:
   GdiplusInitializer() : m_gdiplusToken(0)
   {
      Gdiplus::GdiplusStartupInput gdiplusStartupInput;
      Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
   }

   ~GdiplusInitializer()
   {
      Gdiplus::GdiplusShutdown(m_gdiplusToken);
   }

private:
   ULONG_PTR m_gdiplusToken;
};

class StickerCallback : public IStickerCallback
{
public:
   StickerCallback(HWND parent_window) : m_parent_window(parent_window)
   {
      // no code
   }

   virtual void OnHeaderClick(unsigned long section_index) override
   {
      std::stringstream sstream;
      sstream << "OnHeader event. Section index = " << section_index;
      ::MessageBox(m_parent_window, sstream.str().c_str(), "Event", MB_OK);
   }

   virtual void OnItemClick(unsigned long section_index, unsigned long item_index) override
   {
      std::stringstream sstream;
      sstream << "OnItem event. Section index = " << section_index << ". Item index = " << item_index;
      ::MessageBox(m_parent_window, sstream.str().c_str(), "Event", MB_OK);
   }

   virtual void OnFooterClick(unsigned long section_index) override
   {
      std::stringstream sstream;
      sstream << "OnFooter event. Section index = " << section_index;
      ::MessageBox(m_parent_window, sstream.str().c_str(), "Event", MB_OK);
   }

private:
   HWND m_parent_window;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   GdiplusInitializer gdi_initializer;

   wc::MainWindow main_window;
   main_window.Create("Main Window", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 600, 200, 400, 400);
   ::ShowWindow(main_window.GetHandle(), nCmdShow);

   Sticker sticker;
   sticker.Create(nullptr, WS_CHILD|WS_VISIBLE|WS_DLGFRAME, 0, 0, 100, 22, main_window.GetHandle());

   sticker.SetCallback(std::make_unique<StickerCallback>(main_window.GetHandle()));
   
   sticker.SetRedraw(false);
   {
      sticker.SetSectionCount(3);
      {
         auto& section = sticker.GetSection(0);
         section.SetTitle(ImageType::None, "21.09", "12:45", "Section 1");
         section.SetItemCount(2);
         section.SetItem(0, ImageType::None, "21.09", "12:45", "Send", true);
         section.SetItem(1, ImageType::None, "21.09", "13:00", "Received", false);
      }
      {
         auto& section = sticker.GetSection(1);
         section.SetTitle(ImageType::None, "22.09", "09:00", "Section 2");
         section.SetItemCount(2);
         section.SetItem(0, ImageType::None, "22.09", "09:00", "Send", true);
         section.SetItem(1, ImageType::None, "22.09", "09:15", "Received", false);
      }
      {
         auto& section = sticker.GetSection(2);
         section.SetTitle(ImageType::None, "23.09", "15:00", "Section 3");
         section.SetItemCount(3);
         section.SetItem(0, ImageType::None, "23.09", "15:00", "Send", true);
         section.SetItem(1, ImageType::None, "23.09", "15:15", "Received", false);
         section.SetItem(2, ImageType::None, "23.09", "15:30", "Confirmed", true);
      }
   }
   sticker.SetRedraw(true);

   MSG msg;
   // Main message loop.
   while (GetMessage(&msg, nullptr, 0, 0) != 0)
   {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
   }

   return msg.wParam;
}
