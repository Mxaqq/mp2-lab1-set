// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
	if (len < 0)
	{
		throw std::invalid_argument("Invalid length");
	}
	BitLen = len;
	MemLen = (len + 31) >> 5;
	pMem = new TELEM[MemLen];
	if (pMem != nullptr)
	{
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = 0;
		}
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != nullptr)
	{
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = bf.pMem[i];
		}
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen)
	{
		throw std::out_of_range("Index out of range");
	}
	return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen)
	{
		throw std::out_of_range("Index out of range");
	}
	return 1 << (n % 32);
}

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen)
	{
		throw std::invalid_argument("Invalid length");
	}
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen)
	{
		throw std::invalid_argument("Invalid length");
	}
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen)
	{
		throw std::invalid_argument("Invalid length");
	}
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return (pMem[index] & mask) != 0;
}

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (&bf != this)
	{
		if (MemLen < bf.MemLen) {
			delete[] pMem;
			pMem = new TELEM[bf.MemLen];
		}
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;

		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
	{
		return 0;
	}
	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
		{
			return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField result(maxLen);
	int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

	for (int i = 0; i < minMemLen; i++)
	{
		result.pMem[i] = pMem[i] | bf.pMem[i];
	}

	if (MemLen == minMemLen && bf.MemLen > minMemLen) {
		for (int i = minMemLen; i < bf.MemLen; ++i) {
			result.pMem[i] = bf.pMem[i];
		}
	}
	else if (bf.MemLen == minMemLen && MemLen > minMemLen) {
		for (int i = minMemLen; i < MemLen; ++i) {
			result.pMem[i] = pMem[i];
		}
	}

	return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	// Не нужно выкидывать исключение, если длины BitField разные.
	int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField result(maxLen);
	int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

	for (int i = 0; i < minMemLen; i++)
	{
		result.pMem[i] = pMem[i] & bf.pMem[i];
	}

	// Нет необходимости обнулять дополнительные биты, потому что в конструкторе TBitField они уже обнуляются.

	return result;
}


TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i))
		{
			result.ClrBit(i);
		}
		else
		{
			result.SetBit(i);
		}
	}
	return result;
}

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		int bit;
		istr >> bit;
		if (bit == 1)
		{
			bf.SetBit(i);
		}
		else if (bit == 0)
		{
			bf.ClrBit(i);
		}
		else
		{
			throw std::invalid_argument("Invalid input");
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
