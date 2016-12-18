#pragma one

#include <windows.h>
#include <gdiplus.h>
#include <memory>
#include <vector>
#include <deque>
#include <string>

namespace BGO
{

class Object;

using TObjectPtrVector = std::vector<Object*>;
using TULongVector = std::deque<unsigned long>;

class Object
{
   Object(const Object& rhs) = delete;

public:
   Object();
   virtual ~Object();

   const Gdiplus::RectF& GetBoundary() const;

   virtual void OffsetBoundary(Gdiplus::REAL offset_x, Gdiplus::REAL offset_y);
   virtual void RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics) = 0;
   virtual void Draw(Gdiplus::Graphics* graphics) const = 0;
   
   enum class ClickType { NoClick, ClickDone, ClickDoneNeedResize };
   virtual ClickType ProcessClick(long x, long y, TULongVector& group_indexes);
   virtual void ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects);

protected:
   Gdiplus::RectF m_boundary;
};

class ObjectWithBackground : public Object
{
public:
   ObjectWithBackground(const Gdiplus::Color& back_color);

   // Object overrides
   virtual void Draw(Gdiplus::Graphics* graphics) const override;

private:
   Gdiplus::Color m_back_color;
};

class Text : public ObjectWithBackground
{
public:
   Text(const Gdiplus::Color& back_color, const wchar_t* font_name, 
        unsigned long font_size, unsigned long font_style, const Gdiplus::Color& font_color, unsigned long width = 0);
   
   bool SetText(const char* text);
   bool SetColor(const Gdiplus::Color& color);
   
   // ObjectWithBackground overrides
   virtual void RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics) override;
   virtual void Draw(Gdiplus::Graphics* graphics) const override;

protected:
   // Own virtual methods
   virtual const wchar_t* GetFontName() const;
   virtual unsigned long GetFontSize() const;
   virtual unsigned long GetFontStyle() const;
   virtual const Gdiplus::Color& GetFontColor() const;
   
private:
   std::wstring m_text;
   const wchar_t* m_font_name;
   unsigned long m_font_size;
   unsigned long m_font_style;
   Gdiplus::Color m_font_color;
   unsigned long m_width;
};

class HoverableText : public Text
{
public:
   HoverableText(const Gdiplus::Color& back_color, const wchar_t* font_name,
                 unsigned long font_size, unsigned long font_style, const Gdiplus::Color& font_color, unsigned long width);

   // Text overrides
   virtual void ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects) override;

protected:
   virtual unsigned long GetFontStyle() const override;
   virtual unsigned long GetFontStyleWithoutHover() const;

private:
   bool m_is_hovered;
};

class ClickableText : public HoverableText
{
public:
   ClickableText(const Gdiplus::Color& back_color, const wchar_t* font_name, unsigned long font_size,
                 unsigned long font_style, const Gdiplus::Color& font_color, unsigned long width,
                 const Gdiplus::Color& clickable_font_color);

   bool SetClickable(bool is_clickable);

   // Text overrides
   virtual ClickType ProcessClick(long x, long y, TULongVector& group_indexes) override;
   virtual void ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects) override;

protected:
   virtual const Gdiplus::Color& GetFontColor() const override;

private:
   Gdiplus::Color m_clickable_font_color;
   bool m_is_clickable;
};

class CollapsibleText : public HoverableText
{
public:
   CollapsibleText(const Gdiplus::Color& back_color, const wchar_t* font_name, unsigned long font_size,
                   unsigned long font_style, const Gdiplus::Color& font_color, unsigned long width,
                   unsigned long collapsed_font_style, const Gdiplus::Color& collapsed_font_color);
   
   void SetCollapsed(bool is_collapsed);
   bool GetCollapsed() const;
   
   // Object overrides
   virtual ClickType ProcessClick(long x, long y, TULongVector& group_indexes) override;

protected:
   // Text overrides
   virtual unsigned long GetFontStyleWithoutHover() const override;
   virtual const Gdiplus::Color& GetFontColor() const override;
   
private:
   unsigned long m_collapsed_font_style;
   Gdiplus::Color m_collapsed_font_color;
   bool m_is_collapsed;
};

class Line : public ObjectWithBackground
{
public:
   Line(const Gdiplus::Color& back_color, const Gdiplus::Color& color, unsigned long width);

   // ObjectWithBackground overrides
   virtual void RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics) override;
   virtual void Draw(Gdiplus::Graphics* graphics) const override;

private:
   Gdiplus::Color m_color;
   unsigned long m_width;
};

class Image : public Object
{
public:
   Image(/*int resourse_id*/);

   // Object overrides
   virtual void RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics) override;
   virtual void Draw(Gdiplus::Graphics* graphics) const override;

private:
   //Gdiplus::Image m_image;
};

class Group : public Object
{
public:
   enum class GroupType { Horizontal, Vertical };
   enum class AligningType { Min, Middle, Max };

   Group(GroupType type, Gdiplus::REAL indent_before_x = 0, Gdiplus::REAL indent_before_y = 0);

   bool SetObjectCount(unsigned long count);
   unsigned long GetObjectCount() const;
   
   void SetObject(unsigned long index, std::unique_ptr<Object>&& object,
                  AligningType aligning, Gdiplus::REAL indent_after = 0);
   const Object* GetObject(unsigned long index) const;
   Object* GetObject(unsigned long index);
   
   // Object overrides
   virtual void OffsetBoundary(Gdiplus::REAL offset_x, Gdiplus::REAL offset_y) override;   
   virtual void RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics) override;
   virtual void Draw(Gdiplus::Graphics* graphics) const override;
   virtual ClickType ProcessClick(long x, long y, TULongVector& group_indexes) override;
   virtual void ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects) override;
   
protected:
   // Own virtual method
   virtual bool IsObjectVisible(unsigned long index) const;

protected:
   GroupType m_type;
   Gdiplus::REAL m_indent_before_x;
   Gdiplus::REAL m_indent_before_y;

   struct ObjectInfo
   {
      std::unique_ptr<Object> m_object;
      AligningType m_aligning;
      Gdiplus::REAL m_indent_after;
   };

   std::vector<ObjectInfo> m_object_infos;
};

} // namespace BGO
