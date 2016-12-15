#pragma once

#include "graphic_objects.h"
#include "sticker.h"

// Sticker graphic objects namespace
namespace SGO
{

class ItemDate : public BGO::Text
{
public:
   ItemDate();
};

class ItemTime : public BGO::Text
{
public:
   ItemTime();
};

class ItemDescription : public BGO::ClickableText
{
public:
   ItemDescription();
};

class SectionItem : public BGO::Group
{
public:
   SectionItem();

   bool SetImage(ImageType image);
   bool SetDate(const char* text);
   bool SetTime(const char* text);
   bool SetDescription(const char* text);
   bool SetClickable(bool is_clickable);

private:
   enum Indexes { idxImage, idxDate, idxTime, idxDesc, idxLast };
};

// TODO: Change text filling
class HeaderDescription : public BGO::Text
{
public:
   HeaderDescription();
};

class SectionHeader : public BGO::Group
{
public:
   SectionHeader();

   bool SetImage(ImageType image);
   bool SetDescription(const char* text);

private:
   enum Indexes { idxImage, idxDesc, idxLast };
};

class FooterPrefix : public BGO::Text
{
public:
   FooterPrefix();
};

class FooterDescription : public BGO::Text
{
public:
   FooterDescription();
};

class SectionFooter : public BGO::Group
{
public:
   SectionFooter();

   bool SetImage(ImageType image);
   bool SetPrefix(const char* text);
   bool SetDescription(const char* text);

private:
   enum Indexes { idxImage, idxPrefix, idxDesc, idxLast };
};

class TitleDescription : public BGO::CollapsibleText
{
public:
   TitleDescription();
};

class SectionTitle : public BGO::Group
{
public:
   SectionTitle();

   const TitleDescription& GetDescription() const;
   TitleDescription& GetDescription();

   bool SetImage(ImageType image);
   bool SetDate(const char* text);
   bool SetTime(const char* text);
   bool SetDescription(const char* text);

protected:
   // Group override
   bool IsObjectVisible(unsigned long index) const override;

private:
   enum Indexes { idxImage, idxDate, idxTime, idxDesc, idxLast };
};

class SectionLine : public BGO::Line
{
public:
   SectionLine();
};

class OwnerName : public BGO::Text
{
public:
   OwnerName();
};

class Sections;

class Section : public BGO::Group, public ISection
{
   // In order to access Indexes
   friend class Sections;

public:
   Section(Sticker& sticker);

   const SectionTitle& GetTitle() const;
   SectionTitle& GetTitle();
   
   // ISection overrides
   virtual void SetOwnerName(const char* name) override;
   virtual void SetTitle(ImageType image, const char* date, const char* time, const char* desc) override;
   virtual void SetHeader(ImageType image, const char* desc) override;
   virtual void SetFooter(ImageType image, const char* prefix, const char* desc) override;

   virtual void SetItemCount(unsigned long count) override;
   virtual void SetItem(unsigned long index, ImageType image, const char* date, const char* time,
                        const char* desc, bool is_clickable) override;
protected:
   // Group overrides
   virtual bool IsObjectVisible(unsigned long index) const override;

private:
   enum Indexes { idxLineBefore, idxTitle, idxHeader, idxItems, idxFooter, idxLineAfter, idxLast };
   Sticker& m_sticker;
   OwnerName m_owner_name;
};

class Sections : public BGO::Group
{
public:
   Sections(Sticker& sticker);
   
   void SetSectionCount(unsigned long count);
   unsigned long GetSectionCount() const;
   const Section& GetSection(unsigned long index) const;
   Section& GetSection(unsigned long index);
   
   void SetShorted(bool is_shorted);
   bool GetShorted() const;
   void CollapseAllExcludingFirst();

   // Group overrides
   virtual ClickType ProcessClick(long x, long y, BGO::TULongVector& group_indexes) override;
   
protected:
   virtual bool IsObjectVisible(unsigned long index) const override;
   
private:
   Sticker& m_sticker;
   bool m_is_shorted;
};

class MoreDescription : public BGO::ClickableText
{
public:
   MoreDescription();
   void SetMoreCount(unsigned long count);
};

class More : public BGO::Group
{
public:
   More();
   
   void SetMoreCount(unsigned long count);
   
private:
   enum Indexes { idxImage, idxDesc, idxLast };
};

class StickerObject : public BGO::Group
{
public:
   StickerObject(Sticker& sticker);

   void Initialize(const RECT& boundary);

   void SetSectionCount(unsigned long count);
   unsigned long GetSectionCount() const;
   const Section& GetSection(unsigned long index) const;
   Section& GetSection(unsigned long index);
   
   ClickType ProcessClick(long x, long y);

   // Group overrides
   virtual void RecalculateBoundary(Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::Graphics* graphics) override;
   virtual void Draw(Gdiplus::Graphics* graphics) const override;
   virtual ClickType ProcessClick(long x, long y, BGO::TULongVector& group_indexes) override;
   
protected:
   virtual bool IsObjectVisible(unsigned long index) const override;
   
   const Sections& GetSections() const;
   Sections& GetSections();
   More& GetMore();

private:
   enum Indexes { idxSections, idxMore, idxLast };

   Gdiplus::RectF m_collapsed_boundary;
   bool m_is_collapsed;
   Sticker& m_sticker;
};

} // namespace SGO


