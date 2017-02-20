//
// Amalgamated file containing the whole Sery library.
// Sery is available here : https://github.com/Ninetainedo/Sery
// 
// VERSION : 1.0.0
// 
// Make sure to define SERY_GENERATE_IMPLEMENTATION
// right before including this file in one (and only one)
// of your cpp files.
// 
// Example :
// 
// // File test_sery.cpp :
// #define SERY_GENERATE_IMPLEMENTATION
// #include "Sery_Amalgamated.hh"
// 
// 
// LICENSE
// 
// Sery is licensed under the MIT license available here :
// https://github.com/Ninetainedo/Sery/blob/master/LICENSE
// 
// A copy of this license can be found at the very bottom of this file
// for the sake of completion.
// 

#ifndef SERY_AMALGAMATED_HH_
#define SERY_AMALGAMATED_HH_

////////////////////////////////////////
//              Misc.hh               //
////////////////////////////////////////

#ifndef SERY_MISC_HH_
#define SERY_MISC_HH_

#include <cstdint>
#include <type_traits>

namespace Sery
{

  /**
   *  @brief  Shortcut for using std::enable_if.
   */
  template< bool B, class T = void >
  using enable_if_t = typename ::std::enable_if<B, T>::type;

  /**
   *  @brief  Shortcut for using std::result_of.
   */
  template< class T >
  using result_of_t = typename std::result_of<T>::type;

  /**
   *  @brief  Enum used to represent little of big endian.
   */
  enum Endian
  {
    LittleEndian,
    BigEndian
  };

  typedef std::int8_t   int8;   /**< Shortcut */
  typedef std::int16_t  int16;  /**< Shortcut */
  typedef std::int32_t  int32;  /**< Shortcut */
  typedef std::int64_t  int64;  /**< Shortcut */

  typedef std::uint8_t  uint8;  /**< Shortcut */
  typedef std::uint16_t uint16; /**< Shortcut */
  typedef std::uint32_t uint32; /**< Shortcut */
  typedef std::uint64_t uint64; /**< Shortcut */


#ifndef _DOXYGEN

  namespace       detail
  {

    inline Endian getSoftwareEndian()
    {
      int16       witness = 0x5501;
      int8        test = *((int8*)&witness);
      return (test == 1 ? Endian::LittleEndian : Endian::BigEndian);
    }

  } // namespace detail

#endif // _DOXYGEN

} // namespace Sery

#endif // SERY_MISC_HH_

////////////////////////////////////////
//            IBuffer.hh              //
////////////////////////////////////////

#ifndef SERY_IBUFFER_HH_
#define SERY_IBUFFER_HH_

#include <string>
#include <memory>
#include <functional>

namespace Sery
{

  /**
   * @brief  Base class for Sery::Buffer, represents a byte array
   *         manipulation interface.
   */
  class IBuffer
  {
  public:
    /**
     *  @brief  Virtual destructor for inheritance.
     */
    virtual ~IBuffer()
    {
    }

  public:
    /**
     * @brief  This function appends a raw block of memory to the IBuffer
     * @param  buffer  Pointer to the block to append
     * @param  size    Size of the block to append
     */
    virtual void              writeRaw(const char* buffer, std::size_t size) = 0;

    /**
     *  @brief  This function reads a raw block of memory from the
     *          beginning of the IBuffer. It effectively removes the
     *          @a size first bytes of the internal buffer.
     *
     *  @param[out]   buffer  The output pointer to write the bytes.
     *                        It should be at least @a size long.
     *  @param        size    Size of the block to read
     *
     *  @warning  This function expects @a buffer to be allocated
     *            before the call. Otherwise, it will result in
     *            undefined behavior.
     *
     *  @warning  If @a size is greater than IBuffer::size(), this
     *            is undefined behavior.
     */
    virtual void              readRaw(char* buffer, std::size_t size) = 0;

    /**
     *  @brief   This function will return the current size of the IBuffer.
     *  @return  The size of the internal buffer.
     */
    virtual std::size_t       size() const = 0;

    /**
     *  @brief  This function will return a pointer to a byte array
     *          being the buffer internally hold.
     *  @return A pointer to the internal byte array.
     *
     *  @warning  The pointer returned is read only.
     */
    virtual const char*       data() const = 0;

    /**
     *  @brief  This function completely empties the internal buffer.
     *          Making IBuffer::size() return 0.
     */
    virtual void              clear() = 0;

    /**
     *  @brief  This function removes the first @a num bytes from the internal
     *          buffer.
     *
     *  @param  num The number of bytes to remove.
     */
    virtual void              eraseNBytes(std::size_t num) = 0;

    /**
     *  @brief  Replaces the existing content with given one.
     *  @param  buffer  A pointer to the byte array to use.
     *  @param  size    The size of @a buffer.
     */
    virtual void              setContent(const char* buffer, std::size_t size) = 0;

    /**
    *  @brief   This function will return a string made to represent
    *           the internal data of the buffer.
    *  @param   width The number of bytes written before inserting a
    *                 line break.
    *  @return  A string representing the data stored in the buffer.
    */
    virtual const std::string debug(uint8 width) const = 0;
  };

  /**
   *  @brief  This function is a helper meant to be used with a read-like
   *          callable.
   *
   *  @param  callable    A callable parameter taking two parameters.
   *                      It must be able to take a char * and an IntType.
   *  @param  buffer      The @ref IBuffer to set the content of.
   *  @param  sizeToRead  The size to read from @a callable.
   *
   *  This function will allocate a raw char array and call @a callable
   *  with it. Here is an example passing a lambda :
   *
   *  @code{.cpp}
   *    Sery::Buffer    buffer;
   *    istringstream   stream("Dummy stream");
   *    Sery::readToBuffer([&stream] (char* buf, int size) {
   *                         stream.read(buf, size);
   *                       },
   *                       buffer, 5);
   *    std::cout << buffer.size() << "\n"; // Prints "5";
   *  @endcode
   *
   *  This example will call strea.read to read 5 bytes.
   *  The read bytes will directly be stored inside buffer and
   *  calling buffer.size() after the call will return 5.
   *
   *  @note The template IntType is not needed and is only used
   *        to remove a warning from MSVC.
   *
   */
  template < typename Callable, typename IntType >
  void  readToBuffer(Callable callable,
                     IBuffer& buffer,
                     IntType sizeToRead)
  {
    std::unique_ptr<char[]> charBuffer(new char[sizeToRead]);

    callable(charBuffer.get(), sizeToRead);
    buffer.setContent(charBuffer.get(), sizeToRead);
  }

} // namespace Sery

#endif // SERY_IBUFFER_HH_

////////////////////////////////////////
//             Buffer.hh              //
////////////////////////////////////////

#ifndef SERY_BUFFER_HH_
#define SERY_BUFFER_HH_

#include <vector>

namespace Sery
{

  /**
   * @brief Sery's default implementation of @ref IBuffer
   *        Represents a buffer using a std::vector<char>
   *        to store its data.
   */
  class Buffer final : public IBuffer
  {
  public:
    /**
     *  @brief  Default constructor
     */
    Buffer();

    /**
     *  @brief  Buffer constructor. Will initialize the internal
     *          content to that pointed by @a buffer.
     *  @param  buffer  Pointer to the byte array to store.
     *  @param  size    The length of @a buffer.
     */
    Buffer(const char* buffer, std::size_t size);

    /**
     *  @brief  Destructor.
     */
    ~Buffer();

  public:
    /**
    * @brief  This function appends a raw block of memory to the internal
    *         std::vector<char>.
    * @param  buffer  Pointer to the block to append
    * @param  size    Size of the block to append
    */
    virtual void              writeRaw(const char* buffer, std::size_t size) final;

    /**
    *  @brief  This function reads a raw block of memory from the
    *          beginning of the Buffer. It effectively removes the
    *          @a size first bytes of the internal buffer.
    *
    *  @param[out]   buffer  The output pointer to write the bytes.
    *                        It should be at least @a size long.
    *  @param        size    Size of the block to read
    *
    *  @warning  This function expects @a buffer to be allocated
    *            before the call. Otherwise, it will result in
    *            undefined behavior.
    *
    *  @warning  If @a size is greater than IBuffer::size(), this
    *            is undefined behavior.
    */
    virtual void              readRaw(char* buffer, std::size_t size) final;

    /**
    *  @brief   This function will return the current size of the Buffer.
    *  @return  The size of the internal buffer.
    */
    virtual std::size_t       size() const final;

    /**
    *  @brief  This function will return a pointer to a byte array
    *          being the buffer internally hold.
    *  @return A pointer to the internal byte array.
    *
    *  @warning  The pointer returned is read only.
    */
    virtual const char*       data() const final;

    /**
    *  @brief  This function completely empties the internal buffer.
    *          Making Buffer::size() return 0.
    */
    virtual void              clear() final;

    /**
     *  @brief  This function removes the first @a num bytes from the internal
     *          buffer.
     *
     *  @param  num The number of bytes to remove.
     */
    virtual void              eraseNBytes(std::size_t num) final;

    /**
    *  @brief  Replaces the existing content with given one.
    *  @param  buffer  A pointer to the byte array to use.
    *  @param  size    The size of @a buffer.
    */
    virtual void              setContent(const char* buffer, std::size_t size) final;

    /**
    *  @brief   This function will return a string made to represent
    *           the internal data of the buffer.
    *  @param   width The number of bytes written before inserting a
    *                 line break.
    *  @return  A string representing the data stored in the buffer.
    */
    virtual const std::string         debug(uint8 width) const final;

  private:
    std::vector<char>         _buffer;    /**< Internal buffer */
    std::size_t               _pos;       /**< Current position in buffer */
  };

} // namespace Sery

#endif // SERY_BUFFER_HH_

////////////////////////////////////////
//             Stream.hh              //
////////////////////////////////////////

#ifndef SERY_STREAM_HH_
#define SERY_STREAM_HH_

namespace Sery
{

  /**
   *  @brief  Class used to manipulate @ref IBuffer easier.
   */
  class Stream final
  {
  private:
    // Removing copy and move functions
    Stream(const Stream&) = delete;
    Stream(Stream&&) = delete;
    Stream& operator=(const Stream&) = delete;
    Stream& operator=(Stream&&) = delete;

  private:
    static Endian   globalEndian;   /**< Used as a default value for Stream::_localEndian */

  public:
    /**
     *  @brief  Updates the global endian.
     *  @param  endian  The new endian value.
     */
    static void     setGlobalEndian(Endian endian);

    /**
     *  @brief  Gets the global endian.
     *  @return The global endian.
     */
    static Endian   getGlobalEndian();

  public:
    /**
     *  @brief  Endian constructor.
     *          Allows to manipulate the given @a buffer using a specific endian
     *          (different from the global one)
     *  @param  buffer      The IBuffer to use.
     *  @param  localEndian The endian to use.
     */
    Stream(IBuffer& buffer, Endian localEndian);

    /**
     *  @brief  Basic constructor.
     *          Allows to manipulate the given @a buffer.
     *          Sets Stream::globalEndian as the localEndian.
     *  @param  buffer      The IBuffer to use.
     */
    Stream(IBuffer& buffer);

    /**
     *  @brief  Destructor.
     */
    ~Stream();

  public:
    /**
     *  @brief  Proxy to call IBuffer::writeRaw.
     *  @param  buffer  The buffer to append.
     *  @param  size    The size of @a buffer.
     *
     *  @sa IBuffer::writeRaw
     */
    Stream& writeRaw(const char* buffer, uint32 size);

    /**
     *  @brief  Proxy to call IBuffer::readRaw.
     *  @param[out]  buffer  The output pointer to write the bytes.
     *                       It should be at least @a size long.
     *  @param       size    The size of @a buffer.
     *
     *  @warning  The warnings of @ref IBuffer::readRaw are still relevant.
     *
     *  @sa IBuffer::readRaw
     */
    Stream& readRaw(char* buffer, uint32 size);

    /**
     *  @brief  This overload of operator>> will deserialize an arithmetic type.
     *  @param      stream  The stream to deserialize @a t from.
     *  @param[out] t       The object to set .
     *  @return *this.
     *
     *  @warning  Thanks to SFINAE, this function won't be available for
     *            non-arithmetics types.
     */
    template <class T, enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
    Stream& operator>>(T& t);

  public:
    /**
     *  @brief  Returns the endian used by the current Stream.
     *  @return The local endian.
     */
    Endian  getLocalEndian() const;

  private:
    IBuffer&  _buffer;        /**< The buffer bound to this instance. */
    Endian    _localEndian;   /**< The endian used by this Stream. */
  };


  /**
   *  @brief  This overload of operator<< will serialize a boolean.
   *  @param  stream  The stream to serialize @a value to.
   *  @param  value   The boolean to serialize.
   *  @return *this.
   */
  Stream& operator<<(Stream& stream, const bool value);

  /**
   *  @brief  This overload of operator>> will deserialize a boolean.
   *  @param      stream  The stream to deserialize @a value from.
   *  @param[out] value   The reference to set.
   *  @return *this.
   */
  Stream& operator>>(Stream& stream, bool& value);


  /**
   *  @brief  This overload of operator<< will serialize a C-style string.
   *  @param  stream  The stream to serialize @a str to.
   *  @param  str     The string to serialize.
   *  @return *this.
   */
  Stream& operator<<(Stream& stream, const char* str);

  /**
   *  @brief  This overload of operator>> will deserialize a C-style string.
   *          The memory will be allocated by the function.
   *  @param      stream  The stream to deserialize @a str from.
   *  @param[out] str     The pointer to store the string.
   *  @return *this.
   */
  Stream& operator>>(Stream& stream, char*& str);


  /**
   *  @brief  This overload of operator<< will serialize an arithmetic type.
   *  @param  stream  The stream to serialize @a t to.
   *  @param  t       The arithmetic type to serialize.
   *  @return *this.
   *
   *  @warning  Thanks to SFINAE, this function won't be available for
   *            non-arithmetics types.
   *
   */
  template <class T, enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
  Stream& operator<<(Stream& stream, T t);

} // namespace Sery

////////////////////////////////////////
//           Stream_STD.hh            //
////////////////////////////////////////

#ifndef SERY_STREAM_STD_HH_
#define SERY_STREAM_STD_HH_

///////////////////////////////////////////////////////
// This file is about standard classes serialization //
///////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

namespace Sery
{

  // std::string
  /**
   *  @brief  This overload of operator<< will serialize a std::string.
   *  @param  stream  The stream to serialize @a str to.
   *  @param  str     The std::string to serialize.
   *  @return *this.
   */
  Stream&    operator<<(Stream& stream, const std::string& str);

  /**
   *  @brief  This overload of operator>> will deserialize a std::string.
   *  @param      stream  The stream to deserialize @a str from.
   *  @param[out] str     The std::string to set.
   *  @return *this.
   */
  Stream&    operator>>(Stream& stream, std::string& str);

  // std::vector
  /**
   *  @brief  This overload of operator<< will serialize a std::vector.
   *  @param  stream  The stream to serialize @a vec to.
   *  @param  vec     The std::vector to serialize.
   *  @return *this.
   */
  template <class T>
  Stream&    operator<<(Stream& stream, const std::vector<T>& vec)
  {
    stream << static_cast<uint32>(vec.size());
    for (auto&& element : vec)
      stream << element;
    return (stream);
  }
  /**
   *  @brief  This overload of operator>> will deserialize a std::vector.
   *  @param      stream  The stream to deserialize @a vec from.
   *  @param[out] vec     The std::vector to set.
   *  @return *this.
   */
  template <class T>
  Stream&           operator>>(Stream& stream, std::vector<T>& vec)
  {
    uint32          size = 0;
    std::vector<T>  tmpVec;

    stream>>size;
    tmpVec.reserve(size);
    while (size--)
    {
      T t;
      stream>>t;
      tmpVec.push_back(t);
    }
    vec = std::move(tmpVec);
    return (stream);
  }

  // std::list
  /**
   *  @brief  This overload of operator<< will serialize a std::list.
   *  @param  stream  The stream to serialize @a list to.
   *  @param  list    The std::list to serialize.
   *  @return *this.
   */
  template <class T>
  Stream&    operator<<(Stream& stream, const std::list<T>& list)
  {
    stream << static_cast<uint32>(list.size());
    for (auto&& element : list)
      stream << element;
    return (stream);
  }
  /**
   *  @brief  This overload of operator>> will deserialize a std::list.
   *  @param      stream  The stream to deserialize @a list from.
   *  @param[out] list    The std::list to set.
   *  @return *this.
   */
  template <class T>
  Stream&          operator>>(Stream& stream, std::list<T>& list)
  {
    uint32        size = 0;
    std::list<T>  tmpList;

    stream>>size;
    while (size--)
    {
      T t;
      stream>>t;
      tmpList.push_back(t);
    }
    list = std::move(tmpList);
    return (stream);
  }

  // std::pair
  /**
   *  @brief  This overload of operator<< will serialize a std::pair.
   *  @param  stream  The stream to serialize @a pair to.
   *  @param  pair    The std::pair to serialize.
   *  @return *this.
   */
  template <class T, class U>
  Stream&    operator<<(Stream& stream, const std::pair<T, U>& pair)
  {
    stream << pair.first;
    stream << pair.second;
    return (stream);
  }
  /**
   *  @brief  This overload of operator>> will deserialize a std::pair.
   *  @param      stream  The stream to deserialize @a pair from.
   *  @param[out] pair    The std::pair to set.
   *  @return *this.
   */
  template <class T, class U>
  Stream&    operator>>(Stream& stream, std::pair<T, U>& pair)
  {
    stream>>pair.first;
    stream>>pair.second;
    return (stream);
  }

  // std::map
  /**
   *  @brief  This overload of operator<< will serialize a std::map.
   *  @param  stream  The stream to serialize @a map to.
   *  @param  map     The std::map to serialize.
   *  @return *this.
   */
  template <class T, class U>
  Stream&    operator<<(Stream& stream, const std::map<T, U>& map)
  {
    stream << static_cast<uint32>(map.size());
    for (auto&& oPair : map)
      stream << oPair;
    return (stream);
  }
  /**
   *  @brief  This overload of operator>> will deserialize a std::map.
   *  @param      stream  The stream to deserialize @a map from.
   *  @param[out] map     The std::map to set.
   *  @return *this.
   */
  template <class T, class U>
  Stream&            operator>>(Stream& stream, std::map<T, U>& map)
  {
    uint32          size = 0;
    std::map<T, U>  tmpMap;

    stream>>size;
    while (size--)
    {
      std::pair<T, U> pair;
      stream>>pair;
      tmpMap.insert(pair);
    }
    map = std::move(tmpMap);
    return (stream);
  }

  // std::set
  /**
   *  @brief  This overload of operator<< will serialize a std::set.
   *  @param  stream  The stream to serialize @a set to.
   *  @param  set     The std::set to serialize.
   *  @return *this.
   */
  template <class T>
  Stream&    operator<<(Stream& stream, const std::set<T>& set)
  {
    stream << static_cast<uint32>(set.size());
    for (auto&& element : set)
      stream << element;
    return (stream);
  }
  /**
   *  @brief  This overload of operator>> will deserialize a std::set.
   *  @param      stream  The stream to deserialize @a set from.
   *  @param[out] set     The std::set to set.
   *  @return *this.
   */
  template <class T>
  Stream&          operator>>(Stream& stream, std::set<T>& set)
  {
    uint32        size = 0;
    std::set<T>   tmpSet;

    stream>>size;
    while (size--)
    {
      T t;
      stream>>t;
      tmpSet.insert(t);
    }
    set = std::move(tmpSet);
    return (stream);
  }

} // namespace Sery

#endif // SERY_STREAM_STD_HH_

#endif // SERY_STREAM_HH_

#endif // SERY_AMALGAMATED_HH_

#ifdef SERY_GENERATE_IMPLEMENTATION

////////////////////////////////////////
//             Buffer.cpp             //
////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include <string.h>
#include <cassert>

namespace Sery
{

  Buffer::Buffer()
    : _buffer()
    , _pos(0)
  {
  }

  Buffer::Buffer(const char* buffer, std::size_t size)
    : _buffer(buffer, buffer + size)
    , _pos(0)
  {
  }

  Buffer::~Buffer()
  {
  }

  void        Buffer::writeRaw(const char* buffer, std::size_t size)
  {
    _buffer.insert(_buffer.end(), buffer, buffer + size);
  }

  void        Buffer::readRaw(char* buffer, std::size_t size)
  {
    memcpy(buffer, _buffer.data(), size);
    eraseNBytes(size);
  }

  std::size_t Buffer::size() const
  {
    return _buffer.size() - _pos;
  }

  const char* Buffer::data() const
  {
    return _buffer.data() + _pos;
  }

  void        Buffer::clear()
  {
    _pos = 0;
    _buffer.clear();
  }

  void        Buffer::eraseNBytes(std::size_t num)
  {
    _pos += num;
    if (_pos >= _buffer.size() / 2 && _pos > 32)
    {
      _buffer.erase(_buffer.begin(), _buffer.begin() + _pos);
      _pos = 0;
    }
  }

  void        Buffer::setContent(const char* buffer, std::size_t size)
  {
    clear();
    _buffer.insert(_buffer.end(), buffer, buffer + size);
  }

  const std::string Buffer::debug(uint8 width) const
  {
    assert(width > 0);
    const char        *data = _buffer.data();
    std::stringstream stream;

    stream << std::setfill('0') << std::hex;
    for (uint32 i = 0; i < _buffer.size(); ++i)
    {
      uint32  k = i + 1;
      if (k > 1 && i % width != 0)
        stream << ' ';
      uint8 c = data[i];
      stream << std::setw(2) << (unsigned int)c;
      if (k % width == 0 && k < _buffer.size())
        stream << '\n';
    }
    return stream.str();
  }

} // namespace Sery

////////////////////////////////////////
//             Stream.cpp             //
////////////////////////////////////////

#include <cstring>
#include <iostream>

namespace Sery
{

  Stream::Stream(IBuffer& buffer, Endian localEndian)
    : _buffer(buffer),
    _localEndian(localEndian)
  {
  }

  Stream::Stream(IBuffer& buffer)
    : _buffer(buffer),
    _localEndian(globalEndian)
  {
  }

  Stream::~Stream()
  {
  }

  Stream&  Stream::writeRaw(const char* buffer, uint32 size)
  {
    _buffer.writeRaw(buffer, size);
    return *this;
  }

  Stream&  Stream::readRaw(char* buffer, uint32 size)
  {
    _buffer.readRaw(buffer, size);
    return *this;
  }

  Endian  Stream::getLocalEndian() const
  {
    return _localEndian;
  }

  // Static members
  Endian   Stream::globalEndian = Endian::BigEndian;

  void     Stream::setGlobalEndian(Endian endian)
  {
    globalEndian = endian;
  }

  Endian   Stream::getGlobalEndian()
  {
    return globalEndian;
  }

  //////////////////////////////////////////
  // External functions for serialization //
  //////////////////////////////////////////

  // Templates for serializing arithmetics types
  template <class T, enable_if_t<std::is_arithmetic<T>::value>*>
  Stream&   operator<<(Stream& stream, T t)
  {
    Endian  softwareEndian = detail::getSoftwareEndian();
    Endian  currentEndian = stream.getLocalEndian();
    char    buffer[sizeof(T)];
    const uint8* p = reinterpret_cast<const uint8*>(&t);

    for (size_t index = 0;
         index < sizeof(T);
         ++index)
    {
      if (currentEndian == softwareEndian)
        buffer[index] = *p++;
      else
        buffer[sizeof(T) - index - 1] = *p++;
    }

    stream.writeRaw(buffer, sizeof(T));
    return stream;
  }

  // Explicit instantiations of templates functions
  template Stream& operator<< <int8>        (Stream&, int8);
  template Stream& operator<< <int16>       (Stream&, int16);
  template Stream& operator<< <int32>       (Stream&, int32);
  template Stream& operator<< <int64>       (Stream&, int64);
  template Stream& operator<< <uint8>       (Stream&, uint8);
  template Stream& operator<< <uint16>      (Stream&, uint16);
  template Stream& operator<< <uint32>      (Stream&, uint32);
  template Stream& operator<< <uint64>      (Stream&, uint64);
  template Stream& operator<< <float>       (Stream&, float);
  template Stream& operator<< <double>      (Stream&, double);
  template Stream& operator<< <long double> (Stream&, long double);

  // Templates for deserializing arithmetics types
  template <class T, enable_if_t<std::is_arithmetic<T>::value>*>
  Stream& Stream::operator>>(T& t)
  {
    Stream& stream = *this;
    Endian  softwareEndian = detail::getSoftwareEndian();
    Endian  currentEndian = stream.getLocalEndian();
    const char* buffer = stream._buffer.data();

    uint8*  p = reinterpret_cast<uint8*>(&t);

    for (size_t index = 0;
         index < sizeof(T);
         ++index)
    {
      if (currentEndian == softwareEndian)
        *p++ = buffer[index];
      else
        *p++ = buffer[sizeof(T) - index - 1];
    }

    stream._buffer.eraseNBytes(sizeof(T));
    return stream;
  }


  // Explicit instantiations of templates functions
  template Stream& Stream::operator>><int8>        (int8&);
  template Stream& Stream::operator>><int16>       (int16&);
  template Stream& Stream::operator>><int32>       (int32&);
  template Stream& Stream::operator>><int64>       (int64&);
  template Stream& Stream::operator>><uint8>       (uint8&);
  template Stream& Stream::operator>><uint16>      (uint16&);
  template Stream& Stream::operator>><uint32>      (uint32&);
  template Stream& Stream::operator>><uint64>      (uint64&);
  template Stream& Stream::operator>><float>       (float&);
  template Stream& Stream::operator>><double>      (double&);
  template Stream& Stream::operator>><long double> (long double&);

  // (De)Serialization of C-Style strings
  Stream&   operator<<(Stream& stream, const char* str)
  {
    uint32  len = static_cast<uint32>(std::strlen(str) + 1);

    stream << len;
    stream.writeRaw(str, len);
    return stream;
  }
  Stream&   operator>>(Stream& stream, char*& str)
  {
    uint32  len = 0;

    stream>>len;
    char* buffer = new char[len];
    stream.readRaw(buffer, len);
    str = buffer;
    return stream;
  }

  // (De)Serialization of boolean
  Stream&   operator<<(Stream& stream, const bool value)
  {
    stream << static_cast<const uint8>(value);
    return stream;
  }

  Stream&   operator>>(Stream& stream, bool& value)
  {
    uint8   casted = 0;

    stream>>casted;
    value = casted != 0;
    return stream;
  }

} // namespace Sery

////////////////////////////////////////
//           Stream_STD.cpp           //
////////////////////////////////////////

namespace Sery
{

  // std::string
  Stream& operator<<(Stream& stream, const std::string& str)
  {
    stream << str.c_str();
    return (stream);
  }

  Stream& operator>>(Stream& stream, std::string& str)
  {
    char* raw;

    stream>>raw;
    str = raw;
    return (stream);
  }

} // namespace Sery

#endif // SERY_GENERATE_IMPLEMENTATION

//
// LICENSE
// 
// 
// The MIT License (MIT)
// 
// Copyright (c) 2015-2017
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
