// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

static const short BITS_IN_TELEM = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
    : BitLen(len), MemLen((BitLen - 1) / BITS_IN_TELEM + 1) {
  if (len < 0) {
    throw std::out_of_range("len is too smol");
  }
  pMem = new TELEM[MemLen];
  for (size_t i = 0; i < MemLen; i++) {
    pMem[i] = 0;
  }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
    : BitLen(bf.BitLen), MemLen(bf.MemLen) {
  pMem = new TELEM[MemLen];
  for (size_t i = 0; i < MemLen; i++) {
    pMem[i] = bf.pMem[i];
  }
}

TBitField::~TBitField() { delete[] pMem; }

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return n / BITS_IN_TELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return 1 << (n % BITS_IN_TELEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n < 0 || n >= GetLength()) {
    throw std::out_of_range("n is out of range");
  }
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= GetLength()) {
    throw std::out_of_range("n is out of range");
  }
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n >= GetLength()) {
    throw std::out_of_range("n is out of range");
  }
  return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField &TBitField::operator=(const TBitField &bf) // присваивание
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (size_t i = 0; i < MemLen; i++) {
    pMem[i] = bf.pMem[i];
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (GetLength() != bf.GetLength()) {
    return false;
  }

  for (int i = 0; i < MemLen; i++) {
    if (pMem[i] != bf.pMem[i]) {
      return false;
    }
  }

  return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  TBitField res(std::max(GetLength(), bf.GetLength()));
  int len = std::min(GetLength(), bf.GetLength());
  for (int i = 0; i < len; i++) {
    res.pMem[i] = pMem[i] | bf.pMem[i];
  }
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  TBitField res(std::max(GetLength(), bf.GetLength()));
  int len = std::min(GetLength(), bf.GetLength());
  for (int i = 0; i < len; i++) {
    res.pMem[i] = pMem[i] & bf.pMem[i];
  }
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField bf(BitLen);
  for (int i = 0; i < MemLen - 1; i++) {
    bf.pMem[i] = ~pMem[i];
  }
  TELEM lastlen = GetLength() % BITS_IN_TELEM;
  bf.pMem[MemLen - 1] =
      ~(pMem[MemLen - 1] | (((TELEM)-1 >> lastlen) << lastlen));
  return bf;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  return ostr;
}
