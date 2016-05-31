/*
 * TLV.hpp
 *
 *  Created on: 03 июля 2014 г.
 *      Author: Dmitriy Khaustov aka Dиmon (khaustov.dm@gmail.com)
 */

#ifndef TLV_HPP_
#define TLV_HPP_

#include <stddef.h>
#include <cstdint>

class TLV
{
public:
	typedef struct Out {
		const char *data;
	    const size_t size;
	    Out(const char *d, size_t s): data(d), size(s){};
	    ~Out(){};
	} Out;

	enum class Token {
		END 		= 0,
		FALSE_VAL	= 1,
		TRUE_VAL	= 2,
		NULL_VAL	= 3,
#ifdef NAN
#undef NAN
#endif
		NAN			= 4,
		ZERO		= 10,
		INT_8		= 11,
		UINT_8		= 12,
		INT_16		= 13,
		UINT_16		= 14,
		INT_32		= 15,
		UINT_32		= 16,
		INT_64		= 17,
		UINT_64		= 18,
		STRING		= 20,
		ARRAY		= 30,
		OBJECT		= 40,
		RECURSION	= 50,
		UNAVAILABLE	= 60,
		BINARY		= 100,
		_KEY		= 200,
		_EXTERNAL	= 250,
		UNKNOWN		= 255
	};
	static constexpr char *null = nullptr;

private:
	char _buff0[1<<13];
	char *_buff;
	size_t _buffSize;
	char *_p;

	uint8_t _level;
	uint8_t _t[32];
	bool _finaly;
	const char *_error;

	Token _type;
	union {
		int8_t 		int8;
		uint8_t		uint8;
		int16_t 	int16;
		uint16_t	uint16;
		int32_t 	int32;
		uint32_t	uint32;
		int64_t 	int64;
		uint64_t	uint64;
		const char* str;
		const void* bin;
		const TLV *ext;
	} _value;
	uint32_t _size;

	void compress();
	void pre(size_t size);
	void push();

public:
	TLV();
	virtual ~TLV();

	void s(bool value);
	void s(int8_t value);
	void s(uint8_t value);
	void s(int16_t value);
	void s(uint16_t value);
	void s(int32_t value);
	void s(uint32_t value);
	void s(int64_t value);
	void s(uint64_t value);
	void s(const char *value);
	void s(const void *value, size_t size);
	void s(const TLV *value);
	void k(const char * value);
	void p(const char *key, bool value);
	void p(const char *key, int32_t value);
	void p(const char *key, uint32_t value);
	void p(const char *key, int64_t value);
	void p(const char *key, uint64_t value);
	void p(const char *key, const char *value);
	void p(const char *key, const void *value, uint32_t size);
	void p(const char *key, const TLV *value);
	/// Начало массива
	void A_();

	/// Конец массива
	void _A();

	/// Начало объекта
	void O_();

	/// Конец объекта
	void _O();

	/// Вернуть сериализованные данные
	const void *getData(size_t *size) const;

	/// Вернуть сериализованные данные
	Out getOut() const;

	inline void reset()
	{
		_type = Token::UNKNOWN;
		_p = _buff;
		_error = nullptr;
		_level = 0;
		_t[_level] = 'n';
		_finaly = false;
	}

	inline const char *getError() const
	{
		return _error;
	}

	inline bool isFinalized() const
	{
		return _finaly;
	}
};

#endif /* TLV_HPP_ */
