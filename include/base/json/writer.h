// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_JSON_JSON_WRITER_H_
#define BASE_JSON_JSON_WRITER_H_

#include "value.h"
#include <vector>
#include <string>
#include <ostream>

// Disable warning C4251: <data member>: <type> needs to have dll-interface to
// be used by...
#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING) && defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

#pragma pack(push, 8)
namespace base {
namespace json {

class Value;

/**

Usage:
\code
  using namespace Json;
  void writeToStdout(StreamWriter::Factory const& factory, Value const& value) {
    std::unique_ptr<StreamWriter> const writer(
      factory.newStreamWriter());
    writer->write(value, &std::cout);
    std::cout << std::endl;  // add lf and flush
  }
\endcode
*/
class BASE_EXPORT StreamWriter {
protected:
  JSONCPP_OSTREAM* sout_;  // not owned; will not delete
public:
  StreamWriter();
  virtual ~StreamWriter();
  /** Write Value into document as configured in sub-class.
      Do not take ownership of sout, but maintain a reference during function.
      \pre sout != NULL
      \return zero on success (For now, we always return zero, so check the stream instead.)
      \throw std::exception possibly, depending on configuration
   */
  virtual int write(Value const& root, JSONCPP_OSTREAM* sout) = 0;

  /** \brief A simple abstract factory.
   */
  class BASE_EXPORT Factory {
  public:
    virtual ~Factory();
    /** \brief Allocate a CharReader via operator new().
     * \throw std::exception if something goes wrong (e.g. invalid settings)
     */
    virtual StreamWriter* newStreamWriter() const = 0;
  };  // Factory
};  // StreamWriter

/** \brief Write into stringstream, then return string, for convenience.
 * A StreamWriter will be created from the factory, used, and then deleted.
 */
JSONCPP_STRING BASE_EXPORT writeString(StreamWriter::Factory const& factory, Value const& root);


/** \brief Build a StreamWriter implementation.

Usage:
\code
  using namespace Json;
  Value value = ...;
  StreamWriterBuilder builder;
  builder["commentStyle"] = "None";
  builder["indentation"] = "   ";  // or whatever you like
  std::unique_ptr<Json::StreamWriter> writer(
      builder.newStreamWriter());
  writer->write(value, &std::cout);
  std::cout << std::endl;  // add lf and flush
\endcode
*/
class BASE_EXPORT StreamWriterBuilder : public StreamWriter::Factory {
public:
  // Note: We use a Json::Value so that we can add data-members to this class
  // without a major version bump.
  /** Configuration of this builder.
    Available settings (case-sensitive):
    - "commentStyle": "None" or "All"
    - "indentation":  "<anything>"
    - "enableYAMLCompatibility": false or true
      - slightly change the whitespace around colons
    - "dropNullPlaceholders": false or true
      - Drop the "null" string from the writer's output for nullValues.
        Strictly speaking, this is not valid JSON. But when the output is being
        fed to a browser's JavaScript, it makes for smaller output and the
        browser can handle the output just fine.
    - "useSpecialFloats": false or true
      - If true, outputs non-finite floating point values in the following way:
        NaN values as "NaN", positive infinity as "Infinity", and negative infinity
        as "-Infinity".

    You can examine 'settings_` yourself
    to see the defaults. You can also write and read them just like any
    JSON Value.
    \sa setDefaults()
    */
  json::Value settings_;

  StreamWriterBuilder();
  ~StreamWriterBuilder() JSONCPP_OVERRIDE;

  /**
   * \throw std::exception if something goes wrong (e.g. invalid settings)
   */
  StreamWriter* newStreamWriter() const JSONCPP_OVERRIDE;

  /** \return true if 'settings' are legal and consistent;
   *   otherwise, indicate bad settings via 'invalid'.
   */
  bool validate(json::Value* invalid) const;
  /** A simple way to update a specific setting.
   */
  Value& operator[](JSONCPP_STRING key);

  /** Called by ctor, but you can use this to reset settings_.
   * \pre 'settings' != NULL (but Json::null is fine)
   * \remark Defaults:
   * \snippet src/lib_json/json_writer.cpp StreamWriterBuilderDefaults
   */
  static void setDefaults(json::Value* settings);
};

/** \brief Abstract class for writers.
 * \deprecated Use StreamWriter. (And really, this is an implementation detail.)
 */
class JSONCPP_DEPRECATED("Use StreamWriter instead") BASE_EXPORT Writer {
public:
  virtual ~Writer();

  virtual JSONCPP_STRING write(const Value& root) = 0;
};

/** \brief Outputs a Value in <a HREF="http://www.json.org">JSON</a> format
 *without formatting (not human friendly).
 *
 * The JSON document is written in a single line. It is not intended for 'human'
 *consumption,
 * but may be usefull to support feature such as RPC where bandwith is limited.
 * \sa Reader, Value
 * \deprecated Use StreamWriterBuilder.
 */
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996) // Deriving from deprecated class
#endif
class JSONCPP_DEPRECATED("Use StreamWriterBuilder instead") BASE_EXPORT FastWriter : public Writer {
public:
  FastWriter();
  ~FastWriter() JSONCPP_OVERRIDE {}

  void enableYAMLCompatibility();

  /** \brief Drop the "null" string from the writer's output for nullValues.
   * Strictly speaking, this is not valid JSON. But when the output is being
   * fed to a browser's JavaScript, it makes for smaller output and the
   * browser can handle the output just fine.
   */
  void dropNullPlaceholders();

  void omitEndingLineFeed();

public: // overridden from Writer
  JSONCPP_STRING write(const Value& root) JSONCPP_OVERRIDE;

private:
  void writeValue(const Value& value);

  JSONCPP_STRING document_;
  bool yamlCompatibilityEnabled_;
  bool dropNullPlaceholders_;
  bool omitEndingLineFeed_;
};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

/** \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a
 *human friendly way.
 *
 * The rules for line break and indent are as follow:
 * - Object value:
 *     - if empty then print {} without indent and line break
 *     - if not empty the print '{', line break & indent, print one value per
 *line
 *       and then unindent and line break and print '}'.
 * - Array value:
 *     - if empty then print [] without indent and line break
 *     - if the array contains no object value, empty array or some other value
 *types,
 *       and all the values fit on one lines, then print the array on a single
 *line.
 *     - otherwise, it the values do not fit on one line, or the array contains
 *       object or non empty array, then print one value per line.
 *
 * If the Value have comments then they are outputed according to their
 *#CommentPlacement.
 *
 * \sa Reader, Value, Value::setComment()
 * \deprecated Use StreamWriterBuilder.
 */
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996) // Deriving from deprecated class
#endif
class JSONCPP_DEPRECATED("Use StreamWriterBuilder instead") BASE_EXPORT StyledWriter : public Writer {
public:
  StyledWriter();
  ~StyledWriter() JSONCPP_OVERRIDE {}

public: // overridden from Writer
  /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
   * \param root Value to serialize.
   * \return String containing the JSON document that represents the root value.
   */
  JSONCPP_STRING write(const Value& root) JSONCPP_OVERRIDE;

private:
  void writeValue(const Value& value);
  void writeArrayValue(const Value& value);
  bool isMultilineArray(const Value& value);
  void pushValue(const JSONCPP_STRING& value);
  void writeIndent();
  void writeWithIndent(const JSONCPP_STRING& value);
  void indent();
  void unindent();
  void writeCommentBeforeValue(const Value& root);
  void writeCommentAfterValueOnSameLine(const Value& root);
  bool hasCommentForValue(const Value& value);
  static JSONCPP_STRING normalizeEOL(const JSONCPP_STRING& text);

  typedef std::vector<JSONCPP_STRING> ChildValues;

  ChildValues childValues_;
  JSONCPP_STRING document_;
  JSONCPP_STRING indentString_;
  unsigned int rightMargin_;
  unsigned int indentSize_;
  bool addChildValues_;
};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

/** \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a
 human friendly way,
     to a stream rather than to a string.
 *
 * The rules for line break and indent are as follow:
 * - Object value:
 *     - if empty then print {} without indent and line break
 *     - if not empty the print '{', line break & indent, print one value per
 line
 *       and then unindent and line break and print '}'.
 * - Array value:
 *     - if empty then print [] without indent and line break
 *     - if the array contains no object value, empty array or some other value
 types,
 *       and all the values fit on one lines, then print the array on a single
 line.
 *     - otherwise, it the values do not fit on one line, or the array contains
 *       object or non empty array, then print one value per line.
 *
 * If the Value have comments then they are outputed according to their
 #CommentPlacement.
 *
 * \sa Reader, Value, Value::setComment()
 * \deprecated Use StreamWriterBuilder.
 */
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996) // Deriving from deprecated class
#endif
class JSONCPP_DEPRECATED("Use StreamWriterBuilder instead") BASE_EXPORT StyledStreamWriter {
public:
/**
 * \param indentation Each level will be indented by this amount extra.
 */
  StyledStreamWriter(JSONCPP_STRING indentation = "\t");
  ~StyledStreamWriter() {}

public:
  /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
   * \param out Stream to write to. (Can be ostringstream, e.g.)
   * \param root Value to serialize.
   * \note There is no point in deriving from Writer, since write() should not
   * return a value.
   */
  void write(JSONCPP_OSTREAM& out, const Value& root);

private:
  void writeValue(const Value& value);
  void writeArrayValue(const Value& value);
  bool isMultilineArray(const Value& value);
  void pushValue(const JSONCPP_STRING& value);
  void writeIndent();
  void writeWithIndent(const JSONCPP_STRING& value);
  void indent();
  void unindent();
  void writeCommentBeforeValue(const Value& root);
  void writeCommentAfterValueOnSameLine(const Value& root);
  bool hasCommentForValue(const Value& value);
  static JSONCPP_STRING normalizeEOL(const JSONCPP_STRING& text);

  typedef std::vector<JSONCPP_STRING> ChildValues;

  ChildValues childValues_;
  JSONCPP_OSTREAM* document_;
  JSONCPP_STRING indentString_;
  unsigned int rightMargin_;
  JSONCPP_STRING indentation_;
  bool addChildValues_ : 1;
  bool indented_ : 1;
};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if defined(JSON_HAS_INT64)
JSONCPP_STRING BASE_EXPORT valueToString(Int value);
JSONCPP_STRING BASE_EXPORT valueToString(UInt value);
#endif // if defined(JSON_HAS_INT64)
JSONCPP_STRING BASE_EXPORT valueToString(LargestInt value);
JSONCPP_STRING BASE_EXPORT valueToString(LargestUInt value);
JSONCPP_STRING BASE_EXPORT valueToString(double value);
JSONCPP_STRING BASE_EXPORT valueToString(bool value);
JSONCPP_STRING BASE_EXPORT valueToQuotedString(const char* value);

/// \brief Output using the StyledStreamWriter.
/// \see Json::operator>>()
BASE_EXPORT JSONCPP_OSTREAM& operator<<(JSONCPP_OSTREAM&, const Value& root);

}  // namespace json
}  // namespace base
#pragma pack(pop)

#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(pop)
#endif  // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

#endif  // BASE_JSON_JSON_WRITER_H_
