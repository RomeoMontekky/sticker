#include "sticker_objects.h"

#include <sstream>
#include <cassert>

// Sticker graphic objects namespace
namespace SGO
{

namespace
{

////////////////// Constants //////////////////

const wchar_t g_tahoma_name[] = L"Tahoma";
const auto g_indent_vert = 3UL;
const auto g_indent_horz = 3UL;
const auto g_line_width = 300UL;
const auto g_shorted_section_amount = 2UL;

namespace Colors
{
   const Gdiplus::Color black(0x00, 0x00, 0x00);
   const Gdiplus::Color grey_light(0xCC, 0xCC, 0xCC);
   const Gdiplus::Color grey_very_light(0xF5, 0xF5, 0xF5);
   const Gdiplus::Color grey_dark(0x99, 0x99, 0x99);
   const Gdiplus::Color grey_dark_with_blue(0x89, 0x91, 0xA9);
   const Gdiplus::Color green_dark(0x72, 0xBE, 0x44);
   const Gdiplus::Color blue_dark(0x00, 0x55, 0xBB);
   const Gdiplus::Color red_dark(0xD9, 0x47, 0x00);
}

} // namespace

/////////// class ItemDate //////////

ItemDate::ItemDate() :
   Text(Colors::grey_very_light, g_tahoma_name, 9, Gdiplus::FontStyleRegular, Colors::grey_dark_with_blue)
{}

/////////// class ItemTime //////////

ItemTime::ItemTime() :
   Text(Colors::grey_very_light, g_tahoma_name, 8, Gdiplus::FontStyleRegular, Colors::grey_light)
{}

/////////// class ItemDesctiption //////////

ItemDescription::ItemDescription() :
   ClickableText(Colors::grey_very_light, g_tahoma_name, 9, Gdiplus::FontStyleRegular, Colors::black,
                 Colors::blue_dark)
{}

/////////// class SectionItem //////////

SectionItem::SectionItem() : Group(GroupType::Horizontal)
{
   Group::SetObjectCount(idxLast);
   Group::SetObject(idxImage, std::make_unique<BGO::Image>(), AligningType::Max, g_indent_horz);
   Group::SetObject(idxDate, std::make_unique<ItemDate>(), AligningType::Max, g_indent_horz);
   Group::SetObject(idxTime, std::make_unique<ItemTime>(), AligningType::Max, g_indent_horz);
   Group::SetObject(idxDesc, std::make_unique<ItemDescription>(), AligningType::Max, g_indent_horz);
}

bool SectionItem::SetImage(ImageType image)
{
   //return static_cast<BGO::Image*>(Group::GetObject(idxImage))->SetImage(image);
}

bool SectionItem::SetDate(const char* text)
{
   return static_cast<BGO::Text*>(Group::GetObject(idxDate))->SetText(text);
}

bool SectionItem::SetTime(const char* text)
{
   return static_cast<BGO::Text*>(Group::GetObject(idxTime))->SetText(text);
}

bool SectionItem::SetDescription(const char* text)
{
   return static_cast<BGO::Text*>(Group::GetObject(idxDesc))->SetText(text);
}

bool SectionItem::SetClickable(bool is_clickable)
{
   return static_cast<BGO::ClickableText*>(Group::GetObject(idxDesc))->SetClickable(is_clickable);
}

////////// class HeaderDescription //////////

HeaderDescription::HeaderDescription() :
   Text(Colors::grey_very_light, g_tahoma_name, 9, Gdiplus::FontStyleRegular, Colors::grey_dark)
{}

/////////// class SectionHeader //////////

SectionHeader::SectionHeader() : Group(GroupType::Horizontal)
{
   Group::SetObjectCount(idxLast);
   Group::SetObject(idxImage, std::make_unique<BGO::Image>(), AligningType::Max, g_indent_horz);
   Group::SetObject(idxDesc, std::make_unique<HeaderDescription>(), AligningType::Max, g_indent_horz);
}

bool SectionHeader::SetImage(ImageType image)
{
   //return static_cast<BGO::Image*>(Group::GetObject(idxImage))->SetImage();
}

bool SectionHeader::SetDescription(const char* text)
{
   return static_cast<HeaderDescription*>(Group::GetObject(idxImage))->SetText(text);
}

/////////// class FooterPrefix //////////

FooterPrefix::FooterPrefix() :
   Text(Colors::grey_very_light, g_tahoma_name, 9, Gdiplus::FontStyleRegular, Colors::grey_dark)
{}

/////////// class FooterDescription //////////

FooterDescription::FooterDescription() :
   Text(Colors::grey_very_light, g_tahoma_name, 9, Gdiplus::FontStyleRegular, Colors::black)
{}

///////////// class SectionFooter ////////////////

SectionFooter::SectionFooter() : Group(GroupType::Horizontal)
{
   Group::SetObjectCount(idxLast);
   Group::SetObject(idxImage, std::make_unique<BGO::Image>(), Group::AligningType::Max, g_indent_horz);
   Group::SetObject(idxPrefix, std::make_unique<FooterPrefix>(), Group::AligningType::Max, g_indent_horz);
   Group::SetObject(idxDesc, std::make_unique<FooterDescription>(), Group::AligningType::Max, g_indent_horz);
}

bool SectionFooter::SetImage(ImageType image)
{
   //return static_cast<BGO::Image*>(Group::GetObject(idxImage))->SetImage();
}

bool SectionFooter::SetPrefix(const char* text)
{
   return static_cast<BGO::Text*>(Group::GetObject(idxPrefix))->SetText(text);
}

bool SectionFooter::SetDescription(const char* text)
{
   return static_cast<BGO::Text*>(Group::GetObject(idxDesc))->SetText(text);
}

///////// class TitleDesctiption //////////

TitleDescription ::TitleDescription() :
   CollapsibleText(Colors::grey_very_light, g_tahoma_name, 9, Gdiplus::FontStyleBold, Colors::red_dark,
                   Gdiplus::FontStyleRegular, Colors::grey_dark_with_blue)
{}

/////////// class SectionTitle ////////////

SectionTitle::SectionTitle() : Group(GroupType::Horizontal)
{
   Group::SetObjectCount(idxLast);
   Group::SetObject(idxImage, std::make_unique<BGO::Image>(), AligningType::Max, g_indent_horz);
   Group::SetObject(idxDate, std::make_unique<ItemDate>(), AligningType::Max, g_indent_horz);
   Group::SetObject(idxTime, std::make_unique<ItemTime>(), AligningType::Max, g_indent_horz);
   Group::SetObject(idxDesc, std::make_unique<TitleDescription>(), AligningType::Max, g_indent_horz);
}

const TitleDescription& SectionTitle::GetDescription() const
{
   return *static_cast<const TitleDescription*>(Group::GetObject(idxDesc));
}

TitleDescription& SectionTitle::GetDescription()
{
   return *static_cast<TitleDescription*>(Group::GetObject(idxDesc));
}

bool SectionTitle::SetImage(ImageType image)
{
   //return static_cast<BGO::Image*>(Group::GetObject(idxImage))->SetImage(image);
}

bool SectionTitle::SetDate(const char* text)
{
   return static_cast<ItemDate*>(Group::GetObject(idxDate))->SetText(text);
}

bool SectionTitle::SetTime(const char* text)
{
   return static_cast<ItemTime*>(Group::GetObject(idxTime))->SetText(text);
}

bool SectionTitle::SetDescription(const char* text)
{
   return static_cast<TitleDescription*>(Group::GetObject(idxDesc))->SetText(text);
}

bool SectionTitle::IsObjectVisible(unsigned long index) const
{
   return GetDescription().GetCollapsed() || idxImage == index || idxDesc == index;
}

/////////// class SectionLine /////////////

SectionLine::SectionLine() : BGO::Line(Colors::grey_very_light, Colors::grey_dark, g_line_width)
{}

///////////// class OwnerName /////////////

OwnerName::OwnerName() : BGO::Text(Colors::grey_very_light, g_tahoma_name, 9, Gdiplus::FontStyleRegular, Colors::black)
{}

///////////// class Section ////////////////

Section::Section(Sticker& sticker) : 
   Group(GroupType::Vertical), m_sticker(sticker), m_owner_name()
{
   Group::SetObjectCount(idxLast);
   Group::SetObject(idxLineBefore, std::make_unique<SectionLine>(), AligningType::Min, g_indent_vert);
   Group::SetObject(idxTitle, std::make_unique<SectionTitle>(), AligningType::Min, g_indent_vert);
   Group::SetObject(idxHeader, std::make_unique<SectionHeader>(), AligningType::Min, g_indent_vert);
   Group::SetObject(idxItems, std::make_unique<Group>(GroupType::Vertical), AligningType::Min, g_indent_vert);
   Group::SetObject(idxFooter, std::make_unique<SectionFooter>(), AligningType::Min, g_indent_vert);
   Group::SetObject(idxLineAfter, std::make_unique<SectionLine>(), AligningType::Min, g_indent_vert);
}

const SectionTitle& Section::GetTitle() const
{
   return *static_cast<const SectionTitle*>(Group::GetObject(idxTitle));
}

SectionTitle& Section::GetTitle()
{
   return *static_cast<SectionTitle*>(Group::GetObject(idxTitle));
}

void Section::SetOwnerName(const char* name)
{
   if (m_owner_name.SetText(name))
   {
      m_sticker.SetDirty();
   }
   m_sticker.Update();
}

void Section::SetTitle(ImageType image, const char* date, const char* time, const char* desc)
{
   auto& title = GetTitle();
   if (title.SetImage(image))
   {
      m_sticker.SetDirty();
   }
   if (title.SetDescription(desc))
   {
      m_sticker.SetDirty();
   }
   if (title.SetDate(date))
   {
      m_sticker.SetDirty();
   }
   if (title.SetTime(time))
   {
      m_sticker.SetDirty();
   }
   m_sticker.Update();
}

void Section::SetHeader(ImageType image, const char* desc)
{
   auto header = static_cast<SectionHeader*>(Group::GetObject(idxHeader));
   if (header->SetImage(image))
   {
      m_sticker.SetDirty();
   }
   if (header->SetDescription(desc))
   {
      m_sticker.SetDirty();
   }
   m_sticker.Update();
}

void Section::SetFooter(ImageType image, const char* prefix, const char* desc)
{
   auto footer = static_cast<SectionFooter*>(Group::GetObject(idxFooter));
   if (footer->SetImage(image))
   {
      m_sticker.SetDirty();
   }
   if (footer->SetPrefix(prefix))
   {
      m_sticker.SetDirty();
   }
   if (footer->SetDescription(desc))
   {
      m_sticker.SetDirty();
   }
   m_sticker.Update();
}

void Section::SetItemCount(unsigned long count)
{
   auto items = static_cast<Group*>(Group::GetObject(idxItems));
   if (items->SetObjectCount(count))
   {
      m_sticker.SetDirty();
   }
   m_sticker.Update();
}

void Section::SetItem(unsigned long index, ImageType image, const char* date, const char* time,
                      const char* desc, bool is_clickable)
{
   auto items = static_cast<Group*>(Group::GetObject(idxItems));

   auto item = static_cast<SectionItem*>(items->GetObject(index));
   if (nullptr == item)
   {
      auto item_ptr = std::make_unique<SectionItem>();
      item = item_ptr.get();
      items->SetObject(index, std::move(item_ptr), AligningType::Min, g_indent_vert);
   }

   if (item->SetImage(image))
   {
      m_sticker.SetDirty();
   }
   if (item->SetDate(date))
   {
      m_sticker.SetDirty();
   }
   if (item->SetTime(time))
   {
      m_sticker.SetDirty();
   }
   if (item->SetDescription(desc))
   {
      m_sticker.SetDirty();
   }
   if (item->SetClickable(is_clickable))
   {
      m_sticker.SetDirty();
   }

   m_sticker.Update();
}

void Section::RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics)
{
   Group::RecalculateBoundary(x, y, graphics);
   
   if (!GetTitle().GetDescription().GetCollapsed())
   {
      auto items = static_cast<Group*>(Group::GetObject(idxItems));
      auto first_item = static_cast<SectionItem*>(items->GetObject(0));
      auto& first_item_boundary = first_item->GetBoundary();
      
      // Place owner name on the level of the first section item, margined to left boundary.
      m_owner_name.RecalculateBoundary(first_item_boundary.X, first_item_boundary.Y, graphics);
      m_owner_name.OffsetBoundary(m_boundary.Width - m_owner_name.GetBoundary().Width, 0);
   }
}

void Section::Draw(Gdiplus::Graphics* graphics) const
{
   Group::Draw(graphics);
   if (!GetTitle().GetDescription().GetCollapsed())
   {
      m_owner_name.Draw(graphics);
   }
}

bool Section::IsObjectVisible(unsigned long index) const
{
   return !GetTitle().GetDescription().GetCollapsed() || idxTitle == index;
}

////////// class Sections /////////////

Sections::Sections(Sticker& sticker) : 
   Group(GroupType::Vertical), m_sticker(sticker), m_is_shorted(true)
{
}

void Sections::SetSectionCount(unsigned long count)
{
   if (Group::SetObjectCount(count))
   {
      m_sticker.SetDirty();
      SetShorted(true);
   }
   m_sticker.Update();
}

unsigned long Sections::GetSectionCount() const
{
   return Group::GetObjectCount();
}

const Section& Sections::GetSection(unsigned long index) const
{
   auto section = static_cast<const Section*>(Group::GetObject(index));
   assert(section != nullptr);
   return *section;
}

Section& Sections::GetSection(unsigned long index)
{
   auto section = static_cast<Section*>(Group::GetObject(index));
   if (nullptr == section)
   {
      auto section_ptr = std::make_unique<Section>(m_sticker);
      section = section_ptr.get();
      section->GetTitle().GetDescription().SetCollapsed(index > 0);
      Group::SetObject(index, std::move(section_ptr), AligningType::Min, g_indent_vert);
   }
   return *section;
}

void Sections::SetShorted(bool is_shorted)
{
   m_is_shorted = is_shorted && (Group::GetObjectCount() > g_shorted_section_amount);
}

bool Sections::GetShorted() const
{
   return m_is_shorted;
}

void Sections::CollapseAllExcludingFirst()
{
   const auto count = GetSectionCount();
   GetSection(0).GetTitle().GetDescription().SetCollapsed(false);
   for (auto index = 1UL; index < count; ++index)
   {
      GetSection(index).GetTitle().GetDescription().SetCollapsed(true);
   }
}

// Group overrides
BGO::Object::ClickType Sections::ProcessClick(long x, long y, BGO::TULongVector& group_indexes)
{
   const auto click = Group::ProcessClick(x, y, group_indexes);

   const auto callback = m_sticker.GetCallback();
   if (callback != nullptr && BGO::Object::ClickType::ClickDone == click)
   {
      const auto section_index = group_indexes[0];
      const auto section_object_index = group_indexes[1];

      switch (section_object_index)
      {
         case Section::idxHeader:
         {
            callback->OnHeaderClick(section_index);
            break;
         }
         case Section::idxItems:
         {
            callback->OnItemClick(section_index, group_indexes[2]);
            break;
         }
         case Section::idxFooter:
         {
            callback->OnFooterClick(section_index);
            break;
         }
      }
   }

   return click;
}

bool Sections::IsObjectVisible(unsigned long index) const
{
   return !m_is_shorted || (index < g_shorted_section_amount);
}

///////////// class More ///////////////

More::More() :
   BGO::ClickableText(Colors::grey_very_light, g_tahoma_name, 9, Gdiplus::FontStyleRegular, Colors::black, Colors::blue_dark)
{
   ClickableText::SetClickable(true);
   SetMoreCount(0);
}

void More::SetMoreCount(unsigned long count)
{
   std::stringstream sstream;
   sstream << "More";
   if (count > 0)
   {
      sstream << ' ';
      sstream << count;
   }
   ClickableText::SetText(sstream.str().c_str());
}

////////// class StickerGraphicObject /////////////

StickerObject::StickerObject(Sticker& sticker) :
   Group(GroupType::Vertical), m_collapsed_boundary(), m_is_collapsed(true), m_sticker(sticker)
{
   Group::SetObjectCount(idxLast);
   Group::SetObject(idxSections, std::make_unique<Sections>(sticker), AligningType::Min, g_indent_vert);
   Group::SetObject(idxMore, std::make_unique<More>(), AligningType::Min, g_indent_vert);
}

void StickerObject::Initialize(const RECT& boundary)
{
   m_collapsed_boundary.X = boundary.left;
   m_collapsed_boundary.Y = boundary.top;
   m_collapsed_boundary.Width = boundary.right - boundary.left;
   m_collapsed_boundary.Height = boundary.bottom - boundary.top;
}

void StickerObject::SetSectionCount(unsigned long count)
{
   GetSections().SetSectionCount(count);
   GetMore().SetMoreCount(count - g_shorted_section_amount);
}

unsigned long StickerObject::GetSectionCount() const
{
   return GetSections().GetSectionCount();
}

const Section& StickerObject::GetSection(unsigned long index) const
{
   return GetSections().GetSection(index);
}

Section& StickerObject::GetSection(unsigned long index)
{
   return GetSections().GetSection(index);
}

BGO::Object::ClickType StickerObject::ProcessClick(long x, long y)
{
   BGO::TULongVector group_indexes;
   return ProcessClick(x, y, group_indexes);
}

void StickerObject::RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics)
{
   if (m_is_collapsed)
   {
      GetSection(0).GetTitle().RecalculateBoundary(x, y, graphics);
      m_boundary = m_collapsed_boundary;
   }
   else
   {
      Group::RecalculateBoundary(x, y, graphics);
   }
}

void StickerObject::Draw(Gdiplus::Graphics* graphics) const
{
   Gdiplus::SolidBrush back_brush(Colors::grey_very_light);
   graphics->FillRectangle(&back_brush, GetBoundary());

   if (m_is_collapsed)
   {
      GetSection(0).GetTitle().Draw(graphics);
   }
   else
   {
      Group::Draw(graphics);
   }
}

BGO::Object::ClickType StickerObject::ProcessClick(long x, long y, BGO::TULongVector& group_indexes)
{
   if (m_is_collapsed)
   {
      m_is_collapsed = false;
      return ClickType::ClickDoneNeedResize;
   }
   
   // Call of base implementation
   const auto click = Group::ProcessClick(x, y, group_indexes);

   switch (group_indexes.front())
   {
      case idxSections:
      {
         if (ClickType::ClickDoneNeedResize == click)
         {
            auto& first_section_title = GetSection(0).GetTitle();
            if (first_section_title.GetDescription().GetCollapsed())
            {
               first_section_title.GetDescription().SetCollapsed(false);
               GetSections().CollapseAllExcludingFirst();
               GetSections().SetShorted(true);
               m_is_collapsed = true;
            }
         }
         break;
      }
      case idxMore:
      {
         if (ClickType::ClickDone == click)
         {
            GetSections().SetShorted(false);
            return ClickType::ClickDoneNeedResize;
         }
      }
   }
   
   return click;
}

bool StickerObject::IsObjectVisible(unsigned long index) const
{
   return GetSections().GetShorted() || (idxSections == index);
}

const Sections& StickerObject::GetSections() const
{
   return *static_cast<const Sections*>(Group::GetObject(idxSections));
}

Sections& StickerObject::GetSections()
{
   return *static_cast<Sections*>(Group::GetObject(idxSections));
}

More& StickerObject::GetMore()
{
   return *static_cast<More*>(Group::GetObject(idxMore));
}


} // namespace SGO
