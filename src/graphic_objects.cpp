﻿#include "graphic_objects.h"

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
   auto ret = ::MultiByteToWideChar(CP_ACP, 0, input, input_size, output.get(), output_size);
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

Gdiplus::Color Text::GetFontColor() const
{
   return m_font_color;
}


/////////// class ClickableText ////////////

ClickableText::ClickableText(
   const Gdiplus::Color& back_color, const wchar_t* font_name, unsigned long font_size,
   unsigned long font_style, const Gdiplus::Color& font_color, bool is_clickable) :
      Text(back_color, font_name, font_size, font_style, font_color),
      m_is_clickable(is_clickable), m_is_clickable_view(false)
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
   return (GetBoundary().Contains(x, y) == TRUE) ? ClickType::ClickDone : ClickType::NoClick;
}

void ClickableText::ProcessHover(long x, long y, TObjectPtrVector& invalidated_objects)
{
   if (m_is_clickable)
   {
      const auto does_contain_cursor = (GetBoundary().Contains(x, y) == TRUE);
      if (does_contain_cursor != m_is_clickable_view)
      {
         m_is_clickable_view = does_contain_cursor;
         invalidated_objects.push_back(this);
      }
   }
}

unsigned long ClickableText::GetFontStyle() const
{
   const auto font_style = Text::GetFontStyle();
   return m_is_clickable_view ? (font_style | Gdiplus::FontStyleUnderline) : font_style;
}

///////////// class CollapsibleText ////////////////

CollapsibleText::CollapsibleText(
   const Gdiplus::Color& back_color, const wchar_t* font_name, unsigned long font_size,
   unsigned long font_style, const Gdiplus::Color& font_color,
   unsigned long collapsed_font_size, const Gdiplus::Color& collapsed_font_color) :
      ClickableText(back_color, font_name, font_size, font_style, font_color, true),
      m_collapsed_font_size(collapsed_font_size), m_collapsed_font_color(collapsed_font_color), m_is_collapsed(true)
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
   if (ClickableText::ProcessClick(x, y, group_indexes) != ClickType::NoClick)
   {
      m_is_collapsed = !m_is_collapsed;
      return ClickType::ClickDoneNeedResize;
   }
   return ClickType::NoClick;
}

unsigned long CollapsibleText::GetFontSize() const
{
   return m_is_collapsed ? m_collapsed_font_size : ClickableText::GetFontSize();
}

Gdiplus::Color CollapsibleText::GetFontColor() const
{
   return m_is_collapsed ? m_collapsed_font_color : ClickableText::GetFontColor();
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
         if (AligningType::MaxCoordinate == object_info.m_aligning)
         {
            const auto& object_boundary = object_info.m_object->GetBoundary();

            Gdiplus::REAL offset_x = 0;
            Gdiplus::REAL offset_y = 0;

            if (GroupType::Horizontal == m_type && m_boundary.Height > object_boundary.Height)
            {
               offset_y = m_boundary.Height - object_boundary.Height;
            }
            if (GroupType::Vertical == m_type && m_boundary.Width > object_boundary.Width)
            {
               offset_x = m_boundary.Width - object_boundary.Width;
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

bool Group::IsObjectVisible(unsigned long index) const
{
   return true;
}

} // namespace BGO
