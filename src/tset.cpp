// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
}

TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    BitField = s.BitField;
}

TSet::TSet(const TBitField& bf) : BitField(bf.GetLength())
{
    BitField = bf;
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower() const
{
    return BitField.GetLength();
}

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= GetMaxPower())
    {
        throw std::out_of_range("Element is out of range");
    }


    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= GetMaxPower())
    {
        throw std::out_of_range("Element is out of range");
    }

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= GetMaxPower())
    {
        throw std::out_of_range("Element is out of range");
    }

    BitField.ClrBit(Elem);
}

TSet& TSet::operator=(const TSet& s)
{
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet& s)
{
    TSet result(BitField | s.BitField);
    return result;
}

TSet TSet::operator+(const int Elem)
{
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem)
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet& s)
{
    int MaxSize = std::max(BitField.GetLength(), s.BitField.GetLength());

    TBitField ResultBitField(MaxSize);
    for (int i = 0; i < MaxSize; i++)
    {
        if (i < BitField.GetLength() && i < s.BitField.GetLength() && BitField.GetBit(i) && s.BitField.GetBit(i))
        {
            ResultBitField.SetBit(i);
        }
    }

    TSet result(ResultBitField);
    return result;
}

TSet TSet::operator~()
{
    TSet result(*this);
    for (int i = 0; i < result.GetMaxPower(); i++)
    {
        if (result.IsMember(i))
        {
            result.DelElem(i);
        }
        else
        {
            result.InsElem(i);
        }
    }
    return result;
}

std::istream& operator>>(std::istream& istr, TSet& s)
{
    std::string input;
    istr >> input;

    for (int i = 0; i < input.length(); i++)
    {
        int value = input[i] - '0';
        if (value == 1)
        {
            s.InsElem(i);
        }
    }

    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s)
{
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        ostr << s.IsMember(i);
    }
    return ostr;
}

