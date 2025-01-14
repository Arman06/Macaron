#pragma once

#include <Macaronlib/Common.hpp>
#include <Macaronlib/String.hpp>

namespace VgaTUI {

void Initialize();
void Print(const char* str);
void Print(const String&);
void Printn(int64_t numb, uint32_t s);
void Printd(int64_t);
void Putc(char);
void DecreaseCursor();

}
