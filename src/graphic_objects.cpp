#include "graphic_objects.h"

#include <cstring>
#include <cassert>

namespace
{

std::wstring AsciiToWide(const char* input)
{
   if (nullptr == input || '\0' == *input)
   {
      return std::wstring();
   }

   const auto input_size = std::strlen(input);
   auto output_size = ::MultiByteToWideChar(CP_ACP, 0, input, input_size, nullptr, 0);
   assert(output_size > 0);

   std::unique_ptr<wchar_t[]> output(new wchar_t[output_size]);
   auto ret = ::MultiByteToWideChar(1251, 0, input, input_size, output.get(), output_size);
   assert(ret != 0);

   return std::wstring(output.get(), output.get() + output_size);
}

} // namespace

namespace BGO
{

///////////// class Object ////////

Object::Object() : m_boundary()
{
   // no code
}

Object::~Object()
{
   // no code
}

const Gdiplus::RectF& Object::GetBoundary() const
{
   return m_boundary;
}

void Object::OffsetBoundary(Gdiplus::REAL offset_x, Gdiplus::REAL offset_y)
{
   m_boundary.Offset(offset_x, offset_y);
}

Object::ClickType Object::ProcessClick(long x, long y, TULongVector& group_indexes)
{
   return ClickType::NoClick;
}

void Object::ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects)
{
   // no code
}

//////// class ObjectWithBackground ////////

ObjectWithBackground::ObjectWithBackground(const Gdiplus::Color& back_color) :
   Object(), m_back_color(back_color)
{
   // no code
}

void ObjectWithBackground::Draw(Gdiplus::Graphics* graphics) const
{
   if (GetBoundary().IsEmptyArea() == FALSE)
   {
      Gdiplus::SolidBrush back_brush(m_back_color);
      graphics->FillRectangle(&back_brush, GetBoundary());
   }
}

///////////// class Text /////////////
   
Text::Text(const Gdiplus::Color& back_color, const wchar_t* font_name, 
           unsigned long font_size, unsigned long font_style, const Gdiplus::Color& font_color) :
   ObjectWithBackground(back_color),
   m_font_name(font_name), m_font_size(font_size),
   m_font_style(font_style), m_font_color(font_color)
{
   // no code
}

bool Text::SetText(const char* text)
{
   const auto wide_text = ::AsciiToWide(text);
   if (m_text != wide_text)
   {
      m_text = wide_text;
      return true;
   }
   return false;
}

void Text::RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics)
{
   Gdiplus::RectF origin_rect(x, y, 0, 0);
   Gdiplus::Font font(GetFontName(), GetFontSize(), GetFontStyle());
   graphics->MeasureString(m_text.c_str(), m_text.size(), &font, origin_rect, &m_boundary);
}

void Text::Draw(Gdiplus::Graphics* graphics) const
{
   ObjectWithBackground::Draw(graphics);

   if (GetBoundary().IsEmptyArea() == FALSE)
   {
      Gdiplus::Font font(GetFontName(), GetFontSize(), GetFontStyle());
      Gdiplus::SolidBrush brush(GetFontColor());
      graphics->DrawString(m_text.c_str(), m_text.size(), &font, m_boundary, nullptr, &brush);
   }
}

const wchar_t* Text::GetFontName() const
{
   return m_font_name;
}

unsigned long Text::GetFontSize() const
{
   return m_font_size;
}

unsigned long Text::GetFontStyle() const
{
   return m_font_style;
}

const Gdiplus::Color& Text::GetFontColor() const
{
   return m_font_color;
}

/////////// class HoverableText //////////

HoverableText::HoverableText(
   const Gdiplus::Color& back_color, const wchar_t* font_name,
   unsigned long font_size, unsigned long font_style, const Gdiplus::Color& font_color) :
      Text(back_color, font_name, font_size, font_style, font_color), m_is_hovered(false)
{
   // no code
}

void HoverableText::ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects)
{
   const auto does_contain_cursor = (GetBoundary().Contains(x, y) == TRUE);
   if (does_contain_cursor != m_is_hovered)
   {
      m_is_hovered = does_contain_cursor;
      invalidated_objects.push_back(this);
   }
}

unsigned long HoverableText::GetFontStyle() const
{
   const auto font_style = GetFontStyleWithoutHover();
   return m_is_hovered ? (font_style | Gdiplus::FontStyleUnderline) : font_style;
}

unsigned long HoverableText::GetFontStyleWithoutHover() const
{
   return Text::GetFontStyle();
}

/////////// class ClickableText ////////////

ClickableText::ClickableText(
   const Gdiplus::Color& back_color, const wchar_t* font_name, unsigned long font_size,
   unsigned long font_style, const Gdiplus::Color& font_color, const Gdiplus::Color& clickable_font_color) :
      HoverableText(back_color, font_name, font_size, font_style, font_color),
      m_clickable_font_color(clickable_font_color), m_is_clickable(true)
{
   // no code
}

bool ClickableText::SetClickable(bool is_clickable)
{
   if (is_clickable != m_is_clickable)
   {
      m_is_clickable = is_clickable;
      return true;
   }
   return false;
}

Object::ClickType ClickableText::ProcessClick(long x, long y, TULongVector& group_indexes)
{
   if (m_is_clickable)
   {
      return (GetBoundary().Contains(x, y) == TRUE) ? ClickType::ClickDone : ClickType::NoClick;
   }
   return ClickType::NoClick;
}

void ClickableText::ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects)
{
   if (m_is_clickable)
   {
      HoverableText::ProcessHover(x, y, invalidated_objects);
   }
}

const Gdiplus::Color& ClickableText::GetFontColor() const
{
   return m_is_clickable ? m_clickable_font_color : HoverableText::GetFontColor();
}

///////////// class CollapsibleText ////////////////

CollapsibleText::CollapsibleText(
   const Gdiplus::Color& back_color, const wchar_t* font_name, unsigned long font_size,
   unsigned long font_style, const Gdiplus::Color& font_color,
   unsigned long collapsed_font_style, const Gdiplus::Color& collapsed_font_color) :
      HoverableText(back_color, font_name, font_size, font_style, font_color),
      m_collapsed_font_style(collapsed_font_style), m_collapsed_font_color(collapsed_font_color), m_is_collapsed(true)
{
   // no code
}

void CollapsibleText::SetCollapsed(bool is_collapsed)
{
   m_is_collapsed = is_collapsed;
}

bool CollapsibleText::GetCollapsed() const
{
   return m_is_collapsed;
}

Object::ClickType CollapsibleText::ProcessClick(long x, long y, TULongVector& group_indexes)
{
   if (GetBoundary().Contains(x, y) == TRUE)
   {
      m_is_collapsed = !m_is_collapsed;
      return ClickType::ClickDoneNeedResize;
   }
   return ClickType::NoClick;
}

unsigned long CollapsibleText::GetFontStyleWithoutHover() const
{
   return m_is_collapsed ? m_collapsed_font_style : HoverableText::GetFontStyleWithoutHover();
}

const Gdiplus::Color& CollapsibleText::GetFontColor() const
{
   return m_is_collapsed ? m_collapsed_font_color : HoverableText::GetFontColor();
}

///////////// class Line ////////////////

Line::Line(const Gdiplus::Color& back_color, const Gdiplus::Color& color, unsigned long width) :
   ObjectWithBackground(back_color), m_color(color), m_width(width)
{
   // no code
}

void Line::RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics)
{
   m_boundary.X = x;
   m_boundary.Y = y;
   m_boundary.Width = m_width;
   m_boundary.Height = 2;
}

void Line::Draw(Gdiplus::Graphics* graphics) const
{
   ObjectWithBackground::Draw(graphics);

   // Let's draw two simple lines. First one using main color.
   // Second one using the same color, but with alpha decreased by 2 times.

   Gdiplus::Pen main_pen(m_color, 1);
   graphics->DrawLine(&main_pen, m_boundary.GetLeft(), m_boundary.GetTop(),
                      m_boundary.GetRight(), m_boundary.GetTop());

   Gdiplus::Color shadow_color(m_color.GetA()/2, m_color.GetR(), m_color.GetG(), m_color.GetB());
   Gdiplus::Pen shadow_pen(shadow_color, 1);
   graphics->DrawLine(&shadow_pen, m_boundary.GetLeft(), m_boundary.GetTop() + 1,
                      m_boundary.GetRight(), m_boundary.GetTop() + 1);
}

///////////// class Image ////////////////

Image::Image(/*int resouse_id*/) : Object()
{
   // TODO: Load from resoure
}

void Image::RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics)
{
   // TODO: Get rectangle from the image
   m_boundary.X = x;
   m_boundary.Y = y;
   m_boundary.Width = 10;
   m_boundary.Height = 0; //10;
}

void Image::Draw(Gdiplus::Graphics* graphics) const
{
   // TODO: Draw the image
   graphics;
}

///////////// class Group ////////////////

Group::Group(GroupType type, Gdiplus::REAL indent_before_x, Gdiplus::REAL indent_before_y) :
   m_type(type), m_indent_before_x(indent_before_x), m_indent_before_y(indent_before_y), m_object_infos()
{
   m_object_infos.reserve(10);
}

bool Group::SetObjectCount(unsigned long count)
{
   if (m_object_infos.size() == count)
   {
      return false;
   }
   m_object_infos.resize(count);
   return true;
}

unsigned long Group::GetObjectCount() const
{
   return m_object_infos.size();
}

void Group::SetObject(unsigned long index, std::unique_ptr<Object>&& object,
                      AligningType aligning, Gdiplus::REAL indent_after)
{
   auto& object_info = m_object_infos.at(index);
   object_info.m_object = std::move(object);
   object_info.m_aligning = aligning;
   object_info.m_indent_after = indent_after;
}

const Object* Group::GetObject(unsigned long index) const
{
   return m_object_infos.at(index).m_object.get();
}

Object* Group::GetObject(unsigned long index)
{
   return m_object_infos.at(index).m_object.get();
}

void Group::RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics)
{
   m_boundary.X = x;
   m_boundary.Y = y;
   m_boundary.Width = 0;
   m_boundary.Height = 0;

   Gdiplus::REAL indent_x = m_indent_before_x;
   Gdiplus::REAL indent_y = m_indent_before_y;

   // Recalculation is done in two phases:
   //   1. Recalculated all objects' boundaries and calculate group's bounary as the union.
   //   2. Offset objects' boundaries to fit its alignment.

   for (auto index = 0UL; index < m_object_infos.size(); ++index)
   {
      if (IsObjectVisible(index))
      {
         auto& object_info = m_object_infos[index];
         assert(object_info.m_object);

         object_info.m_object->RecalculateBoundary
         (
            indent_x + ((GroupType::Horizontal == m_type) ? m_boundary.GetRight() : m_boundary.GetLeft()),
            indent_y + ((GroupType::Vertical == m_type) ? m_boundary.GetBottom() : m_boundary.GetTop()),
            graphics
         );

         if (GroupType::Horizontal == m_type)
         {
            indent_x = object_info.m_indent_after;
            indent_y = 0;
         }
         else
         {
            indent_x = 0;
            indent_y = object_info.m_indent_after;
         }
   
         Gdiplus::RectF::Union(m_boundary, m_boundary, object_info.m_object->GetBoundary());
      }
   }

   for (auto index = 0UL; index < m_object_infos.size(); ++index)
   {
      if (IsObjectVisible(index))
      {
         auto& object_info = m_object_infos[index];
         if (object_info.m_aligning != AligningType::Min)
         {
            const auto& object_boundary = object_info.m_object->GetBoundary();

            Gdiplus::REAL offset_x = 0;
            Gdiplus::REAL offset_y = 0;

            if (GroupType::Horizontal == m_type && m_boundary.Height > object_boundary.Height)
            {
               offset_y = m_boundary.Height - object_boundary.Height;
               if (AligningType::Middle == object_info.m_aligning)
               {
                  offset_y /= 2;
               }
            }
            else if (GroupType::Vertical == m_type && m_boundary.Width > object_boundary.Width)
            {
               offset_x = m_boundary.Width - object_boundary.Width;
               if (AligningType::Middle == object_info.m_aligning)
               {
                  offset_x /= 2;
               }
            }

            object_info.m_object->OffsetBoundary(offset_x, offset_y);
         }
      }
   }
}

void Group::Draw(Gdiplus::Graphics* graphics) const
{
   for (auto index = 0UL; index < m_object_infos.size(); ++index)
   {
      if (IsObjectVisible(index))
      {
         m_object_infos[index].m_object->Draw(graphics);
      }
   }
}

Object::ClickType Group::ProcessClick(long x, long y, TULongVector& group_indexes)
{
   for (auto index = 0UL; index < m_object_infos.size(); ++index)
   {
      if (IsObjectVisible(index))
      {
         const auto click = m_object_infos[index].m_object->ProcessClick(x, y, group_indexes);
         if (click != ClickType::NoClick)
         {
            group_indexes.push_front(index);
            return click;
         }
      }
   }
   
   return ClickType::NoClick;
}

void Group::ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects)
{
   for (auto index = 0UL; index < m_object_infos.size(); ++index)
   {
      if (IsObjectVisible(index))
      {
         m_object_infos[index].m_object->ProcessHover(x, y, invalidated_objects);
      }
   }
}

void Group::OffsetBoundary(Gdiplus::REAL offset_x, Gdiplus::REAL offset_y)
{
   Object::OffsetBoundary(offset_x, offset_y);
   for (auto index = 0UL; index < m_object_infos.size(); ++index)
   {
      if (IsObjectVisible(index))
      {
         m_object_infos[index].m_object->OffsetBoundary(offset_x, offset_y);
      }
   }
}

bool Group::IsObjectVisible(unsigned long index) const
{
   return true;
}

} // namespace BGO
