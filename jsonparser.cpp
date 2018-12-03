/*
 *      Copyright (C) 2014 Jean-Luc Barriere
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301 USA
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "jsonparser.h"

#include <QtGlobal>
#include <QDebug>
#include <QString>

///////////////////////////////////////////////////////////////////////////////
////
//// Node
////

JSON::Node::Node()
: m_value(sajson::TYPE_NULL, 0, 0)
{
}

JSON::Node::Node(const sajson::value& value)
: m_value(value)
{
}

bool JSON::Node::IsNull() const
{
  return (m_value.get_type() == sajson::TYPE_NULL);
}

bool JSON::Node::IsObject() const
{
  return (m_value.get_type() == sajson::TYPE_OBJECT);
}

bool JSON::Node::IsArray() const
{
  return (m_value.get_type() == sajson::TYPE_ARRAY);
}

bool JSON::Node::IsString() const
{
  return (m_value.get_type() == sajson::TYPE_STRING);
}

bool JSON::Node::IsDouble() const
{
  return (m_value.get_type() == sajson::TYPE_DOUBLE);
}

bool JSON::Node::IsInt() const
{
  return (m_value.get_type() == sajson::TYPE_INTEGER);
}

bool JSON::Node::IsTrue() const
{
  return (m_value.get_type() == sajson::TYPE_TRUE);
}

bool JSON::Node::IsFalse() const
{
  return (m_value.get_type() == sajson::TYPE_FALSE);
}

std::string JSON::Node::GetStringValue() const
{
  if (m_value.get_type() == sajson::TYPE_STRING)
    return m_value.as_string();
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return std::string();
}

size_t JSON::Node::GetStringSize() const
{
  if (m_value.get_type() == sajson::TYPE_STRING)
    return m_value.get_string_length();
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return 0;
}

double JSON::Node::GetDoubleValue() const
{
  if (m_value.get_type() == sajson::TYPE_DOUBLE)
    return m_value.get_double_value();
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return 0.0;
}

int64_t JSON::Node::GetBigIntValue() const
{
  if (m_value.get_type() == sajson::TYPE_DOUBLE || m_value.get_type() == sajson::TYPE_INTEGER)
    return (int64_t) m_value.get_number_value();
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return 0;
}

int32_t JSON::Node::GetIntValue() const
{
  if (m_value.get_type() == sajson::TYPE_INTEGER)
    return (int32_t) m_value.get_integer_value();
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return 0;
}

size_t JSON::Node::Size() const
{
  if (m_value.get_type() == sajson::TYPE_ARRAY || m_value.get_type() == sajson::TYPE_OBJECT)
    return m_value.get_length();
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return 0;
}

JSON::Node JSON::Node::GetArrayElement(size_t index) const
{
  if (m_value.get_type() == sajson::TYPE_ARRAY)
    return Node(m_value.get_array_element(index));
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return Node();
}

std::string JSON::Node::GetObjectKey(size_t index) const
{
  if (m_value.get_type() == sajson::TYPE_OBJECT)
    return m_value.get_object_key(index).as_string();
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return std::string();
}

JSON::Node JSON::Node::GetObjectValue(size_t index) const
{
  if (m_value.get_type() == sajson::TYPE_OBJECT)
    return Node(m_value.get_object_value(index));
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return Node();
}

JSON::Node JSON::Node::GetObjectValue(const char *key) const
{
  if (m_value.get_type() == sajson::TYPE_OBJECT)
  {
    size_t idx = m_value.find_object_key(sajson::literal(key));
    if (idx < m_value.get_length())
      return Node(m_value.get_object_value(idx));
    return Node();
  }
  qCritical() << QString(__FUNCTION__) + ": bad type";
  return Node();
}

///////////////////////////////////////////////////////////////////////////////
////
//// Document
////

JSON::Document::Document(const std::string& doc)
: m_isValid(false)
, m_document(NULL)
{
  if (!doc.empty())
  {
    // Parse JSON content
    m_document = new sajson::document(sajson::parse(sajson::string(doc.c_str(), doc.length())));
    if (!m_document)
      qCritical() << QString(__FUNCTION__) + ": memory allocation failed";
    else if (!m_document->is_valid())
      qCritical() << QString(__FUNCTION__) + ": failed to parse: " + m_document->get_error_message().c_str();
    else
      m_isValid = true;
  }
  else
  {
    qCritical() << QString(__FUNCTION__) + ": read error";
  }
}

JSON::Node JSON::Document::GetRoot() const
{
  if (m_document)
    return Node(m_document->get_root());
  return Node();
}

