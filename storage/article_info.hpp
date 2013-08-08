#pragma once

#include "../std/string.hpp"
#include "../std/algorithm.hpp"


namespace wr { class Writer; }
namespace rd { class Reader; }
namespace storage { class DoAddEntries; }

const int EMPTY_COORD = 1000;


class ArticleInfo
{
  void GenerateKey();

protected:
  string m_key;
  string m_title;
  string m_url;
  string m_thumbnailUrl;
  uint32_t m_length;
  int32_t m_parentIndex;  // NO_PARENT is the root article
  bool m_redirect;

  static const int32_t NO_PARENT = -1;

  /// Need access for protected fields in Build and Storage classes.
  friend class Storage;
  friend class StorageBuilder;
  friend class storage::DoAddEntries;

public:
  ArticleInfo()
    : m_length(0), m_parentIndex(NO_PARENT),
      m_redirect(false), m_lat(EMPTY_COORD), m_lon(EMPTY_COORD)
  {
  }

  ArticleInfo(string const & title)
    : m_title(title), m_length(0), m_parentIndex(NO_PARENT),
      m_redirect(false), m_lat(EMPTY_COORD), m_lon(EMPTY_COORD)
  {
    GenerateKey();
  }

  ArticleInfo(string const & title, ArticleInfo const & src, bool redirect)
    : m_title(title), m_url(src.m_url), m_thumbnailUrl(src.m_thumbnailUrl),
      m_length(src.m_length), m_parentIndex(NO_PARENT),
      m_redirect(redirect), m_lat(src.m_lat), m_lon(src.m_lon)
  {
    GenerateKey();
  }

  string const & GetTitle() const { return m_title; }
  string const & GetUrl() const { return m_url; }
  string const & GetThumbnailUrl() const { return m_thumbnailUrl; }

  double m_lat, m_lon;
  bool IsValidCoordinates() const;

  void Write(wr::Writer & w) const;
  void Read(rd::Reader & r);

  /// Calculate score for info. Less is better.
  double Score(double currLat, double currLon) const;

  void Swap(ArticleInfo & i);

  bool operator == (ArticleInfo const & r) const;

  struct LessStorage
  {
    bool operator() (ArticleInfo const & i1, ArticleInfo const & i2) const
    {
      return (i1.m_key < i2.m_key);
    }
  };

  class LessPrefix
  {
    int Compare(ArticleInfo const & info, string const & prefix) const
    {
      size_t const count = min(info.m_key.size(), prefix.size());
      for (size_t i = 0; i < count; ++i)
      {
        if (info.m_key[i] < prefix[i])
          return -1;
        else if (info.m_key[i] > prefix[i])
          return 1;
      }
      return (info.m_key.size() < prefix.size() ? -1 : 0);
    }

  public:
    bool operator() (ArticleInfo const & info, string const & prefix) const
    {
      return (Compare(info, prefix) == -1);
    }
    bool operator() (string const & prefix, ArticleInfo const & info) const
    {
      return (Compare(info, prefix) == 1);
    }
  };
};

inline void swap(ArticleInfo & a1, ArticleInfo & a2)
{
  a1.Swap(a2);
}

inline string ToString(ArticleInfo const & i)
{
  return i.GetTitle();
}
