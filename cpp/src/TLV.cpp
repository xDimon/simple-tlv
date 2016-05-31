/*
 * TLV.cpp
 *
 *  Created on: 03 июля 2014 г.
 *      Author: Dmitriy Khaustov aka Dиmon (khaustov.dm@gmail.com)
 */

#include "TLV.hpp"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

TLV::TLV()
{
	_buff = _buff0;
	_buffSize = sizeof(_buff0);
	reset();
}

TLV::~TLV()
{
	if (_buff != _buff0)
	{
		free(_buff);
	}
}

void TLV::s(bool value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = value ? Token::TRUE_VAL : Token::FALSE_VAL;
	push();
}

void TLV::s(int8_t value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = Token::INT_8;
	_value.int8 = value;
	push();
}

void TLV::s(uint8_t value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = Token::UINT_8;
	_value.uint8 = value;
	push();
}

void TLV::s(int16_t value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = Token::INT_16;
	_value.int16 = value;
	compress();
}

void TLV::s(uint16_t value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = Token::UINT_16;
	_value.uint16 = value;
	compress();
}

void TLV::s(int32_t value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = Token::INT_32;
	_value.int32 = value;
	compress();
}

void TLV::s(uint32_t value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = Token::UINT_32;
	_value.uint32 = value;
	compress();
}

void TLV::s(int64_t value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = Token::INT_64;
	_value.int64 = value;
	compress();
}

void TLV::s(uint64_t value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	_type = Token::UINT_64;
	_value.uint64 = value;
	compress();
}

void TLV::s(const char * value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	if (value)
	{
		_type = Token::STRING;
		_value.str = value;
	}
	else
	{
		_type = Token::NULL_VAL;
	}
	push();
}

void TLV::s(const void *value, size_t size)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;
	if (value)
	{
		_type = Token::BINARY;
		_value.bin = value;
		_size = size;
	}
	else
	{
		_type = Token::NULL_VAL;
	}
	push();
}

void TLV::s(const TLV *value)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (!value->isFinalized())
	{
		_error = "Added stream unfinalized";
		return;
	}
	if (_t[_level] == 'o')
	{
		_error = "Unexpected ObjVal without ObjKey for object";
		return;
	}
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_finaly = _level == 0;

	_type = Token::_EXTERNAL;
	_value.ext = value;
	push();
}

void TLV::k(const char * key)
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjKey for no object";
		return;
	}
	if (_level >= (sizeof(_t) - 1))
	{
		_error = "Too big nesting level";
		return;
	}
	_t[++_level] = 'k';
	_type = Token::_KEY;
	_value.str = key;
	push();
}

void TLV::p(const char *key, bool value)
{
	if (_error)
	{
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjVal for no object";
		return;
	}
	_type = Token::_KEY;
	_value.str = key;
	push();

	_type = value ? Token::TRUE_VAL : Token::FALSE_VAL;
	push();
}

void TLV::p(const char *key, int32_t value)
{
	if (_error)
	{
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjVal for no object";
		return;
	}
	_type = Token::_KEY;
	_value.str = key;
	push();

	_type = Token::INT_32;
	_value.int32 = value;
	compress();
}

void TLV::p(const char *key, uint32_t value)
{
	if (_error)
	{
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjVal for no object";
		return;
	}
	_type = Token::_KEY;
	_value.str = key;
	push();

	_type = Token::UINT_32;
	_value.uint32 = value;
	compress();
}

void TLV::p(const char *key, int64_t value)
{
	if (_error)
	{
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjVal for no object";
		return;
	}
	_type = Token::_KEY;
	_value.str = key;
	push();

	_type = Token::INT_64;
	_value.int64 = value;
	compress();
}

void TLV::p(const char *key, uint64_t value)
{
	if (_error)
	{
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjVal for no object";
		return;
	}
	_type = Token::_KEY;
	_value.str = key;
	push();

	_type = Token::UINT_64;
	_value.uint64 = value;
	compress();
}

void TLV::p(const char *key, const char * value)
{
	if (_error)
	{
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjVal for no object";
		return;
	}
	_type = Token::_KEY;
	_value.str = key;
	push();

	_type = Token::STRING;
	_value.str = value;
	push();
}

void TLV::p(const char *key, const void * value, uint32_t size)
{
	if (_error)
	{
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjVal for no object";
		return;
	}
	_type = Token::_KEY;
	_value.str = key;
	push();

	_type = Token::BINARY;
	_value.bin = value;
	_size = size;
	push();
}

void TLV::p(const char *key, const TLV *value)
{
	if (_error)
	{
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjVal for no object";
		return;
	}
	_type = Token::_KEY;
	_value.str = key;
	push();

	_type = Token::_EXTERNAL;
	_value.ext = value;
	push();
}

/// Начало массива
void TLV::A_()
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_level >= (sizeof(_t) - 1))
	{
		_error = "Too big nesting level";
		return;
	}
	_t[++_level] = 'a';
	_type = Token::ARRAY;
	push();
}

/// Конец массива
void TLV::_A()
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] != 'a')
	{
		_error = "Unexpected ArrEnd value";
		return;
	}
	_finaly = --_level == 0;
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_type = Token::END;
	push();
}

/// Начало объекта
void TLV::O_()
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_level >= (sizeof(_t) - 1))
	{
		_error = "Too big nesting level";
		return;
	}
	_t[++_level] = 'o';
	_type = Token::OBJECT;
	push();
}

/// Конец объекта
void TLV::_O()
{
	if (_error)
	{
		return;
	}
	if (_finaly)
	{
		_error = "Stream finalized";
		return;
	}
	if (_t[_level] != 'o')
	{
		_error = "Unexpected ObjEnd value";
		return;
	}
	_finaly = --_level == 0;
	if (_t[_level] == 'k')
	{
		_level--;
	}
	_type = Token::END;
	push();
}

void TLV::compress()
{
	if (_type == Token::INT_64)
	{
		if (_value.int64 >= 0)
		{
			_type = Token::UINT_64;
		}
		else if (_value.int64 >= ((int32_t)(1ul << 31)))
		{
			_type = Token::INT_32;
		}
	}
	if (_type == Token::UINT_64)
	{
		if (_value.uint64 < (uint32_t)(-1))
		{
			_type = Token::UINT_32;
		}
	}

	if (_type == Token::INT_32)
	{
		if (_value.int32 >= 0)
		{
			_type = Token::UINT_32;
		}
		else if (_value.int32 >= ((int16_t)(1ul << 15)))
		{
			_type = Token::INT_16;
		}
	}
	if (_type == Token::UINT_32)
	{
		if (_value.uint32 <= (uint16_t)(-1))
		{
			_type = Token::UINT_16;
		}
	}

	if (_type == Token::INT_16)
	{
		if (_value.int16 >= 0)
		{
			_type = Token::UINT_16;
		}
		else if (_value.int16 >= ((int8_t)(1ul << 7)))
		{
			_type = Token::INT_8;
		}
	}
	if (_type == Token::UINT_16)
	{
		if (_value.uint16 <= (uint8_t)(-1))
		{
			_type = Token::UINT_8;
		}
	}

	if (_type == Token::INT_8)
	{
		if (_value.int8 >= 0)
		{
			_type = Token::UINT_8;
		}
	}
	if (_type == Token::UINT_8)
	{
		if (_value.uint8 == 0)
		{
			_type = Token::ZERO;
		}
	}

	push();
}

void TLV::pre(size_t size)
{
	if (_error)
	{
		return;
	}

	size_t pos = _p - _buff;
	if (_buffSize >= (pos + size))
	{
		return;
	}

	char *newBuff;
	size_t newSize = (1 + (pos + size - 1) / (1<<12)) * (1<<12); // Расширяем пачками кратными 4K
	if (_buff == _buff0)
	{
		newBuff = (char *)malloc(newSize);
		if (!newBuff)
		{
			_error = "Not anough memory";
			return;
		}
//		Log::debugf("Memory allocate for TLV-buffer %u bytes", (int)newSize);
		memcpy(newBuff, _buff0, pos);
	}
	else
	{
		newBuff = (char *)realloc(_buff, newSize);
		if (!newBuff)
		{
			if (_buff != _buff0)
			{
				free(_buff);
			}
			_buff = _buff0;
			_error = "Not anough memory";
			return;
		}
//		Log::debugf("Memory reallocate for TLV-buffer from %u to %u bytes", (int)_buffSize, (int)newSize);
	}

	_buff = newBuff;
	_buffSize = newSize;
	_p = newBuff + pos;
}

void TLV::push()
{
	switch (_type)
	{
		case Token::END:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::END);
			_p += sizeof(uint8_t);
			break;

		case Token::FALSE_VAL:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::FALSE_VAL);
			_p += sizeof(uint8_t);
			break;

		case Token::TRUE_VAL:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::TRUE_VAL);
			_p += sizeof(uint8_t);
			break;

		case Token::NULL_VAL:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::NULL_VAL);
			_p += sizeof(uint8_t);
			break;

		case Token::NAN:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::NAN);
			_p += sizeof(uint8_t);
			break;

		case Token::ZERO:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::ZERO);
			_p += sizeof(uint8_t);
			break;

		case Token::INT_8:
			pre(sizeof(uint8_t) + sizeof(int8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::INT_8);
			_p += sizeof(uint8_t);
			*(int8_t*)_p = _value.int8;
			_p += sizeof(int8_t);
			break;

		case Token::UINT_8:
			pre(sizeof(uint8_t) + sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::UINT_8);
			_p += sizeof(uint8_t);
			*(uint8_t*)_p = _value.uint8;
			_p += sizeof(uint8_t);
			break;

		case Token::INT_16:
			pre(sizeof(uint8_t) + sizeof(int16_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::INT_16);
			_p += sizeof(uint8_t);
			*(int16_t*)_p = _value.int16;
			_p += sizeof(int16_t);
			break;

		case Token::UINT_16:
			pre(sizeof(uint8_t) + sizeof(uint16_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::UINT_16);
			_p += sizeof(uint8_t);
			*(uint16_t*)_p = _value.uint16;
			_p += sizeof(uint16_t);
			break;

		case Token::INT_32:
			pre(sizeof(uint8_t) + sizeof(int32_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::INT_32);
			_p += sizeof(uint8_t);
			*(int32_t*)_p = _value.int32;
			_p += sizeof(int32_t);
			break;

		case Token::UINT_32:
			pre(sizeof(uint8_t) + sizeof(uint32_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::UINT_32);
			_p += sizeof(uint8_t);
			*(uint32_t*)_p = _value.uint32;
			_p += sizeof(uint32_t);
			break;

		case Token::INT_64:
			pre(sizeof(uint8_t) + sizeof(int64_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::INT_64);
			_p += sizeof(uint8_t);
			*(int64_t*)_p = _value.int64;
			_p += sizeof(int64_t);
			break;

		case Token::UINT_64:
			pre(sizeof(uint8_t) + sizeof(uint64_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::UINT_64);
			_p += sizeof(uint8_t);
			*(uint64_t*)_p = _value.uint64;
			_p += sizeof(uint64_t);
			break;

		case Token::STRING:
		{
			size_t slen = strlen(_value.str) + 1;

			pre(sizeof(uint8_t) + slen);
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::STRING);
			_p += sizeof(uint8_t);
			memcpy(_p, _value.str, slen);
			_p += slen;
			break;
		}
		case Token::ARRAY:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::ARRAY);
			_p += sizeof(uint8_t);
			break;

		case Token::OBJECT:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::OBJECT);
			_p += sizeof(uint8_t);
			break;

		case Token::RECURSION:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::RECURSION);
			_p += sizeof(uint8_t);
			break;

		case Token::UNAVAILABLE:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::UNAVAILABLE);
			_p += sizeof(uint8_t);
			break;
		case Token::BINARY:
		{
			pre(sizeof(uint8_t) + sizeof(_size) + _size);
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::BINARY);
			_p += sizeof(uint8_t);
			memcpy(_p, &_size, sizeof(_size));
			_p += sizeof(_size);
			memcpy(_p, _value.bin, _size);
			_p += _size;
			break;
		}

		case Token::_KEY:
		{
			size_t slen = strlen(_value.str) + 1;
			pre(slen);
			if (_error) return;
			memcpy(_p, _value.str, slen);
			_p += slen;
			break;
		}
		case Token::_EXTERNAL:
		{
			size_t size;
			const void *buff = _value.ext->getData(&size);
			pre(size);
			if (_error) return;
			memcpy(_p, buff, size);
			_p += size;
			break;
		}
		default:
			pre(sizeof(uint8_t));
			if (_error) return;
			*((uint8_t *)(_p)) = static_cast<uint8_t>(Token::UNKNOWN);
			_p += sizeof(uint8_t);
			break;
	}
}

const void *TLV::getData(size_t *size) const
{
	if (size)
	{
		*size = _p - _buff;
	}
	if (_error)
	{
//		Log::debugf("Getint data for invalid TLV. Error: %s", _error);
		throw(_error);
	}
//	size_t pos = _p - _buff;
//	if (pos > sizeof(_buff0))
//	{
//		Log::debugf("Big data in TVL: %u bytes", (int)pos);
//	}
	return _buff;
}

TLV::Out TLV::getOut() const
{
	if (_error)
	{
//		Log::debugf("Getint data for invalid TLV. Error: %s", _error);
		throw(_error);
	}
	return Out(_buff, _p - _buff);
}
