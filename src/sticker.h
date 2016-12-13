#pragma once

#include "window.h"

#include <gdiplus.h>

#include <vector>
#include <memory>

enum class ImageType { None, Ok, Expired, Minus, Arrow };

class ISection
{
public:
   virtual ~ISection();

   virtual void SetOwnerName(const char* name) = 0;
   virtual void SetTitle(ImageType image, const char* date, const char* time, const char* desc) = 0;
   virtual void SetHeader(ImageType image, const char* desc) = 0;
   virtual void SetFooter(ImageType image, const char* prefix, const char* desc) = 0;

   virtual void SetItemCount(unsigned long count) = 0;
   virtual void SetItem(unsigned long index, ImageType image, const char* date, const char* time,
                        const char* desc, bool is_clickable) = 0;
};

class IStickerCallback
{
public:
   virtual ~IStickerCallback();

   virtual void OnHeaderClick(unsigned long section_index) = 0;
   virtual void OnItemClick(unsigned long section_index, unsigned long item_index) = 0;
   virtual void OnFooterClick(unsigned long section_index) = 0;
};

namespace SGO
{
   // Forward declaration to use in PIMPL.
   class StickerObject;
}

class Sticker : public wc::Window
{
public:
   Sticker();
   ~Sticker();

   void SetDirty();
   void SetRedraw(bool is_redraw);
   void Update();

   void SetSectionCount(unsigned long count);
   ISection& GetSection(unsigned long index);
   
   void SetCallback(std::unique_ptr<IStickerCallback>&& callback);
   IStickerCallback* GetCallback() const;

protected:
   virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
   
private:
   void OnLButtonUp(long x, long y);
   void OnMouseMove(long x, long y);
   void OnMouseHover(long x, long y);
   void OnMouseLeave();
   void OnPaint(HDC hdc);
   
   void ProcessHover(long x, long y);

private:
   bool m_is_dirty;
   bool m_is_redraw;
   bool m_is_mouse_tracking;
   
   std::unique_ptr<Gdiplus::Bitmap> m_memory_image;
   std::unique_ptr<IStickerCallback> m_callback;
   std::unique_ptr<SGO::StickerObject> m_object;
};
